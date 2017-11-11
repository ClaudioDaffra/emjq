
#include "../lib/emjQuery.hpp"


// emcc -O3 03.prependTo.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 
 
 
	int fEvent ( type::pointer _event,type::address _data,type::pointer _this )
	{
 
		return 0 ;
	}	
 
 

int main( void )
{
	$( "<p>Test</p>" ).prependTo( _('.inner') );
 	
	type::pointer p =  $( "h2" ).address() ;
	$( ".container2" ).after( p );
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	