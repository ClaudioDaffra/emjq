
#include "../lib/emjQuery.hpp"

// emcc -O3 03.Ajax.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1


//#######
//			MAIN
//#######
 
 
extern "C"
{
	type::pointer fPrefilter ( type::pointer _options , type::pointer _originalOptions, type::pointer _jqXHR , type::pointer _data ) 
	{
		printf ( "## fPrefilter\n");
/*		
		if ( options.abortOnRetry ) 
		{
			if ( currentRequests[ options.url ] ) 
			{
				currentRequests[ options.url ].abort();
			}
			currentRequests[ options.url ] = jqXHR;
		}
*/
	 return 0 ;
	}		
	type::pointer fTransport ( type::pointer _options , type::pointer _originalOptions, type::pointer _jqXHR , type::pointer _data ) 
	{
		printf ( "## fTranport\n");

/*
		if( transportCanHandleRequest  ) 
		{
			return 
			{
				send: function( headers, completeCallback ) 
				{
					// Send code
				},
				abort: function() 
				{
					// Abort code
				}
			};
		}
*/
	  return 0;
	}	
	

}


int main( void )
{
	$.EMJQ(); 
	
	type::stringc data ="a,b,c";
	
//	#1

    $.ajaxSetup ( "{  url: 'ping.php' }" ) ;
	
//	#2

    type::stringc _string = "Hello World";
	
	$.ajaxPrefilter( (type::address) _string , "?.fPrefilter" ) ;
	$.ajaxTransport( (type::address) data, "?.fTransport" ) ;			

	
	return 0 ;
}
	
	
	
	
	
	
	
	
	
	
	
	
	