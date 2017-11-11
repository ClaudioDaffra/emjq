
#include "../lib/emjQuery.hpp"


// emcc -O3 02.manipulate.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 
 
 
	int pClick ( type::pointer _event,type::pointer _data,type::pointer _this ) 
	{
		$( _this ).toggleClass( "off" );
		return 0 ;
	}
	int buttonClick ( type::pointer _event,type::pointer _data,type::pointer _this ) 
	{
	  type::pointer p = NULL ;
 	  static bool flag = false ;
	  
	  if ( flag==true ) 
	  {
		$.dump(p);
		$.dump(p,0);
		$.dump(p,1);		
		$(p).appendTo(  _("document.body") );
		flag=false;
	  } 
	  else 
	  {
		p = $( "p" ).detach();
		flag=true;
	  }
  
	  return 0 ;
	}		
 

int main( void )
{

	// ----------------------------------------------------- #1
	
	$( "p" ).click( (type::address) pClick  ) ;

	$( "button" ).click( (type::address) buttonClick  ) ;

	// ------------------------------------------------------ #2
	
	$( ".hello" ).empty();

	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	