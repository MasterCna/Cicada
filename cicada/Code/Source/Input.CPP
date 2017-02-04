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
