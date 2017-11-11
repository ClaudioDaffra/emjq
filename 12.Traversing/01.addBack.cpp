
#include "..\lib\emjQuery.hpp"

// emcc 01.addBack.cpp -o main.js -O3 -std=c++11 


int main () 
{
	$( "div.left, div.right" ).find( _('div, div > p') ).addClass( _('border') );
	 
	// First Example
	$( "div.before-addback" ).find( _('p') ).addClass( _('background')  );
  
	// Second Example
	$( "div.after-addback" ).find( _('p') ).addBack().addClass( _('background2') );

	
  return 0;
}