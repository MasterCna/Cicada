void exec_goto()
  {
       char *loc ;
       get_token() ;
       loc = find_label(token) ;
       if(loc == '\0')
	  serror(7) ;
       else prog = loc ;
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
