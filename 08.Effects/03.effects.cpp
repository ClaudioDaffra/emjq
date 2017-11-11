
#include "../lib/emjQuery.hpp"


// emcc -O3 03.effects.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 
 
 
	int f0 ( void)
	{
		printf ( "f0\n" );
 
		$("#div1").delay(_('slow')).fadeIn();
		$("#div2").delay(_('fast')).fadeIn();
		$("#div3").delay(800).fadeIn();
		$("#div4").delay(2000).fadeIn();
		$("#div5").delay(4000).fadeIn().stop(true,true);
		
		return 0 ;
	}
	int f1 ( void)
	{
		printf ( "f1\n" );
		
 		$("p").fadeTo(_(1000), 0.4 );
 
		
		return 0 ;
	}
 

int main( void )
{

	$( "#f0" ).click( (type::address) f0 ) ;
	$( "#f1" ).click( (type::address) f1 ) ;
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	