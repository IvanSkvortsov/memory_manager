#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__
#include<fcntl.h>// off_t, mode_t, ::open, ::close, O_RDWR, ..., S_IRWXU, S_IWUSR, ...
#include<sys/stat.h>// stat, fstat
#include<unistd.h>// lseek, write, ::open, ::close
#include<cstdlib>// exit

#include<iostream>// clog, cerr, endl
#include<iomanip>// oct, dec

#include"error.msg.h"
#include"log.info.start.h"
//#include"log.info.dummy.h"

#ifdef  __log_info__
template<typename OUT, typename IN> inline static OUT force_cast( IN in )
{
	union{
		IN in;
		OUT out;
	} u{ in };
	return u.out;
}
template<typename IN> inline static void * void_cast( IN in ){ return force_cast<void *, IN>( in ); }
#endif

//#define __FMAN_INFO__

struct fman
{
	typedef off_t off_type;
	typedef mode_t mode_type;

	fman(): _fd(), _flags(){__log_info__(this);}
	fman( const char * __name, off_type __size = -1, int __flags = read_write() | creat_trunc(),
		mode_type __mode = rw_user() | rd_group() | rd_other() ):
		fman( open( __name, __flags, __mode ) )
	{
		__log_info__(this);
		if( (int)__size != -1 && is_open() )
			reserve( __size );
	}
	~fman(){ __log_info__(this); close(); }

	// file open state
	bool is_open()const{ return fd() > 0; }// fail to open
	bool is_failed()const{ fd() == -1; }
	// stat
	static struct stat const & get_stat( int __fd );
	static void reserve( int __fd, off_type __size );
	static mode_type mode( int __fd ) { return get_stat( __fd ).st_mode; }
	static const off_type size( int __fd ) { return get_stat( __fd ).st_size; }

	struct stat const & get_stat()const { return fman::get_stat( fd() ); }
	const mode_type mode()const{ return fman::mode( fd() ); }
	const off_type size()const { return fman::size( fd() ); }
	void reserve( off_type __size ){ return fman::reserve( fd(), __size ); }
	void write( const char * __str, std::size_t __size ){ ::write( fd(), __str, __size ); }
	void read( char * __str, std::size_t __size ){ ::read( fd(), __str, __size ); }
	void lseek( off_type __pos, int __seek_way = seek_set() ){ ::lseek( fd(), __pos, __seek_way ); }
	// file fields:
	const int fd()const{ return this->_fd; }
	const int flags()const{ return this->_flags; }
	// open/close
	int open( const char * __name, int __flags = read_write() | creat_trunc(), mode_type __mode = rw_user() | rd_group() | rd_other() );
	int close();
	// create (syntactic sugar)
	int create( const char * __name, mode_type __mode = rw_user() | rd_group() | rd_other() )
	{
		return fman::open( __name, read_write() | creat_excl(), __mode );
	}
	// protections:
	static int read_write(){ return O_RDWR; }
	static int read_only(){ return O_RDONLY; }
	static int write_only(){ return O_WRONLY; }
	static int excl(){ return O_EXCL; }
	static int trunc(){ return O_TRUNC; }
	static int creat(){ return O_CREAT; }
	static int creat_excl(){  return O_CREAT | O_EXCL; }
	static int creat_trunc(){ return O_CREAT | O_TRUNC; }
	// user mode:
	static mode_type rwx_user(){ return S_IRWXU; }
	static mode_type rw_user(){ return S_IRUSR | S_IWUSR; }
	static mode_type rd_user(){ return S_IRUSR; }
	static mode_type wr_user(){ return S_IWUSR; }
	static mode_type  x_user(){ return S_IXUSR; }
	// group mode:
	static mode_type rwx_group(){ return S_IRWXG; }
	static mode_type rw_group(){ return S_IRGRP | S_IWGRP; }
	static mode_type rd_group(){ return S_IRGRP; }
	static mode_type wr_group(){ return S_IWGRP; }
	static mode_type  x_group(){ return S_IXGRP; }
	// other mode:
	static mode_type rwx_other(){ return S_IRWXO; }
	static mode_type rw_other(){ return S_IROTH | S_IWOTH; }
	static mode_type rd_other(){ return S_IROTH; }
	static mode_type wr_other(){ return S_IWOTH; }
	static mode_type  x_other(){ return S_IXOTH; }
	// seek
	static int seek_set(){ return SEEK_SET; }
	static int seek_cur(){ return SEEK_CUR; }
	static int seek_end(){ return SEEK_END; }
	// info
#ifdef  __FMAN_INFO__
	static void pub_info( int __flags, int         __fd, std::ostream & out = std::cout ){ info( out, __flags, mode( __fd ) ); }
	static void info_pub( int __flags, mode_type __mode, std::ostream & out = std::cout ){ info( out, __flags, __mode ); }
	void pub_info( std::ostream & out = std::cout )const{ info( out, flags(), mode( fd() ), this ); }
#else
	static void pub_info( int __flags, int         __fd, std::ostream & out = std::cout ){}
	static void info_pub( int __flags, mode_type __mode, std::ostream & out = std::cout ){}
	void pub_info( std::ostream & out = std::cout )const{}
#endif
private:
	int _fd, _flags;
	explicit fman( int __fd ): _fd( __fd ){}
	fman( fman const & );
	fman & operator=( fman const & );
	static void info( std::ostream & out, int __flags, mode_type __mode, void const * addr = nullptr );
#ifdef  __FMAN_INFO__
	enum creat_flags_t{ e_creat, e_creat_trunc, e_creat_excl, e_trunc, e_excl };
	enum open_flags_t{ e_read_only, e_write_only, e_read_write };
	static const char * creat_flags_str[];
	static const char * open_flags_str[];
	static const char * get_creat_flags( int __flags );
	static const char * get_open_flags( int __flags );
#endif
};

