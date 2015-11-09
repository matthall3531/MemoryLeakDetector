#include "mld-core.h"
#include "HeapAllocManager.h"
#include "captureStackBackTrace.h"

static HeapAllocManager* pAllocMgr = NULL;

void mld_register_alloc(void *ptr, size_t size)
{
  void *backtrace;
  unsigned short frames;
  unsigned long hash;

  if (pAllocMgr == NULL)
  {
    void *alloc_mgr_ptr = malloc(sizeof(HeapAllocManager));
    if (alloc_mgr_ptr == NULL) {
      throw std::bad_alloc();
    }
    pAllocMgr = new (alloc_mgr_ptr) HeapAllocManager();
  }

  captureStackBackTrace(&backtrace, frames, &hash);
  pAllocMgr->register_alloc(ptr, size, &backtrace, frames, hash);
}

void mld_deregister_alloc(void *ptr)
{
  pAllocMgr->deregister_alloc(ptr);
}

size_t mld_get_number_of_leaks()
{
  return pAllocMgr->getAllocationTableSize();
}

void mld_dump_leaks()
{
  pAllocMgr->dump();
}