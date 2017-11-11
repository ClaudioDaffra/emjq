
#include "../lib/emjQuery.hpp"


// emcc -O3 01.Animate.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 -std=c++11
 

	type::pointer animationcomplete ( void  )
	{	
        std::cout << "Animation complete\n" ;		
		return 0 ;
	}
	
	int clickMe ( type::pointer _event,type::address _data,type::pointer _this )
	{
		printf ( "clickMe\n" );
		
		type::json	properties = 
		{
		  {"opacity",0.25},
		  {"left", "+=50"},
		  {"height", "toggle"}
		};

		$( "#book" ).animate(  properties.dump().c_str()  ,  5000 , _('swing') ,(type::address) animationcomplete  ) ; 
		//$( "#book" ).animate(  properties.dump().c_str()  ,  5000 , _('swing')  ) ; // OK	
		//$( "#book" ).animate(  properties.dump().c_str()  ,  5000    ) ;	// ok

		
		return 0 ;
	}
	
	int clickMeGO ( type::pointer _event,type::address _data,type::pointer _this )
	{
		printf ( "clickMe\n" );
		
		type::json	properties = 
		{
			{"left",100}
		};
		type::json	options = 
		{
			{"duration",1000},
			{"step", "<<< function ( now, fx ){$( '.block:gt(0)' ).css( 'left', now );} >>>" }	// type::json with javascript ONLY , trick ! {{{ javascript }}}  
		};
		
		$( ".block:first" ).animate(  properties.dump().c_str()  , options.dump().c_str()   ) ;

		return 0 ;
	}	
	

	int start ( type::pointer _event,type::address _data,type::pointer _this )
	{
		printf ( "start\n" );
		
        $("#xyz").animate("{height: '300px', opacity: '0.4'}"	, _('slow') );
        $("#xyz").animate("{width: '300px', opacity: '0.8'}"	, _('fast') );
        $("#xyz").animate("{height: '100px', opacity: '0.4'}"	, _('slow') );
        $("#xyz").animate("{width: '100px', opacity: '0.8'}"	, _('slow') );
 
		return 0 ;
	}
	int stop ( type::pointer _event,type::address _data,type::pointer _this )
	{
		printf ( "stop\n" );
/*
		type::json JSONoptions = 
		{
			{"duration",1000},
			{"easing","fast"}
		};

		$("#div3").fadeIn( JSONoptions ) ; // KO	?? Uncaught TypeError: r.easing[this.easing] is not a function
*/	
	
 		type::json JSONoptions = {
			{"duration",1000}
		};
		
		$("#div1").fadeIn( 1000 , "'slow'" , (type::address) animationcomplete , "" ) ;  
		$("#div2").fadeIn( 500 , "'swing'"  ) ;  
		$("#div4").fadeIn() ;  

		$("#div3").fadeIn( JSONoptions ) ; 
		
		return 0 ;
	}


int main( void )
{

	$( "#clickme" ).click( (type::address)clickMe  ) ;

	$( "#go" ).click( (type::address)clickMeGO  ) ;	
	
	$( "#start" ).click( (type::address) start  ) ;

	$( "#stop" ).click( (type::address) stop  ) ;		
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	