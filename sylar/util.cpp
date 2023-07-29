#include "util.h"

pid_t sylar::GetThreadId() { return syscall(SYS_gettid); }

uint32_t sylar::GetFiberId() { return 0; }
