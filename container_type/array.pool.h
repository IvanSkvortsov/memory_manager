#ifndef __ARRAY_POOL_H__
#define __ARRAY_POOL_H__
#include<cstddef>// size_t, nullptr( since c++11 )
#include"log.info.short.h"
template<typename std::size_t _Size>
struct ArrayPool
{
	typedef std::size_t size_type;
	ArrayPool(){__log_info__(this); }
	~ArrayPool(){ __log_info__(this); }
	size_type size()const{ return _Size;}
	void *    data()const{ return (void*)arrayPool; }
protected:
	char arrayPool[_Size];
};
struct array_container_struct
{
	template<std::size_t _Size> struct wraper
	{
		typedef ArrayPool<_Size> container_type;
	};
};
#endif//__ARRAY_POOL_H__
