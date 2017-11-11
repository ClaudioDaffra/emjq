
#include "../lib/emjQuery.hpp"


// emcc -O3 04.replace.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 
 


int main( void )
{
 
 	$( "<h2>New heading</h2>" ).replaceAll( _('.inner') );

	$( "div.second" ).replaceWith( _('<h2>Replace With New heading</h2>') );

	$( ".removeME" ).remove();
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	