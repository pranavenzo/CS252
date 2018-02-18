/*
 * CS-252 Fall 2017
 * fiz.l: lexical analyzer for FIZ interpreter
 *
 */

%{
/********************************************************************************
 * Beginning of Section 1: C definitions to be copied to lex.yy.c               *
 ********************************************************************************/ 

#include <string.h>
#include "y.tab.h"

extern int loading;
extern int tracing;

/********************************************************************************
 * Below is Section 2: Regular expressions and associated code                  *
 ********************************************************************************/ 
%}

/********************************************************************************
 * Beginning LEX code to support loading a FLIZ program file using import       *
 ********************************************************************************/
%x incl
%%
"import"      BEGIN(incl);	/* dealing with import */

<incl>[ \t]*      /* eat the whitespace */
<incl>[^ \t\n]+   { /* got the include file name */
    yyin = fopen( yytext, "r" );
    
    if ( ! yyin ) {
        error("Unable to open file %s.\n", yytext);
    } 
    loading = 1; 	/* Disable printing prompts while reading contents from the file. */
    yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));  /* Put current buffer on stack, and use the file */
    BEGIN(INITIAL);
}
     
<<EOF>> {
    yypop_buffer_state();
    loading = 0;    /* Enable printing prompts again.  This assumes that we only use import for one level. */
    if ( !YY_CURRENT_BUFFER ) {
        yyterminate();
    }
}  /* End of code supporting import. */

"help" {
    printf ("You can use the following commands:\n");
    printf ("  import <file_name>\n");
    printf ("  tracing on\n");
    printf ("  tracing off\n");
    printf ("  (define (<func_name> <<arg_list>>) <<expr>>)\n");
    printf ("  <<expr>>\n");
    printf ("The grammar for <<expr>> is:\n");
    printf ("  <<expr>> ::= (inc <<expr>>)\n");
    printf ("            |  (dec <<expr>>)\n");
    printf ("            |  (ifz <<expr>> <<expr>> <<expr>>)\n");
    printf ("            |  (halt)\n");
    printf ("            |  (<func_name> <<expr_list>>)\n");
}

"tracing on" {
    tracing = 1;   /* In tracing mode, execution of the program show function calls */
}

"tracing off" {
    tracing = 0;
}

"halt" {
    return HALT;
}

"define" {
    return DEFINE;
}

"(" {
    return OPENPAR;
}

")" {
    return CLOSEPAR;
}

;.*$  {
    /* Discard comment */
}

[0-9]+ {
    yylval.number_val = atoi(yytext);
    return NUMBER;
}

[a-zA-Z][a-zA-Z0-9]* {
    yylval.string_val = strdup(yytext);
    return ID;
}


[ \t] {
    /* Discard spaces and tabs */
}

\n {
    prompt();
}

