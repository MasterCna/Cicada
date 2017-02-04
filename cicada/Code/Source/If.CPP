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
