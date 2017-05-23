#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__
#include<cstddef>// size_t, nullptr
#include<iostream>// cout, clog, endl
#include<cassert>// assert
#include<cstring>// memcpy
#include<cstdlib>// exit

#include"array.pool.h"
#include"node.pool.h"

#include"error.msg.h"
#include"log.info.short.h"
class BasePool
{
public:
	virtual ~BasePool(){ __log_info__(this); }
	virtual void *   allocate( std::size_t bytes ) = 0;
	virtual void * reallocate( void * space, std::size_t old_size, std::size_t new_size ) = 0;
	virtual void         free( void * space, std::size_t bytes ) = 0;
	virtual std::size_t  size()const = 0;
	virtual std::size_t  free_size()const = 0;
};

template<std::size_t _Size = 4096u, typename _Container = array_container_struct, typename _Pointer = regular_pointer_struct,
	typename _Node = Node_t<_Pointer> >
class Pool: public BasePool//, public _Container::template wraper<_Size>::container_type
{
public:
	typedef BasePool base_type, * base_pointer;
	typedef _Node node_type;
	typedef typename node_type::pointer_struct pointer_struct;
	typedef std::size_t size_type;
	typedef char char_type;
	typedef void void_type;
	typedef typename pointer_struct::template wraper<void_type>::pointer void_pointer;
	typedef typename pointer_struct::template wraper<char_type>::pointer char_pointer;
	typedef typename pointer_struct::template wraper<node_type>::pointer node_pointer;
	typedef typename _Container::template wraper<_Size>::container_type container_type;

	Pool();
	~Pool(){ __log_info__(this); }
	char_pointer  begin()const{ return listHead->begin; }
	char_pointer   data()const{ return begin(); }
	char_pointer    end()const{ return begin() + size(); }
	size_type      size()const{ return (char*)listTail - listHead->begin; }
	size_type free_size()const{ return nextFree->get_capacity(); }
	node_pointer get_next_free()const{ return nextFree; }
	node_pointer get_list_head()const{ return listHead; }
	node_pointer get_list_tail()const{ return listTail; }
	void *   allocate( size_type bytes );
	void * reallocate( void * space, size_type old_size, size_type new_size );
	void         free( void * space, size_type bytes = 0u );
private:
	Pool( Pool const & );
	Pool & operator=( Pool const & );
	template<std::size_t __Size, typename __Container, typename __Pointer, typename __Node>
	Pool( Pool<__Size, __Container, __Pointer, __Node> const & );
	template<std::size_t __Size, typename __Container, typename __Pointer, typename __Node>
	Pool & operator=( Pool<__Size, __Container, __Pointer, __Node> const & );
	void node_free( node_pointer p );
	container_type pool;
	node_pointer listHead, listTail;
	node_pointer headFree, nextFree;
};

template<std::size_t _Size, typename _Container, typename _Pointer, typename _Node>
inline Pool<_Size, _Container, _Pointer, _Node>::Pool():
	/*
	container_type(),
	listHead( (node_type *)container_type::data() ),
	listTail( node_pointer((char*)container_type::data() + container_type::size() - sizeof(node_type)) ),
	*/
	pool(), listHead( (node_type *)pool.data() ), listTail( node_pointer((char*)pool.data() + pool.size() - sizeof(node_type)) ),
	headFree( listHead ), nextFree( listHead )
{
	__log_info__(this);
	new (listHead) node_type( nullptr, listTail );
	new (listTail) node_type( listHead, nullptr );
}
template<std::size_t _Size, typename _Container, typename _Pointer, typename _Node>
inline void * Pool<_Size, _Container, _Pointer, _Node>::allocate( size_type bytes )
{
	__log_info__(this);
	if( free_size() >= bytes + sizeof(node_type) )
	{
		node_pointer n = nextFree;
		nextFree = new(n->begin + bytes) node_type;
		if( headFree >= n )
			headFree = nextFree;
		n->append( nextFree );
		return n->begin;
	} else {
		__error_msg__( this, "pool overflow" );
		exit(1);
	}
}
template<std::size_t _Size, typename _Container, typename _Pointer, typename _Node>
inline void * Pool<_Size, _Container, _Pointer, _Node>::reallocate( void * space, size_type old_size, size_type new_size )
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
template<std::size_t _Size, typename _Container, typename _Pointer, typename _Node>
inline void Pool<_Size, _Container, _Pointer, _Node>::free( void * space, size_type bytes )
{
	__log_info__(this);
	node_type * p = (node_type *)((char *)space - sizeof(node_type));
	assert( p->get_capacity() >= bytes );
	node_free( p );
}

