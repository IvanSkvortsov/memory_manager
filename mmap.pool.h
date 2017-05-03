#ifndef __MMAP_POOL_H__
#define __MMAP_POOL_H__
#include"fman.h"// fman
#include"memory.mapped.h"// mman
#include"log.info.short.h"
template<std::size_t _Size>// std::string _Filename = "mmap.pool.out">
struct MMapPool
{
	typedef std::size_t size_type;
	MMapPool( const char * name = nullptr );
	~MMapPool(){ __log_info__(this); }
	size_type size()const{ return mmapPool.size(); }
	void *    data()const{ return mmapPool.data(); }
protected:
	fman file;
	mman mmapPool;
};
template<std::size_t _Size>
inline MMapPool<_Size>::MMapPool( const char * name ): file( name, _Size, fman::read_write() | fman::creat() ), mmapPool()
{
	__log_info__(this);
	if( file.is_open() )
		mmapPool.open( file.fd(), file.size(), mman::read_write(), mman::map_shared() );
	else
		mmapPool.open( -1, _Size, mman::read_write(), mman::map_shared() | mman::map_anonymous() );
}
struct mmap_container_struct
{
	template<std::size_t _Size> struct wraper
	{
		typedef MMapPool<_Size> container_type;
	};
};

#endif//__MMAP_POOL_H__
