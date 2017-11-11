
#include "../lib/emjQuery.hpp"

// emcc -O3 09.handlerAttachment.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -std=c++11

 
int funzione ( type::pointer _event,type::address _data,type::pointer _this )
{
	printf ("<<<%u %p %u>>>\n",_event,_data,_this);
	
	printf ("# _data %s \n",(type::stringc)_data);


	return 0 ;
}

//********
//	Main
//********

int main ( void )
{
  type::stringc _data = "Hello World";	
 
  //$("#x").on( _('click') , _('p') , (type::address) _data,(type::address) funzione ); 
  
  //$("#x").on( _('click') , (type::address) funzione );  
   
  //$("#x").on(  _('click') , _('p') , (type::address) funzione ).off(); 
  
  $(".clickme").on( _('click')  , (type::address) _data,(type::address) funzione ); 
  
//  $("#world").one("mousemove",(type::address)funzione);   
  
  return 0;
}





















