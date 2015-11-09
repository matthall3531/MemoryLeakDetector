#ifndef _HEAP_ALLOC_MANAGER_H_
#define _HEAP_ALLOC_MANAGER_H_

#include <map>

struct AllocationData
{
  size_t size;
  void **backtrace;
  unsigned short frames;
  unsigned long hash;
};

template <class T>
struct allocator {
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T value_type;

  template <class U> struct rebind { typedef allocator<U> other; };
  allocator() throw() {}
  allocator(const allocator&) throw() {}

  template <class U> allocator(const allocator<U>&) throw() {}

  ~allocator() throw() {}

  //pointer address(reference x) const { return &x; }
  //const_pointer address(const_reference x) const { return &x; }

  pointer allocate(size_type s, void const * = 0) {
    if (0 == s)
      return NULL;
    pointer temp = (pointer)malloc(s * sizeof(T));
    if (temp == NULL)
      throw std::bad_alloc();
    return temp;
  }

  void deallocate(pointer p, size_type) {
    free(p);
  }

  /*size_type max_size() const throw() {
  return std::numeric_limits<size_t>::max() / sizeof(T);
  }*/

  void construct(pointer p, const T& val) {
    new((void *)p) T(val);
  }

  void destroy(pointer p) {
    p->~T();
  }
};


typedef std::map<void*, AllocationData, std::less<void*>, allocator<std::pair<void*, AllocationData>>> AllocMap;
typedef std::map<unsigned long, size_t, std::less<unsigned long>, allocator<std::pair<unsigned long, int>>> HashMap;

class HeapAllocManager
{
public:
  HeapAllocManager() {}
  virtual ~HeapAllocManager() {}

  void register_alloc(void* ptr, size_t size, void **backtrace, unsigned short frames, unsigned long hash);
  void deregister_alloc(void* ptr);

  void dump();

  size_t getAllocationTableSize();

private:
  AllocMap allocationMap;
  HashMap hashMap;

  void dumpAllocation(AllocationData& data);

};

#endif /* _HEAP_ALLOC_MANAGER_H_ */
