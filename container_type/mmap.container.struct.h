#ifndef __MMAP_CONTAINER_STRUCT_H__
#define __MMAP_CONTAINER_STRUCT_H__
#include"mmap.pool.h"
struct mmap_container_struct
{
	template<std::size_t _Size, typename _Pointer>
	struct wraper
	{
		typedef MMapPool<_Size, _Pointer> container_type;
	};
};
#endif//__MMAP_CONTAINER_STRUCT_H__