inline void fman::info( std::ostream & out, int __flags, mode_type __mode, void const * addr)// static
{
#ifdef  __FMAN_INFO__
	if( addr )
	{
		out << "[" << addr << "] fman" << std::endl;
		out << "file   size : " << ((fman*)addr)->size() << std::endl;
	}
	out << "open  flags : '" << get_open_flags( __flags ) << "'" << std::endl;
	out << "creat flags : '" << get_creat_flags( __flags ) << "'" << std::endl;
	out << "access mode : '" << std::oct << (__mode & (rwx_user() | rwx_group() | rwx_other()) ) << "'" << std::dec << std::endl;
#endif
}
#ifdef  __FMAN_INFO__
const char * fman::creat_flags_str[] = { "creat", "creat_trunc", "creat_excl", "trunc", "excl" };
const char * fman::open_flags_str[] = {"read_only", "write_only", "read_write" };

inline const char * fman::get_creat_flags( int __flags )// static
{
	creat_flags_t x = e_creat_trunc;
	if( (__flags & creat()) && (__flags & trunc()) )
		x = e_creat_trunc;
	else if( (__flags & creat()) && (__flags & excl()) )
		x = e_creat_excl;
	else if( __flags & creat() )
		x = e_creat;
	else if( __flags & trunc() )
		x = e_trunc;
	else if( __flags & excl() )
		x = e_excl;
	return creat_flags_str[x];
}
inline const char * fman::get_open_flags( int __flags )// static
{
	open_flags_t x = e_read_write;
	if( __flags & read_write() ) x = e_read_write;
	else if( __flags & read_only() ) x = e_read_only;
	else if( __flags & write_only() ) x = e_write_only;
	return open_flags_str[x];
}
#endif//__FMAN_INFO__

inline int fman::open( const char * __name, int __flags, mode_type __mode )
{
	__log_info__( this );
	if( __name == nullptr )
		return (_fd = -1);
	_fd = ::open( __name, __flags, __mode );
	if( _fd == -1 )
	{
		__error_msg__( this, "can't open file" );
		std::cerr << "file name   : '" << __name << "'" << std::endl;
		info( std::cerr, __flags, __mode );
		exit(1);
	}
	_flags = __flags;
	return _fd;
}

inline int fman::close()
{
	__log_info__( this );
	if( fd() == -1 )
		return 0;
	int _z = ::close( fd() );
	if( _z == -1 )
	{
		__error_msg__( this, "invalid file descriptor" );
		std::cerr << "file descriptor: '" << fd() << std::endl;
		exit(1);
	}
	_fd = -1;
	return _z;
}
inline void fman::reserve( int __fd, off_type __size )// static
{
	__log_info__( void_cast<void (*)( int, off_type )>( fman::reserve ) );
	if( fman::size( __fd ) >= __size )
		return;
	::lseek( __fd, __size-1, SEEK_SET );
	::write( __fd,       "", 1 );
	::lseek( __fd,        0, SEEK_SET );
}
inline struct stat const & fman::get_stat( int __fd )// static
{
	//__log_info__( void_cast<struct stat const & (*)(int)>( fman::get_stat ) );
	static struct stat __stat;
	int _st = fstat( __fd, &__stat );
	if( _st )
	{
		__error_msg__( void_cast<struct stat const & (*)(int)>( fman::get_stat ), "can't get stat" );
		std::cerr << "fd: " << __fd << std::endl;
		exit(1);
	}
	return __stat;
}

#endif//__FILE_MANAGER_H__
