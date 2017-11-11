
#include "..\lib\emjQuery.hpp"

// emcc 02.nextAll.cpp -o main.js -O3 -std=c++11 


int main () 
{
 $( "div:first" ).nextAll().addClass( _('after') );
	
  return 0;
}