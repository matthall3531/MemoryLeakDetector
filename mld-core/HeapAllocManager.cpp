#include "HeapAllocManager.h"
#include <Windows.h>
#include <DbgHelp.h>
#include <map>

void HeapAllocManager::register_alloc(void* ptr, size_t size, void **backtrace, unsigned short frames, unsigned long hash)
{
  struct AllocationData data;
  data.size = size;
  data.backtrace = (void**)malloc(sizeof(void**));
  *data.backtrace = *backtrace;
  data.frames = frames;
  data.hash = hash;
  allocationMap[ptr] = data;
  hashMap[hash]++;
}

void HeapAllocManager::deregister_alloc(void* ptr)
{
  AllocationData &data = allocationMap[ptr];
  hashMap[data.hash]--;
  allocationMap.erase(ptr);
}

void HeapAllocManager::dumpAllocation(AllocationData& data)
{
  SYMBOL_INFO* symbol;
  IMAGEHLP_LINE64 *line = (IMAGEHLP_LINE64 *)malloc(sizeof(IMAGEHLP_LINE64));
  line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
  symbol = (SYMBOL_INFO *)calloc(1, sizeof(SYMBOL_INFO) + 256 * sizeof(char));
  symbol->MaxNameLen = 255;
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
  HANDLE process = GetCurrentProcess();
  const unsigned short  MAX_CALLERS_SHOWN = 20;

  DWORD64* trace = (DWORD64*)(*data.backtrace);

  printf("\n%zu allocations not freed (%zu bytes in total) ->\n", hashMap[data.hash], hashMap[data.hash] * data.size);
  hashMap.erase(data.hash);
  printf("%zu bytes not freed was allocated at :\n", data.size);
  unsigned short frames = data.frames < MAX_CALLERS_SHOWN ? data.frames : MAX_CALLERS_SHOWN;
  for (unsigned int i = 0; i < frames; i++)
  {
    SymFromAddr(process, trace[i], 0, symbol);
    DWORD dwDisplacement;
    SymGetLineFromAddr64(process, trace[i], &dwDisplacement, line);
    printf("\t%d : %s, %s - line %d\n", i, symbol->Name, line->FileName, line->LineNumber);
  }
}

void HeapAllocManager::dump()
{
  size_t totalLeaked = 0;
  for (AllocMap::iterator it = allocationMap.begin(); it != allocationMap.end(); it++)
  {
    struct AllocationData &data = (*it).second;
    if (hashMap[data.hash] != 0)
    {
      totalLeaked += hashMap[data.hash] * data.size;
      dumpAllocation((*it).second);
    }
  }
  printf("\nTOTAL LEAKED : %zu bytes\n", totalLeaked);
}

size_t HeapAllocManager::getAllocationTableSize()
{
  return allocationMap.size();
}
