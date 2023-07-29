#include <iostream>
#include "../sylar/log.h"
#include "../sylar/util.h"
int main(int argc, char const *argv[])
{
    sylar::Logger::ptr logger(new sylar::Logger);
    // std::cout<< "hello world"<<std::endl;
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));
    // sylar::LogEvent::ptr event(new  sylar::LogEvent(__FILE__,__LINE__,0,sylar::GetThreadId(),sylar::GetFiberId(),time(0)));
    // event->getSS() << "hello sylar log";
    // logger->log(sylar::LogLevel::DEBUG,event);
    SYLAR_LOG_DEBUG(logger) << "test macro";
    return 0;
}
