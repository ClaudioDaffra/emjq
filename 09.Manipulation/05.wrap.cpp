
#include "../lib/emjQuery.hpp"


// emcc -O3 05.wrap.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 
 


int main( void )
{
	$( ".inner" ).wrap( _('<div class="new"></div>') 	);

	$( ".innerx" ).wrapAll( _('<div class="new" />')	);
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	