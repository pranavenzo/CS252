
/*
 * CS252: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <string>
#include <signal.h>
#include <fcntl.h>
#include "command.h"
#include <pwd.h>
#include <vector>
#include <sstream>
int *backgrounds;
int MAX_PROC = 256;
int source = 0;
int moveCount = 29;
int defin;
int defout;
int deferr;
int retCode = -1;
int bretCode = -1;
int flag = 0;
using namespace std;
string path;
extern "C" char * history[];
extern "C" int history_index;
int lastExec;
SimpleCommand::SimpleCommand()
{
	// Create available space for 5 arguments
	_numOfAvailableArguments = 5;
	_numOfArguments = 0;
	_arguments = (char **) malloc( _numOfAvailableArguments * sizeof( char * ) );
}
int contains(char * input, char x) {
	int pos = -1;
	int i = 0;
	for(i = 0; input[i] != '\0';i++) {
		if(input[i] == x) {
			pos = i;
			break;
		}
	}
	return pos;
}

string numToS(int x) {
	stringstream ss;
	ss << x;
	return ss.str();
}
string builtinEnv(string arg) {
	if(arg == "SHELL") return path;	
	if(arg.size() != 1) return "";
	char c = arg[0];
	string output("");
	switch(c) {
		case '$': output = numToS(getpid()); break;//make String
		case '?': output = numToS(retCode); break;
		case '!': output = numToS(bretCode); break;
		case '_': {
			if(history_index > 1) {
				output = history[lastExec - 1];
				flag = 1;
			}
			break;
		}

	}
	return output;
}


	void
SimpleCommand::insertArgument( char * argument )
{
	int pos = contains(argument, '$');
	while(pos != -1 && !flag) {
		int pos1_0 = contains((argument+pos),'{');
		if(pos1_0 == 1) {
			int pos2 = contains((argument+pos),'}');
			if(pos2 >= 2) {
				string arg(argument);
				string half1(arg.substr(0,pos));
				string half3(arg.substr(pos2+pos+1));
				string middle(arg.substr(pos+2,pos2-2));
				string replaced("");
				if(builtinEnv(middle) == "") {
					char * x =  getenv(middle.c_str());
					if(!x) {
						lastExec--;
						perror("Bad variable");
						Command::_currentCommand.clear();
						return;
					}
					replaced = getenv(middle.c_str());

				} else {
					replaced = builtinEnv(middle);
				}					
				arg = half1 + replaced + half3;
				argument = strdup(arg.c_str());
			} 

		} else {
			perror("Error in syntax");
			Command::_currentCommand.clear();
			return;
		}
		pos = contains(argument, '$');
	}
	flag = 0;
	if(argument[0] == '~') {
		if(strlen(argument) == 1) {
			free(argument);
			argument = NULL;
			argument = (Command::_currentCommand.findHome());
		} else {
			passwd * x = (getpwnam(argument+1));
			if(x) argument = strdup(x->pw_dir);
		}
	}

	if ( _numOfAvailableArguments == _numOfArguments  + 1 ) {
		// Double the available space
		_numOfAvailableArguments *= 2;
		_arguments = (char **) realloc( _arguments,
				_numOfAvailableArguments * sizeof( char * ) );
	}

	_arguments[ _numOfArguments ] = argument;

	// Add NULL argument at the end
	_arguments[ _numOfArguments + 1] = NULL;

	_numOfArguments++;
}

Command::Command()
{
	// Create available space for one simple command
	_numOfAvailableSimpleCommands = 1;
	_simpleCommands = (SimpleCommand **)
		malloc( _numOfSimpleCommands * sizeof( SimpleCommand  ) );

	_numOfSimpleCommands = 0;
	_outFile = 0;
	_inFile = 0;
	_errFile = 0;
	_background = 0;
}

	void
Command::insertSimpleCommand( SimpleCommand * simpleCommand )
{
	if ( _numOfAvailableSimpleCommands == _numOfSimpleCommands ) {
		_numOfAvailableSimpleCommands *= 2;
		SimpleCommand ** tmp =  (SimpleCommand **) realloc( _simpleCommands,
				_numOfAvailableSimpleCommands * sizeof( SimpleCommand  ) );
		if(tmp != NULL)
		_simpleCommands = tmp;
		else exit(1);
	}
	_simpleCommands[ _numOfSimpleCommands ] = simpleCommand;
	_numOfSimpleCommands++;
}

	void
Command:: clear()
{
	for ( int i = 0; i < _numOfSimpleCommands; i++ ) {
		for ( int j = 0; j < _simpleCommands[ i ]->_numOfArguments; j ++ ) {
			free ( _simpleCommands[ i ]->_arguments[ j ] );
		}

		free ( _simpleCommands[ i ]->_arguments );
		free ( _simpleCommands[ i ] );
	}

	if ( _outFile ) {
		free( _outFile );
	}

	if ( _inFile ) {
		free( _inFile );
	}

	if ( _errFile ) {
		free( _errFile );
	}

	_numOfSimpleCommands = 0;
	_outFile = 0;
	_inFile = 0;
	_errFile = 0;
	_background = 0;
}

	void
Command::print()
{
	printf("\n\n");
	printf("              COMMAND TABLE                \n");
	printf("\n");
	printf("  #   Simple Commands\n");
	printf("  --- ----------------------------------------------------------\n");

	for ( int i = 0; i < _numOfSimpleCommands; i++ ) {
		printf("  %-3d ", i );
		for ( int j = 0; j < _simpleCommands[i]->_numOfArguments; j++ ) {
			printf("\"%s\" \t", _simpleCommands[i]->_arguments[ j ] );
		}
	}

	printf( "\n\n" );
	printf( "  Output       Input        Error        Background\n" );
	printf( "  ------------ ------------ ------------ ------------\n" );
	printf( "  %-12s %-12s %-12s %-12s\n", _outFile?_outFile:"default",
			_inFile?_inFile:"default", _errFile?_errFile:"default",
			_background?"YES":"NO");
	printf( "\n\n" );

}

	void
Command::execute()
{
	// Don't do anything if there are no simple commands
	// print();
	if ( _numOfSimpleCommands == 0 ) {
		moveCount--;
		prompt();
		return;
	}

	if (strcmp(_simpleCommands[0]->_arguments[0], "exit") == 0) {
		printf("Process exited\n\n");
		clear();
		close(0);
		close(1);
		close(2);
		exit(0);
	}// Print contents of Command data structure
	/*print();*/
	if(checkBuiltin(0)) {
		clear();
		prompt();
		return;
	}
	if(!strcmp(_simpleCommands[0]->_arguments[0], "cd")) {
		int p;
		if(_simpleCommands[0]->_numOfArguments > 1) {
			p = chdir(_simpleCommands[0]->_arguments[1]);
		} else {
			char * home = findHome();
			p = chdir(home);
			free(home);
		}   
		//if(!p) perror("chdir");
		clear();
		prompt();
		return;
	}
	defin = dup(0);
	defout = dup(1);
	deferr = dup(2);

	int fdin=0;
	int fdout=0;
	int fderr=0;
	if(!_inFile) fdin = dup(defin);
	else fdin = open(_inFile,O_RDONLY);

	if(!_errFile) fderr = dup(deferr);
	else {
		if(_append) fderr = open(_errFile,O_WRONLY|O_CREAT|O_APPEND,0600);
		else fderr = open(_errFile,O_WRONLY|O_CREAT|O_TRUNC,0600);
	}
	dup2(fderr,2);
	close(fderr);
	// Add execution here
	// For every simple command fork a new process
	// Setup i/o redirection
	// and call exec
	int i,pid;
	for(i = 0; i < _numOfSimpleCommands; i++) {
		dup2(fdin,0);
		close(fdin);
		if(!lastIteration(&fdin, &fdout, &fderr, i)) {
			int fdpipe[2];
			pipe(fdpipe);
			fdin = fdpipe[0];
			fdout = fdpipe[1];
		}
		dup2(fdout, 1);
		close(fdout);
		//start children
		pid = fork();
		if(pid == -1) {
			perror("Error in ,_simpleCommands[i]->_arguments[0]");
			clear();
			prompt();
			return;
		}
		if(!pid && !checkPrintEnv(i)) {
			execvp(_simpleCommands[i]->_arguments[0],_simpleCommands[i]->_arguments);
			perror(_simpleCommands[i]->_arguments[0]); //error if returns;
			close(defin);
			close(defout);
			close(deferr);
			close(fderr);
			close(0);
			close(1);
			close(2);
			_exit(1);
		}
	}
	dup2(defin,0);
	dup2(defout,1);
	dup2(deferr,2);
	close(defin);
	close(defout);
	close(deferr);
	if(!_background) {
		waitpid(pid,&retCode,0);
		clear();
		prompt();
	} else 	{
		int i;
		for (i = 0; i < MAX_PROC; i++)
		{
			if (backgrounds[i] == 0)
				break;
		}
		backgrounds[i] = pid;
		clear();
	}
	close(fderr);
	lastExec = history_index;
	if(retCode != 0) lastExec--;
}

