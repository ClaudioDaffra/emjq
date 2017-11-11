
#include "../lib/emjQuery.hpp"


// emcc -O3 01.utilities.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 


 

int main( void )
{
	$.EMJQ() ;
	
    type::pointer _target = $.pointerNew("[]") ; // define ARRAY
 	$.dump ( _target ) ;

	type::json jo1 = {
		{ "a",1 },
		{ "b",2 },
		{ "c",3 }		
	} ;
 	type::json jo2 = {
		{ "x",10 },
		{ "y",20 },
		{ "z",30 }		
	} ;
 
    type::pointer _obj1 = $.pointerNew( _JSON( jo1 ) );  
    type::pointer _obj2 = $.pointerNew( _JSON( jo2 ) ); 

	$.dump ( _obj1 ) ;
	$.dump ( _obj2 ) ;

//.................................
	
	$(_target).extend ( _obj1 , _obj2 ) ;
 		
	$.dump ( _target ) ;
 
/*
   TODO : array.toString()

 	std::cout << "from JSON.pointer::" << "\n" << $(_target).arrayGet() << "\n" ;
	
	auto jNew = JSON.parse ( $(_target).pointer()  ) ;
	
	std::cout << "from JSON.parse::"<< "\n" << JSON.stringify ( jNew ) << "\n" ;
*/	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	