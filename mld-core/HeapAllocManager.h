#ifndef _HEAP_ALLOC_MANAGER_H_
#define _HEAP_ALLOC_MANAGER_H_

#include <map>

/// AllocationData is used to store the stack trace information for each allocation
struct AllocationData
{
  AllocationData() : size(0), backtrace(NULL), frames(0), hash(0) {}
  size_t size;
  void **backtrace;
  unsigned short frames;
  unsigned long hash;
};

/// Custom allocator for the maps used to hold data. Basically uses malloc
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
    if (s == 0)
    {
      return NULL;
    }
    pointer ptr = (pointer)malloc(s * sizeof(T));
    if (ptr == NULL)
    {
      throw std::bad_alloc();
    }
    return ptr;
  }

  void deallocate(pointer p, size_type) 
  {
    free(p);
  }

  void construct(pointer p, const T& val) 
  {
    new((void *)p) T(val);
  }

  void destroy(pointer p) 
  {
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
  bool deregister_alloc(void* ptr);

  void dumpStackTrace(void **backtrace, unsigned short iFrames);

  void dump();

  size_t getAllocationTableSize();

private:
  AllocMap allocationMap;
  HashMap hashMap;

  void dumpAllocation(AllocationData& data);

};

#endif /* _HEAP_ALLOC_MANAGER_H_ */
