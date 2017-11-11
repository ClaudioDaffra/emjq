
#include "../lib/emjQuery.hpp"

// emcc -O3 02.garbage.cpp -o main.js -s NO_EXIT_RUNTIME=1 --std=c++11

//********
//	Main
//********
 
int main() 
{

    type::pointer p0,p1,p2,p3 ;

	p0 = $("#hello").address()  ;
	p1 = $("#world").address()   ;

	p2 = $.pointerNew ( OBJ(123) ) ;  
	p3 = $.pointerNew ( STR(123) ) ;  
	
	$.dump ( p2 ) ;
	$.dump ( p3 ) ;
	
	type::stringc s0,s1 ;
	
 	s0 = $("#hello").pointer("[0].id")  ;
	s1 = $("#world").pointer("[0].id")  ;

	std::cout << s0 << " :: " << s1 << "\n";
	
	eval( "console.log( m )" )  ;
	
	std::cout << "\n ";
	
	$.EMJQ(end) ;
	
 return 0 ;
}
























