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