int
Command::checkBuiltin(int index) {
	return !( !checkSetEnv(index) && !checkUnsetEnv(index));
}

int 
Command::checkPrintEnv(int index) {
	if(!strcmp(_simpleCommands[index]->_arguments[0],"printenv")) {
		char **p = environ;
		while(*p!=NULL) printf("%s\n",*(p++));
		exit(0);
	}
	return 0;
}
int 
Command::checkSetEnv(int index) {
	if(!strcmp(_simpleCommands[index]->_arguments[0],"setenv")) {
		int code = setenv(_simpleCommands[0]->_arguments[1], _simpleCommands[0]->_arguments[2], 1);
		if(code) perror("setenv");
		return 1;
	}
	return 0;
}

int
Command::checkUnsetEnv(int index) {
	if (strcmp(_simpleCommands[0]->_arguments[0], "unsetenv") == 0) {
		int code = unsetenv(_simpleCommands[0]->_arguments[1]);
		if (code != 0) perror("unsetenv");
		return 1;
	}
	return 0;
}

int 
Command::lastIteration(int *fdin, int *fdout, int *fderr, int i) {

	if(i == _numOfSimpleCommands - 1) {
		if(!_outFile) *fdout = dup(defout);
		else {
			if(_append) *fdout = open(_outFile, O_WRONLY|O_CREAT|O_APPEND, 0600);
			else *fdout = open(_outFile, O_WRONLY|O_CREAT|O_TRUNC, 0600);
		}
		if(!_errFile) *fderr = dup(deferr);
		else {
			if(_append) *fderr = open(_errFile, O_WRONLY|O_CREAT|O_APPEND, 0600);
			else open(_errFile, O_WRONLY|O_CREAT|O_TRUNC, 0600);
		}
		return 1;
	} else return 0;
}

