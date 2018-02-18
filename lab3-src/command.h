
#ifndef command_h
#define command_h

// Command Data Structure
struct SimpleCommand {
	// Available space for arguments currently preallocated
	int _numOfAvailableArguments;

	// Number of arguments
	int _numOfArguments;
	char ** _arguments;
	
	SimpleCommand();
	void insertArgument( char * argument );
};

struct Command {
	
	
	int _numOfAvailableSimpleCommands;
	int _numOfSimpleCommands;
	SimpleCommand ** _simpleCommands;
	char * _outFile;
	char * _inFile;
	char * _errFile;
	int _background;

	void prompt();
	void print();
	void execute();
	void clear();
	
	int _append;

	//int defin = dup(0);
//	int defout = dup(1);
//	int deferr = dup(2);
	
	char* findHome();
	int lastIteration(int *fdin, int *fdout, int *fderr, int i); 
	int checkPrintEnv(int index);
	int checkUnsetEnv(int index);
	int checkSetEnv(int index);
	int checkBuiltin(int index);	
	Command();
	void insertSimpleCommand( SimpleCommand * simpleCommand );

	static Command _currentCommand;
	static SimpleCommand *_currentSimpleCommand;

};

#endif
