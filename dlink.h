#ifndef __DLINK_H__
#define __DLINK_H__
#include<cstddef>// nullptr (C++11)
#include<cassert>
#include"regular.pointer.struct.h"

template<typename _Pointer = regular_pointer_struct>
class dlink
{
public:
	typedef _Pointer pointer_struct;
	typedef typename pointer_struct::template wraper<dlink>::pointer dlink_pointer;
	dlink(): prev(), next(){}
	//dlink( dlink const & v ): prev( v.prev ), next( v.next ){}
	dlink( dlink_pointer n ){ this->append( n ); }
	dlink( dlink_pointer p, dlink_pointer n): prev( p ), next( n ){}
	~dlink(){--count;}

	dlink_pointer get_next()const{ return next; }
	dlink_pointer get_prev()const{ return prev; }
	void append( dlink_pointer );
	static void remove( dlink_pointer );
	static int get_count(){ return count; }
	static dlink_pointer get_head( dlink_pointer );
	static dlink_pointer get_tail( dlink_pointer );
private:
	dlink_pointer prev;
	dlink_pointer next;
	static int count;
	void operator=(dlink const &);
	dlink( dlink const & v);
};

template<typename T, typename _Pointer = regular_pointer_struct>
class dlink_t: public dlink<_Pointer>
{
public:
	typedef dlink<_Pointer> dlink_type;
	typedef typename dlink_type::pointer_struct pointer_struct;
	typedef typename dlink_type::dlink_pointer dlink_pointer;
	typedef typename pointer_struct::template wraper<dlink_t<T, _Pointer> >::pointer dlink_t_pointer;
	dlink_t(): dlink_type(), data{}{}
	dlink_t( T const & d): dlink_type(), data(d){}
	dlink_t( dlink_pointer p, dlink_pointer n ): dlink_type(p, n), data(){}
	dlink_t( dlink_pointer p, dlink_pointer n, T const & d ): dlink_type(p, n), data(d){}
	~dlink_t(){}
	T & get_data(){ return data; }
	T const & get_data()const{ return data; }
	dlink_t_pointer get_next(){ return (dlink_t_pointer)this->dlink_type::get_next(); }
	dlink_t_pointer get_prev(){ return (dlink_t_pointer)this->dlink_type::get_prev(); }
	static dlink_t_pointer get_head( dlink_pointer r ){ return (dlink_t_pointer)dlink_type::get_head(r); }
	static dlink_t_pointer get_tail( dlink_pointer r ){ return (dlink_t_pointer)dlink_type::get_tail(r); }
	void append( dlink_t_pointer p ){ this->dlink_type::append( p ); }
	static void remove( dlink_t_pointer p ){ dlink_type::remove( p ); }
private:
	T data;
	void operator=(dlink_t const &);
	dlink_t( dlink_t const & v);
};

template<typename _Pointer> int dlink<_Pointer>::count = 0;

template<typename _Pointer>
inline void dlink<_Pointer>::append( dlink_pointer p )
{
	++count;
	p->next = next;
	p->prev = this;
	if( next )
		next->prev = p;
	next = p;
}
template<typename _Pointer>
inline void dlink<_Pointer>::remove( dlink_pointer p )
{
	--count;
	dlink_pointer next = p->next;
	dlink_pointer prev = p->prev;
	if( next )
		next->prev = prev;
	if( prev )
		prev->next = next;
}
template<typename _Pointer>
inline typename dlink<_Pointer>::dlink_pointer dlink<_Pointer>::get_head( dlink_pointer p )
{
	while( p->prev )
		p = p->prev;
	return p;
}
template<typename _Pointer>
inline typename dlink<_Pointer>::dlink_pointer dlink<_Pointer>::get_tail( dlink_pointer p )
{
	while( p->next )
		p = p->next;
	return p;
}

#endif//__DLINK_H__
