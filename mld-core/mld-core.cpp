#include "mld-core.h"
#include "HeapAllocManager.h"
#include "captureStackBackTrace.h"

/// The instance of the HeapAllocManager that keeps track of allocated data.
static HeapAllocManager* pAllocMgr = NULL;

/**
 * Register allocation with the HeapAllocationManager.
 * @param ptr Memory pointer
 * @param size Size of allocation
 */
void mld_register_alloc(void *ptr, size_t size)
{
  void *backtrace;
  unsigned short frames;
  unsigned long hash;

  if (pAllocMgr == NULL)
  {
    void *alloc_mgr_ptr = malloc(sizeof(HeapAllocManager));
    if (alloc_mgr_ptr == NULL) 
    {
      throw std::bad_alloc();
    }
    pAllocMgr = new (alloc_mgr_ptr) HeapAllocManager();
  }

  captureStackBackTrace(3, &backtrace, frames, &hash);
  pAllocMgr->register_alloc(ptr, size, &backtrace, frames, hash);
}

/**
 * Remove allocation data from the HeapAllocationManager.
 * @ptr Memory pointer
 */
void mld_deregister_alloc(void *ptr)
{
  if (!pAllocMgr->deregister_alloc(ptr))
  {
    // The ptr was not in the allocation table
    void *backtrace;
    unsigned short frames;
    unsigned long hash;
    captureStackBackTrace(2, &backtrace, frames, &hash);
    pAllocMgr->dumpStackTrace(&backtrace, frames);
  }
}

/**
 * Returns the number of allocations currently in the allocation table.
 */
size_t mld_get_number_of_leaks()
{
  return pAllocMgr->getAllocationTableSize();
}

/**
 * Dump the current allocation table (considered leaks) to stdout.
 */
void mld_dump_leaks()
{
  if (pAllocMgr != NULL)
  {
    pAllocMgr->dump();
  }
}