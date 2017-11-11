
#include "../lib/emjQuery.hpp"

// emcc -O3 02.mouseEvent.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -std=c++11

int funzione ( type::pointer _event,type::address _data,type::pointer _this )
{
	printf ("<<<%d %d %d>>>\n",_event,(int)_data,_this);

	type::stringc  temp1 ;

	printf ( "#1 _this::%d \n",_this);
 
	temp1 = $( _this ).pointer ( _(['0']['id']) ) ;
	printf ( "#1a this::id::%s\n",temp1);
	
	
	printf ( "#3 data:: %s\n",(type::stringc)_data);

	return 0 ;
}

//********
//	Main
//********

int main ( void )
{
	type::stringc qui = "Huey" ;
	type::stringc quo = "Dewey" ;
	type::stringc qua = "Louie" ;
 
	$(".eventMe").mouseenter( (type::address)qui, (type::address)funzione ).mousedown( (type::address)quo, (type::address)funzione ) ;
	$("#world").mouseup( (type::address)funzione ).mousemove( (type::address)qua, (type::address)funzione ) ;
 
	
  return 0;
}





















