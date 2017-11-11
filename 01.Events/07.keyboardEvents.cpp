
#include "../lib/emjQuery.hpp"


// emcc -O3 07.keyboardEvents.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -std=c++11

 
int funzione ( type::pointer _event,type::address _data,type::pointer _this )
{
	printf ("<<<%u %p %u>>>\n",_event,_data,_this);

	type::stringc  temp1 ;

	printf ( "# _this::%d \n",_this);

 	temp1 = $( _event ).pointer ( "[0].type" ) ;
	printf ( "#17 event:type::%s\n",temp1) ;	
	
	temp1 = $( _event ).pointer ( "[0].metaKey" ) ;
	printf ( "#7 event:metaKey::%s\n",temp1);

	temp1 = $( _event ).pointer ( "[0].which" ) ;
	printf ( "#18 event:which::%s\n",temp1);

	printf ( "#3 data:: %s\n",(type::stringc)_data);

	return 0 ;
}
 

//********
//	Main
//********

int main ( void )
{
	$( "#world" ).keydown( (type::address)funzione ) ;
 	$( "#hello" ).click  ( (type::address)funzione ) ;
	
  return 0;
}





















