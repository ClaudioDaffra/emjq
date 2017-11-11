
#include "../lib/emjQuery.hpp"
 

// emcc -O3 04.Ajax.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 


 
	type::pointer fSuccess1 ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## fSuccess1\n");

		type::stringc temp = NULL;

		$.dump(_data);
		
		temp = $(_data).pointer();
		printf ("# data :: %s \n" , temp );	 
	
	 return 0 ;
	}	

	type::pointer fSuccess2 ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## fSuccess2\n");

		type::stringc temp = NULL;

		temp = $(_data).pointer();
		printf ("# data :: %s \n" , _data );	 		
		
		temp = $(_data).pointer("[0].type");
		printf ("# data.type :: %s \n" , temp );	 

		temp = $(_data).pointer("[0].value");
		printf ("# data.type :: %s \n" , temp );	
		
	 return 0 ;
	}	
	
 

int main( void )
{
	$.EMJQ(); 

	type::pointer p = NULL ;
	
    $.get ( _('demo_test.txt') , (type::address) fSuccess1 ) ;

    $.getJSON 	( _('demo_json.txt') , (type::address) fSuccess2 ) ;


 
	return 0 ;
}
	
	
	
	
	
	
	
	
	
	
	
	
	