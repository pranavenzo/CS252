
/*
 * CS-252
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */

%token	<string_val> WORD

%token 	TAB NOTOKEN GREAT LESS NEWLINE DGREAT DGREATAMP GREATAMP PIPE AMP

%union	{
	char   *string_val;
}

%{
	//#define yylex yylex
#include <stdio.h>
#include <string.h>
#include <string>
#include "command.h"
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <algorithm>
	using namespace std;
	void yyerror(const char * s);
	int yylex();
	vector<string> expandSingleWC(char * argument);	
	int match(char const *needle, char const *haystack);
	void expandAllWC(char * argument);
	vector<string> putSubshell(char * argument);
	%}

	%%

	goal:	
	commands
	;

commands: 
command
| commands command 
;

command: simple_command
;

simple_command:	
pipe_list io_modifier_list background_opt NEWLINE {
	//		printf("   Yacc: Execute command\n");
	Command::_currentCommand.execute();
}
| NEWLINE {
	Command::_currentCommand.execute();
}
| error NEWLINE { yyerrok; }
;
command_and_args:
command_word argument_list {
	Command::_currentCommand.
		insertSimpleCommand( Command::_currentSimpleCommand );
}
;

argument_list:
argument_list argument
| /* can be empty */
;

argument:
WORD {
	//      printf("   Yacc: insert argument \"%s\"\n", $1);
	vector<string> args = putSubshell($1);	
	for(int i = 0; i < args.size();i++) expandAllWC(strdup(args[i].c_str()));
	//Command::_currentSimpleCommand->insertArgument( $1 );
}
;

command_word:
WORD {
	//      printf("   Yacc: insert command \"%s\"\n", $1);

	Command::_currentSimpleCommand = new SimpleCommand();
	Command::_currentSimpleCommand->insertArgument( $1 );
}
|
;
pipe_list:
pipe_list PIPE command_and_args
|
command_and_args
;

iomodifier_opt:
DGREAT WORD {
	//printf("   Yacc: insert output append\"%s\"\n", $2);
	if(Command::_currentCommand._outFile) yyerror("Ambiguous output redirect.\n");
	Command::_currentCommand._outFile = strdup($2);
	Command::_currentCommand._append = 1;

}
| GREAT WORD {
	//	printf("   Yacc: insert output \"%s\"\n", $2);
	if(Command::_currentCommand._outFile) yyerror("Ambiguous output redirect.\n");
	Command::_currentCommand._outFile = strdup($2);
}
| DGREATAMP WORD {
	//	printf("   Yacc: insert output append \"%s\"\n", $2);
	if(Command::_currentCommand._outFile) yyerror("Ambiguous output redirect.\n");
	Command::_currentCommand._outFile = strdup($2);
	Command::_currentCommand._errFile = strdup($2);
	Command::_currentCommand._append = 1;
}
| GREATAMP WORD {
	//	printf("   Yacc: insert output \"%s\"\n", $2);
	if(Command::_currentCommand._outFile) yyerror("Ambiguous output redirect.\n");
	Command::_currentCommand._outFile = strdup($2);
	Command::_currentCommand._errFile = strdup($2);

}
| LESS WORD {
	//	printf("   Yacc: insert input \"%s\"\n", $2);
	Command::_currentCommand._inFile = strdup($2);

}
/* can be empty */ 
;
io_modifier_list:
io_modifier_list iomodifier_opt
| iomodifier_opt
|/*empty*/
;
background_opt:
AMP {
	Command::_currentCommand._background = 1;
}
|
;
%%
	void
yyerror(const char * s)
{
	fprintf(stderr,"%s", s);
}

