#ifndef __NODE_POOL_H__
#define __NODE_POOL_H__

#include"dlink.h"
#include"log.info.short.h"
template<typename _Pointer = regular_pointer_struct, typename _DLink = dlink<_Pointer> >
struct NodePool: public _DLink
{
	typedef _Pointer pointer_struct;
	typedef _DLink dlink_type;
	typedef NodePool node_type;
	typedef typename pointer_struct::template wraper<node_type>::pointer node_pointer;
	NodePool(): dlink_type(){ __log_info__(this); }
	NodePool( node_pointer p, node_pointer n ): dlink_type( (dlink_type *)p, (dlink_type *)n ){ __log_info__(this); }
	//node_pointer get_next(){ return this->dlink_type::get_next(); }// error, invalid convertion from dlink_pointer to node_pointer
	node_pointer get_next(){ return (node_type *)(this->dlink_type::get_next()); }
	node_pointer get_prev(){ return (node_type *)(this->dlink_type::get_prev()); }
	void append( node_pointer p ){ __log_info__(this); this->dlink_type::append( (dlink_type *)p ); }
	static void remove( node_pointer p ){ __log_info__(p); dlink_type::remove( (dlink_type *)p ); }
	std::size_t get_capacity(){return (char*)(this->dlink_type::get_next()) - this->begin; }
	char begin[];
};
#endif//__NODE_POOL_H__
