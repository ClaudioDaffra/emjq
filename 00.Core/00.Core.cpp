
#include "..\lib\emjQuery.hpp"

// emcc 00.Core.cpp -o main.js -O3 -std=c++11 


int main () 
{
  int p0 = $("#hello").address() ;
  int p1 = $("#world").address() ; 
  
  $.dump( p0 ) ;
  $.dump( p1 ) ;
  $.dump( $("*").address() ) ;  
  
 
  return 0;
}