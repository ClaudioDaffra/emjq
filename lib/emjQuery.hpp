
//________________________________________________________________________________
//
//   _____  __   __        _   _____
//  |  ___||  \_/  |      | | |  _  | EMJQ ver. 0.1
//  | |__  |       |      | | | | | | Claudio Daffra
//  |  __| | |\_/| |  __  | | | |_| | daffra.claudio@gmail.com
//  | |___ | |   | | | |__| | |  \ \| Copyright (2017)
//  |_____||_|   |_| |______| |___\_\ MIT/GPL v3  License
//  
//________________________________________________________________________________
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <string>
#include <array>
#include <iostream> 
#include <sstream> 
#include <vector>
#include <math.h>
 
 
#include <emscripten.h>

#include "json.hpp"

/**/

namespace emjQuery
{
	// ___________________________________________________________________________________________________________________	
	// ___________________________________________________________________________________________________________________ Global

	//######
	//		§CONST
	//######	
	
	const char *window 		= "window" ;
	const char *screen 		= "window.screen" ;	
	const char *document 	= "window.document" ;	
	const char *body 		= "window.document.body" ;	
	const char *begin 		= "begin" ;	
	const char *init 		= "begin" ;		
	const char *end 		= "end" ;	
		
	const int  BufferSize 	= 1024 ;

	//######
	//		§TYPE
	//######	
	
	namespace type
	{
		typedef int 		  pointer ;	
		typedef void * 		  address ;
		typedef const char *  stringc ;
		typedef int (*thenable)(void) ;  
	};		
	
	typedef int  (*callback_EventData) ( int _event,int _data,int _this ) ;	/*?*/
	
	//######
	//		§HELPER
	//######	

	// https://stackoverflow.com/questions/5343190/how-do-i-replace-all-instances-of-a-string-with-another-string /*?*/

