
#include "../lib/emjQuery.hpp"


// emcc -O3 01.data.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1
  
 

int main( void )
{
	$.EMJQ(); 

    type::stringc temp = "" ;
    type::pointer p = NULL ;
 

//#1a
 
    type::pointer doc = $("document.body").address() ;

	$.dump ( doc ) ;
	
	$(doc).data( _('foo'), _('52') 	);
	
	$(doc).data( _('bar'), _('test') 	);
	
	temp = $(doc).data();
	printf ( "<<<%s>>>\n",temp );	
 
//#1b

    type::pointer doc2 = $("document.body").address() ;

	$.dump ( doc2 ) ;
	
	$.data( doc2,_('foo'), _('52') 	);
	
	$.data( doc2,_('bar'), _('test') 	);
	
	temp = $.data(doc2);
	printf ( "<<<%s>>>\n",temp );	
	
//#2
 
	$( body ).data( _('foo')	, _(52)  );
	$( body ).data( _('bar')	, _({ myType: "test", count: 40 }) );
	$( body ).data( _({ baz: [ 1, 2, 3 ] } )	);
	
	temp = $( body ).data( _('foo')	); 	// 52
	printf ( "<<<%s>>>\n",temp );	
	
	temp = $( body ).data(); // { foo: 52, bar: { myType: "test", count: 40 }, baz: [ 1, 2, 3 ] }
 	printf ( "<<<%s>>>\n",temp );	
 
//#3
 
	$( "div" ).data( _('test1'), _('VALUE-1') );
	$( "div" ).data( _('test2'), _('VALUE-2') );
 
	temp = $( "div" ).data( _('test1')	);
 	$( "#s1" ).text( temp );  
 
 	temp = $( "div" ).data( _('test2')	);
 	$( "#s2" ).text( temp );  
	
	$( "#ss1" ).text( $( "div" ).data( _('test1') ) );  
	$( "#ss2" ).text( $( "div" ).data( _('test2') ) ); 

    $("div").removeData( _('test1') );

	temp = $( "div" ).data( _('test1')	);
 	$( "#s3" ).text( temp );  
 
 	temp = $( "div" ).data( _('test2')	);
 	$( "#s4" ).text( temp );  

//#4 


	p = $( ".a" ).address(); 
	$.dump(p);
	
	temp = $.hasData( p ) ;
	printf ( ".a has Data (%s)\n",temp ); // false
	
	$.data( p, _('testing'), _('123') );	

	temp = $.hasData( p ) ;
	printf ( ".a has Data (%s)\n",temp ); // true

	//
 
	type::pointer pb2 = $( ".b" ).address(2); 
	$.dump(pb2);
	
	temp = $.hasData( pb2 ) ;
	printf ( ".b[2] has Data (%s)\n",temp ); // true
	
	$.data( pb2, _('testing3'), _('12345') );	

	temp = $.hasData( pb2 ) ;
	printf ( ".b[2] has Data (%s)\n",temp ); // true 

	//
		
 	temp = $( pb2 ).data(_('testing3'));
	printf ( "## %s \n", temp );

	
	return 0 ;
	 
}
	
	
	
	
	
	
	
	 
	
	
	
	
	