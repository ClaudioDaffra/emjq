
#include "../lib/emjQuery.hpp"


// emcc Callbacks.cpp -o main.js -std=c++11 -s NO_EXIT_RUNTIME=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 -O3 
 
	type::pointer  f1( type::pointer _data ) 
	{
		std::cout << "\n ____________________ f1" ;		
 
		std::cout << "\n## data :: " <<(type::stringc) _data ;

		std::cout << "\n" ;
		
	  return 0 ;
	}
 	 
	type::pointer  f2( type::pointer _data ) 
	{
		std::cout << "\n ____________________ f2" ;		
 
		std::cout << "\n## data  :: " <<(type::stringc) _data ;
 
		std::cout << "\n" ;

	  return 0 ;	  
	}

	type::pointer  fFireWith( type::pointer _data ) 
	{
		std::cout << "\n ____________________ fFireWith" ;		
 
		std::cout << "\n## data  :: " <<(type::stringc) _data ;
 
		std::cout << "\n" ;

	  return 0 ;	  
	}
	
 
int main( void )
{
	$.EMJQ();

	// --------------------------------------------------------------------------------
	
 	printf ("\n#1\n");
	
	emjq::Callbacks c = emjq::Callbacks() ;	
 
	c.add ( (type::address)f1 );
	c.fire ( (type::address)_("'foo!'") ) ;

	c.add ( (type::address)f2 );
	c.fire ( (type::address)_('bar!')   );
 
	printf ("\n c has f1 ((%d))\n",c.has( (type::address)f1 ) );  
 
 	printf ( "\n?[%d]\n",c.remove( (type::address) f1 ) ) ;
	c.fire( (type::address)_('foobar!') );
	
 	c.disable() ;
	printf ("\nc is disabled ((%d))\n",c.disabled() );
	

	c.empty() ;
	
	printf ("\n after empty : c has f2 ((%d))\n",c.has( (type::address) f2 ) );  
	
 
	// --------------------------------------------------------------------------------
 
	printf ("\n#2\n");

	emjq::Callbacks d = emjq::Callbacks() ;	
 
	d.add  (  (type::address)f1   );
	d.fire ( (type::address)_('foo!')   );
	
 	d.add  (  (type::address)f2    );
	d.fire ( (type::address)_('bar!')   );	
	
	printf ("\nfired f1 ((%d))\n",d.fired( (type::address)f1 ) );
	
	d.fireWith( (type::address) fFireWith, (type::address)_("f1,f2") )	;
	
 	d.remove( (type::address)f1  );
	d.fire  ( (type::address)_('foobar!') );
  

	return 0 ;
}
	
	
	
	
	
	
	
	
	
	
	
	
	