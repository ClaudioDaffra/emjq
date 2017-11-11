
#include "../lib/emjQuery.hpp"


// emcc -O3 06.insert.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 
 


int main( void )
{
	$( "<p>XYZ</p>" ).insertAfter( _('.inner') ).insertBefore( _('.inner') );	
 
 
    type::pointer p = $.pointerNew ( "jQuery.cssNumber" )  ;
	
	std::cout << $(p).pointer("[0].zIndex") << "\n";
	std::cout << $(p).pointer("[0].fontWeight") << "\n";
	std::cout << $(p).pointer("[0].opacity") << "\n";
	std::cout << $(p).pointer("[0].zoom") << "\n";	
	std::cout << $(p).pointer("[0].lineHeight") << "\n";	
	std::cout << $(p).pointer("[0].widows") << "\n";	
	std::cout << $(p).pointer("[0].orphans") << "\n";	
	std::cout << $(p).pointer("[0].fillOpacity") << "\n";		
	std::cout << $(p).pointer("[0].columnCount") << "\n";	
	std::cout << $(p).pointer("[0].order") << "\n";	
	std::cout << $(p).pointer("[0].flexGrow") << "\n";	
	std::cout << $(p).pointer("[0].flexShrink") << "\n";		
 
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	