#include <stdlib.h>
#include <conio.h>
#include <setjmp.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define NUM_LAB 100
#define LAB_LEN 10
#define FOR_NEST 25
#define SUB_NEST 25
#define PROG_SIZE 10000
#define DELIMITER 1
#define VARIABLE  2
#define NUMBER    3
#define COMMAND   4
#define STRING    5
#define QUOTE     6

#define PRINT     1
#define INPUT     2
#define IF        3
#define THEN      4
#define FOR       5
#define NEXT      6
#define TO        7
#define GOTO      8
#define EOL       9
#define FINISHED  10
#define GOSUB     11
#define RETURN    12
#define END       13
int load_program(char *p ,char  *fname);
char get_token();
void assignment();
int get_next_label(char *s);
int iswhite(char c);
int isdelim(char c);
char look_up(char *s);
int find_var(char *s);
char *gpop();
char *find_label(char *s);
struct for_stack fpop() ;
void print() , scan_labels() , find_eol(),exec_goto() ;
void exec_if() , exec_for(), next() ,fpush(struct for_stack i) ,input() ;
void gosub() , greturn() , gpush(char *s) , label_init() ;
void putback() , serror(int) , get_exp(int *) , primitive(int *result) ;
void level2(int *), level3(int *), level4(int *), level5(int *), level6(int *);
void arith(char o , int *r , int *h) , unary(char o , int *r) ;

char *prog ;
jmp_buf e_buf ;
int variable[26] = {0} ;
struct commands {
		   char commands[20] ;
		   char tok ;
		} table[] = {
		"print", PRINT, "input", INPUT, "if",IF,
		"then", THEN, "goto", GOTO, "for", FOR ,
		"next", NEXT , "to", TO , "gosub", GOSUB ,
		"return", RETURN , "end", END , "", END } ;
char token[80], token_type, tok ;
struct label {
	       char name[LAB_LEN] ;
	       char *p ;
	     } ;
struct label label_table[NUM_LAB] ;

struct for_stack {
		   int var ;
		   int target ;
		   char * loc ;
		 } fstack[FOR_NEST] ;
char *gstack[SUB_NEST] ;
int ftos, gtos ;

int main(int argc,  char *argv[])
 {
       char in[80] ;
       int answer ;
       char *p_buf , *t ;
       clrscr();
       if(argc != 2)
       {
	 printf("\n usage :  run < filename > ") ;
	 getch() ;
	 exit(1) ;
       }
       p_buf = (char *) malloc(PROG_SIZE) ;
       if(!p_buf)
       {
	 printf("\n allocation failure ") ;
	 getch() ;
	 exit(1) ;
       }
       if(!load_program(p_buf, argv[1]))
       {
	 printf("\n file not loaded ") ;
	 getch() ;
	 exit(1) ;
       }
       if(setjmp(e_buf))
	  exit(1) ;
       prog = p_buf ;
       scan_labels() ;
       ftos = 0 ;
       gtos = 0 ;
       do {
	   token_type = get_token() ;
	   if(token_type == VARIABLE) {
	       putback() ;
	       assignment() ;
	   }
	   else
	       switch(tok) {
		   case PRINT :
			 print() ;  break ;
		   case GOTO :
			 exec_goto() ; break ;
		   case IF :
			 exec_if() ;  break ;
		   case FOR :
			 exec_for() ; break ;
		   case NEXT :
			 next() ;   break ;
		   case INPUT :
			 input() ;  break ;
		   case GOSUB :
			 gosub() ;  break ;
		   case RETURN :
			 greturn() ; break ;
		   case END :
			 exit(0) ;
		}
       } while (tok != FINISHED) ;
       free(p_buf) ;
       return 0;
  }
//************
int load_program(char *p , char  *fname)
 {
       FILE *fp ;
       int i = 0 ;
       fp = fopen(fname, "rb") ;
       if(!fp)
	   return 0 ;
       i = 0 ;
       do {
	    *p = getc(fp) ;
	    p++ ; i++ ;
       } while(!feof(fp) && i < PROG_SIZE) ;
       *(p - 2) = '\0' ;
       fclose(fp) ;
       return 1 ;
 }
//****************
void assignment()
 {
      int var , value ;
      get_token() ;
      if(!isalpha(*token))  {
	  serror(4) ;
	  return ;
      }
      var = toupper(*token) - 'A';
      get_token() ;
      if(*token != '=') {
	  serror(3) ;
	  return ;
      }
      get_exp(&value) ;
      variable[var]=value ;
 }
