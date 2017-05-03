#include"memory.manager.h"
#include<iostream>
#include<iomanip>
#include<cstdlib>// rand
#include<ctime>// time
using namespace std;

template<typename T> class Singleton;

inline void randomize_index( int * idx, const size_t size )
{
	srand( time(0) );
	for(int i = 0; i < size; ++i )
	{
		int n = rand()%size;
		swap( idx[i], idx[n] );
	}
}
//template<std::size_t size> using memory_pool = SinglePool<size>;
//template<std::size_t size> using memory_pool = SinglePool<size, offset_pointer_struct, mmap_container_struct>;
template<std::size_t size> using memory_pool = SinglePool<size, offset_pointer_struct, vector_container_struct>;
void demo_mem()
{
	const size_t memory_size = 4096;// 4KB
	typedef memory_pool<memory_size> pool;
	typedef typename pool::node_pointer node_pointer;
	typedef typename pool::pool_type pool_type;
	typedef typename pool_type::node_type node_type;

	cout << "list_head: [" << pool::get_list_head() << "]" << endl;
	cout << "list_tail: [" << pool::get_list_tail() << "]" << endl;
	cout << "next_free: [" << pool::get_next_free() << "]" << endl;
	const size_t size = 12;
	int index[size];
	void * pa[size];
	for( int i = 0; i < size; ++i)
	{
		index[i] = i;
		pa[i] = pool::allocate( sizeof(double) + size_t(i/4) );
	}
	cout << "amount of nodes: " << node_type::get_count() << endl;
	double & d = *(double*)(pa[0]);
	d = 3.14153;
	cout << "double   : [" << &d  << "] := " << d << endl;
	cout << "next_free: [" << pool::get_next_free() << "]" << endl;
	cout << "free_size: " << pool::free_size() << endl;
	randomize_index( index, size );
	node_pointer lh = pool::get_list_head(), le = pool::get_list_tail();
	int k = 0;
	for( node_pointer np = lh; np != le; np = np->get_next(), ++k )
	{
		cout << setw(4) << k << ": node [" << np << "]" << endl;
		if( np == le->get_prev() )
			cout << setw(4) << k+1 << ": node [" << le << "]" << endl;
	}
	for(int i = 0; i < size; ++i)
	{
		pool::free( pa[ index[i] ] );
	}
	cout << "next_free: [" << pool::get_next_free() << "]" << endl;
	cout << "nf->prev : [" << pool::get_next_free()->get_prev() << "]" << endl;
	cout << "nf->next : [" << pool::get_next_free()->get_next() << "]" << endl;
}
int main()
{
	demo_mem();
}
