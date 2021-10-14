%{
/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */

#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;
int len;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */


%}
%option noyywrap 

/*
 * Define names for regular expressions here.
 */

UPPER			[A-Z]
LOWER			[a-z]
CHR			[a-zA-Z0-9_]

DARROW          =>

CLASS           [Cc][Ll][Aa][Ss][Ss]
ELSE            [Ee][Ll][Ss][Ee]
FI              [Ff][Ii]
IF              [Ii][Ff]
IN              [Ii][Nn]
INHERITS        [Ii][Nn][Hh][Ee][Rr][Ii][Tt][Ss]
ISVOID          [Ii][Ss][Vv][Oo][Ii][Dd]
LET             [Ll][Ee][Tt]
LOOP            [Ll][Oo][Oo][Pp]
POOL            [Pp][Oo][Oo][Ll]
THEN            [Tt][Hh][Ee][Nn]
WHILE           [Ww][Hh][Ii][Ll][Ee]
CASE            [Cc][Aa][Ss][Ee]
ESAC            [Ee][Ss][Aa][Cc]
NEW             [Nn][Ee][Ww]
OF              [Oo][Ff]
NOT             [Nn][Oo][Tt]
FaLsE           f[Aa][Ll][Ss][Ee]
TrUe            t[Rr][Uu][Ee]


%s sCOMMENT
%s mCOMMENT
%s STR
%s STResp
%s STRbad

%%


 /*
  *  Nested comments
  */

<sCOMMENT><<EOF>>   {
						BEGIN(INITIAL);
						yylval.error_msg = "EOF in comment";
						return ERROR;
					}

<sCOMMENT>\n		{
						BEGIN(INITIAL);
						curr_lineno++;
					}

<INITIAL>"--"	{
					BEGIN(sCOMMENT);
				}


<mCOMMENT><<EOF>>	{
						BEGIN(INITIAL);
						yylval.error_msg = "EOF in comment";
						return ERROR;
					}

<mCOMMENT>"*)"	{
					BEGIN(INITIAL);
				}

<INITIAL>"*)"	{
					yylval.error_msg = "Unmatched *)";
					return ERROR;
				}

<INITIAL>"(*"	{
					BEGIN(mCOMMENT);
				}

<mCOMMENT>\n 	{curr_lineno++;}

<sCOMMENT,mCOMMENT>. {}

 


 /*
  *  The multiple-character operators.
  */
<INITIAL>{DARROW}		{ return (DARROW); }
<INITIAL><=				{ return LE;}
<INITIAL><-				{ return ASSIGN;}



 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */

<INITIAL>{CLASS}        { return (CLASS);}
<INITIAL>{ELSE}			{ return ELSE;}
<INITIAL>{FI}			{ return FI; }
<INITIAL>{IF}			{ return IF;}
<INITIAL>{IN}			{ return IN;}
<INITIAL>{INHERITS}		{ return INHERITS;}
<INITIAL>{ISVOID}		{ return ISVOID;}
<INITIAL>{LET}			{ return LET;}
<INITIAL>{LOOP}			{ return LOOP;}
<INITIAL>{POOL}			{ return POOL;}
<INITIAL>{THEN}			{ return THEN;}
<INITIAL>{WHILE}		{ return WHILE;}
<INITIAL>{CASE}			{ return CASE;}
<INITIAL>{ESAC}			{ return ESAC;}
<INITIAL>{NEW}			{ return NEW;}
<INITIAL>{OF}			{ return OF;}
<INITIAL>{NOT}			{ return NOT;}
<INITIAL>{FaLsE}		{ yylval.boolean = false;
        	    			return BOOL_CONST;}
<INITIAL>{TrUe}			{ yylval.boolean = true;
        	   		   		return BOOL_CONST;}


 /*
 	symbols
 */

<INITIAL>[0-9]+		{
						inttable.add_string(yytext);
						yylval.symbol = inttable.lookup_string(yytext);
						return INT_CONST;
					}
				
