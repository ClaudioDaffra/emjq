
#include "../lib/emjQuery.hpp"


// emcc -O3 01.miscellaneous.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 
 


int main( void )
{
	std::cout << "index::" << $( "#bar" ).index() << "\n";
	
	type::pointer p = $( "li" ).toArray() ;
	
	$.dump( p ) ;
	$.dump( p,0 ) ;
	$.dump( p,1 ) ;
	$.dump( p,2 ) ;
			
	type::stringc length = $(p).pointer("['length']") ;
	std::cout << "length::" << length << "\n";

	for (int i=0;i<atoi(length);i++)
		std::cout << $(p).pointer(i,"id") << "\n";
 
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	