#ifndef __ARRAY_CONTAINER_STRUCT_H__
#define __ARRAY_CONTAINER_STRUCT_H__
#include"array.pool.h"
struct array_container_struct
{
	template<std::size_t _Size, typename _Pointer>
	struct wraper
	{
		typedef ArrayPool<_Size, _Pointer> container_type;
	};
};
#endif//__ARRAY_CONTAINER_STRUCT_H__