char * 
Command::findHome() {
	char** p = environ;
	while(*p!=NULL) {
		if(!strncmp(*p,"HOME",4)) break;
		p++;
	}
	char * home = strdup(*p+5); //check
	return home;
}
// Shell implementation

	void
Command::prompt()
{
	if(isatty(0) && !source) printf("enzsh %d > ",moveCount++);
	fflush(stdout);
}

Command Command::_currentCommand;
SimpleCommand * Command::_currentSimpleCommand;

int yyparse(void);

extern "C" void disp(int sig) {
	printf("\n");
	moveCount--;
	Command::_currentCommand.prompt();
}



extern "C" void zombieElimination(int sig) {
	int pid = wait3(0, 0, NULL);
	bretCode = pid;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	int hunted = 0;
	int i;
	for (i = 0; i < MAX_PROC; i++) {
		if (backgrounds[i] == pid) {
			hunted = 1;
			backgrounds[i] = 0;
		}
	}

	if (hunted == 1) {
		printf("[%d] exited.\n", pid);    
		Command::_currentCommand.prompt();
	}
}
int main(int argc,char* argv[]) {
	//path = "";
	path = argv[0];
	struct sigaction signalAction1;

	signalAction1.sa_handler = disp;
	sigemptyset(&signalAction1.sa_mask);
	signalAction1.sa_flags = SA_RESTART;

	int error1 = sigaction(SIGINT, &signalAction1, NULL);
	if (error1)
	{
		perror("sigaction");
		exit(-1);
	}

	struct sigaction signalAction2;

	signalAction2.sa_handler = zombieElimination;
	sigemptyset(&signalAction2.sa_mask);
	signalAction2.sa_flags = SA_RESTART;

	int error2 = sigaction(SIGCHLD, &signalAction2, NULL);
	if (error2) 
	{
		perror("sigaction");
		exit(-1);
	}

	backgrounds = (int*)malloc(sizeof(int)*MAX_PROC);
	Command::_currentCommand.prompt();
	yyparse();
}

