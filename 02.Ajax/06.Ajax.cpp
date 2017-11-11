
#include "../lib/emjQuery.hpp"


// emcc -O3 06.Ajax.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1   

 
	type::pointer fSuccess ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## fSuccess\n");
	
		printf ( "## jqXHR.statusText=%s\n" , $( _jqXHR ).pointer("[0].statusText") );
		
	 return 0 ;
	}	
	type::pointer fComplete ( type::pointer _responseText , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## fSuccess\n");
	
		printf ( "## jqXHR.statusText=%s\n" , $( _jqXHR ).pointer("[0].statusText") );
		
	 return 0 ;
	}	 

//#######
//			MAIN
//#######

int main( void )
{
	$.EMJQ(); 

    type::pointer p=$.getScript	( _('demo_test.js') , (type::address) fSuccess ) ;
	
	type::stringc s ="ciao";
 
    $("#target").load 	( _("demo_test.txt") , (type::address) fComplete) ;
	
	return 0 ;
}
	
	
	
	
	
	
	
	
	
	
	
	
	