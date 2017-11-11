
#include "../lib/emjQuery.hpp"
#include "../lib/json.hpp"
 

// emcc -O3 01.css.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1   -s AGGRESSIVE_VARIABLE_ELIMINATION=1
// emcc -O0 01.css.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1  

 	int divClick0 ( type::pointer _event,type::address _data,type::pointer _this )
	{
		std::cout << "divClick0\n" ;
		std::cout << (int) _event << " " <<(int) _data << " " << (int) _this << "\n" ;
	 	
		$.dump ( _this ) ;

		std::cout <<  $( _this ).css( s("background-color") ) << "\n" ;	
	 
		return 0 ;
	}
 
	int divClick ( type::pointer _event,type::address _data,type::pointer _this )
	{
		std::cout << "divClick\n" ;
		std::cout << (int) _event << " " <<(int) _data << " " << (int) _this << "\n" ;

		$.dump ( _this ) ;
 
 		$( "#result" 		).html( s($( _this ).css( s("background-color"))  ) );	
		$( "#width" 		).html( s($( _this ).width()		) );	
		$( "#height" 		).html( s($( _this ).height()		) );	
		$( "#innerWidth" 	).html( s($( _this ).innerWidth() 	) );	
		$( "#innerHeight" 	).html( s($( _this ).innerHeight() 	) );	
		$( "#outerWidth"  	).html( s($( _this ).outerWidth()	) );	
		$( "#outerHeight" 	).html( s($( _this ).outerHeight()	) );
		
		$( "#offset" 		).html( s($( _this ).offset()		) );	
		$( "#position" 		).html( s($( _this ).position()		) );	

 		type::json joffset 			= json::parse(  $( _this ).offset()	 );
		type::json jposition 		= json::parse( $( _this ).position()  );	
	
		printf ( "# offset   [%d , %d]\n",(int)joffset["top"],(int)joffset["left"] ); 
		printf ( "# position [%d , %d]\n",(int)jposition["top"],(int)jposition["left"] );
 	
		return 0 ;
	}


int main( void )
{

	$("#x").width( s("35px") ).click( (type::address) divClick0 ) ;
 
	$("div").click( (type::address) divClick ) ;

	$("#main").scrollLeft( s(50) ).scrollTop( s(50) ) ;
 	
	$("#main").addClass( s("XYZ ABC OK") ).removeClass( s("XYZ")).toggleClass( s("ABC"));
	
	std::cout << $("#main")/.hasClass(s("OK")) << "\n" ;
 
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	