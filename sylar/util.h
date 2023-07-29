#pragma once
#include<sys/syscall.h>
#include<unistd.h>
#include <pthread.h>
#include <cstdint>
namespace sylar {
    pid_t GetThreadId();
    uint32_t GetFiberId();
}