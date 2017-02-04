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
