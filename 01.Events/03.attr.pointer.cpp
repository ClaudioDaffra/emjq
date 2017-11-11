
#include "../lib/emjQuery.hpp"

// emcc -O3 03.attr.pointer.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -std=c++11

int funzione ( type::pointer _event,type::address _data,type::pointer _this )
{
 
	printf ("<<<%u %p %u>>>\n",_event,_data,_this);

	printf ( "#1 _this::%d \n",_this);	
	
	printf ( "#2a this::id::%s\n",$( _this ).pointer("['0'].id") );
 
	printf ( "#3 data:: %s\n",(type::stringc)_data);
 
	return 0 ;
}

//********
//	Main
//********

int main ( void )
{
	
	printf ( "#id:: %s\n" ,$("#x").pointer("['0'].id")   );

	type::pointer p ;
	p = $("#x").address() ;
	printf ( "#id:: %s\n" ,$(p).pointer("['0'].id")   );  
  
	printf ( "#set title\n" ) ; 
	$("#x").pointer( _(['0'].title)	,_('emjQuery') );
	printf ( "#title:: %s\n" ,$("#x").pointer("['0'].title")  );
 
	type::stringc qui = "Huey" ;
	type::stringc quo = "Dewey" ;
	type::stringc qua = "Louie" ;

	$("#hello").click( (type::address)qui, (type::address)funzione ) ;
	$("#world").click( (type::address)quo, (type::address)funzione ) ;
	$(".clickme").dblclick( (type::address)qua, (type::address)funzione ) ;	
	
  return 0;
}

