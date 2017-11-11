
#include "../lib/emjQuery.hpp"


// emcc -O3 02.utilities.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s EMULATE_FUNCTION_POINTER_CASTS=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 
 


int main( void )
{
	$.EMJQ() ;

	//	$.fn.extend : Not Planned

	$.globalEval ( _(var newVar=true;) );
	
	eval ( "console.log(newVar)" );

	type::pointer arr 	= $.pointerNew ( "[1,2,3]" ) ;
	type::pointer num 	= $.pointerNew ( "10" ) ;
	
	type::pointer fff 	= $.pointerNew ( "f=function(){};" ) ;
	type::pointer obj 	= $.pointerNew ( _JSON("{}") ) ;

	type::pointer obj2 	= $.pointerNew ( "window" ) ;

	type::pointer obj3 	= $.pointerNew ( _XML("<root><child><subchild>.....</subchild></child></root>") ) ;

	$.dump ( arr );
	$.dump ( num );
	$.dump ( fff );
	$.dump ( obj );
	$.dump ( obj2 );
	$.dump ( obj3 );
	
	std::cout <<"isArray :: "			<< $.isArray 		( arr ) 			<< "\n" ;
	std::cout <<"inArray :: "			<< $.inArray 		(  _(2) , arr ) 	<< "\n" ;
	std::cout <<"inArray :: "			<< $.inArray 		(  _('2') , arr ) 	<< "\n" ;	
	std::cout <<"inArray :: "			<< $.inArray 		(  _(-1 ) , arr ) 	<< "\n" ;	
	std::cout <<"inNumeric :: "			<< $.isNumeric 		(  num ) 			<< "\n" ;		
	std::cout <<"isFunction :: "		<< $.isFunction 	(  fff ) 			<< "\n" ;
	std::cout <<"isEmptyObject :: "		<< $.isEmptyObject 	(  obj ) 			<< "\n" ;
	std::cout <<"isPlainObject :: "		<< $.isPlainObject 	(  obj ) 			<< "\n" ;
	std::cout <<"isWindow :: "			<< $.isWindow 		(  obj2 ) 			<< "\n" ;
	std::cout <<"isXMLDoc :: "			<< $.isXMLDoc 		(  obj3 ) 			<< "\n" ;
	std::cout <<"type RegExp :: "		<< $.type			( "/test/" ) 		<< "\n" ;
 
	type::pointer  p;
	
	p = $.parseJSON ( _({"firstName":"John", "lastName":"Doe"}) )  ;
	$.dump ( p );
	
	p = $.parseHTML ( _(<p>hello, <b>my name is</b> jQuery.<p>) )  ;
	$.dump ( p );	

	p = $.parseXML ( _(<root><child><subchild>.....</subchild></child></root>) )  ;
	$.dump ( p );	

	std::cout <<"TIME :: "			<< $.now () 			<< "\n" ;
		
	$.noop() ;
 	
	return 0 ;
	 
}
	

	
	
	
	 
	
	
	
	
	