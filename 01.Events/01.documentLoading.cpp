
#include "../lib/emjQuery.hpp"

// emcc -O3 01.documentLoading.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -std=c++11


int  funzione ( type::pointer _event,type::address _data,type::pointer _this )
{
	printf ("<<<%u %p %u>>>\n",_event,_data,_this);

	type::stringc  temp1 ;

	printf ( "# _this::%d \n",_this);

	printf ( "# data:: %s\n",(type::stringc)_data);

	return 0 ;
}


//********
//	Main
//********

int main ( void )
{
	$.EMJQ() ;
	
 	$.holdReady(true) ;

	$.holdReady(false) ; 

	$( document ).ready( (type::address)funzione );
	
	// $( window ).unload ( (type::address)funzione );  // REMOVED !!!

 
  return 0;
}
























