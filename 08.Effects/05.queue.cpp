
#include "../lib/emjQuery.hpp"


// emcc -O3 05.queue.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 
 
// $( 'div' ).show( 'slow' ).animate({ left: '+=200" }, 2000 ).queue( function() { $( this ).addClass( "newcolor" ).dequeue(); }).animate({ left: "-=200" }, 500 ).queue( function() { $( this ).removeClass( "newcolor" ).dequeue(); }).slideUp(); 

 
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
		
	
	
	
	
	
	 
	
	
	
	
	