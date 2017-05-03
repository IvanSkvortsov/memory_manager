#ifndef __VECTOR_POOL_H__
#define __VECTOR_POOL_H__
#include<cstddef>// size_t, nullptr
#include"node.pool.h"
#include<vector>

#include"log.info.short.h"
struct vector_pool_tag{};
template<std::size_t _Size, typename _Pointer = regular_pointer_struct, typename _Node = NodePool<_Pointer> >
struct VectorPool
{
	typedef char value_type;
	typedef char char_type;
	typedef _Node node_type;
	typedef _Pointer pointer_struct;
	typedef typename pointer_struct::template wraper<node_type>::pointer node_pointer;
	typedef typename pointer_struct::template wraper<char_type>::pointer char_pointer;
	typedef vector_pool_tag pool_category;
	VectorPool(): vectorPool( _Size + 2*sizeof(node_type) ),
		listHead( (node_pointer)( &vectorPool[0] ) ),
		listTail( (node_pointer)( &vectorPool[_Size+sizeof(node_type)] ) )
	{
		__log_info__(this);
		new ( listHead ) node_type( nullptr, listTail );
		new ( listTail ) node_type( listHead, nullptr );
	}
	~VectorPool(){ __log_info__(this); }
	node_pointer get_list_head(){ return listHead; }
	node_pointer get_list_tail(){ return listTail; }
	static std::size_t size(){ _Size; }
protected:
	std::vector<char_type> vectorPool;
	node_pointer listHead, listTail;
};

#endif//__VECTOR_POOL_H__
