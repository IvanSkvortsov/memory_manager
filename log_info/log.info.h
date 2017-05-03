#ifndef __LOG_INFO_H__
#define __LOG_INFO_H__
#include<iostream>
inline static void __log_info( void * address, const char * function ) { std::clog << "LOG: [" << address << "] " << function << std::endl; }
inline std::string smart_function_tclass( std::string const & fs );
inline static void __log_info_short( void * address, const char * function ) { __log_info( address, smart_function_tclass( function ).c_str() ); }
#define __log_info_dummy__( address )
#define __log_info_start__( address ) __log_info( (void*)(address), __PRETTY_FUNCTION__ )
#define __log_info_short__( address ) __log_info_short( (void*)(address), __PRETTY_FUNCTION__ )
#define __log_info__( address ) __log_info_start__( address )
// reduce template parameter instanciation-list
inline std::string smart_function_tparam( std::string const & fs )
{
	int pos = fs.find( "[with " );
	if( pos < 0 )
		return fs;
	pos = fs.find( ";", pos );
	if( pos < 0 )
		return fs;
	return fs.substr(0, pos) + "]";
}
// reduce template parameter list:
// e.i. class_name<T1, T2, T3, .....>::method_name --> class_name<T1>::method_name
inline std::string smart_function_tclass( std::string const & fs )
{
	static std::string sbuf;
	int pos = 0, beg, end;
	sbuf = smart_function_tparam( fs );
	while( true )
	{
		pos = sbuf.find( ">", pos );
		if( pos < 0 )
			break;
		beg = sbuf.rfind( "<", pos );
		if( beg < 0 )
			continue;
		end = sbuf.find( ",", beg );
		if( end < 0 )
			end = pos;
		sbuf = sbuf.substr( 0, end) + sbuf.substr( pos, -1 );
		pos += 1;
	}
	return sbuf;
}

#endif//__LOG_INFO_H__
