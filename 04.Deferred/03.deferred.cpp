
#include "../lib/emjQuery.hpp"
 

// emcc -O3 03.deferred.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1  -s AGGRESSIVE_VARIABLE_ELIMINATION=1

//********
//	CALLBACK
//********
 
	type::pointer f1 ( void  ) 
	{
		printf ( "$.get succeeded\n");
	 return 0 ;
	}
 

	type::pointer f2 ( void  ) 
	{
		printf ( "$.get failed!\n");
	 return 0 ;
	}

	type::pointer fSuccess ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## fSuccess1\n");

		type::stringc temp = NULL;

		$.dump(_data);
		
		temp = $(_data).pointer();
		printf ("# data :: %s \n" , temp );	 


	 return 0 ;
	}
 
 
//######### 
//		MAIN
//######### 

int main( void )
{

	$.EMJQ(); 
	
    $.get ( _('demo_test.txt') , (type::address)fSuccess ).
		always( $.function ( (type::address)f1 , "" ) ).
			Catch( $.function ( (type::address)fSuccess , "data,textStatus,jqXHR" ) ).
				fail( $.function ( (type::address)f2 , "data,textStatus,jqXHR" ) ) ;
  
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	