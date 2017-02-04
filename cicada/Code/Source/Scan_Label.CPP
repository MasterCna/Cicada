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
