
#include "../lib/emjQuery.hpp"
 

// emcc -O3 02.when.done.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1   -s AGGRESSIVE_VARIABLE_ELIMINATION=1
   
	type::pointer f1 ( type::pointer _v1 , type::pointer _v2, type::pointer _v3  ) 
	{
		printf ( "## f1 ## %d %d %d\n",_v1,_v2,_v3);

		$.dump ( _v1 ) ; // v1 is undefined
		$.dump ( _v2 ) ; // v2 is "abc"
		$.dump ( _v3 ) ; // v3 is an array [ 1, 2, 3, 4, 5 ]

	 return 0 ;
	}
 

int main( void )
{
	$.EMJQ();

  	type::pointer pd1 = $.var( "d1" , $.Deferred() ) ;		
 	type::pointer pd2 = $.var( "d2" , $.Deferred() ) ;
 	type::pointer pd3 = $.var( "d3" , $.Deferred() ) ;

    $.when( _(d1,d2,d3) ).done ( 
		$.function ( (type::address) f1 , _ ( v1,v2,v3 ) )
	) ;
	
	$(pd1).resolve() ;	
	$(pd2).resolve( _('abc') ) ;	
	$(pd3).resolve( _(1,2,3,4,5) ) ;	

	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	