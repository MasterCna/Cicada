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
