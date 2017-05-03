#ifndef __REGULAR_POINTER_STRUCT_H__
#define __REGULAR_POINTER_STRUCT_H__
struct regular_pointer_struct
{
	template<typename T> struct wraper { typedef T * pointer; };
};
#endif//__REGULAR_POINTER_STRUCT_H__
