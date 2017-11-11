
#include "../lib/emjQuery.hpp"
#include "../lib/json.hpp" 

// emcc -O3 07.Ajax.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1   

 
	type::pointer fSuccess ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## fSuccess\n");
		
	 return 0 ;
	}	
 

//#######
//			MAIN
//#######
 
int main( void )
{
	$.EMJQ(); 

	type::json obj = 
	{
	  {"pi", 3.141},
	  {"happy", true},
	  {"name", "Niels"},
	  {"nothing", nullptr},
	  {"answer", {
		{"everything", 42}
	  }},
	  {"list", {1, 0, 2}},
	  {"object", {
		{"currency", "USD"},
		{"value", 42.99}
	  }}
	};

	type::stringc shallowEncoded = $.param( obj.dump().c_str(),false  );
 
	printf ( "shallowEncoded <<<%s>>>\n",shallowEncoded );
/*
	type::stringc shallowDecoded = $.decodeURIComponent( shallowEncoded ); // ?? ERROR
 
	printf ( "shallowDecoded <<<%s>>>\n",shallowDecoded );
*/
	type::stringc ser = NULL;
	
	ser =  $( "form" ).serialize();
	
	printf ( "serialize <<<%s>>>\n",ser );

	type::pointer serArr =  $( "form" ).serializeArray();
	
	$.dump(serArr);
	
	return 0 ;
}
	
	
	
	
	
	
	
	
	
	
	
	
	