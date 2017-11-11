
Emscripten jQuery ( preAlpha Version )
--------------------------------------


This is a simple porting of jQuery Library to Emscripten. 
Now in preAlpha version and not suitable for production.
( and never will be ! )
I have implemented several commands and others are still being ported.

Features :

- chain of commands like jQuery;
- nested chain ;
- garbage collector;
- one file header;
- include JSON library (Niels Lohmann)
- write a little less and do a little less than more !

I have written this for myself, and i want to share it with you.

Every comment, criticism, suggestion, test, and input of new code is well accepted.

You can see how to use it, look at numbered examples. Take this order as a small tutorial to understand how it works.

Here, what is done and what is missing :



Here aresome examples :

-----------
MOUSE EVENT
-----------

	#include "../lib/emjQuery.hpp"

	// emcc -O3 02.mouseEvent.cpp -o main.js -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -std=c++11

	int funzione ( type::pointer _event,type::address _data,type::pointer _this )
	{
		printf ("<<<%d %d %d>>>\n",_event,(int)_data,_this);

		printf ( "#1 _this::%d \n",_this);
	 
		printf ( "#1a this::id::%s\n",$( _this ).pointer ( _(['0']['id']) ));
		
		printf ( "#3 data:: %s\n",(type::stringc)_data);

		return 0 ;
	}

	int main ( void )
	{
		type::stringc qui = "Huey" ;
		type::stringc quo = "Dewey" ;
		type::stringc qua = "Louie" ;
	 
		$(".eventMe").mouseenter( (type::address)qui, (type::address)funzione ).mousedown( (type::address)quo, (type::address)funzione ) ;
		$("#world").mouseup( (type::address)funzione ).mousemove( (type::address)qua, (type::address)funzione ) ;
	 }
	
-----------
AJAX
-----------
	#include "../lib/emjQuery.hpp"

	// emcc -O3 05.Ajax.JSON.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1  

	 
		type::pointer fSuccess ( type::pointer _data , type::pointer _textStatus, type::pointer _jqXHR  ) 
		{
			printf ( "## fSuccess\n");

			printf ("# data :: %s \n" , $(_data).pointer("[0]") );	 		

			type::json j = json::parse( $(_data).pointer("[0]") ) ;

			std::cout << "\n..type.. << " << j["type"] << " >> \n" ;
			std::cout << "\n..value.. << " << j["value"] << " >> \n" ;	

			std::cout << "\n..items[0].value << "  << j["items"][0]["value"] << " >> \n" ;
			std::cout << "\n..items[0].action << " << j["items"][0]["action"] << " >> \n" ;		
			
		 return 0 ;
		}	


	int main( void )
	{
		$.EMJQ(); 

		type::pointer p=$.getJSON 	( _('demo_json.txt') , (type::address) fSuccess ) ;

		$.dump ( p ) ;
		
		return 0 ;
	}
	
-----------
DEFERRED
-----------

	#include "../lib/emjQuery.hpp"
	 

	// emcc -O3 02.when.done.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1   -s AGGRESSIVE_VARIABLE_ELIMINATION=1
	   
		type::pointer f1 ( type::pointer _v1 , type::pointer _v2, type::pointer _v3  ) 
		{
			printf ( "## f1 ## %d %d %d\n",_v1,_v2,_v3);

			$.dump ( _v1 ) ; // v1 is undefined
			$.dump ( _v2 ) ; // v2 is "abc"
			$.dump ( _v3 ) ; // v3 is an array [ 1, 2, 3, 4, 5 ]

		 return 0 ;
		}
	 

	int main( void )
	{
		$.EMJQ();

		type::pointer pd1 = $.var( "d1" , $.Deferred() ) ;		
		type::pointer pd2 = $.var( "d2" , $.Deferred() ) ;
		type::pointer pd3 = $.var( "d3" , $.Deferred() ) ;

		$.when( _(d1,d2,d3) ).done ( 
			$.function ( (type::address) f1 , _ ( v1,v2,v3 ) )
		) ;
		
		$(pd1).resolve() ;	
		$(pd2).resolve( _('abc') ) ;	
		$(pd3).resolve( _(1,2,3,4,5) ) ;	

		return 0 ;
		 
	}

---------------- 
ANIMATED / QUEUE
---------------- 


#include "../lib/emjQuery.hpp"


	// emcc -O3 05.queue.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 

		int effect2 ( type::pointer _this )
		{
			printf ( "effect2\n" );
			$( _this ).removeClass( _("newcolor") ).dequeue();
			return 0 ;
		}
		
		int effect1 ( type::pointer _this )
		{
			printf ( "effect1\n" );
			$( _this ).addClass( _("newcolor") ).dequeue();
			return 0 ;
		}

		int  start ( type::pointer _event,type::address _data,type::pointer _this )
		{
			printf ( "start\n" );
			
			// do you like chain ?
			
			// $( 'div' ).show( 'slow' ).animate({ left: '+=200" }, 2000 ).queue( function() { $( this ).addClass( "newcolor" ).dequeue(); }).animate({ left: "-=200" }, 500 ).queue( function() { $( this ).removeClass( "newcolor" ).dequeue(); }).slideUp(); 
			
			$( "div" ).show(_('slow')).animate(_({ left: '+=200' }), _(2000) )
				.queue( (type::address) effect1 ,"_this=this" ).animate(_({ left: '-=200' }), _(500) )
				.queue( (type::address) effect2 ,"_this=this" ).slideUp();

		return 0 ;
		}

	int main( void )
	{
		$( "#start" ).click( (type::address) start ) ;
		
		return 0 ;
	}
		
	
	
(-) TODO (V) DONE (X) Not Planned (![0,2]) Tested (€) Working ($) for production ...


V	Ajax

	V	Global Ajax Event Handlers
	
	V	Helper Functions
	
	V	Low-Level Interface
	
	V	Shorthand Methods
	
V	Attributes

V	Callbacks Object

V	Core

V	CSS

V	Data

V	Deferred Object

V	Dimensions

V	Effects

	V	Basics
	
	V	Custom
	
	V	Fading
	
	v	Sliding
	
V	Events

	V	Browser Events
	
	V	Document Loading
	
	V	Event Handler Attachment
	
	V	Event Object
	
	V	Form Events
	
	V	Keyboard Events
	
	V	Mouse Events
	
V	Forms

X	Internals

V	Manipulation

	V	Class Attribute
	
	V	Copying
	
	V	DOM Insertion, Around
	
	V	DOM Insertion, Inside
	
	V	DOM Insertion, Outside
	
	V	DOM Removal
	
	v	DOM Replacement
	
	V	General Attributes
	
	V	Style Properties
	
V	Miscellaneous

	V	Collection Manipulation
	
	V	Data Storage
	
	V	DOM Element Methods
	
	V	Setup Methods
	
V	Offset

V	Traversing


	V	Filtering
	V	Miscellaneous Traversing
	
	V	Tree Traversal
	
V	Utilities




