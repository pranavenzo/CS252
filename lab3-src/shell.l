
/*
 *
 * CS-252 Fall 2013
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */

%{
#include <unistd.h>
#include "command.h"
#include <pwd.h>
#include <vector>
#include <stdio.h>
#include "y.tab.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <string>

extern "C" char * read_line();
int mygetc(FILE * f) {
	static char *p;
	char ch;
	if (!isatty(0)) {
  	// stdin is not a tty. Call real getc
    		return getc(f);
    	}
    // stdin is a tty. Call our read_line.
    	if (p==NULL || *p == 0) {
      		char * s = read_line();
        	p = s;
	}
	ch = *p;
	p++;
	return ch;
}
#undef getc
#define getc(f) mygetc(f)




using namespace std;
static  void yyunput (int c,char *buf_ptr  );
extern int source;
void myunputc(int c) {
    unput(c);
}
int isSpecialChar(char c) {
	return (c=='|'||c=='&'||c=='>'||c=='<');
}
int isSpecial(const char * arg) {
	int y,z,w;
	int x = y = z = 0;
	if(strlen(arg) > 0)
		x = arg[0] == '>';
	if(strlen(arg) > 1) {
		y = arg[1] == '>';
		w = arg[1] == '&';
	}
	if(strlen(arg) > 2) z = arg[2] == '&';
	if(x && y && z)return 3;
	if(x && y || x && w) return 2;
	return 0;
}

int unputSpecialChars(char * arg) {
	string s(arg);
	int spec = 0;
	for(int i = 0;i < s.size();i++) {
		if(isSpecialChar(s[i])) {
			spec++;
			break;
		}
	}
	if(!spec) return 1;
	string result("");
	for(int i = 0; i < s.size(); i++) {
		if(i > 0 && isSpecialChar(s[i]) && s[i-1] == '\\') spec--;
		if(i > 0 && isSpecialChar(s[i]) && s[i-1] != '\\') {
			int l = isSpecial(s.substr(i).c_str());
			if(!l) result = result + " " + s[i] + " ";
			else if(l == 2) result = result + " " + s[i] + s[(i++) + 1] + " ";
			else result = result + " " + s[i] + s[i+1] + s[(i++) + 2] + " ";
		}
		else result += s[i];
	}
	if(!spec) return 1;
	for(int i = result.size(); i >=0; i--) unput(result[i]);
	return 0;
}

%}
%x incl
%%

"source"      BEGIN(incl);      /* dealing with import */

<incl>[ \t]*      /* eat the whitespace */
<incl>[^ \t\n]+   { /* got the include file name */
    yyin = fopen( yytext, "r" );
        
    if ( ! yyin ) { 
            perror("Unable to open file");
    }   
	    source = 1;        /* Disable printing prompts while reading contents from the file. */
	    yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));  /* Put current buffer on stack, and use the file */
	    BEGIN(INITIAL);
}
		        
<<EOF>> {
	    yypop_buffer_state();
	    source = 0;    /* Enable printing prompts again.  This assumes that we only use import for one level. */
	    if ( !YY_CURRENT_BUFFER ) { 
		    yyterminate();
	    }   
}  /* End of code supporting import. */








\n 	{
		return NEWLINE;
	}

[\t] 	{
		return TAB;
     	}
[ ]	{
		/* Discard spaces */
	}

\\>|\\<|\\\||\\& {
		yytext+=1;
		yylval.string_val = strdup(yytext);
		return WORD;
}
">" 	{
		return GREAT;
	}
"<"	{
		return LESS;
	}

">>&"   {
		return DGREATAMP;
	}
">&"	{	
		return GREATAMP;
	}
	
">>"	{
		return DGREAT;
	}
"|" 	{
		return PIPE;
	}
"&"	{
		return AMP;
	}


\`[^\`]+\` {
	yylval.string_val = strdup(yytext);	
	return WORD;
}


\"[^\"]+\" {
	char * str = (char*) malloc(strlen(yytext) - 2);
	int i;
	for(i = 0;i < strlen(yytext) - 2;i++) str[i] = yytext[i+1];
	str[i] = '\0';

	yylval.string_val = str;	
	return WORD;

}
[^ \t\n][^ \t\n]*  {
		char *  arg2 = (char *)malloc(sizeof(char)*strlen(yytext) + 1);
//		printf("%d\n",(int)strlen(yytext));
		char * arg = yytext;
		if(unputSpecialChars(arg)) {
			int nD = 0;
			int i;
			int k = 0;
			int bs = 1;
			for(i = 0;i < strlen(yytext);i++) {
				if(bs && arg[i] == '\\') {
					bs = 0;
					nD++;	 
				} else {
					bs = 1;
					arg2[k++] = arg[i];	
				} 
			}
			arg2[k] = '\0';
			//check leaks
			if(nD) {
				string s("");
				for(int i = 0;i < k;i++) s+=arg2[i];
				free(arg2);
				arg2 = strdup(s.c_str());
			} else {
				free(arg2);
				arg2 = strdup(yytext);
			}
			yylval.string_val = arg2;
			return WORD;
		}
		
	}

