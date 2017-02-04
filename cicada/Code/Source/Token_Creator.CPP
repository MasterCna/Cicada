#define DELIMITER 1
#define VARIABLE  2
#define NUMBER    3
#define COMMAND   4
#define STRING    5
#define QUOTE     6
#include <stdlib.h>
#include <conio.h>
#include <setjmp.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

extern jmp_buf e_buf;
extern int variable[26];
extern struct commands {
       char command[20];
       char tok;
} table[];
extern char token_type;
extern char tok;
void putback() , serror(int) , get_exp(int *) , primitive(int *result) ;
void level2(int *), level3(int *), level4(int *), level5(int *), level6(int *);
void arith(char o , int *r , int *h) , unary(char o , int *r) ;

void get_exp(int *result)
 {
       get_token() ;
       if(!*token)  {
	     serror(2) ;
	     return ;
       }
       level2(result) ;
       putback() ;
 }
//**********************
void level2(int *result)
  {
      register char op ;
      int hold ;
      level3(result) ;
      while((op = *token) == '+' || op == '-')  {
	  get_token() ;
	  level3(&hold) ;
	  arith(op , result , &hold) ;
      }//end of while
  }
//***************************
void level3(int * result)
 {
       register char op ;
       int hold ;
       level4(result) ;
       while((op = *token) == '*' || op == '/' || op == '%')
       {
	 get_token() ;
	 level4(&hold) ;
	 arith(op , result , &hold) ;
       } //end of while
 }
//******************
void level4(int * result)
 {
      int hold ;
      level5(result) ;
      if(*token == '^') {
	   get_token() ;
	   level4(&hold) ;
	   arith('^' , result , &hold) ;
      }
 }
//********************
void level5(int *result)
  {
       register char op ;
       op = 0 ;
       if((token_type == DELIMITER) && *token == '+' || *token == '-')
       {
	 op = *token ;
	 get_token() ;
       }
       level6(result) ;
       if(op)
	 unary(op , result) ;
 }
//*******************************
void level6(int *result)
 {
    if((*token == '(') &&(token_type == DELIMITER))
    {
	 get_token() ;
	 level2(result) ;
	 if(*token != ')')
	    serror(1) ;
	 get_token() ;
    }
    else
	 primitive(result) ;
 }
//***********************
void primitive(int *result)
 {
    switch(token_type) {
	 case VARIABLE :
	      *result = find_var(token) ;
	      get_token() ;
	      return ;
	 case NUMBER :
	      *result = atoi(token) ;
	      get_token() ;
	      return ;
	 default :
	      serror(0) ;
    }//end of switch
 }
//*********************
void arith(char o , int *r , int *h)
 {
     register int t , ex ;
     switch(o)  {
	 case '-' :
	     *r = *r - *h ;
	     break ;
	 case '+' :
	     *r=*r + *h ;
	     break ;
	 case '*' :
	     *r=*r * *h ;
	     break ;
	 case '/' :
	     *r=(*r) / (*h) ;
	     break ;
	 case '%' :
	     t = (*r) / (*h) ;
	     *r = *r - (t * (*h)) ;
	     break ;
	 case '^' :
	     ex = *r ;
	     if(*h == 0) {
		*r = 1 ;
		break ;
	     }
	     for(t = *h - 1 ; t > 0 ; -- t)
		*r=(*r) * ex ;
	     break ;
     }//end of switch
 }
//*************************
void unary(char o , int *r)
 {
       if(o == '-')
	  *r = -(*r) ;
 }
//***************
int find_var(char *s)
  {
      if(!isalpha(*s))   {
	   serror(4) ;
	   return 0 ;
      }
      return variable[toupper(*token)-'A'] ;
 }
//**********************
//*****************
void putback()
  {
       char *t ;
       t = token ;
       for(; *t ; t++)
	  prog -- ;
 }
//*****************
void serror(int error)
     {
       static char *e[] = {
	    "syntax error ",
	    "un balanced parantheses",
	    "no expressoin present" ,
	    "equals sign expexted" ,
	    "not a variable" ,
	    "label table full" ,
	    "dulicate label" ,
	    "undifined label" ,
	    "THEN expected" ,
	    "TO expected" ,
	    "too many nested FOR loops" ,
	    "NEXT without FOR" ,
	    "too many nested GOSUB",
	    "RETURN without GOSUB" } ;
	printf("\n %s \n", e[error]) ;
	longjmp(e_buf , 1) ;
  }
//*****************

