#ifndef _ALLOCATOR_HPP
#define _ALLOCATOR_HPP

#include <xmemory>

// Pre-declaration of IDA function
void* qalloc(size_t size);
// Pre-declaration of IDA function
void  qfree(void* alloc);

namespace mcore_n
{
	/************************************************************************/
	/* The goal is to use own allocator within the STL containers           */
	/* IDA requires the memory to be (de)allocated with qfree, qalloc etc.  */
	/************************************************************************/
	namespace allocator_n
	{
		template <typename T> class allocator_c : public std::_Allocator_base<T>
		{
		public:
			typedef size_t                   size_type;
			typedef ptrdiff_t                difference_type;
			typedef T*                       pointer;
			typedef const T*                 const_pointer;
			typedef T&                       reference;
			typedef const T&                 const_reference;
			template <typename Other> struct rebind
			{
				typedef allocator_c<Other> other;
			};
		public:
			allocator_c() {}
			allocator_c(const allocator_c<T>&) {}
			template <class Other> allocator_c(const allocator_c<Other> &) {}
			template <class Other> allocator_c<T> &operator =(const allocator_c<Other> &) { return (*this); }
			~allocator_c() {}
			// return address of mutable ref
			pointer address(reference ref) { return &ref; }
			// return address of non-mutable _Val
			const_pointer address(const_reference ref) { return &ref; }
			// allocate array of count elements, ignore hint
			pointer allocate(size_type count, const void* = 0) { return count != 0 ? (pointer)::qalloc(count * sizeof T) : 0; }
			// deallocate object at ptr, ignore size
			void deallocate(pointer ptr, size_type) { ::qfree(ptr); }
			// estimate maximum array size
			size_type max_size() const { return (size_type)(-1) / sizeof(T); }
			// construct object at ptr with value val
			void construct(pointer ptr, const_reference val) { std::_Construct(ptr, val); }
			// destroy object at ptr
			void destroy(pointer ptr) { std::_Destroy(ptr); }
		};
		// Allocators are always equal
		template <class T1, class T2>
			inline bool operator ==(const allocator_c<T1>&, const allocator_c<T2>&)
		{
			return true;
		}
		template <class T1, class T2>
			inline bool operator !=(const allocator_c<T1>&, const allocator_c<T2>&)
		{
			return false;
		}
	}
}
#endif // #ifndef _ALLOCATOR_HPP