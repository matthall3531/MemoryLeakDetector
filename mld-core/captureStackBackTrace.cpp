#include "captureStackBackTrace.h"
#include "HeapAllocManager.h"
#include "mld-core.h"
#include <Windows.h>
#include <DbgHelp.h>
#include <new>

typedef USHORT(WINAPI *CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);

static CaptureStackBackTraceType captureFnk = NULL;

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