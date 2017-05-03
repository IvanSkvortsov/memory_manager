#ifndef __MMAP_POOL_H__
#define __MMAP_POOL_H__
#include<cstddef>// size_t, nullptr
#include"node.pool.h"// NodePool
#include"fman.h"// fman
#include"memory.mapped.h"// mman

#include"log.info.short.h"
struct mmap_pool_tag{};
template<std::size_t _Size, typename _Pointer = offset_pointer_struct, typename _Node = NodePool<_Pointer> >
struct MMapPool: public fman, public mman
{
	typedef char value_type;
	typedef char char_type;
	typedef _Node node_type;
	typedef _Pointer pointer_struct;
	typedef typename pointer_struct::template wraper<node_type>::pointer node_pointer;
	typedef typename pointer_struct::template wraper<char_type>::pointer char_pointer;
	typedef mmap_pool_tag pool_category;
	MMapPool( char const * __name = "pool.map.txt" ): fman( __name, _Size + 2*sizeof(node_type) ), mman( fman::fd(), fman::size() ),
		listHead( (node_type *)mman::get_address() ), listTail( listHead + _Size + sizeof(node_type) )
	{
		__log_info__(this);
		if( mman::map_failed() ) exit(1);
		new ( listHead ) node_type( nullptr, listTail );
		new ( listTail ) node_type( listHead, nullptr );
		fman::close();
	}
	~MMapPool(){ __log_info__(this); }
	node_pointer get_list_head(){ return listHead; }
	node_pointer get_list_tail(){ return listTail; }
	static std::size_t size(){ return _Size;}
protected:
	node_type * listHead;
	node_type * listTail;
};

#endif//__MMAP_POOL_H__
