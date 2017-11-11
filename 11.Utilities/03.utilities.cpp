
#include "../lib/emjQuery.hpp"


// emcc -O3 03.utilities.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 
// emcc -O1 03.utilities.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 

 
	int f0 ( type::pointer _element, type::pointer _index )
	{
		$.dump( _element ) ;
		$.dump( _index ); 
		
		return 0 ;
	}
 
int main( void )
{
	$.EMJQ() ;

    std::string trimMe = " \n Claudio \t Daffra " ;
	
	std::cout << "before::"<< trimMe << "\n" << "after::" << $.trim ( trimMe ) << "\n" ; 	


	// ......................................................... uniqueSort
	
	type::pointer divs 	= $( "div" ).address() ;
	type::pointer u 	= $.uniqueSort ( divs ) ;
	
	$.dump ( divs ) ;
	$.dump ( u ) ;	
	
	std::cout << "<<<" << $(u).pointer("['length']") << ">>> \n" ; 
	
	// ......................................................... makeArray
	
	type::pointer arr = $.makeArray( divs );
	$.dump ( arr ) ;	

	// ......................................................... merge
	
	type::pointer divs_arr  = $.merge( divs , arr  );
	$.dump ( divs_arr ) ;	
 
	// ......................................................... each

	$.each ( divs_arr , (type::address)f0 ) ;

	// ......................................................... grep
	
	type::pointer araw = $.pointerNew ( "[ 1, 9, 3, 8, 6, 1, 5, 9, 4, 7, 3, 8, 6, 9, 1 ]" ) ;
	type::pointer g = $.grep ( araw , _(	function( element, index ) {return ( element !== 5 && index > 4 );} ) ) ;
	$.dump ( g ) ;

	// ......................................................... map
	type::pointer araw2 = $.pointerNew ( _([ 'a', 'b', 'c', 'd', 'e' ]) );
	type::pointer g2 = $.map ( araw2 , _(	function( element, index ) {return ( element.toUpperCase() + index );} ) ) ;
	$.dump ( g2 ) ;	
	
	//
	
	std::cout << "\n" ; // maledetto carattere newLine finale !	
 
	return 0 ;
	 
}
	

	
	
	 
	
	
	
	
	