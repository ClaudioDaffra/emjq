
#include "../lib/emjQuery.hpp"
 

// emcc -O3 02.attributeMulti.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1   -s AGGRESSIVE_VARIABLE_ELIMINATION=1
 

int main( void )
{
	$.EMJQ(); 
	
    type::pointer p = $(".xx").address() ;
	
	$.dump ( p ) ;
	$.dump ( p,0 ) ;
	$.dump ( p,2 ) ;	
	
	std::cout << " id 2 :: " << $(p).pointer("[2].id") << "\n" ;

	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	