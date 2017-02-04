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
