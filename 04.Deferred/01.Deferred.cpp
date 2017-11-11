
#include "../lib/emjQuery.hpp"
 
// emcc -O3 01.Deferred.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1
 


//********
//	CALLBACK
//********

 
	type::pointer f1 ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## f1\n");

	 return 0 ;
	}
 
	type::pointer f2 ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## f2\n");

	 return 0 ;
	}
 
	type::pointer f3 ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## f3\n");

	 return 0 ;
	}
 
//********
//	BODY
//********

int main( void )
{
	$.EMJQ();
	
	// __________________________________________________________________________________________
	 
 	type::pointer pf1 = $.var( "f1" , callback ( (type::address) f1 , _( _data , _testStatus , _jqXHR) ).c_str() ) ;	
 	type::pointer pf2 = $.var( "f2" , callback ( (type::address) f2 , _( _data , _testStatus , _jqXHR) ).c_str() ) ;
 	type::pointer pf3 = $.var( "f3" , callback ( (type::address) f3 , _( _data , _testStatus , _jqXHR) ).c_str() ) ;
 
	$.dump ( pf1 );
	//$.dump ( pf2 );	
 	//$.dump ( pf3 );
 	$.dump ( "window.f1" );
 	//$.dump ( window.f2 );
 	//$.dump ( window.f3 );
	// __________________________________________________________________________________________
	
	
    type::pointer pd1 = $.var( "d1" , $.Deferred() ) ;
	
	$.dump ( pd1 );
	$.dump ( "window.d1" );	

	// __________________________________________________________________________________________
	
	printf ( "\n#1\n" );	
	
		$.when( _(d1) ).done ( 
			_([ f1, f2 ], f3, [ f2, f1 ])
		) ;

	$(pd1).resolve() ;	
 
	// __________________________________________________________________________________________
	
	printf ( "\n#2\n" );
	
		$.when( pd1 ).done ( _([ f1, f2 ], f3, [ f2, f1 ])) ;
		$(pd1).resolve() ;	

	printf ( "\n#3\n" );
	
		$.when( pd1 ).done ( _([ f1, f2 ], f3, [ f2, f1 ])).resolve().notify() ;	
 
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	