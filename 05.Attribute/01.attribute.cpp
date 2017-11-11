
#include "../lib/emjQuery.hpp"
 

// emcc -O3 01.attribute.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1   -s AGGRESSIVE_VARIABLE_ELIMINATION=1
 
 
	type::pointer f0 ( type::address _event , type::address _data , type::address _this )
	{
		printf ( "f0\n" );
 
	   
 		//temp =  temp + ".attr( \"checked\" ): <b>" + std::string( $("input").attr( s("checked") ) ) + "</b><br>"  ; 
		//temp =  temp + ".prop( \"checked\" ): <b>" + std::string( $("input").prop( s("checked") ) ) + "</b><br>"  ;
		//
		// does not work, because $("input") return more elements, use pointer instead [0],[1] ... 
 
		std::string temp = "" ;
		temp =  temp + ".attr( \"checked\" ): <b>" + std::string( $("#check1").attr( s("checked") ) ) + "</b><br>"  ;
		temp =  temp + ".prop( \"checked\" ): <b>" + std::string( $("#check1").prop( s("checked") ) ) + "</b><br>"  ;
		temp =  temp + ".is( \":checked\" ): <b>"  + std::string( $("input").is( s(":checked") )  ) + "</b>" ;

		//std::cout << temp << "\n";
		
		$("#XXX").html( s(temp.c_str()) ) ;
	
		return 0 ;
	}
 	

int main( void )
{
//#0

	$( "input" ).change( (type::address)f0 ) ;

//#1
 
    std::cout << $( "#_input" ).val() << "\n";

	$( "#_input" ).val(	s("Hello World!")	); // .................................................................. s() , funzione( template ) --> stringc ' ' 

//#2	

	$( "#_output" ).html( s( $( "#_input" ).val() ) );	// ........................................................ s() , funzione( template ) --> stringc ' '

//#3
	$( "#_input" ).val( (type::address) f0 );

//#4
	$( "#_output" ).addClass	( _('classe1 classe2') 	);	// .................................................... _()	, macro   <-- object    --> stringc ' '
	$( "#_output" ).removeClass	( _('classe1') 			);	

//#5
	printf ( "#_output Has Class 'classe1' <<%s>>\n" ,$( "#_output" ).hasClass( _('classe1')  ) ) ;

//#6

	$( "#_output" ).attr( s("title") , s("titolo") );		

	printf ( "#BEFORE REMOVE ATTR :_output prop 'title'  <<%s>>\n" ,$( "#_output" ).attr( _('title') ) );
	
//#7	

	$( "#_output" ).removeAttr( s("title")  ) ;
	
	printf ( "#AFTER REMOVE ATTR :_output prop 'title'  <<%s>>\n" ,$( "#_output" ).attr( _('title') ) ); 
	
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	 
	
	
	
	
	