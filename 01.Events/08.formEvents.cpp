
#include "../lib/emjQuery.hpp"

// emcc -O3 08.formEvents.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -std=c++11

 
int funzioneBlur ( type::pointer _event,type::address _data,type::pointer _this )
{
	printf ("<<<%u %p %u>>>\n",_event,_data,_this);

	type::stringc  temp1 ;

	printf ( "element : %s blur \n", $(_this).pointer ( "[0].id" ) ); 

	return 0 ;
}
 
int funzioneChange ( type::pointer _event,type::address _data,type::pointer _this )
{
	printf ("<<<%u %p %u>>>\n",_event,_data,_this);

	type::stringc  temp1 ;

	printf ( "element : %s value \n", $(_this).pointer ( "[0].value" ) );

	return 0 ;
}
//********
//	Main
//********

int main ( void )
{
	$( ".clickme" ).blur( (type::address)funzioneBlur ) ;
 	
	$( "#hello" ).change( (type::address)funzioneChange ) ;
	
  return 0;
}





















