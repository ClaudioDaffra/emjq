
#include "../lib/emjQuery.hpp"


// emcc -O3 02.Animate.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1
 
 
	type::pointer animationcomplete ( void  )
	{	
        std::cout << "Animation complete\n" ;		
		return 0 ;
	}
	
	int f0 ( void)
	{
		printf ( "f0\n" );
 
		$("p").fadeOut() ;
		
		return 0 ;
	}
	
	int f1 ( void )
	{
		printf ( "f1\n" );
 		$("p").slideUp().delay(3000) ;
		return 0 ;
	}	
	

	int f2 ( void )
	{
		printf ( "f2\n" );
        $("#div1").fadeToggle();
		

        $("#div2").fadeToggle( _('slow') );
		
        $("#div3").fadeToggle(3000);
		return 0 ;
	}
	
	int f3 ( void )
	{
		printf ( "f3\n" );
  		$("p").show().clearQueue() ;
		return 0 ;
	}
 
int main( void )
{

	$( "#f0" ).click( (type::address) f0 ) ;

	$( "#f1" ).click( (type::address)f1  ) ;	
	
	$( "#f2" ).click( (type::address) f2  ) ;

	$( "#f4" ).click( (type::address) f3  ) ;	

	$.fx.off(false);
	$.fx.interval(1000);
	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	