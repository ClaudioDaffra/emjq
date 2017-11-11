
#include "../lib/emjQuery.hpp"

// emcc -O3 12.proxy.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -std=c++11


//********
//	Main
//********

int main ( void )
{
	$.EMJQ() ;

	$.proxy ( _(objPerson) , _(test) , _(10) ) ; 						// call (arg) objPerson.test(10)
	
	$.proxy ( _(objPerson) , _(age)  , _(10) ) ; 						// set  	objPerson.age = 10 
	
	printf ( "<<%s>>\n",  $.proxy ( _(objPerson) , _(age)  ) ) ; 		// get  	objPerson.age

  return 0;
}





















