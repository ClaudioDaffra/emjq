
#include "../lib/emjQuery.hpp"

// emcc -O3 01.emjq.cpp -o main.js -s NO_EXIT_RUNTIME=1 --std=c++11

//********
//	Main
//********
 
int main() 
{
	type::pointer p0 ;
	p0=$("#x").address();
	printf ( "#p0:: %d\n" ,p0   );
	$.dump(p0);

	type::pointer p1 = $.pointerNew ( _('VALORE') ) ; // var *p1 = "VALORE" ;
	std::cout << "p1::" << p1 << "\n";	
	std::cout << "dump p1\n";
	$.dump(p1);
	
	std::cout << "\n---------------------------------\n";
	
	type::pointer pp1 = $.pointerNew ( p1 ) ; 		  // var *pp1 = &p1
	
	std::cout << "pp1::" << pp1 << "\n";
	std::cout << "dump pp1 (&pp1)\n";
	$.dump(pp1);
	
	eval ( "console.log ( m ) " ) ;	
	eval ( "console.log ( m[%d] ) " , pp1) ;
	eval ( "console.log ( m[m[%d]] ) " , pp1) ;	
	
	std::cout << "\n---------------------------------\n";

	std::cout << "pp1::" << pp1 << "\n ";

	$.dump($.pointer( pp1 ) );


	
	std::cout << "\n ";
	
 return 0 ;
}
























