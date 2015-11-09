#include <stdlib.h>
#include <new>
#include <mld-core.h>
#include <stdint.h>

void* operator new(std::size_t size)
{
  void *ptr = malloc(size);

  mld_register_alloc(ptr, size);
  
  return ptr;
}

void * operator new[](size_t size)
{
  return operator new(size);
}

void operator delete[](void *ptr)
{
  delete ptr;
}

void operator delete(void *ptr)
{
  free(ptr);
  mld_deregister_alloc(ptr);
}