//****************
void print()
     {
       int answer , len=0 , spaces ;
       char last_delim ;
       do {
	    get_token() ;
	    if(tok == EOL || tok == FINISHED)
	       break ;
	    if(token_type == QUOTE)  {
		 printf(token) ;
		 len += strlen(token) ;
		 get_token() ;
	    }
	    else {
		putback() ;
		get_exp(&answer) ;
		get_token() ;
		len += printf("%d",answer) ;
	    }
	    last_delim=*token ;
	    if(*token == ';') {
		 spaces = 8 - (len % 8) ;
		 len += spaces ;
		 while(spaces) {
		      printf(" ") ;
		      spaces -- ;
		 }
	    }
	    else if(*token == ',') ;//do nothing
		else if(tok != EOL && tok != FINISHED)
		   serror(0) ;
       } while(*token == ';' || *token == ',') ;
       if(tok == EOL || tok == FINISHED) {
	   if(last_delim != ';' && last_delim !=',')
		   printf("\n") ;
	}
	else serror(0) ;
  }
//*************************
void scan_labels()
 {
       int addr ;
       char *temp ;
       label_init() ;
       temp = prog ;
       get_token() ;
       if(token_type == NUMBER) {
	 strcpy(label_table[0].name, token) ;
	 label_table[0].p = prog ;
       }
       find_eol() ;
       do {
	   get_token() ;
	   if(token_type == NUMBER) {
	       addr = get_next_label(token) ;
	       if(addr == -1 || addr == -2) {
			(addr == -1) ? serror(5) : serror(6) ;
	       } //end of if
	       strcpy(label_table[addr].name , token) ;
	       label_table[addr].p = prog ;
	   }//end of if
	   if(tok !=EOL)
	       find_eol() ;
       } while(tok != FINISHED) ;
	 prog = temp ;
 }
//******************
void find_eol()
 {
     while(*prog != '\n' && *prog != '\0')
	  ++prog ;
       if(*prog)
	  prog ++ ;
  }
//******************
int get_next_label(char *s)
  {
       register int t ;
       for(t = 0 ; t < NUM_LAB ; t++) {
	 if(label_table[t].name[0] == 0)
	     return t ;
	 if(!strcmp(label_table[t].name,s))
	    return -2 ;
       } //end of for
       return -1 ;
  }
//*********************
char *find_label(char *s)
  {
       register int t ;
       for(t = 0 ; t < NUM_LAB ; t++)
	 if(!strcmp(label_table[t].name, s))
	     return label_table[t].p ;
	  return '\0' ;
  }
//******************
void exec_goto()
  {
       char *loc ;
       get_token() ;
       loc = find_label(token) ;
       if(loc == '\0')
	  serror(7) ;
       else prog = loc ;
  }
//*******************
void label_init()
 {
      register int t ;
      for(t = 0 ; t < NUM_LAB ; t++)
	 label_table[t].name[0] = '\0' ;
 }
//*********************
void exec_if()
  {
       int x , y , cond ;
       char op ;
       get_exp(&x) ;
       get_token() ;
       if(!strchr("=<>", *token))  {
	   serror(0) ;
	   return ;
       }
       op = *token ;
       get_exp(&y) ;
       cond = 0 ;
       switch(op) {
	 case '<' :
	    if(x < y) cond=1 ;
	    break ;
	 case '>' :
	    if(x > y) cond=1 ;
	    break ;
	 case '=' :
	    if(x == y) cond=1 ;
	    break ;
       }//end of switch
       if(cond)	 {
	   get_token() ;
	   if(tok != THEN)  {
	       serror(8) ;
	       return ;
	   }
       }//end of if
       else find_eol() ;
 }
//********************
void exec_for()
 {
       struct for_stack i ;
       int value ;
       get_token() ;
       if(!isalpha(*token)) {
	     serror(4) ;
	     return ;
       }
       i.var = toupper(*token) - 'A' ;
       get_token() ;
       if(*token != '=') {
		serror(3) ;
		return ;
       }
       get_exp(&value) ;
       variable[i.var] = value ;
       get_token() ;
       if(tok != TO) serror(9) ;
       get_exp(&i.target) ;
       if(value >= variable[i.var]) {
	      i.loc = prog ;
	      fpush(i) ;
       }
       else
	   while(tok!=NEXT) get_token() ;
 }
