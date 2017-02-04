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
