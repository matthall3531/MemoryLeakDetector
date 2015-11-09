# MemoryLeakDetector
Small library that makes it possible to track memory leaks and dump a stack trace of where the allocations were made.

Basically the lib overloads new/delete and captures the stack trace where the allocation was made. The trace data is removed
when the corresponding delete is made.

Include the 2 libs, mld-core and mld-op in your application and issue the dump command before exiting. The stack traces
are currently dumped to stdout.

If you are overloading new/delete yourself, you only have to link with the mld-core lib. Call the mld-core functions from
your new/delete code to make it work. See the mld-op lib code for an example of how to do this.

Note that the stack trace depends on the SymInitialize() call finding the correct .pdb file. The code could be extended by adding a number of search paths to look for .pdb files in more places.