//***************************
void next()
  {
       struct for_stack i ;
       i = fpop() ;
       variable[i.var]++ ;
       if(variable[i.var] > i.target) return ;
       fpush(i);
       prog = i.loc ;
 }
//***********************
void fpush(struct for_stack i)
  {
       if(ftos > FOR_NEST)
	     serror(10) ;
       fstack[ftos] = i ;
       ftos++ ;
  }
//*********************
struct for_stack fpop()
 {
       ftos -- ;
       if(ftos < 0) serror(11);
       return(fstack[ftos]);
 }
//***********************
void input()
   {
       char str[80], var ;
       int i , ch , digit = 0 ;
       get_token() ;
       if(token_type == QUOTE) {
	   printf(token) ;
	   get_token() ;
	   if(*token != ',') {
	      serror(1) ;
	   }
	   get_token() ;
       }
       else printf("?") ;
       var = toupper(*token) - 'A' ;
       while(1)	 {
	       ch = getche() ;
	       if(ch <= 57 && ch >= 48)  {
		  ch = ch - 48 ;
		  digit = (digit * 10) + ch ;
	       }
	       else if(ch == 13)
		     break ;
	       else {
		   printf("\Redo from start ?\n") ;
		   continue ;
	       }
	 }//end of while
	 variable[var]=digit ;
  }
//**************
void gosub()
  {
       char *loc ;
       get_token() ;
       loc = find_label(token) ;
       if(loc == '\0')
	  serror(7) ;
       else {
	      gpush(prog) ;
	      prog = loc ;
       }//end of else
  }
//********************
void greturn()
     {
       prog = gpop() ;
     }
//**************
void gpush(char *s)
  {
       gtos++ ;
       if(gtos == SUB_NEST) {
	    serror(12) ;
	    return ;
       }
	gstack[gtos] = s ;
  }
//*******************
char *gpop()
  {
       if(gtos == 0) {
	   serror(13) ;
	   return 0 ;
       } //end of return
       return(gstack[gtos --]) ;
  }
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
char get_token()
 {
      register char *temp ;
      token_type = 0 ;
      tok = 0 ;
      temp = token ;
      if(*prog == '\0')  {
	 *token = 0 ;
	 tok = FINISHED ;
	 return(token_type = DELIMITER) ;
      }//end of if
      while(iswhite(*prog))
	   ++ prog ;
      if(*prog == '\r')  {
	  ++ prog ;
	  ++ prog ;
	  tok = EOL ;
	  *token = '\r' ;
	  token[1] = '\n' ;
	  token[2] = 0 ;
	  return(token_type = DELIMITER) ;
      }
      if(strchr("+-*^/%=;(),><" , *prog))  {
	  *temp=*prog ;
	  prog ++ ;
	  temp ++ ;
	  *temp = 0 ;
	  return(token_type = DELIMITER) ;
	}
	if(*prog == '"') {
	    prog ++ ;
	    while(*prog != '"' && *prog != '\r')
	       *temp ++ = *prog ++ ;
	    if(*prog == '\r')
		serror(1) ;
	    prog ++ ;
	    *temp = 0 ;
	    return(token_type = QUOTE) ;
	} //end of if
	if(isdigit(*prog)) {
	     while(!isdelim(*prog))
		*temp ++= *prog ++ ;
	     *temp='\0' ;
	     return(token_type=NUMBER) ;
	}
	if(isalpha(*prog)) {
	   while(!isdelim(*prog))
	      *temp ++= *prog ++ ;
	   token_type = STRING ;
	}
	*temp='\0' ;
	if(token_type == STRING) {
	   tok = look_up(token) ;
	   if(!tok)
	      token_type = VARIABLE ;
	   else
	      token_type = COMMAND ;
	 }
	 return token_type ;
 }
//******************
char look_up(char *s)
 {
       register int i , j ;
       char *p ;
       p = s ;
       while(*p) {
	  *p = tolower(*p) ;
	  p ++ ;
       }
       for( i= 0 ; *table[i].commands ; i++)
	  if(!strcmp(table[i].commands , s))
	      return table[i].tok ;
       return 0 ;
  }
//************
int isdelim(char c)
  {
    if(strchr(" ;,+-<>/*%^=()",c) || c == 9 ||
	  c=='\r' || c==0)
	  return 1 ;
     return 0 ;
  }
//**************
int iswhite(char c)
  {
       if(c == ' ' || c == '\t')
	  return 1 ;
       else
	  return 0 ;
  }
//***********************
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













