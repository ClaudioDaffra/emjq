
#include "../lib/emjQuery.hpp"


// emcc -O3 04.queue.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1
 

	int f0 ( void)
	{
		printf ( "f0\n" );
 
		$(".diva").animate(	_({height: 300})	, _('slow')	);
		$(".diva").animate(	_({width: 300})		, _('slow')	);
		$(".diva").animate(	_({height: 100})	, _('slow')	);
		$(".diva").animate(	_({width: 100})		, _('slow')	);

		type::pointer p2 = $.pointerNew( _($("div").queue()	) ) ;		

        $("#s2").text( s($(p2).pointer("[0].length") ) );
		
		return 0 ;
	}

int main( void )
{

	$( "#start" ).click( (type::address) f0 ) ;
 
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	