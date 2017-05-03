#ifndef __ARRAY_POOL_H__
#define __ARRAY_POOL_H__
#include<cstddef>// size_t, nullptr
#include"node.pool.h"

#include"log.info.short.h"
struct array_pool_tag{};

template<std::size_t _Size, typename _Pointer = regular_pointer_struct, typename _Node = NodePool<_Pointer> >
struct ArrayPool
{
	typedef char value_type;
	typedef char char_type;
	typedef _Node node_type;
	typedef _Pointer pointer_struct;
	typedef typename pointer_struct::template wraper<node_type>::pointer node_pointer;
	typedef typename pointer_struct::template wraper<char_type>::pointer char_pointer;
	typedef array_pool_tag pool_category;
	ArrayPool()
	{
		__log_info__(this);
		new ( listHead ) node_type( nullptr, listTail );
		new ( listTail ) node_type( listHead, nullptr );
	}
	~ArrayPool(){ __log_info__(this); }
	node_pointer get_list_head(){ return listHead; }
	node_pointer get_list_tail(){ return listTail; }
	static std::size_t size(){ return _Size;}
protected:
	node_type listHead[];
	char_type arrayPool[_Size + sizeof(node_type)];
	node_type listTail[];
	char_type _tailPool[sizeof(node_type)];
};

#endif//__ARRAY_POOL_H__
