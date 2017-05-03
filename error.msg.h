#ifndef __ERROR_MSG_H__
#define __ERROR_MSG_H__
#include<iostream>
static inline void __error_msg( void * address, const char * function, const char * message )
{
	std::cerr << "ERROR: [" << address << "] " << function << ", " << message << std::endl;
}
#define __error_msg__( address, message ) __error_msg( (void*)(address), __PRETTY_FUNCTION__, (message) )
#endif//__ERROR_MSG_H__
