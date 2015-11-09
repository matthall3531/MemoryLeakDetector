#ifndef _CAPTURE_STACK_BACK_TRACE_H_
#define _CAPTURE_STACK_BACK_TRACE_H_

void captureStackBackTrace(int skipFrames, void **backtrace, unsigned short& frames, unsigned long* hash);

#endif /* _CAPTURE_STACK_BACK_TRACE_H_ */
