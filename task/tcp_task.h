#ifndef __TCP_TASK__H__
#define __TCP_TASK__H__
class TcpTask
{
public:
    TcpTask() = default;
    TcpTask(const TcpTask &other) = default;
    TcpTask(TcpTask &&other) = default;
    virtual void execute() = 0;
    virtual ~TcpTask() = default;
};
#endif //!__TCP_TASK__H__