
#include "../lib/emjQuery.hpp"

// emcc -O3 05._this.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -std=c++11

int funzione ( type::pointer _event,type::address _data,type::pointer _this )
{
	printf ("<<<%u %p %u>>>\n",_event,_data,_this);

	type::stringc  temp1 ;
	printf ( "# _this::%d \n",_this);

	//
	
	temp1 = $( _this ).pointer ( "[0].id" ) ;
	printf ( "# this::id::%s\n",temp1);

	temp1 = $( _this ).pointer ( "[0].innerHTML" ) ;
	printf ( "# this::innerHTML::%s\n",temp1);			

	temp1 = $( _this ).pointer ( "[0].innerText" ) ;
	printf ( "# this::innerText::%s\n",temp1);

	temp1 = $( _this ).pointer ( "[0].outerHTML" ) ;
	printf ( "# this::outerHTML::%s\n",temp1);			

	temp1 = $( _this ).pointer ( "[0].outerText" ) ;
	printf ( "# this::outerText::%s\n",temp1);	

	temp1 = $( _this ).pointer ( "[0].className" ) ;
	printf ( "# this::className::%s\n",temp1);			

	temp1 = $( _this ).pointer ( "[0].tagName" ) ;
	printf ( "# this::tagName::%s\n",temp1);		

	temp1 = $( _this ).pointer ( "[0].textContent" ) ;
	printf ( "# this::textContent::%s\n",temp1);

	temp1 = $( _this ).pointer ( "[0].value" ) ;
	printf ( "# this::value::%s\n",temp1);	

	temp1 = $( _this ).pointer ( "[0].type" ) ;
	printf ( "# this::type::%s\n",temp1);	

	temp1 = $( _this ).pointer ( "[0].nodeName" ) ;
	printf ( "# this::nodeName::%s\n",temp1);			

	temp1 = $( _this ).pointer ( "[0].localName" ) ;
	printf ( "# this::localName::%s\n",temp1);	

	temp1 = $( _this ).pointer ( "[0].formAction" ) ;
	printf ( "# this::formAction::%s\n",temp1);	

	temp1 = $( _this ).pointer ( "[0].baseURI" ) ;
	printf ( "# this::baseURI::%s\n",temp1);

	temp1 = $( _this ).pointer ( "[0].title" ) ;
	printf ( "# this::title::%s\n",temp1);	

	//***

	printf ( "#3 data:: %s\n",(type::stringc)_data);

	return 0 ;
}

//********
//	Main
//********

int main ( void )
{

	$("#hello").click( (type::address)funzione ) ;
	  
  return 0;
}





















