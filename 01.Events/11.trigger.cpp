
#include "../lib/emjQuery.hpp"

// emcc -O3 11.trigger.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -std=c++11

 
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
  
  $("#x").click( (type::address)_data, (type::address)funzione ).trigger( _('click') ); 
  
  $("#world").click( (type::address)funzione ).triggerHandler(_('click')); 

   
  return 0;
}





















