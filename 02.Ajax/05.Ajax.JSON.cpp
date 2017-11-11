
#include "../lib/emjQuery.hpp"

// emcc -O3 05.Ajax.JSON.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1  

 
	type::pointer fSuccess ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## fSuccess\n");
		
		type::stringc temp = NULL;

		temp = $(_data).pointer("[0]");
		printf ("# data :: %s \n" , temp );	 		

		type::json j = json::parse( temp ) ;

		std::cout << "\n..type.. << " << j["type"] << " >> \n" ;
		std::cout << "\n..value.. << " << j["value"] << " >> \n" ;	

		std::cout << "\n..items[0].value << "  << j["items"][0]["value"] << " >> \n" ;
		std::cout << "\n..items[0].action << " << j["items"][0]["action"] << " >> \n" ;		
		
	 return 0 ;
	}	
	
 

int main( void )
{
	$.EMJQ(); 

    $.getJSON 	( _('demo_json.txt') , (type::address) fSuccess ) ;

	
	return 0 ;
}
	
	
	
	
	
	
	
	
	
	
	
	
	