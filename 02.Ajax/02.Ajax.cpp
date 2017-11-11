
#include "../lib/emjQuery.hpp"
 
 
// emcc -O3 02.Ajax.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1

	// HEADER
 
	// ( beforeSend  	, "jqXHR , settings" )
	//  	type::pointer beforeSend  ( type::pointer _jqXHR , type::pointer _settings  )

	// ( complete  		, "jqXHR , textStatus")
	// 	 	type::pointer complete  ( type::pointer _jqXHR , type::pointer _textStatus  )

	// ( dataFilter  	, "data , type")
	//  	type::pointer dataFilter  ( type::pointer _data , type::pointer _type  )

	// ( error			,"jqXHR,textStatus,errorTrhown")
	// 	 	type::pointer error  ( type::pointer _jqXHR , type::pointer _textStatus, type::pointer _errorTrhown  )

	// ( jsonpCallback	,"")
	// 	 	type::pointer jsonpCallback  ()

	// ( success		,"data,textStatus,jqXHR") 
	// 	 	type::pointer success  ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  )

	// ( xhr			,"")
	// 	 	type::pointer xhr  ()

	//
	
	// ( Ajax_send , "event, jqXHR, ajaxOptions" ) 
	// 	 	type::pointer Ajax_send ( type::pointer event, type::pointer jqXHR, type::pointer ajaxOptions )	
	
	// ( Ajax_start , "" ) 
	// 	 	type::pointer Ajax_start ( void )
	
	// ( Ajax_stop , "" ) 
	// 	 	type::pointer Ajax_stop ( void )			
	
	
	type::pointer fComplete  ( type::pointer _jqXHR , type::pointer _textStatus  )
	{
		printf ( "## fComplete\n");
	 return 0 ;
	}		
	type::pointer fSuccess ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## fSuccess\n");
	 return 0 ;
	}	
	type::pointer fError  ( type::pointer _jqXHR , type::pointer _textStatus, type::pointer _errorTrhown  )
	{
		printf ( "## fError\n");
	 return 0 ;
	}	
	type::pointer Ajax_send ( type::pointer event, type::pointer jqXHR, type::pointer ajaxOptions )	
	{
		printf ( "## Ajax_send\n");
	 return 0 ;
	}		
	type::pointer Ajax_start ( void )	
	{
		printf ( "## Ajax_start\n");
	 return 0 ;
	}
	type::pointer Ajax_stop ( void )	
	{
		printf ( "## Ajax_stop\n");
	 return 0 ;
	}	


int main( void )
{
	$.EMJQ(); 

    emjq::AjaxSettings x = emjq::AjaxSettings();
	
	x.url 		= _( 'demo_test.txt' ) ;

	$( document ).ajaxComplete ( callback ( (type::address)	fComplete 	, "jqXHR , textStatus" 				).c_str() );	
	$( document ).ajaxSuccess  ( callback ( (type::address)	fSuccess 	, "data,textStatus,jqXHR" 			).c_str() );
	$( document ).ajaxError    ( callback ( (type::address)	fError 		, "jqXHR,textStatus,errorTrhown" 	).c_str() );
	$( document ).ajaxSend 	   ( callback ( (type::address) Ajax_send 	, "event, jqXHR, ajaxOptions"		).c_str() );
	$( document ).ajaxStart    ( callback ( (type::address) Ajax_start 	, ""								).c_str() );
	$( document ).ajaxStop 	   ( callback ( (type::address) Ajax_stop 	, ""								).c_str() );	

	$.ajax (x);
	
	return 0 ;
}
	
	
	
	
	
	
	
	
	
	
	
	
	