<INITIAL>{UPPER}{CHR}*	{
							idtable.add_string(yytext);
							yylval.symbol = idtable.lookup_string(yytext);
							return TYPEID;
						}
							
<INITIAL>{LOWER}{CHR}*	{
							idtable.add_string(yytext);
							yylval.symbol = idtable.lookup_string(yytext);
							return OBJECTID;
						}

 /* .@~*+/-<={};=():, */

<INITIAL>"."|"@"|"~"|"*"|"/"|"+"|"-"|"<"|"="|"{"|"}"|";"|"("|")"|":"|"," {
						return yytext[0];
					}


 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */

 /* !!!!!!!!!!!!!!!!!!!!string NULL character  */

<STR,STResp>\0		{
						BEGIN(STRbad);
						yylval.error_msg = "String contains null character";
						return ERROR;
					}


<STR,STResp><<EOF>> {
						BEGIN(INITIAL);
						yylval.error_msg = "EOF in string constant";
						return ERROR;
					}

<STRbad>\n	{
				curr_lineno++;
				BEGIN(INITIAL);
			}
			
<STRbad>"\"" {BEGIN(INITIAL);}

<STRbad><<EOF>> {BEGIN(INITIAL);}

<STRbad>. {}


<STResp>n	{
					*string_buf_ptr = '\n';
					string_buf_ptr++;
					len++;
					if ( len == MAX_STR_CONST ) {
						BEGIN(STRbad);
						yylval.error_msg = "String constant too long";
						return ERROR;
					}
					BEGIN(STR);
				}

<STResp>\n	{
					curr_lineno++;
					*string_buf_ptr = '\n';
					string_buf_ptr++;
					len++;
					if ( len == MAX_STR_CONST ) {
						BEGIN(STRbad);
						yylval.error_msg = "String constant too long";
						return ERROR;
					}
					BEGIN(STR);
				}

<STResp>b	{
				*string_buf_ptr = '\b';
				string_buf_ptr++;
				len++;
				if ( len == MAX_STR_CONST ) {
					BEGIN(STRbad);
					yylval.error_msg = "String constant too long";
					return ERROR;
				}
				BEGIN(STR);
			}

<STResp>t	{
				*string_buf_ptr = '\t';
				string_buf_ptr++;
				len++;
				if ( len == MAX_STR_CONST ) {
					BEGIN(STRbad);
					yylval.error_msg = "String constant too long";
					return ERROR;
				}
				BEGIN(STR);
			}

<STResp>f	{
				*string_buf_ptr = '\f';
				string_buf_ptr++;
				len++;
				if ( len == MAX_STR_CONST ) {
					BEGIN(STRbad);
					yylval.error_msg = "String constant too long";
					return ERROR;
				}
				BEGIN(STR);
			}
				
<STResp>.	{
				*string_buf_ptr = yytext[0];
				string_buf_ptr++;
				len++;
				if ( len == MAX_STR_CONST ) {
					BEGIN(STRbad);
					yylval.error_msg = "String constant too long";
					return ERROR;
				}
				BEGIN(STR);
			}


<STR>"\\"		{
					BEGIN(STResp);
				}

<STR>\n			{
					curr_lineno++;
					BEGIN(INITIAL);
					yylval.error_msg = "Unterminated string constant";
					return ERROR;
				}

<STR>"\""		{
					BEGIN(INITIAL);
					*string_buf_ptr = '\0';
					stringtable.add_string(string_buf);
					yylval.symbol = stringtable.add_string(string_buf);
					return STR_CONST;
				}

<STR>.			{
					*string_buf_ptr = yytext[0];
					string_buf_ptr++;
					len++;
					if ( len == MAX_STR_CONST ) {
						BEGIN(STRbad);
						yylval.error_msg = "String constant too long";
						return ERROR;
					}
				}

<INITIAL>"\""	{
					string_buf_ptr = string_buf;
					len = 0;
					BEGIN(STR);
				}


<INITIAL>[ \t\f\r\v]+			{;}
<INITIAL>\n				{curr_lineno++;}

<INITIAL>.		{yylval.error_msg = yytext;
				return ERROR;}

%%