	void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace) 
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos) {
			 subject.replace(pos, search.length(), replace);
			 pos += replace.length();
		}
	} 

	// ................................................................. eval
	
	int eval (const char *  format, ...)
	{
	  char buffer[BufferSize];
	  va_list args;
	  va_start (args, format);
	  vsnprintf (buffer, BufferSize-1, format, args);
	  va_end (args);
	//
	//_____________________________________________________
	//
	//	printf ( "# EVAL <<<%s>>>\n",buffer ) ; // q1w2e3r4
	//_____________________________________________________	
	//
	  return emscripten_run_script_int ( buffer  );
	}	

	// ................................................................. Trim	

	// Trim from left
	inline std::string& lTrim(std::string& s, const char* t = " \t\n\r\f\v")
	{
		s.erase(0, s.find_first_not_of(t));
		return s;
	}

	// Trim from right
	inline std::string& rTrim(std::string& s, const char* t = " \t\n\r\f\v")
	{
		s.erase(s.find_last_not_of(t) + 1);
		return s;
	}

	// Trim from left & right
	inline std::string& Trim(std::string& s, const char* t = " \t\n\r\f\v")
	{
		return lTrim(rTrim(s, t), t);
	}
	
	// ................................................................. explode
	
	// https://stackoverflow.com/questions/12966957/is-there-an-equivalent-in-c-of-phps-explode-function	
	std::vector<std::string> explode(std::string const & s, char delim)
	{
		std::vector<std::string> result;
		std::istringstream iss(s);

		for (std::string token; std::getline(iss, token, delim); )
		{
			result.push_back(std::move(Trim(token)));
		}

		return result;
	}	

	// ........................................................................... setTimeout , setInterval
	
	void setTimeout ( type::address f1, type::address arg, int milliSec=0 )
	{
		emscripten_async_call( (em_arg_callback_func) f1, (type::address) arg , milliSec ) ;	
	}
	void setinterval ( type::address f1, type::address arg, int milliSec=0 )
	{
		emscripten_async_call( (em_arg_callback_func) f1, (type::address) arg , milliSec ) ;	
	}
	void callback ( type::address f1, type::address arg  )
	{
		emscripten_async_call( (em_arg_callback_func) f1, (type::address) arg , 0 ) ;	
	}
	
	// *********  
	//			§callback §function 
	// ********* 

	// ............................................................................................ callbackEvent ( type::address _callback )

		
	std::string callbackEvent  ( type::address _callback , type::address _data )
	{
		std::string code = "";
        code="";
		code+="\n function(event){";	
		code+="\n if ( m['result'] == undefined ) m['result']=0;";
		code+="\n if ( event == undefined ) event = {};";
		code+="\n event.result = m['result'];  ";
		code+="\n var p1 = window.pointerSet ( 'object' , event );  ";
		code+="\n var p3 = window.pointerSet ( 'object' , $(this) ); ";
		code+="\n m['result'] = Runtime.dynCall('iiii', "+std::to_string((int)_callback)+" , [ p1,"+std::to_string((int)_data)+",p3 ] );" ;			
		code+="\n }";

		return code ;
	}
		
 	// ............................................................................................ callback ( type::address _callback )
	
	std::string callback ( type::address _callback , type::stringc _argc ) //§callback §function
	{
		std::string _arg = std::string( _argc ) ;
		std::vector<std::string> varg = explode ( _arg , ',' );
		std::string code = "" ;
		std::string _arg2="i" ;
		std::string _arg3=""  ;
		std::string _temp=""  ;	
		
		if ( _callback==NULL )
		{
			code = "function(){}";
		}
		else
		{		
			code+= "\n function("+_arg+") {";	
			for (int i=0;i< varg.size(); i++)
			{
				_temp = (i+1<varg.size()) ? " , " : "" ;
				_arg3 += "p"+std::to_string(i) + _temp ;		
				code += "\n var p"+std::to_string(i)+" = window.pointerSet ( 'object' , "+varg[i]+" ) ; " ;
				_arg2 += "i";
			}
			code+="\nreturn m['result'] = Runtime.dynCall('"+_arg2+"', "+std::to_string( (int) _callback )+" , [ "+_arg3+" ] );";
			code+="}";
		}
		//printf ( "@@@%s\n" , code.c_str() );  
		
		return code ; 
	}

	// ............................................................................................ callback ( type::stringc ,  )
		
	std::string callback ( type::stringc _callback , type::stringc _argc ) //§callback §function
	{
		std::string _arg = std::string( _argc ) ;		
		std::vector<std::string> varg = explode ( _arg , ',' );
		std::string code = "" ;
		std::string _arg2="" ;
		std::string _arg3=""  ;
		std::string _temp=""  ;	
		
		if ( _callback==NULL )
		{
			code = "function(){}";
		}
		else
		{
			code+= "\n function("+_arg+") {";	
			for (int i=0;i< varg.size(); i++)
			{
				_temp = (i+1<varg.size()) ? " , " : "" ;
				_arg3 += "p"+std::to_string(i) + _temp ;		
				code += "\n var p"+std::to_string(i)+" = window.pointerSet ( 'object' , "+varg[i]+" ) ; " ;
				_arg2 += "'number'"+_temp;
			}
			code+="\nreturn m['result'] = Module.ccall ( '"+std::string(_callback)+"' , 'number' , ["+_arg2+"],["+_arg3+"] );";
			code+="\n}";
		}
		//printf ( "@@@%s\n" , code.c_str() );
		
		return code ; 
	}
	
	
	// ___________________________________________________________________________________________________________________	
	// ___________________________________________________________________________________________________________________ Class emjq
	
	class emjq
	{
		private:
			int pMain=-1 ;			//	main pointer : che percorre la catena
			int pBase=-1 ;			//	main pointer : alla abase della catena			
			static bool init ;		//	flag init garbage Collector, (indipendente dalle altre istanze)

		public:
			// .................................................. unique<*emjq> <- _emjq()  
			std::unique_ptr<emjq> _emjq ( void )
			{
				std::unique_ptr<emjq> p( new emjq() ) ;
				return p ;
			}
			
			// *********  
			//			§core
			// ********* 

			emjq& 	jQuery 		( const char * sel );
			emjq& 	jQuery 		( int p );
			int 	jQueryBegin ( void );
			int 	garbageInit ( void );
			int 	jQueryEnd 	( int _exit=0 );			

			// .................................................. dump	
			int dump ( int p )
			{
				return eval ( "%s%d%s" , "console.debug( m[" ,p, "] )" );
			}
			int dump ( const char * s )
			{
				return eval ( "console.debug( %s )",s  );
			}
			int dump ( int p,int _index )
			{
				return eval ( "console.debug( m[%d][%d] )",p,_index );
			}

			// .................................................................... jquery 		
			void	EMJQ	( std::string cmd="" ) 
			{
				    if (( cmd=="" ) || ( cmd == "begin" )) 
					{
						jQueryBegin ();
						garbageInit ();
						init=false;
					}
				    if (  cmd == "end" ) 
					{
						jQueryEnd(0);
					}					
			}	
			
			// *********  
			//			§traversing
			// ********* 
			
			// .................................................................... add
			emjq& 	add( type::stringc _sel  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).add ( %s ) )",pMain,_sel    );
				return *this ;
 			}			
			emjq& 	add	( type::pointer _sel  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).add(jQuery(m[%d])) )",pMain,_sel   );
				return *this ;				
			}	
			emjq& 	add( type::stringc _sel,type::stringc _ctx  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).add( %s,%s ) )",pMain,_sel,_ctx    );
				return *this ;
 			}
			// .................................................................... closest
			emjq& 	closest( type::stringc _sel  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).closest ( %s ) )",pMain,_sel    );
				return *this ;
 			}			
			emjq& 	closest	( type::pointer _sel  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).closest(jQuery(m[%d])) )",pMain,_sel   );
				return *this ;				
			}	
			emjq& 	closest( type::stringc _sel,type::stringc _ctx  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).closest( %s,%s ) )",pMain,_sel,_ctx    );
				return *this ;
 			}			
			// .................................................................... addBack			
			emjq& 	addBack( void )			
			{
				pMain = eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).addBack () )",pMain  );  
				if (pMain<0) {
						++pMain; ++pMain;
				}else 	 	 {
						--pMain ;--pMain ;
				}
				return *this ;
 			}				
			// .................................................................... find
			emjq& 	find( type::stringc _sel="" )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).find ( %s ) )",pMain,_sel   );
				return *this ;
 			}
			emjq& 	find	( type::pointer _sel  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).find(jQuery(m[%d])) )",pMain,_sel   );
				return *this ;				
			}
			// .................................................................... children
			emjq& 	children( type::stringc _sel="" )			
			{
				pMain = eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).children (%s) )",pMain,_sel  );  
				return *this ;
 			}							
			// .................................................................... contents 	
			emjq& 	contents( void )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).contents () )",pMain   );
				return *this ;
 			}
			// .................................................................... end 	
			emjq& 	end( void )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).end () )",pMain   );
				return *this ;
 			}
			// .................................................................... eq 	
			emjq& 	eq( int _obj1 )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).eq ( %d ) )",pMain,_obj1   );
				return *this ;
 			}			
			// .................................................................... filter
			emjq& 	filter( type::stringc _sel  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).filter ( %s ) )",pMain,_sel    );
				return *this ;
 			}			
			emjq& 	filter	( type::pointer _sel  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).filter(jQuery(m[%d])) )",pMain,_sel   );
				return *this ;				
			}	
			emjq& 	filter( type::address _cb )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).filter ( %s ) )",pMain,callback ( _cb , "_index,_element,_this=this" ).c_str()  );
				return *this ;
 			}
			// .................................................................... first 	
			emjq& 	first( void )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).first () )",pMain   );
				return *this ;
 			}	
			// .................................................................... has
			emjq& 	has( type::stringc _sel  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).has ( %s ) )",pMain,_sel    );
				return *this ;
 			}			
			emjq& 	has	( type::pointer _sel  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).has(jQuery(m[%d])) )",pMain,_sel   );
				return *this ;				
			}
			// .................................................................... §is
			type::stringc 	is( type::stringc _sel  )			
			{
				//return eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).is ( %s ) )",pMain,_sel    );
				return (type::stringc)eval ( "strdup ( jQuery(m[%d]).is ( %s ) )",pMain,_sel    );				
 			}			
			type::stringc 	is( type::pointer _sel  )			
			{
				//return eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).is(jQuery(m[%d])) )",pMain,_sel   );
				return (type::stringc)eval ( "strdup ( jQuery(m[%d]).is ( m[%d] ) )",pMain,_sel    );	
			}	
			type::stringc 	is( type::address _cb,type::stringc _sel )			
			{
				//return eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).is ( %s ) )",pMain,callback ( _cb , "_index,_element,_this=this" ).c_str()  );
				return (type::stringc)eval ( "strdup ( jQuery(m[%d]).is (%s) )",pMain,_sel    );	
 			}
			// .................................................................... parent
			emjq& 	parent( type::stringc _sel="" )			
			{
				pMain = eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).parent (%s) )",pMain,_sel  );  
				return *this ;
 			}
			// .................................................................... last 	
			emjq& 	last( void )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).last () )",pMain   );
				return *this ;
 			}
			// .................................................................... nextAll
			emjq& 	nextAll( type::stringc _sel="" )			
			{
				pMain = eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).nextAll (%s) )",pMain,_sel  );  
				return *this ;
 			}			
			// .................................................................... prevAll
			emjq& 	prevAll( type::stringc _sel="" )			
			{
				pMain = eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).prevAll (%s) )",pMain,_sel  );  
				return *this ;
 			}
			// .................................................................... prev
			emjq& 	prev( type::stringc _sel="" )			
			{
				pMain = eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).prev (%s) )",pMain,_sel  );  
				return *this ;
 			}
			// .................................................................... next
			emjq& 	next( type::stringc _sel="" )			
			{
				pMain = eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).next (%s) )",pMain,_sel  );  
				return *this ;
 			}				
			// .................................................................... siblings
			emjq& 	siblings( type::stringc _sel="" )			
			{
				pMain = eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).siblings (%s) )",pMain,_sel  );  
				return *this ;
 			}	
			// .................................................................... parents
			emjq& 	parents( type::stringc _sel="" )			
			{
				pMain = eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).parents (%s) )",pMain,_sel  );  
				return *this ;
 			}	
			// .................................................................... nextUntil	
			emjq& 	nextUntil( type::stringc _sel,type::stringc _filter=""   )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).nextUntil( %s,%s ) )",pMain,_sel,_filter    );
				return *this ;
 			}	
			emjq& 	nextUntil( type::pointer _sel,type::stringc _filter=""   )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).nextUntil( jQuery(m[%d]) ,%s ) )",pMain,_sel,_filter    );
				return *this ;
 			}	
			// .................................................................... prevUntil	
			emjq& 	prevUntil( type::stringc _sel,type::stringc _filter=""   )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).prevUntil( %s,%s ) )",pMain,_sel,_filter    );
				return *this ;
 			}	
			emjq& 	prevUntil( type::pointer _sel,type::stringc _filter=""   )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).prevUntil( jQuery(m[%d]) ,%s ) )",pMain,_sel,_filter    );
				return *this ;
 			}	
			// .................................................................... parentsUntil	
			emjq& 	parentsUntil( type::stringc _sel,type::stringc _filter=""   )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).parentsUntil( %s,%s ) )",pMain,_sel,_filter    );
				return *this ;
 			}	
			emjq& 	parentsUntil( type::pointer _sel,type::stringc _filter=""   )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).parentsUntil( jQuery(m[%d]) ,%s ) )",pMain,_sel,_filter    );
				return *this ;
 			}	
			// .................................................................... Not
			emjq& 	Not( type::stringc _sel  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).not ( %s ) )",pMain,_sel    );
				return *this ;
 			}			
			emjq& 	Not	( type::pointer _sel  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).not(jQuery(m[%d])) )",pMain,_sel   );
				return *this ;				
			}	
			emjq& 	Not( type::address _cb )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).not ( %s ) )",pMain,callback ( _cb , "_index,_element,_this=this" ).c_str()  );
				return *this ;
 			}
			// .................................................................... offsetParent 	
			emjq& 	offsetParent( void )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).offsetParent () )",pMain   );
				return *this ;
 			}
			
			// *********  
			//			§style
			// ********* 

			// .................................................................... css 
			
			type::stringc 	css( type::stringc _obj1="" )		
			{
				return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).css ( %s ) )",pMain,_obj1   );
 			}
			emjq& 	css( type::stringc _obj1 , type::stringc _obj2  )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).css ( %s,%s ) )",pMain,_obj1,_obj2   );
				return *this ;
 			}
			emjq& 	css( type::stringc _obj1 ,type::address _cb )			
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).css ( %s,%s ) )",pMain,_obj1,callback ( _cb , "_index,_value,_this=this" ).c_str()  );
				return *this ;
 			}	

			// ********* 
			//			§documentLoading 
			// *********	

			// .................................................................... noConflict 		
			emjq& 		noConflict		( bool flag=true ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery.noConflict(%s) )",flag==true?"true":"false"  ) ;
				return *this ;
			}
			// .................................................................... holdReady 			
			emjq& 		holdReady		( bool flag=true ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery.holdReady(%s) )",flag==true?"true":"false"  ) ;
				return *this ;
			}
 			emjq& 		ready			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).ready(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... readyException			
 			emjq& 		readyException			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery.readyException(%s,%s) )",std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
			

			// ********* 
			//			§mouse
			// *********	
		
			// .................................................................... click			
 			emjq& 		click			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).click(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		click			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).click(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... contextmenu			
 			emjq& 		contextmenu			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).contextmenu(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		contextmenu			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).contextmenu(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}	
			// .................................................................... dblclick			
 			emjq& 		dblclick			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).dblclick(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		dblclick			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).dblclick(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... hover			
 			emjq& 		hover			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).hover(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		hover			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).hover(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}	
			// .................................................................... mousedown			
 			emjq& 		mousedown			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).mousedown(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		mousedown			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).mousedown(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}	
			// .................................................................... mouseenter			
 			emjq& 		mouseenter			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).mouseenter(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		mouseenter			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).mouseenter(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}	
			// .................................................................... mouseleave			
 			emjq& 		mouseleave			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).mouseleave(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		mouseleave			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).mouseleave(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}	
			// .................................................................... mousemove			
 			emjq& 		mousemove			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).mousemove(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		mousemove			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).mousemove(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... mouseout			
 			emjq& 		mouseout			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).mouseout(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		mouseout			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).mouseout(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... mouseup			
 			emjq& 		mouseup			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).mouseup(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		mouseup			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).mouseup(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}				

			//

			// ............................................................................................ preventDefault  
			emjq& preventDefault  ( int _index=0 ) 
			{
				pMain=eval ( "jQuery(m[%d])[%d].preventDefault()",pMain,_index );
				return *this;
			}	
 			// ............................................................................................ stopImmediatePropagation  
			emjq& stopImmediatePropagation ( int _index=0 ) 
			{
				pMain=eval ( "jQuery(m[%d])[%d].stopImmediatePropagation()",pMain,_index  );
				return *this;
			}	
			// ............................................................................................ isDefaultPrevented  
			int isDefaultPrevented  ( int _index=0 ) 
			{
				int retVal = eval ( "jQuery(m[%d])[%d].isDefaultPrevented()",pMain,_index  );
				return retVal;
			}	
 			// ............................................................................................ isImmediatePropagationStopped  
			int isImmediatePropagationStopped  ( int _index=0 ) 
			{
				int retVal = eval ( "jQuery(m[%d])[%d].isImmediatePropagationStopped()",pMain,_index  );
				return retVal;
			}	
 			// ............................................................................................ isPropagationStopped  
			int isPropagationStopped  ( int _index=0 ) 
			{
				int retVal = eval ( "jQuery(m[%d])[%d].isPropagationStopped()",pMain,_index  );
				return retVal;
			}	
 			// ............................................................................................ stopPropagation  
			emjq& stopPropagation ( int _index=0 ) 
			{
				pMain=eval ( "jQuery(m[%d])[%d].stopPropagation()",pMain,_index  );
				return *this;
			}	

			// ********* 
			//			§browser
			// *********	
		
			// .................................................................... resize			
 			emjq& 		resize			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).resize(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		resize			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).resize(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}		

			// .................................................................... error			
 			emjq& 		error			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).error(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		error			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).error(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... scroll			
 			emjq& 		scroll			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).scroll(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		scroll			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).scroll(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}

			// ********* 
			//			§keyboard
			// *********	
		
			// .................................................................... keydown			
 			emjq& 		keydown			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).keydown(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		keydown			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).keydown(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... keyup			
 			emjq& 		keyup			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).keyup(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		keyup			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).keyup(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}			
			// .................................................................... keypress			
 			emjq& 		keypress			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).keypress(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		keypress			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).keypress(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}

			// ********* 
			//			§form
			// *********	
		
			// .................................................................... blur			
 			emjq& 		blur			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).blur(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		blur			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).blur(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... change			
 			emjq& 		change			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).change(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		change			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).change(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... focus			
 			emjq& 		focus			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).focus(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		focus			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).focus(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}			
			// .................................................................... focusin			
 			emjq& 		focusin			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).focusin(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		focusin			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).focusin(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... focusout			
 			emjq& 		focusout			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).focusout(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		focusout			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).focusout(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... select			
 			emjq& 		select			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).select(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		select			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).select(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}
			// .................................................................... submit			
 			emjq& 		submit			( type::address _data,type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).submit(%s,%s) )",pMain,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;				
				return *this ;
			}
 			emjq& 		submit			( type::address _callback ) 
			{
				pMain=eval ( "window.pointerSet ( 'object' , $(m[%d]).submit(%s) )",pMain,callbackEvent ( _callback , NULL ).c_str() )  ;				
				return *this ;
			}			

			// ********* 
			//			§HANDLER ATTACHMENT
			// *********	
		
			// ............................................................................................ on	
			
			emjq& on	( type::stringc _event,	type::stringc _selector, type::address _data,type::address _callback ) 	// $$ii
			{
				pMain=eval("jQuery(m[%d]).on(  %s , %s , %s , %s ) ;",pMain,_event,_selector,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;
				return *this;
			}
			emjq& on	( type::stringc _event,	type::stringc _selector,type::address _callback ) 						// $$_i
			{
				pMain=eval("jQuery(m[%d]).on(  %s , '%s' , %s ) ;",pMain,_event,_selector, callbackEvent ( _callback , NULL ).c_str() ) ;
				return *this;
			}	
			emjq& on	( type::stringc _event,	type::address _data,type::address _callback )							// $_ii 
			{
				pMain=eval("jQuery(m[%d]).on(  %s , %s , %s ) ;",pMain,_event,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;	
				return *this;
			}		
			emjq& on	( type::stringc _event,type::address _callback )												// $__i 
			{
				pMain=eval("jQuery(m[%d]).on(  %s ,%s ) ;",pMain,_event,callbackEvent ( _callback , NULL ).c_str() ) ;				
				return *this;
			}	
			emjq& on	( void )																						//  
			{
				pMain=eval("jQuery(m[%d]).on() ;",pMain ) ;				
				return *this;
			}

			// ............................................................................................ off	
			
			emjq& off	( type::stringc _event,	type::stringc _selector, type::address _data,type::address _callback ) 	// $$ii
			{
				pMain=eval("jQuery(m[%d]).off(  %s , %s , %s , %s ) ;",pMain,_event,_selector,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;
				return *this;
			}
			emjq& off	( type::stringc _event,	type::stringc _selector,type::address _callback ) 						// $$_i
			{
				pMain=eval("jQuery(m[%d]).off(  %s , '%s' , %s ) ;",pMain,_event,_selector, callbackEvent ( _callback , NULL ).c_str() ) ;
				return *this;
			}	
			emjq& off	( type::stringc _event,	type::address _data,type::address _callback )							// $_ii 
			{
				pMain=eval("jQuery(m[%d]).off(  %s , %s , %s ) ;",pMain,_event,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;	
				return *this;
			}		
			emjq& off	( type::stringc _event,type::address _callback )												// $__i 
			{
				pMain=eval("jQuery(m[%d]).off(  %s ,%s ) ;",pMain,_event,callbackEvent ( _callback , NULL ).c_str() ) ;				
				return *this;
			}	
			emjq& off	( void )																						//  
			{
				pMain=eval("jQuery(m[%d]).off() ;",pMain ) ;				
				return *this;
			}
			
			// ............................................................................................ one	
			
			emjq& one	( type::stringc _event,	type::stringc _selector, type::address _data,type::address _callback ) 	// $$ii
			{
				pMain=eval("jQuery(m[%d]).one(  %s , %s , %s , %s ) ;",pMain,_event,_selector,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;
				return *this;
			}
			emjq& one	( type::stringc _event,	type::stringc _selector,type::address _callback ) 						// $$_i
			{
				pMain=eval("jQuery(m[%d]).one(  %s , '%s' , %s ) ;",pMain,_event,_selector, callbackEvent ( _callback , NULL ).c_str() ) ;
				return *this;
			}	
			emjq& one	( type::stringc _event,	type::address _data,type::address _callback )							// $_ii 
			{
				pMain=eval("jQuery(m[%d]).one(  %s , %s , %s ) ;",pMain,_event,std::to_string((int)_data).c_str(), callbackEvent ( _callback , _data ).c_str() ) ;	
				return *this;
			}		
			emjq& one	( type::stringc _event,type::address _callback )												// $__i 
			{
				pMain=eval("jQuery(m[%d]).one(  %s ,%s ) ;",pMain,_event,callbackEvent ( _callback , NULL ).c_str() ) ;				
				return *this;
			}	
			emjq& one	( void )																						//  
			{
				pMain=eval("jQuery(m[%d]).one() ;",pMain ) ;				
				return *this;
			}
			// ............................................................................................ trigger	
		
			emjq& trigger	( type::stringc _event  )												 
			{
				pMain=eval("jQuery(m[%d]).trigger( %s ) ;",pMain,_event ) ;				
				return *this;
			}	
			emjq& trigger	( type::stringc _event,type::stringc _extra )												 
			{
				pMain=eval("jQuery(m[%d]).trigger( %s , %s ) ;",pMain,_event,_extra ) ;				
				return *this;
			}	
			// ............................................................................................ triggerHandler	
		
			emjq& triggerHandler	( type::stringc _event  )												 
			{
				pMain=eval("jQuery(m[%d]).triggerHandler( %s ) ;",pMain,_event ) ;				
				return *this;
			}	
			emjq& triggerHandler	( type::stringc _event,type::stringc _extra )												 
			{
				pMain=eval("jQuery(m[%d]).triggerHandler( %s , %s ) ;",pMain,_event,_extra ) ;				
				return *this;
			}	

			// ............................................................................................ proxy
			
			type::stringc proxy	( type::stringc _function,type::stringc _contextName ,type::stringc _arg=""  ) 
			{
				int retVal=EM_ASM_INT(  
				{
					var _function 		= Pointer_stringify ( $0 );
					var _contextName 	= Pointer_stringify ( $1 );			
					var _arg 			= Pointer_stringify ( $2 );

					var flag = eval ( "( typeof  "+_function+"."+_contextName+" === 'function' ) ? true : false ;" ) ;
					
					var cmd = "" ;
					var arg = "" ;

					cmd += " "+_function+"."+_contextName+"";
					if ( flag==true ) 
							cmd += _arg=="" ? "()" : "(" +_arg+ ")" ;
					else
							cmd += _arg=="" ? "" : "=" +_arg+ "" ;				
					cmd = " retVal = " + cmd;
					
					eval ( cmd );

					return window.strdup ( retVal ) ;		 
					
				}, _function,_contextName,_arg ) ;

				return (type::stringc)retVal ;
			}	

			
			// ********* 
			//			§address
			// ********* 
			
			int address( void )
			{ 
				return pMain ;
			}	
			int address( int _index  )
			{ 
				int retVal=EM_ASM_INT(
				{
					var _jQuery	= $0;
					var _index	= $1;
					
					var _value = $(m[ _jQuery ])[_index] ;
					if ( _value == undefined ) _value = null ;
					
					return window.pointerSet ( 'object' , _value ) ;
				}, pMain,_index );		
				
				return pMain=retVal ;	 
			}	
			// ********* 
			//			§pointer
			// *********

			type::stringc  pointer( void ) // ............................................. @@@ get /*OK*/
			{ 
				int retVal=EM_ASM_INT(
				{
					var _jQuery	= $0;
					var _value = jQuery(m[_jQuery]) ;
					return window.strdup ( JSON.stringify(_value) ) ;
				}, pMain );		
				
				return (type::stringc) retVal ;	 
			}	

			const char*  pointer( type::stringc _key ) // .................................. @@@ get key /*OK*/
			{ 
				int retVal=EM_ASM_INT(
				{
					var _jQuery	= $0;
					var _key	= Pointer_stringify($1);
					var _value = eval( "jQuery(m["+_jQuery+"])" + _key ); 
					return window.strdup ( JSON.stringify(_value) ) ;
				}, pMain,_key );		
				
				return (type::stringc) retVal ;
			}
			
			type::pointer  pointer( type::pointer _p ) // ................................ @@@  get Pointer of Pointer /*OK*/
			{ 
				int retVal=EM_ASM_INT(
				{
					var _jQuery	= $0;
					var _p	= $1;
					return m[_p];
				}, pMain,_p );		
				
				return (type::pointer) retVal ;
			}	
			
			// ********* 
			//			§pointerNew
			// ********* 
			
			int  pointerNew ( type::stringc _obj="null" ) 
			{
				int retVal = EM_ASM_INT (
				{
					var _obj = Pointer_stringify($0);
					
					var code="";

					code+="var p = ++m[0];";
					code+="p *= -1;";			
					code+="m[ p ] = {};";
					eval( code ) ;
					var X = eval ( _obj ) ;
					m[ p ] = X ;
					
					return 	p;
				},_obj ) ;
				return (int)retVal;
			}
			
			int  pointerNew ( type::pointer _obj ) 
			{
				int retVal = EM_ASM_INT (
				{
					var _obj = $0;
					
					var code="";
					
					code+="var p = ++m[0];";
					code+="p *= -1;";			
					code+="m[ p ] = {};";
					eval( code ) ;
					m[ p ] = _obj ;
					
					return 	p;
				},_obj ) ;
				return (int)retVal;
			}			
			// ********* 
			//			§pointer[]
			// *********

			type::stringc pointer( int _index ,type::stringc _key ) //	............................................. m[?][i] GET
			{ 
				int retVal=EM_ASM_INT(
				{
					var _jQuery	= $0;
					var _key		= Pointer_stringify($1);
					var _index		= $2;
					
					var _value = $(m[ _jQuery ])[_index][ _key ] ;

					if ( _value == undefined ) _value = null ;
					
					return window.strdup ( JSON.stringify(_value) ) ;
				}, pMain,_key,_index );		
				
				return (type::stringc) retVal ;
			}			
			// ********* 
			//			§VAR
			// *********	
			
			type::pointer  var ( type::stringc _var,type::stringc _value ) 
			{
				int retVal = EM_ASM_INT (
				{
					var _var 	= Pointer_stringify($0);
					var _value  = Pointer_stringify($1);

					var code = "";
					code+="\n var p1 = window.pointerSet ( 'object' , window['"+_var+"']="+_value+" ) ; ";	
					eval( code ) ;		
						
					return 	p1;
			 
				},_var,_value ) ;
				
				return (type::pointer)retVal ;
			}

			type::stringc var ( type::stringc _var  ) 
			{
				int retVal = EM_ASM_INT (
				{
					var _var 	= Pointer_stringify($0);
					var code="";
					code +="retVal=window['"+_var+"'] ;";
					eval(code);
					
					return 	window.strdup ( retVal.toString()  ) ;
				},_var  ) ;

				return (type::stringc)retVal;
			}	

			// ********* 
			//			§call
			// *********
	
			int pointerCall( type::stringc  _call ) // @@@ call
			{ 
				int retVal=EM_ASM_INT(
				{
					var _jQuery	= $0;
					var _call		= Pointer_stringify($1);
					
					var code ="" ;
					code += "retVal=m[ "+_jQuery+"  ]." + _call +";";
					eval ( code );

					var r = window.pointerSet ( 'object' , retVal ) ;
					return r ;
					
				}, pMain,_call);		
				
				return pMain=retVal ;
			}
			
			// ********* 
			//			§Ajax
			// *********
			
			class AjaxSettings
			{
				private:
				
					std::string settings = "" ;
	
				public:
					AjaxSettings( void )
					{
					}
				
					std::string		async 			= "" ;	 
					type::address	beforeSend		= NULL ;  	// 		f ( jqXHR , settings )
					std::string		cache			= "" ;	 
					type::address	complete		= NULL ;	// 		f ( jqXHR , textStatus )
					std::string		contents		= "" ;
					std::string		contentType		= "" ;	
					std::string		context			= "" ;	
					std::string		converters		= "" ;	
					std::string		crossDomain		= "" ;	 	
					std::string		data			= "" ;	
					type::address	dataFilter		= NULL ;	// 		f ( data , type )
					std::string		dataType		= "" ;
					type::address	error			= NULL ;  	// 		f ( jqXHR , textStatus , errorTrhown )
					std::string		global			= "" ;  	
					std::string		headers			= "" ;
					std::string		ifModified		= "" ;			
					std::string		isLocal			= "" ;
					std::string		jsonp			= "" ;	
					type::address	jsonpCallback	= NULL ; 	//		f()	
					std::string		method			= "" ;	
					std::string		mimeType		= "" ;	
					std::string		password		= "" ;
					std::string		scriptCharset	= "" ;		
					std::string		statusCode		= "" ;
					type::address	success			= NULL ;	// 		f ( data,textStatus,jqXHR )
					std::string		timeout			= "" ;
					std::string		traditional		= "" ;
					std::string		type			= "" ;
					std::string		url				= "" ;		
					std::string		username		= "" ;	
					type::address	xhr				= NULL ; 	//		f()			
					std::string		xhrFields		= "" ;

					// methods
				
				public:
				
					std::string	to_string( void )
					{
						settings = "\n{ ";
						
						if ( async!="") 				settings += "\n async : "			+ async 										+ "," ;
						if ( beforeSend!=NULL)			settings += "\n beforeSend : " 		+ callback( beforeSend  , "jqXHR , settings" )	+ "," ;
						if ( cache!="") 				settings += "\n cache : " 			+ cache 										+ "," ;
						if ( complete!=NULL)			settings += "\n complete : " 		+ callback( complete  	, "jqXHR , textStatus") + "," ;
						if ( contents!="") 				settings += "\n contents : "		+ contents 										+ "," ;
						if ( contentType!="") 			settings += "\n contentType : "		+ contentType									+ "," ;
						if ( context!="") 				settings += "\n context : "			+ context										+ "," ;
						if ( converters!="") 			settings += "\n converters : "		+ converters									+ "," ;
						if ( crossDomain!="") 			settings += "\n crossDomain : "		+ crossDomain									+ "," ;
						if ( data!="") 					settings += "\n data : "			+ data											+ "," ;			
						if ( dataFilter!=NULL)			settings += "\n dataFilter : " 		+ callback( dataFilter  	, "data , type") 	+ "," ;
						if ( dataType!="") 				settings += "\n dataType : " 		+ dataType 										+ "," ;
						if ( error!=NULL)				settings += "\n error : " 			+ callback(error,"jqXHR,textStatus,errorTrhown")+ "," ;			
						if ( global!="") 				settings += "\n global : "			+ global 										+ "," ;
						if ( headers!="") 				settings += "\n headers : "			+ headers										+ "," ;
						if ( ifModified!="") 			settings += "\n ifModified : "		+ ifModified									+ "," ;
						if ( isLocal!="") 				settings += "\n isLocal : "			+ isLocal										+ "," ;
						if ( jsonp!="") 				settings += "\n jsonp : "			+ jsonp											+ "," ;
						if ( jsonpCallback!=NULL)		settings += "\n jsonpCallback : " 	+ callback(jsonpCallback,"")					+ "," ;
						if ( method!="") 				settings += "\n method : "			+ method 										+ "," ;
						if ( mimeType!="") 				settings += "\n mimeType : "		+ mimeType										+ "," ;
						if ( password!="") 				settings += "\n password: "			+ password										+ "," ;
						if ( scriptCharset!="") 		settings += "\n scriptCharset : "	+ scriptCharset									+ "," ;
						if ( statusCode!="") 			settings += "\n statusCode : "		+ statusCode									+ "," ;			
						if ( success!=NULL)				settings += "\n success : " 		+ callback(success,"data,textStatus,jqXHR")		+ "," ;			
						if ( timeout!="") 				settings += "\n timeout : "			+ timeout 										+ "," ;
						if ( traditional!="") 			settings += "\n traditional : "		+ traditional									+ "," ;
						if ( type!="") 					settings += "\n type : "			+ type											+ "," ;
						if ( url!="") 					settings += "\n url : "				+ url											+ "," ;
						if ( username!="") 				settings += "\n username : "		+ username										+ "," ;
						if ( xhr!=NULL)					settings += "\n xhr : " 			+ callback(xhr,"")								+ "," ;
						if ( xhrFields!="") 			settings += "\n xhrFields : "		+ xhrFields 									+ "," ;
						
						settings = settings.erase( settings.size() - 1 ); // mangia l'ultimo carattere , la ',' se c'è o lo spazio iniziale;
						settings += "\n}\n";	

						//printf ("\n##\n%s\n##\n",settings.c_str());
						
						return settings  ;
					}
			}  ;			
			
			// ............................................................................................ ajax			
			emjq& 			ajax			(  AjaxSettings x )
			{
				std::string code = "";
				code +="\n jqHXR  = jQuery.ajax( "+  x.to_string()   +" );  ";
				code +="\n retVal = window.pointerSet ( 'object' , jqHXR ); ";
				pMain = eval( code.c_str() )  ;
				return *this;
			}
			emjq& 			ajax 			( type::stringc _url ,  AjaxSettings x )
			{
				std::string code = "";				
				code +="\n jqHXR  = jQuery.ajax( "+std::string(_url)+" , "+  x.to_string()  +" );  ";
				code +="\n retVal = window.pointerSet ( 'object' , jqHXR ); ";
				pMain = eval( code.c_str() )  ;
				return *this;
			}
			// ............................................................................................ ajaxComplete			
			type::pointer 	ajaxComplete 	( type::stringc _callback )
			{
				return pMain=eval("jQuery(m[%d]).ajaxComplete( %s ) ;",pMain,_callback ) ;				
			}
			// ............................................................................................ ajaxError			
			type::pointer 	ajaxError ( type::stringc _callback )
			{
				return pMain=eval("jQuery(m[%d]).ajaxError( %s ) ;",pMain,_callback ) ;
			}
			// ............................................................................................ ajaxSend				
			type::pointer 	ajaxSend ( type::stringc _callback )
			{
				return pMain=eval("jQuery(m[%d]).ajaxSend( %s ) ;",pMain,_callback ) ;
			}
			// ............................................................................................ ajaxStart			
			type::pointer 	ajaxStart ( type::stringc _callback )
			{
				return pMain=eval("jQuery(m[%d]).ajaxStart( %s ) ;",pMain,_callback ) ;
			}	
			// ............................................................................................ ajaxStop					
			type::pointer 	ajaxStop ( type::stringc _callback )
			{
				return pMain=eval("jQuery(m[%d]).ajaxStop( %s ) ;",pMain,_callback ) ;
			}
			// ............................................................................................ ajaxSuccess			
			type::pointer 	ajaxSuccess ( type::stringc _callback )
			{
				return pMain=eval("jQuery(m[%d]).ajaxSuccess( %s ) ;",pMain,_callback ) ;
			}
			// ............................................................................................ ajaxPrefilter					
			type::pointer 	ajaxPrefilter ( type::address _data, type::stringc _callback ) // TODO
			{
				printf ( "ajaxPrefilter COMING SOON\n" ); 
				return -1;
			}	
			type::pointer 	ajaxPrefilter ( type::stringc _callback ) // TODO
			{
				printf ( "ajaxPrefilter COMING SOON\n" ); 
				return -1;
			}	
			// ............................................................................................ ajaxTransport		
			type::pointer 	ajaxTransport ( type::address _data, type::stringc _callback ) // TODO
			{
				printf ( "ajaxTransport COMING SOON\n" ); 
				return -1;
			}
			// ............................................................................................ ajaxSetup				
			type::pointer 	ajaxSetup ( type::stringc _settings ) // TODO
			{
				printf ( "ajaxSetup COMING SOON\n" ); 
				return -1;
			}		

			// ............................................................................................ get		
			
			emjq& 	get ( type::stringc _url,	type::stringc _data,	type::address _success,	type::stringc _dataType	)  // _dataType = xml, json, script, text, html
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.get( %s,%s,%s,%s ) );",_data,callback ( _success , "data,textStatus,jqXHR" ).c_str(),_dataType ) ;
				return *this;
			}
			emjq& 	get ( type::stringc _url,	type::stringc _data,	type::address _success )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.get( %s,%s,%s ) );",_data,callback ( _success , "data,textStatus,jqXHR" ).c_str()  ) ;
				return *this;
			}			
			emjq& 	get ( type::stringc _url,type::address _success )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.get( %s,%s ) );",_url,callback ( _success , "data,textStatus,jqXHR" ).c_str()  ) ;
				return *this;			
			}				
			emjq& 	get ( type::stringc _url  )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.get( %s ) );",_url  ) ;
				return *this;				
			}				
			emjq& 	get ( void  )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).get() );",pMain  ) ;
				return *this;				
			}			

			// ............................................................................................ getJSON		
 
			emjq& 	getJSON  ( type::stringc _url,	type::stringc _data,	type::address _success )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.getJSON( %s,%s,%s ) );",_data,callback ( _success , "data,textStatus,jqXHR" ).c_str()  ) ;
				return *this;					
			}			
			emjq& 	getJSON ( type::stringc _url,type::address _success )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.getJSON( %s,%s ) );",_url,callback ( _success , "data,textStatus,jqXHR" ).c_str()  ) ;
				return *this;	
			}				
			emjq& 	getJSON ( type::stringc _url  )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.getJSON( %s ) );",_url  ) ;
				return *this;	
			}				
   
 			// ............................................................................................ post	
			
			emjq& 	post ( type::stringc _url,	type::stringc _data,	type::address _success,	type::stringc _dataType	)  // _dataType = xml, json, script, text, html
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.post( %s,%s,%s,%s ) );",_data,callback ( _success , "data,textStatus,jqXHR" ).c_str(),_dataType ) ;
				return *this;					
			}
			emjq& 	post ( type::stringc _url,	type::stringc _data,	type::address _success )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.post( %s,%s,%s ) );",_data,callback ( _success , "data,textStatus,jqXHR" ).c_str()  ) ;				
				return *this;	
			}			
			emjq& 	post ( type::stringc _url,type::address _success )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.post( %s,%s ) );",_url,callback ( _success , "data,textStatus,jqXHR" ).c_str()  ) ;
				return *this;	
			}				
			emjq& 	post ( type::stringc _url  )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.post( %s ) );",_url  ) ;
				return *this;	
			}				

			// ............................................................................................ getScript		

			emjq& 	getScript ( type::stringc _url,type::address _success )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.getScript( %s,%s ) );",_url,callback ( _success , "data,textStatus,jqXHR" ).c_str()  ) ;
				return *this;	
			}				
			emjq& 	getScript ( type::stringc _url  )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery.getScript( %s ) );",_url  ) ;
				return *this;	
			}	

			// ............................................................................................ load		

			emjq& 	load ( type::stringc _url,	type::stringc _data,	type::address _complete )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).load( %s,%s,%s ) );",pMain,_data,callback ( _complete , "responseText,textStatus,jqXHR" ).c_str()  ) ;
				return *this;	
			}			
			emjq& 	load ( type::stringc _url,type::address _complete )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).load( %s,%s ) );",pMain,_url,callback ( _complete , "responseText,textStatus,jqXHR" ).c_str()  ) ;
				return *this;	
			}				
			emjq& 	load ( type::stringc _url  )     
			{
				pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).load( %s ) );",pMain,_url  ) ;
				return *this;	
			}	

			
			// ............................................................................................ param		
			type::stringc param( type::stringc _obj,bool _traditional=true )  
			{ 
 				return (type::stringc)eval ( "window.strdup ( jQuery.param ( %s , %s  ) )" ,_obj , _traditional==true?"true":"false" );
			}
			// ............................................................................................ decodeURIComponent				
			type::stringc decodeURIComponent( type::stringc _obj )  
			{ 
  				return (type::stringc)eval ( "window.strdup ( decodeURIComponent ( %s ) )" ,_obj );
			}
			// ............................................................................................ serialize			
			type::stringc  serialize (void)
			{
   				return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).serialize () )" ,pMain );
			}
			// ............................................................................................ serializeArray				
			type::pointer serializeArray (void)
			{
   				//return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).serializeArray () )" ,pMain );
				return pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).serializeArray () );",pMain  ) ;				
			}			

			//******
			//		§Callbacks
			//******

			class Callbacks
			{
				private:
				
					enum $cbStatus { $null,$removed,$fired,$disabled,$locked };

					int cMain = 0;
					
					std::vector<unsigned int> 			cbList ;
					
					std::vector<$cbStatus> 				cbStatus ; 

					int _disabled=0;
					
				public:
				
				// once:   Ensures the callback list can only be fired once (like a Deferred).
				// memory: Keeps track of previous values and will call any callback added after the list has been 
				//		   fired right away with the latest "memorized" values (like a Deferred).
				// unique: Ensures a callback can only be added once (so there are no duplicates in the list).
				// stopOnFalse: Interrupts callings when a callback returns false.
				
				Callbacks  ( type::stringc _init=""   )
				{
 					cMain = eval  ( "\nwindow.pointerSet ( 'object' , $.Callbacks(%s) ) ;",_init   ) ;
				}
				// ............................................................................................ add				
				void add  ( type::address _cb  )
				{
 				    cbList.push_back ( (unsigned int) _cb );
 				    cbStatus.push_back ( $null );
					
					eval  ( "m[%d].add(%s)" ,cMain,callback (_cb , "data" ).c_str() );  
				}
				// ............................................................................................ fire				
				void fire ( type::address _arg=NULL  )
				{
					for (int i=0; i<cbList.size(); i++)
					{
						if ( _disabled==0 )
						{
							if  ( ( cbStatus[i]==$null	)  || ( cbStatus[i]==$fired ) )					
							{
								eval ( "Runtime.dynCall ( 'ii',%u, [%d] );" ,cbList[i],(int)_arg )  ;
								cbStatus[i] = $fired ;
							}
						}
					}
				}
				// ............................................................................................ fired
				int fired( type::address _cb  )
				{
					for (int i=0; i<cbList.size(); i++)
					{
						if ( cbStatus[i] == $fired ) return 1;
					}					
					return 0 ;
				}					
				// ............................................................................................ has	
				int has( type::address _cb  )
				{
					for (int i=0; i<cbList.size(); i++)
					{
						if ( cbList[i] == (unsigned int)_cb ) return 1;
					}					
					return 0 ;
				}				
				// ............................................................................................ disable					
				void disable ( void  )
				{
					_disabled=true;
				}
				// ............................................................................................ disabled
				int disabled( void  )
				{
					return _disabled;
				}					
				// ............................................................................................ lock					
				int lock ( type::address _cb  )
				{
					for (int i=0; i<cbList.size(); i++)
					{
						if ( cbList[i] == (unsigned int)_cb ) 
						{
								cbStatus[i] = $locked ;
								return 1 ;
						}
					}
					return 0 ;	
				}
				// ............................................................................................ locked
				int locked( type::address _cb  )
				{
					for (int i=0; i<cbList.size(); i++)
					{
						if ( cbStatus[i] == $locked ) return 1;
					}					
					return 0 ;
				}		
				// ............................................................................................ remove					
				int remove ( type::address _cb  )
				{
					for (int i=0; i<cbList.size(); i++)
					{
						if ( cbList[i] == (unsigned int)_cb ) 
						{
								cbStatus[i] = $removed ;
								return 1 ;
						}
					}
					return 0 ;	
				}
				// ............................................................................................ removed
				int removed( type::address _cb  )
				{
					for (int i=0; i<cbList.size(); i++)
					{
						if ( cbStatus[i] == $removed ) return 1;
					}					
					return 0 ;
				}	
				// ............................................................................................ empty	
				int empty ( void  )
				{
					cbStatus.clear();
					cbList.clear();
					return cbList.size();
				}	
				// ............................................................................................ fireWith
				int fireWith( type::address _cb , type::address _arg  )
				{
					fire();
 					eval ( "Runtime.dynCall ( 'ii',%u, [%d] );" ,_cb,(int)_arg )  ;
					return 0 ;
				}
			 
			}  ;
			
			//******
			//		§Deferred
			//******
				
				// ............................................................................................ Deferred	
				type::stringc  Deferred	(void)
				{
					return (type::stringc) "jQuery.Deferred()" ;
				}	
				type::stringc  Deferred	( type::address _callback )		
				{
					std::string code = "jQuery.Deferred(" + std::string( callback ( _callback , "_data , _testStatus , _jqXHR" ) ) + ")" ;

					return (type::stringc) code.c_str() ;
				} 		
				// ............................................................................................ when		
				type::stringc function		( type::address _cb,type::stringc _arg )	
				{
					return callback ( _cb,_arg ).c_str() ;
				}
				// ............................................................................................ when		
				emjq& when		( type::stringc _arg )	
				{
					templateDeferred ( "when",_arg,""   ); 	 			
					return *this;
				}
				emjq&  when		( int _p ) 	
				{
					pMain = _p ;			
					return *this;
				}
				emjq& when		( type::thenable _thenable )  	
				{
					pMain =  _thenable() ;
					return *this;
				}
				// ............................................................................................ done		
				emjq& done		( type::stringc _arg )	
				{
					templateDeferred ( "done",_arg,""   ); 	 			
					return *this;
				}
				// ............................................................................................ resolve 	
				emjq& resolve	( type::stringc _arg="" )	
				{
					templateDeferred ( "resolve",_arg,""   ); 	 			
					return *this;
				}	
				// ............................................................................................ always	
				emjq& always	( type::stringc _arg )	
				{
					templateDeferred ( "always",_arg,""   ); 	 			
					return *this;
				}
				// ............................................................................................ Catch	
				emjq& Catch	( type::stringc _arg )	
				{
					templateDeferred ( "catch",_arg,""   ); 	 			
					return *this;
				}
				// ............................................................................................ fail	
				emjq& fail	( type::stringc _arg )	
				{
					templateDeferred ( "fail",_arg,""   ); 	 			
					return *this;
				}
				// ............................................................................................ resolveWith		
				emjq& resolveWith	( type::stringc _context,type::stringc _arg )	
				{
					templateDeferred ( "resolveWith",_arg,_context   ); 	 			
					return *this;
				}
				// ............................................................................................ notify 		
				emjq& notify	( type::stringc _arg )	
				{
					templateDeferred ( "notify",_arg,""   ); 	 			
					return *this;
				}
				// ............................................................................................ notifyWith	
				emjq& notifyWith	( type::stringc _context,type::stringc _arg )	
				{
					templateDeferred ( "notifyWith",_arg,_context   ); 	 			
					return *this;
				}
				// ............................................................................................ reject 	
				emjq& reject	( type::stringc _arg )	
				{
					templateDeferred ( "reject",_arg,""   ); 	 			
					return *this;
				}
				// ............................................................................................ rejectWith		
				emjq& rejectWith	( type::stringc _context,type::stringc _arg )	
				{
					templateDeferred ( "rejectWith",_arg,_context   ); 	 			
					return *this;
				}	
				
				// ............................................................................................ template Deferred
				int templateDeferred ( type::stringc _method, type::stringc _arg,type::stringc _context )
				{
					int retVal=EM_ASM_INT(
					{
						var _jQuery 	= $0 ;			
						var _method   	= Pointer_stringify($1) ;
						var _arg	   	= Pointer_stringify($2) ; 
						var _context	= Pointer_stringify($3) ;
						
						if ( _context != "" ) _context += " , " ;
						
						code ="\n var retVal=_jQuery;";	
						
						// .......................................... done , resolve
						if ( _method!= 'when' )
						{
							code +="\n m["+_jQuery+"]."+_method+"( "+_context+ " " + _arg +" );";
							eval(code);
							return retVal ;				
						}
						// .......................................... when
						if ( _method== 'when' )
						{
							code +="\n retVal=jQuery.when("+_arg+");" ;
							eval(code);
							return window.pointerSet('object',retVal ) ;				
						}
					
					}, pMain,_method,_arg,_context );	
					
					return pMain=retVal;
				}
				
				// ............................................................................................ then		
				
				emjq& then	( type::stringc _done,type::stringc _fail,type::stringc _progress )	
				{
					int retVal=EM_ASM_INT(
					{
						var _jQuery 	= $0 ;			
						var _done	   	= Pointer_stringify($1) ; 
						var _fail	   	= Pointer_stringify($2) ; 
						var _progress  	= Pointer_stringify($3) ; 
						
						var code ="";	

						var _arg = "" ;
						if ( _done != "" ) 		_arg += " " + _done + " " ;
						if ( _fail != "" )      _arg += "," + _fail + " " ;
						if ( _progress != "" )  _arg += "," + _progress + " " ;						
						
						code +="\n m["+_jQuery+"].then( " + _arg +" );";
						
						eval(code);
						
						return -1;				
					}, pMain, _done,_fail,_progress  );	
					return *this;
				}
				
				// ............................................................................................ promise
				
				int promise	( type::stringc _arg="" )	
				{
					int retVal=EM_ASM_INT(
					{
						var _jQuery 	= $0 ;			
						var _arg	   	= Pointer_stringify($1) ; 

						var code ="";	
						code +="\n retVal=m["+_jQuery+"].promise( " + _arg +" );";
						eval(code);
						
						return window.pointerSet ( 'object' , retVal )  ;				
					}, pMain, _arg  );	
					
					return retVal;
				}	

				//******
				//		§attribute  §prop
				//******

					// .................................................................... val
					
					type::stringc 	val( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).val () )",pMain   );
					}
					emjq& 	val( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).val ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	val( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).val ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}	
					// .................................................................... removeProp

					type::stringc 	removeProp( type::stringc _obj  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).removeProp (%s) )",pMain,_obj   );
					}
					// .................................................................... prop

					type::stringc 	prop( type::stringc _obj  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).prop (%s) )",pMain,_obj   );
					}
					type::stringc 	prop( type::stringc _obj,type::address _cb  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).prop (%s,%s) )",pMain,_obj,callback ( _cb , "_this=this" ).c_str()   );
					}
					type::stringc 	prop( type::stringc _obj1,type::stringc _obj2  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).prop (%s,%s) )",pMain,_obj1,_obj2   );
					}
					// .................................................................... removeAttr

					type::stringc 	removeAttr( type::stringc _obj  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).removeAttr (%s) )",pMain,_obj   );
					}

					// .................................................................... attr

					type::stringc 	attr( type::stringc _obj  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).attr (%s) )",pMain,_obj   );
					}
					type::stringc 	attr( type::stringc _obj,type::address _cb  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).attr (%s,%s) )",pMain,_obj,callback ( _cb , "_this=this" ).c_str()   );
					}
					type::stringc 	attr( type::stringc _obj1,type::stringc _obj2  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).attr (%s,%s) )",pMain,_obj1,_obj2   );
					}

					// .................................................................... html
					
					type::stringc 	html( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).html () )",pMain   );
					}
					emjq& 	html( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).html ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	html( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).html ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}	
					// .................................................................... text
					
					type::stringc 	text( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).text () )",pMain   );
					}
					emjq& 	text( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).text ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	text( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).text ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}
					
					// .................................................................... addClass 12345
					
					emjq& 	addClass( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).addClass ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	addClass( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).addClass ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}	
					// .................................................................... removeClass 12345
					
					emjq& 	removeClass( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).removeClass ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	removeClass( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).removeClass ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}	

					// .................................................................... hasClass  
					
					type::stringc 	hasClass( type::stringc _obj  )		
					{
						return (type::stringc)eval ( "window.strdup (  jQuery(m[%d]).hasClass (%s) )",pMain,_obj   );
					}
					// .................................................................... height 12345
					
					type::stringc 	height( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).height () )",pMain   );
					}
					emjq& 	height( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).height ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	height( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).height ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}	
					// .................................................................... width 12345
					
					type::stringc 	width( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).width () )",pMain   );
					}
					emjq& 	width( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).width ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	width( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).width ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}	
					// .................................................................... innerWidth
					
					type::stringc 	innerWidth( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).innerWidth () )",pMain   );
					}
					emjq& 	innerWidth( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).innerWidth ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	innerWidth( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).innerWidth ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}	
					// .................................................................... innerHeight  
					
					type::stringc 	innerHeight( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).innerHeight () )",pMain   );
					}
					emjq& 	innerHeight( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).innerHeight ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	innerHeight( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).innerHeight ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}	
					// .................................................................... outerWidth 12345
					
					type::stringc 	outerWidth( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).outerWidth () )",pMain   );
					}
					emjq& 	outerWidth( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).outerWidth ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	outerWidth( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).outerWidth ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}	
					// .................................................................... outerHeight  
					
					type::stringc 	outerHeight( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).outerHeight () )",pMain   );
					}
					emjq& 	outerHeight( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).outerHeight ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	outerHeight( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).outerHeight ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}
					// .................................................................... offset  
					type::stringc 	offset( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( JSON.stringify(jQuery(m[%d]).offset ()) )",pMain   );
					}
					emjq& 	offset( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).offset ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	offset( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).offset ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}	
					// .................................................................... position  
					
					type::stringc 	position( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( JSON.stringify(jQuery(m[%d]).position ()) )",pMain   );
					}
					// .................................................................... scrollLeft 12345
					
					type::stringc 	scrollLeft( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).scrollLeft () )",pMain   );
					}
					emjq& 	scrollLeft( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).scrollLeft ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					// .................................................................... scrollTop 12345
					
					type::stringc 	scrollTop( void  )		
					{
						return (type::stringc)eval ( "window.strdup ( jQuery(m[%d]).scrollTop () )",pMain   );
					}
					emjq& 	scrollTop( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).scrollTop ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					// .................................................................... toggleClass 12345
	
					emjq& 	toggleClass( type::stringc _obj1  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).toggleClass ( %s ) )",pMain,_obj1    );
						return *this ;
					}
					emjq& 	toggleClass( type::stringc _obj1,bool _flag  )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).toggleClass ( %s,%s ) )",pMain,_obj1,_flag==true?"true":"false"    );
						return *this ;
					}
					emjq& 	toggleClass( type::address _cb )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).toggleClass ( %s ) )",pMain,callback ( _cb , "_this=this" ).c_str()  );
						return *this ;
					}	
					emjq& 	toggleClass( type::address _cb,bool _flag )			
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).toggleClass ( %s,%s ) )",pMain,callback ( _cb , "_this=this" ).c_str(),_flag==true?"true":"false"  );
						return *this ;
					}	

				//******
				//		§data
				//*******
				
					// ............................................................................................ data() 	
					emjq& 			data ( type::stringc _key , type::stringc _value )
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery(m[%d]).data ( %s,%s ) )",pMain,_key,_value   );
						return *this ;
					}
					type::stringc 	data( type::stringc _key  )		
					{
						return (type::stringc)eval ( "window.strdup ( JSON.stringify(jQuery(m[%d]).data (%s)) )",pMain,_key   );
					} 
					type::stringc 	data( void )		
					{
						return (type::stringc)eval ( " var retVal=jQuery(m[%d]).data () ;  window.strdup ( JSON.stringify( retVal==undefined?'{}':retVal  ) )",pMain   );
					} 
					// ............................................................................................ $.data 	
					emjq& 			data ( type::pointer _obj,type::stringc _key , type::stringc _value )
					{
						pMain=eval ( "window.pointerSet ( 'object' ,jQuery.data ( m[%d],%s,%s ) )",_obj,_key,_value   );
						return *this ;
					}
					type::stringc 	data( type::pointer _obj,type::stringc _key  )		
					{
						return (type::stringc)eval ( "window.strdup ( JSON.stringify(jQuery.data (m[%d],%s)) )",_obj,_key   );
					} 
					type::stringc 	data( type::pointer _obj )		
					{
						return (type::stringc)eval ( " var retVal=jQuery.data (m[%d]) ;  window.strdup ( JSON.stringify( retVal==undefined?'{}':retVal  ) )",_obj   );
					} 
 					// ............................................................................................ removeData 	
					type::stringc 	removeData( type::stringc _key  )		
					{
						return (type::stringc)eval ( "window.strdup ( JSON.stringify(jQuery(m[%d]).removeData (%s)) )",pMain,_key   );
					} 
					type::stringc 	removeData( type::pointer _obj,type::stringc _key  )		
					{
						return (type::stringc)eval ( "window.strdup ( JSON.stringify(jQuery.removeData (m[%d],%s)) )",_obj,_key   );
					} 					
					// ............................................................................................ hasData 	
					type::stringc 	hasData( type::stringc _key  )		
					{
						return (type::stringc)eval ( "window.strdup ( JSON.stringify(jQuery(m[%d]).hasData (%s)) )",pMain,_key   );
					} 
					type::stringc 	hasData( type::pointer _obj )		
					{
						return (type::stringc)eval ( " var retVal=jQuery.hasData (m[%d]) ;  window.strdup ( JSON.stringify( retVal==undefined?'{}':retVal  ) )",_obj   );
					} 				
			
				// ********* 
				//			§effects
				// ********* 
						
					// ----------------------------------------------------------------------------------------------------------------- Animate
					emjq& animate 
					( type::stringc _propertiesc	,	int _duration=400	,	type::stringc _easingc="'swing'"	,	type::address _fcomplete=NULL,type::stringc _argc="" )
					{
						std::string _properties = std::string (_propertiesc ) ;
						std::string _easing		= std::string (_easingc ) ;
						std::string _arg		= std::string (_argc ) ;
						
						std::cout << "animate #1\n";			
						std::string properties 	= _properties ;
						std::string duration 	=  std::to_string(_duration) ;
						std::string easing 		= _easing ;
						
						std::string fcomplete 	= callback ( _fcomplete , _argc ) ;
						ReplaceStringInPlace(fcomplete, "\"<<<", "" ) ; 
						ReplaceStringInPlace(fcomplete, ">>>\"", "" ) ; 

						eval ( "jQuery(m[%d]).animate ( %s , %s , %s , %s )",pMain , properties.c_str() , duration.c_str() , easing.c_str() , fcomplete.c_str()  );
						
						return *this;			
					}
					emjq& animate ( type::stringc _propertiesc="{}"	,	type::stringc _optionsc="{}"  )
					{
						std::string _properties	= std::string ( _propertiesc ) ;
						std::string _options	= std::string ( _optionsc ) ;
						
						//std::cout << "animate #2\n";	
						
						std::string properties 	= _properties ;
						ReplaceStringInPlace(properties, "\"<<<", "" ) ; 
						ReplaceStringInPlace(properties, ">>>\"", "" ) ; 			
						
						std::string options 	= _options ;
						ReplaceStringInPlace(options, "\"<<<", "" ) ; 
						ReplaceStringInPlace(options, ">>>\"", "" ) ; 
						
						//std::cout << "\n" << properties << "\n" << options << "\n" ;

						eval ( "jQuery(m[%d]).animate ( %s , %s  )",pMain , properties.c_str() , options.c_str()  );
					
						return *this;			
					}
				 
					// ------------------------------------------------------------------------------------------------------------------------------ §effects	
					// effects  ( int 		string 	address=NULL )
					// effects  ( string   address)
					// effects  ( string  )
					// effects  ( void )

					// ------------------------------------------------------------------------------------------------------------------------------ fadeIn		
					emjq& 				fadeIn ( int _duration	,	type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg="" )
					{
						templateEffects (  "fadeIn" , NULL ,_duration, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				fadeIn ( type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg=""  )
					{
						templateEffects (  "fadeIn" , NULL ,0, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				fadeIn ( type::json _options  )
					{
						templateEffects (  "fadeIn" , _options ,0, "", NULL , "" ) ;
						return *this;			
					}
					emjq& 				fadeIn ( void  )
					{
						templateEffects (  "fadeIn" , NULL, 0, "", NULL , "" ) ;
						return *this;			
					}	
					// ------------------------------------------------------------------------------------------------------------------------------ fadeOut		
					emjq& 				fadeOut( int _duration	,	type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg="" )
					{
						templateEffects (  "fadeOut" , NULL ,_duration, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				fadeOut( type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg=""  )
					{
						templateEffects (  "fadeOut" , NULL ,0, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				fadeOut( type::json _options  )
					{
						templateEffects (  "fadeOut" , _options ,0, "", NULL , "" ) ;
						return *this;			
					}
					emjq& 				fadeOut( void  )
					{
						templateEffects (  "fadeOut" , NULL, 0, "", NULL , "" ) ;
						return *this;			
					}
					// ------------------------------------------------------------------------------------------------------------------------------ slideUp		
					emjq& 				slideUp( int _duration	,	type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg="" )
					{
						templateEffects (  "slideUp" , NULL ,_duration, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				slideUp( type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg=""  )
					{
						templateEffects (  "slideUp" , NULL ,0, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				slideUp( type::json _options  )
					{
						templateEffects (  "slideUp" , _options ,0, "", NULL , "" ) ;
						return *this;			
					}
					emjq& 				slideUp( void  )
					{
						templateEffects (  "slideUp" , NULL, 0, "", NULL , "" ) ;
						return *this;			
					}
					// ------------------------------------------------------------------------------------------------------------------------------ slideDown		
					emjq& 				slideDown( int _duration	,	type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg="" )
					{
						templateEffects (  "slideDown" , NULL ,_duration, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				slideDown( type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg=""  )
					{
						templateEffects (  "slideDown" , NULL ,0, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				slideDown( type::json _options  )
					{
						templateEffects (  "slideDown" , _options ,0, "", NULL , "" ) ;
						return *this;			
					}
					emjq& 				slideDown( void  )
					{
						templateEffects (  "slideDown" , NULL, 0, "", NULL , "" ) ;
						return *this;			
					}
					// ------------------------------------------------------------------------------------------------------------------------------ fadeToggle		
					emjq& 				fadeToggle( int _duration	,	type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg="" )
					{
						templateEffects (  "fadeToggle" , NULL ,_duration, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				fadeToggle( type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg=""  )
					{
						templateEffects (  "fadeToggle" , NULL ,0, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				fadeToggle( type::json _options  )
					{
						templateEffects (  "fadeToggle" , _options ,0, "", NULL , "" ) ;
						return *this;			
					}
					emjq& 				fadeToggle( void  )
					{
						templateEffects (  "fadeToggle" , NULL, 0, "", NULL , "" ) ;
						return *this;			
					}
					// ------------------------------------------------------------------------------------------------------------------------------ slideToggle		
					emjq& 				slideToggle( int _duration	,	type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg="" )
					{
						templateEffects (  "slideToggle" , NULL ,_duration, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				slideToggle( type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg=""  )
					{
						templateEffects (  "slideToggle" , NULL ,0, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				slideToggle( type::json _options  )
					{
						templateEffects (  "slideToggle" , _options ,0, "", NULL , "" ) ;
						return *this;			
					}
					emjq& 				slideToggle( void  )
					{
						templateEffects (  "slideToggle" , NULL, 0, "", NULL , "" ) ;
						return *this;			
					}
					// ------------------------------------------------------------------------------------------------------------------------------ hide		
					emjq& 				hide( int _duration	,	type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg="" )
					{
					templateEffects (  "hide" , NULL ,_duration, _easing, _fcomplete,_arg ) ;
					return *this;			
					}
					emjq& 				hide( type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg=""  )
					{
						templateEffects (  "hide" , NULL ,0, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				hide( type::json _options  )
					{
						templateEffects (  "hide" , _options ,0, "", NULL , "" ) ;
						return *this;			
					}
					emjq& 				hide( void  )
					{
						templateEffects (  "hide" , NULL, 0, "", NULL , "" ) ;
						return *this;			
					}
					// ------------------------------------------------------------------------------------------------------------------------------ show		
					emjq& 				show( int _duration	,	type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg="" )
					{
						templateEffects (  "show" , NULL ,_duration, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				show( type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg=""  )
					{
						templateEffects (  "show" , NULL ,0, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				show( type::json _options  )
					{
						templateEffects (  "show" , _options ,0, "", NULL , "" ) ;
						return *this;			
					}
					emjq& 				show( void  )
					{
						templateEffects (  "show" , NULL, 0, "", NULL , "" ) ;
						return *this;			
					}
					// ------------------------------------------------------------------------------------------------------------------------------ speed		
					emjq& 				speed( int _duration	,	type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg="" )
					{
						templateEffects (  "speed" , NULL ,_duration, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				speed( type::stringc _easing	,	type::address _fcomplete=NULL,type::stringc _arg=""  )
					{
						templateEffects (  "speed" , NULL ,0, _easing, _fcomplete,_arg ) ;
						return *this;			
					}
					emjq& 				speed( type::json _options  )
					{
						templateEffects (  "speed" , _options ,0, "", NULL , "" ) ;
						return *this;			
					}
					emjq& 				speed( void  )
					{
						templateEffects (  "speed" , NULL, 0, "", NULL , "" ) ;
						return *this;			
					}

					// ----------------------------------------------------------------------------------------------------------------- fx
					class _fx
					{
						public:

						void off ( bool flag=true )
						{
							std::string temp = ( flag == true ) ? "true" : "false" ;
							eval ( "jQuery.fx.off = %s",temp.c_str() )  ;
						}
						void interval ( int _interval=1000 )
						{
							eval ( "jQuery.fx.interval = %s",std::to_string(_interval).c_str() )  ;
						}				
					} fx ;
					
					// ----------------------------------------------------------------------------------------------------------------- delay
					emjq& 				delay ( int _duration , type::stringc _queue="'fx'"  )
					{
						eval (  "jQuery( m[%d] ).delay( %d , %s )  ",pMain,_duration,_queue ) ;
						return *this;			
					}
					// ----------------------------------------------------------------------------------------------------------------- delay
					emjq& 				delay ( type::stringc x , type::stringc _queue="'fx'"  )
					{
						eval (  "jQuery( m[%d] ).delay( %s , %s )  ",pMain,x,_queue ) ;
						return *this;			
					}			
					// ----------------------------------------------------------------------------------------------------------------- clearQueue
					emjq& 				clearQueue ( type::stringc _queue="'fx'"  )
					{
						eval (  "jQuery( m[%d] ).clearQueue( %s )  ",pMain,_queue ) ;
						return *this;			
					}			
					// -----------------------------------------------------------------------------------------------------------------finish
					emjq& 				finish ( type::stringc _queue="'fx'"  )
					{
						eval (  "jQuery( m[%d] ).finish( %s )  ",pMain,_queue ) ;
						return *this;			
					}	
					// ----------------------------------------------------------------------------------------------------------------- stop
					emjq& 				stop( type::stringc _queue="'fx'" , bool _clearQueue=false , bool _jumpToEnd=false  )
					{
					type::stringc  clearQueue = _clearQueue==true ? "true" : "false" ;
					type::stringc  jumpToEnd  = _jumpToEnd==true ? "true" : "false" ;
					
					eval (  "jQuery( m[%d] ).stop( %s,%s,%s )  ",pMain,_queue,clearQueue,jumpToEnd ) ;
					return *this;			
					}	
					// ----------------------------------------------------------------------------------------------------------------- stop
					emjq& 				stop( bool _clearQueue=false , bool _jumpToEnd=false  )
					{
						type::stringc  clearQueue = _clearQueue==true ? "true" : "false" ;
						type::stringc  jumpToEnd  = _jumpToEnd==true ? "true" : "false" ;
						
						eval (  "jQuery( m[%d] ).stop( %s,%s )  ",pMain,clearQueue,jumpToEnd ) ;
						return *this;			
					}	
					// ------------------------------------------------------------------------------------------------------------------------------ fadeTo		
					emjq& 				fadeTo( type::stringc	_duration,	float _opacity	 )
					{
						eval (  "jQuery( m[%d] ).fadeTo( %s , %s )  ",pMain,_duration, std::to_string(_opacity).c_str()  ) ;
						return *this;			
					}
					// ------------------------------------------------------------------------------------------------------------------------------ queue		
					int 	queue( type::stringc _queue="'fx'" 	 )	{
						return eval (  "window.pointerSet('_queueect',jQuery( m[%d] ).queue( %s ))  ",pMain,_queue   ) ;
					}
					emjq& 	queue( type::address _callback,type::stringc _arg=""	 )	{
						eval (  "jQuery( m[%d] ).queue(%s)",pMain , callback ( _callback , _arg ).c_str()   ) ;
						return *this ;
					}		
					emjq& 	queue( type::stringc _queue ,type::address _callback,type::stringc _arg=""		 )	{
						eval (  "jQuery( m[%d] ).queue(%s,%s)",pMain , _queue,callback ( _callback , _arg ).c_str()   ) ;
						return *this ;
					}	
					emjq& 	queue( type::stringc _queue1 ,type::stringc _queueNew	 )	{
						eval (  "jQuery( m[%d] ).queue(%s,%s)",pMain , _queue1, _queueNew   ) ;
						return *this ;
					}	
					// ------------------------------------------------------------------------------------------------------------------------------ dequeue		
					int 	dequeue( type::stringc _dequeue="'fx'" 	 )	{
						return eval (  "window.pointerSet('_dequeueect',jQuery( m[%d] ).dequeue( %s ))  ",pMain,_dequeue   ) ;
					}
					emjq& 	dequeue( type::address _callback,type::stringc _arg=""	 )	{
						eval (  "jQuery( m[%d] ).dequeue(%s)",pMain , callback ( _callback , _arg ).c_str()   ) ;
						return *this ;
					}		
					emjq& 	dequeue( type::stringc _dequeue ,type::address _callback,type::stringc _arg=""		 )	{
						eval (  "jQuery( m[%d] ).dequeue(%s,%s)",pMain , _dequeue,callback ( _callback , _arg ).c_str()   ) ;
						return *this ;
					}	
					emjq& 	dequeue( type::stringc _dequeue1 ,type::stringc _dequeueNew	 )	{
						eval (  "jQuery( m[%d] ).dequeue(%s,%s)",pMain , _dequeue1, _dequeueNew   ) ;
						return *this ;
					}		
						
					// *********
					// 			§templateEffects
					// *********
							
					void templateEffects 
					( std::string _method, type::json _properties , int _duration , std::string _easing ,	type::address _fcomplete , std::string _arg )
					{
						// ............................................. method
						std::string method 	   = _method ;
						// ............................................. properties
						std::string properties ;
						if ( _properties == NULL )
						{
							properties = "{}" ;
						}
						else
						{
							properties = _properties.dump() ;
							ReplaceStringInPlace(properties, "\"<<<", "" ) ; 
							ReplaceStringInPlace(properties, ">>>\"", "" ) ; 
						}
						// ............................................. duration
						std::string duration = std::to_string ( _duration) ;
						// ............................................. easing			
						std::string easing 	   = _easing ;	
						if ( _easing == "" )
							easing="'swing'";
						else 
							easing=_easing;
						// ............................................. fcomplete						
						std::string fcomplete 	= callback ( _fcomplete , _arg.c_str() ) ;
						ReplaceStringInPlace(fcomplete, "\"<<<", "" ) ; 
						ReplaceStringInPlace(fcomplete, ">>>\"", "" ) ; 

						std::string code = "" ;

						if ( properties == "{}" )
							code = "jQuery(m[ " +std::to_string(pMain) + " ] )."+_method+"( {} , " + duration + " , " + easing + ", " +  fcomplete +" ); ";
						else
							code = "jQuery(m[ " +std::to_string(pMain) + " ] )."+_method+"( JSON.parse('" + properties + "') )";				
						
						eval ( code.c_str() ) ;
					}		

				// ********* 
				//			§manipulation
				// *********

					// .................................................................... clone
					
					emjq& 			clone		(  bool _withDataAndEvents=false , bool _deepWithDataAndEvents=false  )
					{
						eval ( "jQuery(m[%d]).clone( %s , %s  )",pMain , _withDataAndEvents==true?"true":"false",_deepWithDataAndEvents==true?"true":"false"   );
						return *this ;
					}
					// .................................................................... append
					emjq& 			append		( type::address _callback,type::stringc _arg=""  )			
					{
						eval ( "jQuery(m[%d]).append( %s )",pMain , callback ( _callback , _arg  ).c_str() ); 
						return *this ;
					}
					emjq& 			append		( int _content1 , int _content2=-1  )		
					{
						if (_content2!=-1) eval ( "jQuery(m[%d]).append( m[%d] )",pMain , _content1  );
						if (_content2>= 0) eval ( "jQuery(m[%d]).append( m[%d] , m[%d] )",pMain , _content1 , _content2  );				
						return *this ;
					}
					emjq& 			append		( int _content1 , type::stringc _content2   )		
					{
						eval ( "jQuery(m[%d]).append( m[%d] ,%s )",pMain , _content1 , _content2  );
						return *this ;
					}
					emjq& 			append		( type::stringc _content1, int _content2  )			
					{
						eval ( "jQuery(m[%d]).append( %s , m[%d] )",pMain , _content1 , _content2  );
						return *this ;
					}
					emjq& 			append		( type::stringc _content1, type::stringc _content2=NULL  )			
					{
						if ( _content2==NULL) 	eval ( "jQuery(m[%d]).append( %s  )",pMain , _content1    );				
						if ( _content2!=NULL) 	eval ( "jQuery(m[%d]).append( %s , %s )",pMain , _content1 , _content2  );
						return *this ;
					}
					// .................................................................... after
					emjq& 			after		( type::address _callback,type::stringc _arg=""  )			
					{
						eval ( "jQuery(m[%d]).after( %s )",pMain , callback ( _callback , _arg  ).c_str() ); 
						return *this ;
					}
					emjq& 			after		( int _content1 , int _content2=-1  )		
					{
						if (_content2!=-1) eval ( "jQuery(m[%d]).after( m[%d] )",pMain , _content1  );
						if (_content2>= 0) eval ( "jQuery(m[%d]).after( m[%d] , m[%d] )",pMain , _content1 , _content2  );				
						return *this ;
					}
					emjq& 			after		( int _content1 , type::stringc _content2   )		
					{
						eval ( "jQuery(m[%d]).after( m[%d] ,%s )",pMain , _content1 , _content2  );
						return *this ;
					}
					emjq& 			after		( type::stringc _content1, int _content2  )			
					{
						eval ( "jQuery(m[%d]).after( %s , m[%d] )",pMain , _content1 , _content2  );
						return *this ;
					}
					emjq& 			after		( type::stringc _content1, type::stringc _content2=NULL  )			
					{
						if ( _content2==NULL) 	eval ( "jQuery(m[%d]).after( %s  )",pMain , _content1    );				
						if ( _content2!=NULL) 	eval ( "jQuery(m[%d]).after( %s , %s )",pMain , _content1 , _content2  );
						return *this ;
					}
					// .................................................................... before
					emjq& 			before		( type::address _callback,type::stringc _arg=""  )			
					{
						eval ( "jQuery(m[%d]).before( %s )",pMain , callback ( _callback , _arg  ).c_str() ); 
						return *this ;
					}
					emjq& 			before		( int _content1 , int _content2=-1  )		
					{
						if (_content2!=-1) eval ( "jQuery(m[%d]).before( m[%d] )",pMain , _content1  );
						if (_content2>= 0) eval ( "jQuery(m[%d]).before( m[%d] , m[%d] )",pMain , _content1 , _content2  );				
						return *this ;
					}
					emjq& 			before		( int _content1 , type::stringc _content2   )		
					{
						eval ( "jQuery(m[%d]).before( m[%d] ,%s )",pMain , _content1 , _content2  );
						return *this ;
					}
					emjq& 			before		( type::stringc _content1, int _content2  )			
					{
						eval ( "jQuery(m[%d]).before( %s , m[%d] )",pMain , _content1 , _content2  );
						return *this ;
					}
					emjq& 			before		( type::stringc _content1, type::stringc _content2=NULL  )			
					{
						if ( _content2==NULL) 	eval ( "jQuery(m[%d]).before( %s  )",pMain , _content1    );				
						if ( _content2!=NULL) 	eval ( "jQuery(m[%d]).before( %s , %s )",pMain , _content1 , _content2  );
						return *this ;
					}
					// .................................................................... prepend
					emjq& 			prepend		( type::address _callback,type::stringc _arg=""  )			
					{
						eval ( "jQuery(m[%d]).prepend( %s )",pMain , callback ( _callback , _arg  ).c_str() ); 
						return *this ;
					}
					emjq& 			prepend		( int _content1 , int _content2=-1  )		
					{
						if (_content2!=-1) eval ( "jQuery(m[%d]).prepend( m[%d] )",pMain , _content1  );
						if (_content2>= 0) eval ( "jQuery(m[%d]).prepend( m[%d] , m[%d] )",pMain , _content1 , _content2  );				
						return *this ;
					}
					emjq& 			prepend		( int _content1 , type::stringc _content2   )		
					{
						eval ( "jQuery(m[%d]).prepend( m[%d] ,%s )",pMain , _content1 , _content2  );
						return *this ;
					}
					emjq& 			prepend		( type::stringc _content1, int _content2  )			
					{
						eval ( "jQuery(m[%d]).prepend( %s , m[%d] )",pMain , _content1 , _content2  );
						return *this ;
					}
					emjq& 			prepend		( type::stringc _content1, type::stringc _content2=NULL  )			
					{
						if ( _content2==NULL) 	eval ( "jQuery(m[%d]).prepend( %s  )",pMain , _content1    );				
						if ( _content2!=NULL) 	eval ( "jQuery(m[%d]).prepend( %s , %s )",pMain , _content1 , _content2  );
						return *this ;
					}

					// .................................................................... appendTo			
					int 		appendTo		( type::stringc _target  )			
					{
						return eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).appendTo( %s ) )",pMain ,_target  );
					}
					int 		appendTo		( int _target  )			
					{
						return eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).appendTo( m[%d] ) )",pMain,_target   );
					}
					// .................................................................... prependTo			
					int 		prependTo		( type::stringc _target  )			
					{
						return eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).appendTo( %s ) )",pMain ,_target  );
					}
					int 		prependTo		( int _target  )			
					{
						return eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).prependTo( m[%d] ) )",pMain,_target   );
					}
					
					// .................................................................... replaceAll			
					int 		replaceAll		( type::stringc _target  )			
					{
						return eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).appendTo( %s ) )",pMain ,_target  );
					}
					int 		replaceAll		( int _target  )			
					{
					return eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).replaceAll( m[%d] ) )",pMain,_target   );
					}
					
					// .................................................................... empty
					emjq& 			empty		( void )			
					{
						eval ( "jQuery(m[%d]).empty()",pMain   );
						return *this ;
					}
					
					// .................................................................... deatch
					int 		detach		( void )			
					{
						return eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).detach() )",pMain   );
					}
					int 		detach		( int p )			
					{
						return eval ( "window.pointerSet ( 'object' ,  jQuery(m[%d]).detach(jQuery(m[%d]) )",pMain,p   );
					}			
					int 		detach		( type::stringc s )			
					{
						return eval ( "window.pointerSet ( 'object' ,  jQuery(m[%d]).detach(%s) )",pMain,s   );
					}	

					// .................................................................... replaceWith
					emjq& 			replaceWith		( int p )			
					{
						eval ( "jQuery(m[%d]).replaceWith(jQuery(m[%d]) ",pMain,p   );
						return *this;				
					}			
					emjq& 			replaceWith		( type::stringc s )			
					{
						eval ( "jQuery(m[%d]).replaceWith(%s) ",pMain,s   );
						return *this;
					}	
					emjq& 			replaceWith		( type::address f,type::stringc _arg="" )			
					{
						eval ( "jQuery(m[%d]).replaceWith(%s) ",pMain,callback( f , _arg ).c_str()   );
						return *this;				
					}

					// .................................................................... remove
					emjq& 			remove		( void )			
					{
						eval ( "jQuery(m[%d]).remove() ",pMain   );
						return *this;				
					}				
					emjq& 			remove		( int p )			
					{
						eval ( "jQuery(m[%d]).remove(jQuery(m[%d]) ",pMain,p   );
						return *this;				
					}			
					emjq& 			remove		( type::stringc s )			
					{
						eval ( "jQuery(m[%d]).remove(%s) ",pMain,s   );
						return *this;
					}			

					// .................................................................... wrap
					emjq& 			wrap		( int p )			
					{
						eval ( "jQuery(m[%d]).wrap(jQuery(m[%d]) ",pMain,p   );
						return *this;				
					}			
					emjq& 			wrap		( type::stringc s )			
					{
						eval ( "jQuery(m[%d]).wrap(%s) ",pMain,s   );
						return *this;
					}	
					emjq& 			wrap		( type::address f,type::stringc _arg="index" )			
					{
						eval ( "jQuery(m[%d]).wrap(%s) ",pMain,callback( f , _arg ).c_str()   );
						return *this;				
					}
					// .................................................................... wrapAll
					emjq& 			wrapAll		( int p )			
					{
						eval ( "jQuery(m[%d]).wrapAll(jQuery(m[%d]) ",pMain,p   );
						return *this;				
					}			
					emjq& 			wrapAll		( type::stringc s )			
					{
						eval ( "jQuery(m[%d]).wrapAll(%s) ",pMain,s   );
						return *this;
					}	
					emjq& 			wrapAll		( type::address f,type::stringc _arg="" )			
					{
						eval ( "jQuery(m[%d]).wrapAll(%s) ",pMain,callback( f , _arg ).c_str()   );
						return *this;				
					}
					// .................................................................... wrapInner
					emjq& 			wrapInner		( int p )			
					{
						eval ( "jQuery(m[%d]).wrapInner(jQuery(m[%d]) ",pMain,p   );
						return *this;				
					}			
					emjq& 			wrapInner		( type::stringc s )			
					{
						eval ( "jQuery(m[%d]).wrapInner(%s) ",pMain,s   );
						return *this;
					}	
					emjq& 			wrapInner		( type::address f,type::stringc _arg="" )			
					{
						eval ( "jQuery(m[%d]).wrapInner(%s) ",pMain,callback( f , _arg ).c_str()   );
						return *this;				
					}
					// .................................................................... unwrap
					emjq& 			unwrap		( void )			
					{
						eval ( "jQuery(m[%d]).unwrap() ",pMain   );
						return *this;				
					}					
					emjq& 			unwrap		( int p )			
					{
						eval ( "jQuery(m[%d]).unwrap(jQuery(m[%d]) ",pMain,p   );
						return *this;				
					}			
					emjq& 			unwrap		( type::stringc s )			
					{
						eval ( "jQuery(m[%d]).unwrap(%s) ",pMain,s   );
						return *this;
					}	
					// .................................................................... insertAfter
					emjq& 			insertAfter		( int p )			
					{
						eval ( "jQuery(m[%d]).insertAfter(jQuery(m[%d]) ",pMain,p   );
						return *this;				
					}			
					emjq& 			insertAfter		( type::stringc s )			
					{
						eval ( "jQuery(m[%d]).insertAfter(%s) ",pMain,s   );
						return *this;
					}	
					// .................................................................... insertBefore
					emjq& 			insertBefore		( int p )			
					{
						eval ( "jQuery(m[%d]).insertBefore(jQuery(m[%d]) ",pMain,p   );
						return *this;				
					}			
					emjq& 			insertBefore		( type::stringc s )			
					{
						eval ( "jQuery(m[%d]).insertBefore(%s) ",pMain,s   );
						return *this;
					}	
					// .................................................................... htmlPrefilter
					void 				htmlPrefilter		( void )			
					{
						printf ( "COMING SOON!\n" );
					}
							
				// ********* 
				//			§miscellaneous
				// ********* 

					// .................................................................... index
					int					index		( void )			
					{
						return eval ( "jQuery(m[%d]).index() ",pMain   );
					}					
					int					index		( int p )			
					{
						return eval ( "jQuery(m[%d]).index(jQuery(m[%d]) ",pMain,p   );
					}			
					int					index		( type::stringc s )			
					{
						return eval ( "jQuery(m[%d]).index(%s) ",pMain,s   );
					}
					// .................................................................... deatch
					int 		toArray		( void )			
					{
						return eval ( "window.pointerSet ( 'object' , jQuery(m[%d]).toArray() )",pMain   );
					}							

				// ********* 
				//			§utilities
				// ********* 

					// .................................................................... extend
					int 		extend		( int _obj1 , int _obj2 )			
					{
						return eval ( "jQuery.extend(m[%d],m[%d],m[%d]	) ",pMain,_obj1,_obj2   );
					}	
					// .................................................................... globalEval			
					int 		globalEval		( type::stringc _code )			
					{
						return eval ( "window.pointerSet( 'object' ,  jQuery.globalEval('%s') )",_code   );
					}
					// .................................................................... isArray			
					int 		isArray		( int _obj1  )			
					{
						return eval ( "jQuery.isArray ( m[%d] 	) ",_obj1   );
					}
					// .................................................................... inArray			
					int 		inArray		( type::stringc _value, int _obj1,int _fromIndex=0  )				
					{
						return eval ( "jQuery.inArray ( %s , m[%d] , %d )",_value, _obj1 , _fromIndex   );
					}
					// .................................................................... isNumeric		
					int 		isNumeric	( int _obj1  )			
					{
						return eval ( "jQuery.isNumeric ( m[%d] ) ",_obj1   );
					}
					// .................................................................... isFunction				
					int 		isFunction	( int _obj1  )			
					{
						return eval ( "jQuery.isFunction ( m[%d] ) ",_obj1   );
					}
					// .................................................................... isEmptyObject		
					int 		isEmptyObject	( int _obj1  )			
					{
						return eval ( "jQuery.isEmptyObject ( m[%d] ) ",_obj1   );
					}
					// .................................................................... isPlainObject				
					int 		isPlainObject	( int _obj1  )			
					{
						return eval ( "jQuery.isPlainObject ( m[%d] ) ",_obj1   );
					}	
					// .................................................................... isWindow			
					int 		isWindow	( int _obj1  )			
					{
						return eval ( "jQuery.isWindow ( m[%d] ) ",_obj1   );
					}
					// .................................................................... isXMLDoc			
					int 		isXMLDoc	( int _obj1  )			
					{
						return eval ( "jQuery.isXMLDoc ( m[%d] ) ",_obj1   );
					}
					// .................................................................... parseJSON			
					int		parseJSON	( type::stringc _obj1 )			
					{
						return eval ( "window.pointerSet( 'object' , JSON.parse ( '%s' ) )",_obj1   );
					}
					// .................................................................... parseHTML				
					int		parseHTML	( type::stringc _data,type::stringc _context="document",bool _keepScript=true )			
					{
						return eval ( "window.pointerSet( 'object' , jQuery.parseHTML ( '%s',%s,%s ) )",_data,_context,_keepScript==true?"true":"false"   );
					}
					// .................................................................... parseXML		
					int		parseXML	( type::stringc _obj1 )			
					{
						return eval ( "window.pointerSet( 'object' , jQuery.parseXML ( '%s' ) )",_obj1   );
					}
					// .................................................................... now				
					unsigned int 		now	( void  )			
					{
						return eval ( "jQuery.now () "  );
					}
					// .................................................................... noop	
					void 		noop	( void  )			
					{
						eval ( "jQuery.noop () "  );
					}			
					// .................................................................... type	
					type::stringc  type	( type::stringc _obj )
					{
						int retVal=EM_ASM_INT(
						{
							var _obj = Pointer_stringify($0);
							var retVal=  jQuery.type(eval(_obj)); 
							
							if ( typeof retVal === 'object' ) retVal=JSON.stringify(retVal);

							return window.strdup ( retVal ) ;
							
						}, _obj );	
						return (type::stringc)retVal;
					}
					// .................................................................... trim			
					std::string&		trim	(std::string& s )			
					{
						 return Trim(s);
					}
					// .................................................................... uniqueSort			
					int 		uniqueSort	( int _obj1  )	 		
					{
						return eval ( "jQuery.uniqueSort ( m[%d] ) ",_obj1   );
					}
					// .................................................................... makeArray		
					int		makeArray	( int _obj1 )			
					{
						return eval ( "window.pointerSet( 'object' , jQuery.makeArray ( m[%d] ) )",_obj1   );
					}
					// .................................................................... merge		
					int		merge	( int _obj1 , int _obj2 )			
					{
						return eval ( "window.pointerSet( 'object' , jQuery.merge ( m[%d] , m[%d] ) )",_obj1,_obj2  );
					}
					// .................................................................... each 
					int		each	( int _arr , type::address _cb, bool _invert=false)			
					{
						return eval ( "window.pointerSet( 'object' , jQuery.each ( m[%d] , %s , %s ) )",_arr,callback(_cb,"element,index").c_str(),_invert==true?"true":"false"  ) ;
					}			
					// .................................................................... grep
					int		grep	( int _arr , type::stringc _function)			
					{
						return eval ( "window.pointerSet( 'object' , jQuery.grep( m[%d] , %s ) )",_arr,_function  ) ;
					}	
					// .................................................................... map 
					int		map	( int _arr , type::stringc _function)			
					{
						return eval ( "window.pointerSet( 'object' , jQuery.map ( m[%d] , %s) )",_arr,_function ) ;
					}	
		
			//
			//
			//
	} $ ;
	 
	bool emjq::init = true; //   non-const static data member must be initialized out of line 

	// ___________________________________________________________________________________________________________________	
	// ___________________________________________________________________________________________________________________ end Class emjq

	//
	//
	//
			
	// #############
	//				§core
	// #############

	// ................................................................................... §jQuery #1

	emjq& emjq::jQuery ( const char * sel )
	{ 
		if (init==true)
		{
			jQueryBegin();			
			garbageInit();
			init=false;
		}
		
		int retVal=EM_ASM_INT(
		{
			var _jQuery = Pointer_stringify($0);
			
			if ( _jQuery == 'window' ) _jQuery = window ;			
			if ( ( _jQuery == 'window.screen' ) || (_jQuery == 'screen' ) ) _jQuery = window.screen ;	
			if ( ( _jQuery == 'window.document' ) || (_jQuery == 'document' ) ) _jQuery = window.document ;	
			if ( ( _jQuery == 'window.document.body' ) || (_jQuery == 'document.body' ) || (_jQuery == 'body' ) ) _jQuery = window.document.body ;	

			return window.pointerSet ( 'object' , jQuery(_jQuery) ) ;
		}, sel );

		pBase=pMain=retVal ;
		
		return *this ;
	}

	// ................................................................................... §jQuery #1

	emjq& emjq::jQuery ( int p )
	{
		pBase=pMain = p ;	
		
		if (init==true)
		{
			jQueryBegin();			
			garbageInit();
			init=false;
		}

		return *this ;
	}

	// ................................................................................... §jQueryBegin

	int emjq::jQueryBegin ( void )
	{
		EM_ASM(
		{
			console.log ( "emjq::init()" ) ;
			window.m = Module ;

			// ..................................................... counter 
			// + Pointer ,  0 counter ,  - Object	
			m[0] = 0; 
			// ................................... Address for Garbage
			m['$'] = {}; 
			m['$'].len = 0 ;
			m['$'].arr = []	;
			
			// ..................................................... set object/pointer with value/address				
			window.pointerSet = function ( type, value )
			{
				if ( value == undefined ) value = null ;
				var p = ++m[0];
				if ( type=='object' ) p*=-1;
				m[p] = value;
				return p ;
			} ;	

			// ..................................................... *pointer 		
			window.pointerGet = function (p)
			{
				return m[m[p]] ;
			};
		
			// ..................................................... strdup	
			window.strdup = function ( $0 )
			{
				if ( $0 == undefined ) $0 = "" ;
				$0 = $0.toString();

				var pstr = allocate(intArrayFromString($0), 'i8', ALLOC_NORMAL);
				m['$'].arr [ m['$'].len ]	= pstr;				
				++m['$'].len ;

				return pstr ;
			} ;
			/*
				// ........................................................ unload : garbage clean				
				$(window).bind('unload',function() 
				{ 
					m['$'].garbageClean(); 
				}); 
			*/
		});
		
		return 0 ;
	}

	// ................................................................................... §garbageInit 

	int emjq::garbageInit ( void )
	{
		EM_ASM(
		{
			console.log ( "emjq::garbage('open')" ) ;
			// ........................................................................... garbage clean
			m['$'].garbageClean = function ()
			{
				var bytes=0;
				for ( i=0 ; i<m['$'].arr.length; i++)		
				{
					var stringa = Pointer_stringify( m['$'].arr[i] ) ;
					var lunghezza = stringa.length;
 					bytes += lunghezza ;
					/*console.debug ( stringa, lunghezza ) ;*/
					if ( lunghezza>0 ) 
					{
						_free ( m['$'].arr[i] ) ;
					}
				}			
				console.log ( "emjq::garbage('close')" ) ; 
				console.log ( "emjq::free:: ",bytes," bytes" ) ; 				
			};
		});
		return 0 ;		
	}

	// ................................................................................... §jQueryEnd 

	int emjq::jQueryEnd ( int _exit )
	{
		printf ( "emjq.end()\n" ) ;	

		// .............................. garbage Close
		EM_ASM({
			m['$'].garbageClean();
		});
		// .............................. clear Dom
		EM_ASM({
			document.removeChild(document.documentElement);
		});
		// .............................. emscripten Close
		printf ( "Emscripten.exit()\n" ) ;			
		emscripten_force_exit(_exit);
		
		return _exit;
	}	

	//
	// end namespace
	//
  
};	

// __________________________________________________________________________________________________________________________
// __________________________________________________________________________________________________________________________
//

using namespace emjQuery ;

// __________________________________________________________________________________________________________________________
// __________________________________________________________________________________________________________________________
//
#define $(...) $._emjq()->jQuery(__VA_ARGS__)

#define _( ... ) (type::stringc)#__VA_ARGS__


#define OBJ( ... ) (type::stringc)#__VA_ARGS__
#define STR( ... ) s(#__VA_ARGS__)

// __________________________________________________________________________________________________________________________
// __________________________________________________________________________________________________________________________
//

type::stringc _garbage( type::stringc p )
{
	int retVal=EM_ASM_INT({
		var pstr = $0 ;
		// qui non devo duplicare c'e l'ho già il puntatore
		m['$'].arr [ m['$'].len ]	= pstr;				
		++m['$'].len ;		
	}, p) ;
	return p;
}

type::stringc s ( type::stringc s )
{
	std::string str( s ) ;
	str = "'" + str + "'" ;
	return _garbage(  strdup(str.c_str())  );
}

template <class T> 
type::stringc s ( T s )
{
	std::string str( std::to_string(s) ) ;
	str = "'" + str + "'" ;
	return _garbage(  strdup(str.c_str())  );
}

class _json 
{
	public:
	type::stringc stringify( type::json obj )
	{
		return obj.dump().c_str() ;
	}
	type::json parse( type::stringc obj )
	{
		return json::parse( obj ) ;
	}	
	
	
} JSON ;
 
type::stringc _JSON( type::stringc obj )
{
	return std::string("JSON.parse('" + std::string(obj) + "')").c_str() ;
}
type::stringc _JSON( type::json obj )
{
	return std::string("JSON.parse('" + obj.dump() + "')").c_str() ;
}	
type::stringc _XML( type::stringc obj )
{
	return std::string("jQuery.parseXML('" + std::string(obj) + "')").c_str() ;
}


// __________________________________________________________________________________________________________________________
// __________________________________________________________________________________________________________________________
//

//_____ 
//
// EMJQ 
//_____ 
//|***/
//|**/
//|*/
//|/
//°	