vector<string> expandSingleWC(char * argument) {
	vector<string> retval;
	vector<string> out;
	out.clear();
	string arg(argument);
	int ast = arg.find("*");
	int qmark = arg.find("?");
	if(ast == -1) ast = qmark;
	else if(qmark != -1 && qmark < ast) ast = qmark;
	if(ast == -1) {
		out.push_back((argument));
		return out;
	}
	int slash = arg.find_last_of("/",ast);
	int slashAfter = arg.find("/",ast);
	string path("");
	char * cuD = get_current_dir_name();
	string currentwd(cuD);
	free(cuD);
	string lookup("");
	int end = slashAfter == -1?arg.size():slashAfter;
	lookup = arg.substr(slash+1,end - slash - 1);
	string tail = arg.substr(end,arg.size());
	if(slash != -1) {
		path = path + arg.substr(0,slash + 1);
	} else {
		path = path + currentwd;
	}

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if(match(lookup.c_str(),ent->d_name)) {
				if(slash != -1 && ent->d_name[0]!='.') out.push_back(path + ent->d_name + tail);
				else out.push_back(ent->d_name + tail);
			}
		}
		closedir (dir);
	} else {
		return out;
	}

	sort(out.begin(),out.end());

	for(int i = 0; i < out.size(); i++) {
		if((out[i][0]=='.') && lookup[0] != '.' && path[0] != '.') {
		} else	{
			retval.push_back(out[i]);
			//Command::_currentSimpleCommand->insertArgument(strdup(out[i].c_str()));
		}
	}
	return retval;
}
void expandAllWC(char * argument) {
	vector<string> arguments = expandSingleWC(argument);
	free(argument);
	int y = 0;
	for(int i = 0;i < arguments.size();i++) {
		if(arguments[i].find("*") == -1 && arguments[i].find("?") == -1){}
		else {
			expandAllWC(strdup(arguments[i].c_str()));
			y++;
		}
	}
	if(!y) {
		for(int i = 0;i < arguments.size();i++) {
			Command::_currentSimpleCommand->insertArgument(strdup(arguments[i].c_str()));
		}
	}
}


int match(char const *needle, char const *haystack) {
	for (; *needle!='\0'; ++needle) {
		switch (*needle) {
			case '?': ++haystack;   
				  break;
			case '*': {
					  size_t max = strlen(haystack);
					  if (needle[1] == '\0' || max == 0)
						  return 1;
					  for (size_t i = 0; i < max; i++)
						  if (match(needle+1, haystack + i))
							  return 1;
					  return 0;
				  }
			default:
				  if (*haystack != *needle)
					  return 0;
				  ++haystack;
		}       
	}
	return *haystack == '\0';
}

vector<string> putSubshell(char * argument) {
	//if(true) return 0;
	vector<string> out;
	if(argument[0] != '`' || argument[strlen(argument) - 1] != '`') {
		out.push_back(argument);
		free(argument);
		return out;
	}
	argument[strlen(argument)-1] = '\0';
	argument++;
	int fdpipeDown[2];
	int fdpipeUp[2];
	pipe(fdpipeDown);
	pipe(fdpipeUp);
	int defin = dup(0);
	int defout = dup(1);


	string data(argument);
	data = data + "\nexit\n";
	write(fdpipeDown[1],data.c_str(),strlen(data.c_str()));
	close(fdpipeDown[1]);
	int parent = fork();
	if(parent == -1) {
		perror("fork failed");
		exit(1);
	}
	if(!parent) {
		dup2(fdpipeDown[0],0);
		close(fdpipeDown[0]);
		dup2(fdpipeUp[1],1);
		close(fdpipeUp[1]);
		execlp("/proc/self/exe", "/proc/self/exe", NULL);	
		//execvp("./shell",NULL);
		perror("subshell messed up");
		close(fdpipeUp[0]);
		parent = -1;
		_exit(1);
	}   
	//wait(NULL);
	string subshellIO("");
	int n;
	char buf[1025];
	if ((n = read(fdpipeUp[0], buf, 1024)) >= 0 && parent != -1) {
		close(fdpipeUp[0]);
		buf[n] = 0;     /* terminate the string */
		subshellIO += buf;
		dup2(defin,0);
		dup2(defout,1);
		close(defin);
		close(defout);
		//for(int i = subshellIO.size(); i >=0; i--) unput(subshellIO[i]);
		//printf("%s\n",subshellIO.c_str());
		string individual("");
		for(int i = 0;i < subshellIO.size();i++) {
			if(subshellIO[i]!='\n') individual+=subshellIO[i];
			else if(individual != ""){
				out.push_back(individual);
				individual = "";
			}
		}
		return out;
	}
	perror("failed subshell");
	return out;


}



#if 0
main()
{
	yyparse();
}
#endif
