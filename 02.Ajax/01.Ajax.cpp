
#include "../lib/emjQuery.hpp"


// OK
// emcc -O3 01.Ajax.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1

 
	// HEADER
 
	// callback ( beforeSend  	, "jqXHR , settings" )
	//  	type::pointer beforeSend  ( type::pointer _jqXHR , type::pointer _settings  )

	// callback ( complete  		, "jqXHR , textStatus")
	// 	 	type::pointer complete  ( type::pointer _jqXHR , type::pointer _textStatus  )

	// callback ( dataFilter  	, "data , type")
	//  	type::pointer dataFilter  ( type::pointer _data , type::pointer _type  )

	// callback ( error			,"jqXHR,textStatus,errorTrhown")
	// 	 	type::pointer error  ( type::pointer _jqXHR , type::pointer _textStatus, type::pointer _errorTrhown  )

	// callback ( jsonpCallback	,"")
	// 	 	type::pointer jsonpCallback  ()

	// callback ( success		,"data,textStatus,jqXHR") 
	// 	 	type::pointer success  ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  )

	// callback ( xhr			,"")
	// 	 	type::pointer xhr  ()
	 

	type::pointer fSuccess ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
	{
		printf ( "## fSuccess\n");
 
		printf ( "<<< %d %d %d >>>\n" , _data , _textStatus , _jqXHR ) ;

		type::stringc temp = NULL;
		
		temp = $(_data).pointer();
		printf ("# data :: %s \n" , temp );	  

		
		temp = $(_textStatus).pointer();
		printf ("# _textStatus :: %s \n" , temp );	 
		

		temp = $(_jqXHR).pointer("[0].responseText");
		printf ("# _responseText :: %s \n" , temp );	 
		
		temp = $(_jqXHR).pointer("[0].statusText");
		printf ("# _statusText :: %s \n" , temp );	
		
		temp = $(_jqXHR).pointer("[0].readyState");
		printf ("# _readyState :: %s \n" , temp );	
 
		
	 return 0 ;
	}
	
	type::pointer fComplete  ( type::pointer _jqXHR , type::pointer _textStatus  )
	{

		printf ( "## fComplete\n");
 
		printf ( "<<< %d %d >>>\n" , _textStatus , _jqXHR ) ;

		type::stringc temp = NULL;


		temp = $(_textStatus).pointer();
		printf ("# _textStatus :: %s \n" , temp );	 
 		
	 return 0 ;
	}	
	type::pointer fError ( type::pointer _jqXHR , type::pointer _textStatus, type::pointer _errorTrhown  ) 
	{

		printf ( "## fError\n");
 
		printf ( " <<< %d %d>>>\n"  , _textStatus , _jqXHR ) ;

		type::stringc temp = NULL;
		


		temp = $(_textStatus).pointer();
		printf ("# _textStatus :: %s \n" , temp );	 
 		
	 return 0 ;
	}

 


int main( void )
{
	$.EMJQ();

    emjq::AjaxSettings x = emjq::AjaxSettings();
	
	x.url 		= _( 'demo_test.txt' ) ;
	x.complete 	= (type::address) fComplete ;
	x.success 	= (type::address) fSuccess ;
	x.error 	= (type::address) fError ;

	//std::cout << x.to_string() << "\n" ;

	$.ajax (x);	
 
	$( document ).ajaxComplete ( callback ( (type::address)fComplete , "jqXHR , textStatus" ).c_str() );	
	
	$.ajax (x);
 
	return 0 ;
}
	
	
	
	
	
	
	
	
	
	
	
	
	