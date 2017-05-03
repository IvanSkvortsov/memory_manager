#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__
#include<cstddef>// size_t, nullptr
#include<iostream>// cout, clog, endl
#include<cassert>
#include<cstring>// memcpy
#include<cstdlib>// exit
#include"error.msg.h"

#include"container.struct.h"

#include"log.info.short.h"
template<std::size_t _Size = 4096u, typename _Pointer = regular_pointer_struct, typename _Container = array_container_struct >
class Pool : protected _Container::template wraper<_Size, _Pointer>::container_type
{
public:
	typedef void void_type;
	typedef std::size_t size_type;
	typedef typename _Container::template wraper<_Size, _Pointer>::container_type container_type;
	typedef typename container_type::value_type value_type;
	typedef typename container_type::char_type char_type;
	typedef typename container_type::node_type node_type;
	typedef typename container_type::pointer_struct pointer_struct;
	typedef typename pointer_struct::template wraper<void_type>::pointer void_pointer;
	typedef typename pointer_struct::template wraper<char_type>::pointer char_pointer;
	typedef typename pointer_struct::template wraper<node_type>::pointer node_pointer;
	typedef typename container_type::pool_category pool_category;

	Pool(): container_type(), nextFree( container_type::get_list_head() ){ __log_info__(this); }
	~Pool(){ __log_info__(this); }
	node_pointer get_list_head(){ return container_type::get_list_head(); }
	node_pointer get_list_tail(){ return container_type::get_list_tail(); }
	char_pointer begin(){ return container_type::get_list_head()->begin; }
	char_pointer data() { return begin(); }
	char_pointer end()  { return begin() + size(); }
	size_type size(){ return container_type::size(); }
	node_pointer get_next_free(){ return nextFree; }
	size_type free_size(){ return nextFree->get_capacity(); }
	void * allocate( size_type bytes )
	{
		__log_info__(this);
		if( nextFree->get_capacity() >= bytes )
		{
			node_pointer n = nextFree;
			nextFree = new(n->begin + bytes) node_type;
			n->append( nextFree );
			return n->begin;
		} else {
			__error_msg__( this, "pool overflow" );
			exit(1);
		}
	}
	void * reallocate( void * space, size_type old_size, size_type new_size )
	{
		__log_info__(this);
		node_type * p = (node_type *)((char*)space - sizeof(node_type));
		if( p->get_capacity() >= new_size )
			return space;
		void * new_space = this->allocate( new_size );
		memcpy( new_space, space, (old_size < new_size ? old_size : new_size ) );
		node_free( p );
		return new_space;
	}
	void free( void * space, size_type bytes = 0u )
	{
		__log_info__(this);
		node_type * p = (node_type *)((char *)space - sizeof(node_type));
		assert( p->get_capacity() >= bytes );
		node_free( p );
	}
private:
	Pool( Pool const & );
	Pool & operator=( Pool const & );
	template<std::size_t __Size, typename __Pointer, typename __Container> Pool( Pool<__Size, __Pointer, __Container> const & );
	template<std::size_t __Size, typename __Pointer, typename __Container> Pool & operator=( Pool<__Size, __Pointer, __Container> const & );
	void node_free( node_pointer p )
	{
		if( p->get_next() == nextFree )
		{
			node_type::remove( nextFree );
			nextFree = p;
		}
		else
			node_type::remove( p );
	}
	node_pointer nextFree;
};

template<std::size_t _Size = 4096u, typename _Pointer = regular_pointer_struct, typename _Container = array_container_struct>
class SinglePool
{
public:
	typedef std::size_t size_type;
	typedef SinglePool single_pool_type;
	typedef Pool<_Size, _Pointer, _Container> pool_type;
	typedef typename Pool<_Size, _Pointer, _Container>::container_type container_type;
	typedef typename pool_type::pool_category pool_category;
	typedef typename pool_type::pointer_struct pointer_struct;
	typedef typename pool_type::node_pointer node_pointer;
	typedef typename pool_type::char_pointer char_pointer;
	typedef typename pool_type::void_pointer void_pointer;

	template<std::size_t __Size>
	Pool<__Size, pointer_struct, container_type> & get_pool_s(){ return SinglePool<__Size, pointer_struct, container_type>::get_pool(); }
	static pool_type & get_pool()
	{
		static pool_type pool;
		return pool;
	}

	static void_pointer allocate( size_type bytes ){ return get_pool().allocate( bytes ); }
	static void_pointer reallocate( void * space, size_type old_size, size_type new_size ){return get_pool().reallocate(space, old_size, new_size);}
	static void free( void * space, size_type bytes = 0u ){ get_pool().free( space, bytes ); }
	static size_type size(){ return get_pool().size(); }
	static size_type free_size(){ return get_pool().free_size(); }

	static char_pointer begin(){ return get_pool().begin();}
	static char_pointer end(){ return get_pool().end();}
	static node_pointer get_next_free(){ return get_pool().get_next_free(); }
	static node_pointer get_list_head(){ return get_pool().get_list_head(); }
	static node_pointer get_list_tail(){ return get_pool().get_list_tail(); }
};

#endif//__MEMORY_POOL_H__
