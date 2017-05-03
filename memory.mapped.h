#ifndef __MEMORY_MAPPED_H__
#define __MEMORY_MAPPED_H__

#include<iostream>
#include"error.msg.h"// __error_msg__
#include<stdlib.h>// exit
//#include<stdio.h>// printf, fprintf, FILE
//#include<fcntl.h>// off_t, mode_t, open, close, O_RDWR, O_RDONLY, O_WRONLY, O_CREAT, ..., S_IRWXU, S_IRWXG, S_IRWXO, ...
//#include<unistd.h>// lseek, write, ::open, ::close
//#include<sys/stat.h>// stat, fstat
#include<sys/mman.h>// mmap, munmap
//#include<string.h>// memset
#include"fman.h"// size( fd ) - get file size by file descriptor fd

#include"log.info.start.h"
class mman
{
public:
	typedef off_t off_type;
	typedef mode_t mode_type;
	typedef std::size_t size_type;
private:
	mman( mman const & );
	mman & operator=( mman const & );
	void clear()
	{
		_size = 0u;
		_data = nullptr;
		_offset = 0ull;
	}
	void info( std::ostream & out = std::cout )const;
	static void info( int __fd, size_type __size, int __prot, int __flags, off_type __offset, std::ostream & out = std::cout );
	static off_type pa_offset( off_type __offset ){ return __offset & ~(sysconf(_SC_PAGE_SIZE)-1); }
protected:
	off_type _offset;
	size_type _size;
	char * _data;
public:
	mman(): _offset(), _size(), _data(){ __log_info__( this ); }
	mman( int __fd, size_type __size, int __prot = read_write(), int __flags = map_shared(), off_type __offset = 0ull );
	~mman(){ __log_info__( this ); close(); }

	int open( int __fd, size_type __size, int __prot = read_write(), int __flags = map_shared(), off_type __offset = 0ull );
	int close();

	size_type size()const{ return _size; }
	off_type offset()const{ return _offset; }
	void * data()const{ return _data; }
	void * get_address()const{ return _data + _offset - pa_offset( _offset ); }

	void pub_info(std::ostream & out = std::cout)const{ info( out ); }
	// static member methods
	static bool map_failed( void const * __data ){ return __data == MAP_FAILED; }
	bool map_failed()const{ return map_failed( data() ); }
	// protections:
	static int read_only(){ return PROT_READ; }
	static int read_write(){ return PROT_READ | PROT_WRITE; }
	static int write_only(){ return PROT_WRITE; }
	static int exec(){ return PROT_EXEC; }
	static int none(){ return PROT_NONE; }
	// visibility of updates to mapping region:
	static int map_shared(){ return MAP_SHARED; }// visible to other processes, and carried through to underlying file
	static int map_private(){ return MAP_PRIVATE; }// not visible to other processes, and carried through to underlying file
	// can be ORed to shared and private flags:
	static int map_anonymous(){ return MAP_ANONYMOUS; }// mapping is not backed by any file; fd is ignored (some implem. requires fd to be -1)
	static int map_fixed(){ return MAP_FIXED; }// place mapping at exectly the given address
	//static int map_locked(){ return MAP_LOCKED; }// lock mapped region, preventing that mapped region being paged to the swap area
	//static int map_populate(){ return MAP_POPULATE; }// populate page tables for mapping; for file mapping, this causes read-ahead on the file
};

inline void mman::info( int __fd, size_type __size, int __prot, int __flags, off_type __offset, std::ostream & out )
{
	out << "fd    : " << __fd << std::endl;
	out << "size  : " << __size << std::endl;
	out << "prot  : " << __prot << std::endl;
	out << "flags : " << __flags << std::endl;
	out << "offset: " << __offset << std::endl;
}
inline void mman::info( std::ostream & out )const
{
	out << "data  : " << data() << std::endl;
	out << "size  : " << size() << std::endl;
	out << "offset: " << offset() << std::endl;
}

inline mman::mman( int __fd, size_type __size, int __prot, int __flags, off_type __offset ): _offset(), _size(), _data()
{
	__log_info__( this );
	int _z = mman::open( __fd, __size, __prot, __flags, __offset );
	if( _z == -1 )
	{
		__error_msg__( this, "mapping failed" );
		info( __fd, __size, __prot, __flags, __offset, std::cerr );
		exit(1);
	}
}
inline int mman::open( int __fd, size_type __size, int __prot, int __flags, off_type __offset )
{
	__log_info__( this );
	if( data() )
	{
		__error_msg__( this, "mapping failed, one should unmap before start mapping" );
		exit(1);
	}
	off_type __file_size = __size;
	if( !(__flags & mman::map_anonymous()) )
		__file_size = fman::size( __fd );
	if( __offset > __file_size )
	{
		__error_msg__( this, "mapping failed, invalid offset" );
		std::cerr << "file size: " << __file_size << std::endl;
		info( __fd, __size, __prot, __flags, __offset );
		return -1;
	}
	else if( __size + __offset > __file_size )
		__size = __file_size - __offset;
	off_type __pa_offset = pa_offset( __offset );// offset should be page aligned: pa_offset
	_data = (char *)mmap( nullptr, __size + __offset - __pa_offset, __prot, __flags, __fd, __pa_offset );
	if( map_failed() )
	{
		__error_msg__( this, "mapping failed" );
		std::cerr << "data: [" << data() << "]" << std::endl;
		info( __fd, __size, __prot, __flags, __offset );
		return -1;
	}
	_size = __size;
	_offset = __offset;
	return 0;
}

inline int mman::close()
{
	__log_info__( this );
	if( size() || data() )
		munmap( data(), size() + offset() - pa_offset( offset() ) );
	clear();
}

#endif//__MEMORY_MAPPED_H__
