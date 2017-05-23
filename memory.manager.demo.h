#include"memory.manager.h"
#include<iostream>
#include<iomanip>
#include<cstdlib>// rand
#include<ctime>// time
#include"vector.pool.h"
#include"mmap.pool.h"
using namespace std;

inline void randomize_index( int * idx, const size_t size )
{
	srand( time(0) );
	for(int i = 0; i < size; ++i )
	{
		int n = rand()%size;
		swap( idx[i], idx[n] );
		//idx[i] = size - i - 1;
	}
}
//template<std::size_t size> using memory_pool = SinglePool<size>;
template<std::size_t size> using memory_pool = SinglePool<size, mmap_container_struct>;//, offset_pointer_struct>;
//template<std::size_t size> using memory_pool = SinglePool<size, vector_container_struct>;//, offset_pointer_struct>;
void demo_mem()
{
	const size_t memory_size = 4096;// 4KB
	typedef memory_pool<memory_size> spool;
	typedef typename spool::node_pointer node_pointer;
	typedef typename spool::pool_type pool_type;
	typedef typename pool_type::node_type node_type;
	typedef typename pool_type::base_type base_type;
	base_type * pool_p = &spool::get_pool();

	spool::info();
	const size_t size = 12;
	int index[size];
	void * pa[size];
	for( int i = 0; i < size; ++i)
	{
		index[i] = i;
		//pa[i] = spool::allocate( sizeof(double) * (i+1) );
		pa[i] = pool_p->allocate( sizeof(double) * (i+1) );
	}
	double & d = *(double*)(pa[0]);
	d = 3.14153;
	cout << "double   : [" << &d  << "] := " << d << endl;
	spool::info();
	randomize_index( index, size );
	node_pointer lh = spool::get_list_head(), le = spool::get_list_tail();
	int k = 0;
	for( node_pointer np = lh; np != le; np = np->get_next(), ++k )
	{
		cout << setw(4) << k << ": node [" << np << "]" << endl;
		if( np == le->get_prev() )
			cout << setw(4) << k+1 << ": node [" << le << "]" << endl;
	}
	for(int i = 0; i < size; ++i)
	{
		spool::free( pa[ index[i] ] );
	}
	spool::info();
	cout << "--------------------------------------------------------------------" << endl;
	cout << "END" << endl;
}
int main()
{
	demo_mem();
}
