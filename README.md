# MemoryLeakDetector
Small library that makes it possible to track memory leaks and dump a stack trace of where the allocations were made.

Basically the lib overloads new/delete and captures the stack trace where the allocation was made. The trace data is removed
when the corresponding delete is made. 

Include the 2 libs, mld-core and mld-op in your application and issue the dump command before exiting. The stack traces
are currently dumped to stdout.
