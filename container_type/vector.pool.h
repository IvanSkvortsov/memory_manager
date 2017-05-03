#ifndef __VECTOR_POOL_H__
#define __VECTOR_POOL_H__
#include<vector>
#include"log.info.start.h"
template<typename std::size_t _Size>
struct VectorPool
{
	typedef std::size_t size_type;
	VectorPool(): vectorPool( _Size ){__log_info__(this);}
	~VectorPool(){ __log_info__(this); }
	size_type size()const{ return vectorPool.size(); }
	void *    data()const{ return (void*)vectorPool.data(); }
protected:
	std::vector<char> vectorPool;
};
struct vector_container_struct
{
	template<std::size_t _Size> struct wraper
	{
		typedef VectorPool<_Size> container_type;
	};
};
#endif//__VECTOR_POOL_H__