template<std::size_t _Size, typename _Container, typename _Pointer, typename _Node>
inline void Pool<_Size, _Container, _Pointer, _Node>::node_free( node_pointer p )
{
	assert( p < nextFree );
	assert( nextFree < listTail );
	if( headFree == nextFree )// first call of node_free
	{
		headFree = p;
	}
	else if( p < headFree )
	{
		node_type::remove( headFree );
		headFree = p;
	}
	else if( p > headFree )
	{
		node_type::remove( p );
	} else {
		__error_msg__( this, "invalid node" );
		exit(1);
	}
	if( headFree->get_next() == nextFree )
	{
		node_type::remove( nextFree );
		nextFree = headFree;
	}
}
template<std::size_t _Size = 4096u, typename _Container = array_container_struct, typename _Pointer = regular_pointer_struct,
	typename _Node = Node_t<_Pointer> >
class SinglePool
{
	SinglePool();
	~SinglePool(){info();}
public:
	typedef std::size_t size_type;
	typedef SinglePool single_pool_type;
	template<std::size_t __Size> using        pool_type_s =       Pool<__Size, _Container, _Pointer, _Node>;
	template<std::size_t __Size> using single_pool_type_s = SinglePool<__Size, _Container, _Pointer, _Node>;
	typedef Pool<_Size, _Container, _Pointer, _Node> pool_type;
	typedef typename pool_type::container_type container_type;
	typedef typename pool_type::pointer_struct pointer_struct;
	typedef typename pool_type::node_type node_type;
	typedef typename pool_type::node_pointer node_pointer;
	typedef typename pool_type::char_pointer char_pointer;
	typedef typename pool_type::void_pointer void_pointer;

	template<std::size_t __Size> pool_type_s<__Size> & get_pool_s()
	{
		return single_pool_type_s<__Size>::get_pool();
	}
	static pool_type & get_pool()
	{
		static pool_type pool;
		return pool;
	}

	static void *     allocate( size_type bytes ){ return get_pool().allocate( bytes ); }
	static void *   reallocate( void * space, size_type old_size, size_type new_size ){return get_pool().reallocate(space, old_size, new_size);}
	static void           free( void * space, size_type bytes = 0u ){ get_pool().free( space, bytes ); }
	static size_type      size(){ return get_pool().size(); }
	static size_type free_size(){ return get_pool().free_size(); }

	static char_pointer begin(){ return get_pool().begin();}
	static char_pointer end(){ return get_pool().end();}
	static node_pointer get_next_free(){ return get_pool().get_next_free(); }
	static node_pointer get_list_head(){ return get_pool().get_list_head(); }
	static node_pointer get_list_tail(){ return get_pool().get_list_tail(); }

	static void info( std::ostream & out = std::cout )
	{
		out << "pool address   : [" << &get_pool() << "]" << std::endl;
		out << "amount of nodes: " << node_type::get_count() << std::endl;
		out << "size           : " << size() << " bytes" << std::endl;
		out << "list_head      : [" << get_list_head() << "]" << std::endl;
		out << "list_tail      : [" << get_list_tail() << "]" << std::endl;
		out << "free_size      : " << free_size() << " bytes" << std::endl;
		out << "next_free      : [" << get_next_free() << "]" << std::endl;
		out << "next_free->prev: [" << get_next_free()->get_prev() << "]" << std::endl;
		out << "next_free->next: [" << get_next_free()->get_next() << "]" << std::endl;
		out << "free_capacity  : " << (char*)get_list_tail() - (char*)get_next_free() << " bytes" << std::endl;
		out << "----------------" << std::endl;
	}
};

#endif//__MEMORY_POOL_H__
