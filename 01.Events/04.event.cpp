
#include "../lib/emjQuery.hpp"

// emcc -O3 04.event.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -std=c++11

int funzione ( type::pointer _event,type::address _data,type::pointer _this )
{
	    printf ("<<<%u %p %u>>>\n",_event,_data,_this);

		type::stringc temp1 ;
		bool 		  temp2 ;

		$.dump ( _event ) ;
//		
		temp1 = $( _event ).pointer ( "[0].currentTarget" ) ;
		printf ( "#1 event:currentTarget::%s\n",temp1);

//		
		temp1 = $( _event ).pointer ( "[0].data" ) ;
		printf ( "#2 event:data::%s\n",temp1);

		temp1 = $( _event ).pointer ( "[0].delegateTarget" ) ;
		printf ( "#3 event:delegateTarget::%s\n",temp1);
		
//
		temp2 = $( _event ).isDefaultPrevented() ; 					//***
		printf ( "#4 event:isDefaultPrevented::%d\n",temp2);
 
		temp2 = $( _event ).pointer ( "[0].isDefaultPrevented()" );	//***
		printf ( "#4 event:isDefaultPrevented::%d\n",temp2);
 //
 
		temp2 = $( _event ).isImmediatePropagationStopped() ; 		//
		printf ( "#5 event:isImmediatePropagationStopped::%d\n",temp2);		

		temp2 = $( _event ).isPropagationStopped() ; 				//***
		printf ( "#6 event:isPropagationStopped::%d\n",temp2);	
	
		temp1 = $( _event ).pointer ( "[0].metaKey" ) ;
		printf ( "#7 event:metaKey::%s\n",temp1);

		temp1 = $( _event ).pointer ( "[0].namespace" ) ;
		printf ( "event:namespace::%s\n",temp1);

		temp1 = $( _event ).pointer ( "[0].pageX" ) ;
		printf ( "#8 event:pageX::%s\n",temp1);

		temp1 = $( _event ).pointer ( "[0].pageY" ) ;
		printf ( "#9 event:pageY::%s\n",temp1);
		
		$(_event).preventDefault() ; 								//***	
		printf ( "#10 event:.preventDefault()\n");
		
		temp1 = $( _event ).pointer ( "[0].relatedTarget" ) ;
		printf ( "#11 event:relatedTarget::%s\n",temp1);
		
		temp1 = $( _event ).pointer ( "[0].result" ) ; 
		printf ( "#12 event:result::%s\n",temp1);

		$(_event).stopImmediatePropagation() ;						//***
		printf ( "#13 event:.stopImmediatePropagation()\n");
		
		$(_event).stopPropagation() ;								//***
		printf ( "#14 event:.stopPropagation()\n");
		
		temp1 = $( _event ).pointer ( "[0].target" ) ;
		printf ( "#15 event:event.target::%s\n",temp1);

		temp1 = $( _event ).pointer ( "[0].timeStamp" ) ;
		printf ( "#16 event:event.timeStamp::%s\n",temp1);		
	
		temp1 = $( _event ).pointer ( "[0].type" ) ;
		printf ( "#17 event:type::%s\n",temp1);
		
		temp1 = $( _event ).pointer ( "[0].which" ) ;
		printf ( "#18 event:which::%s\n",temp1) ;

		//
		
		printf ( "#19 data:: %p\n",_data);
		printf ( "#20 data:: %s\n",(type::stringc)_data);
 
		return 123 ;
	
}

//********
//	Main
//********

int main ( void )
{
	type::stringc data = "Huey, Dewey, and Louie " ;

	$("#hello").click( (type::address)data, (type::address)funzione ) ;
	$("#world").click( (type::address)funzione ) ;
	  
  return 0;
}





















