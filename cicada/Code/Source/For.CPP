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
