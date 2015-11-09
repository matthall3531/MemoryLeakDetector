#include "captureStackBackTrace.h"
#include "HeapAllocManager.h"
#include "mld-core.h"
#include <Windows.h>
#include <DbgHelp.h>
#include <new>

typedef USHORT(WINAPI *CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);

static HeapAllocManager* pAllocMgr = NULL;

static CaptureStackBackTraceType captureFnk;

void captureStackBackTrace(void **backtrace, unsigned short& frames, unsigned long* trace_hash)
{
  const int kMaxCallers = 62;

  if (captureFnk == NULL)
  {
    captureFnk = (CaptureStackBackTraceType)(GetProcAddress(LoadLibrary("kernel32.dll"), "RtlCaptureStackBackTrace"));
  }
  if (captureFnk == NULL)
  {
    abort();
  }

  void* callers_stack[kMaxCallers];
  HANDLE process;
  process = GetCurrentProcess();
  SymInitialize(process, NULL, TRUE);
  frames = (*captureFnk)(2, kMaxCallers, callers_stack, trace_hash);

  *backtrace = calloc(frames, sizeof(void*));
  memcpy(*backtrace, callers_stack, frames * sizeof(void*));
}

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