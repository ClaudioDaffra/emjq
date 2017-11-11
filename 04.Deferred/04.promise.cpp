
#include "../lib/emjQuery.hpp"
 

// emcc -O3 04.promise.cpp -o main.js -std=c++11 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1
 
 	// ............................................................................ working
	void working ( type::address _pdfd ) 
	{
		printf ( "\n..working..\n" );
		
		type::pointer pdfd = (type::pointer) _pdfd ;
 	
		//$.dump ( pdfd ) ;

		type::pointer pstate = $(pdfd).pointerCall ("state()"); 
		
		// $.dump(state);
		//printf ( "[[%s]]\n", $(pstate).pointer() ) ;	
		
		std::string state ( $(pstate).pointer() ) ;
		std::cout << "STATE::"<< state << "\n";
		
		if ( state == "pending"  )
		{
			$(pdfd).notify( _( 'working... ' ) ) ;	
			setTimeout ( (type::address) working , (type::address) pdfd, 500 ) ;			  
		}	
 		
	}
	 
	// ............................................................................ f1
	type::pointer f1 ( type::address _pdfd ) 
	{
		printf ( "\n..f1..\n" );
		
		type::pointer pdfd = (type::pointer) _pdfd ;
		//$.dump ( pdfd ) ;
		
		$(pdfd).resolve( _( 'hurray' ) ) ;	
		
		$(pdfd).reject( _( 'sorry' ) ) ;	
		
		setTimeout ( (type::address) working , (type::address) pdfd, 1 ) ;		
		
		return 	0;
	}
  
	 
	// ............................................................................ asyncEvent (type::thenable)
	type::pointer asyncEvent ( void  ) 
	{
		printf ( "\n..asyncEvent..\n" );
		
		type::pointer pdfd = $.var( "dfd" , $.Deferred() ) ;	

		setTimeout ( (type::address) f1, (type::address) pdfd, 500 ) ;
		
	
		return 	$(pdfd).promise() ;
	}
 
 

int main( void )
{

	$.EMJQ(); 
 
	   $.when( (type::thenable) asyncEvent  ).then
	   ( 
			_( function(status)
			{
					console.log( status + ", things are going well" );
			})  ,
			_( function(status)
			{
					console.log( status + ", you fail this time" ); 
			})  ,
			_( function(status)
			{
					console.log(status);
			})  		
	 ) ; 
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	