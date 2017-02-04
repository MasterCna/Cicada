struct for_stack {
		   int var ;
		   int target ;
		   char * loc ;
		 } fstack[FOR_NEST] ;
int ftos;
void fpush(struct for_stack i)
  {
       if(ftos > FOR_NEST)
	     serror(10) ;
       fstack[ftos] = i ;
       ftos++ ;
  }
//*********************
struct for_stack fpop()
 {
       ftos -- ;
       if(ftos < 0) serror(11);
       return(fstack[ftos]);
 }
