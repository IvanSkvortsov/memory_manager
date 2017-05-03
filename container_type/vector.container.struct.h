#ifndef __VECTOR_CONTAINER_STRUCT_H__
#define __VECTOR_CONTAINER_STRUCT_H__
#include"vector.pool.h"
struct vector_container_struct
{
	template<std::size_t _Size, typename _Pointer>
	struct wraper
	{
		typedef VectorPool<_Size, _Pointer> container_type;
	};
};
#endif//__VECTOR_CONTAINER_STRUCT_H__
