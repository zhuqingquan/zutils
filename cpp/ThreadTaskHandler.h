/**
 * @file ThreadTaskHandler.h
 * @author zhuqingquan
 * @brief  将任务调度到指定的线程中 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _THREAD_TASK_DISPATCHER_H_
#define _THREAD_TASK_DISPATCHER_H_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <deque>
#include <vector>

namespace zUtils
{

/**
 * @name	ThreadTask
 * @brief	将外部对HYTransMod的调用封装成Task调度到TransportThread中执行
 **/
class ThreadTask
{
public:
    typedef std::function<void(void*)> TaskFunction;

	ThreadTask(uint32_t reqType, TaskFunction func, void* context)
		: m_reqType(reqType), m_func(func), m_ctx(context)
	{

	}

	ThreadTask(const ThreadTask& robj)
		: m_reqType(robj.m_reqType), m_func(robj.m_func), m_ctx(robj.m_ctx)
	{

	}

	ThreadTask(TaskFunction f, void* ctx = nullptr)
		: m_reqType(0), m_func(f), m_ctx(ctx)
	{

	}

	ThreadTask& operator=(const ThreadTask& robj)
	{
		if(this==&robj)
			return *this;
		m_reqType = robj.m_reqType;
		m_func = robj.m_func;
		m_ctx = robj.m_ctx;
        return *this;
	}

    explicit operator bool() const { return m_func!=nullptr; }

	uint32_t getRequestType() const { return m_reqType; }

	bool operator()() const
	{
		if(m_func==nullptr)
			return false;
		m_func(m_ctx);
		return true;
	}
private:
	uint32_t m_reqType;
	TaskFunction m_func = nullptr;
	void* m_ctx = nullptr;
};

/**
 * @brief 启动一个线程，并不断执行通过add接口添加到队列中的ThreadTask
 */
class ThreadTaskHandler
{
public:
    ThreadTaskHandler();
    ~ThreadTaskHandler();

    bool start();
    void stop();

    /**
     * @brief 添加可执行函数代码到线程中执行
     * @return true--添加成功 false--失败，当Handler未启动时调用add将返回失败
     **/
    bool add(ThreadTask::TaskFunction f, void* ctx = nullptr);

    /**
     * @brief 添加一个定时器，该对象内的线程将按照intervalMillsec指定的时间间隔调用f函数
     *        这里实现的定时器的精度并不太高，而且是使用单个线程回调多个定时器因此定时器的回调函数不建议执行太复杂的操作或者不要添加多个定时器
     * @param intervalMillsec 时间间隔，毫秒数
     * @param f 触发时调用的线程
     * @param ctx 用户数据，回调时将带上
     * @return 返回Timer的ID
     **/
    uint64_t addTimer(int intervalMillsec, ThreadTask::TaskFunction f, void* ctx = nullptr);

    /**
     * @brief 根据addTimer时返回的Timer ID删除Timer
     * @param timerId Timer的ID
     * @return true--成功 false--失败
     **/
    bool removeTimer(uint64_t timerId);
private:
    ThreadTaskHandler(const ThreadTaskHandler& obj) = delete;
    ThreadTaskHandler& operator=(const ThreadTaskHandler& obj) = delete;

    struct TimerContext
    {
        ThreadTask::TaskFunction func = nullptr;
        void* userData = nullptr;
        int intervalMillsec = 0;
        uint64_t lastTriggle = 0;       // 最近一次触发的时间记录
                                        //
        TimerContext(ThreadTask::TaskFunction f, void* _userData, int interval)
            : func(f), userData(_userData), intervalMillsec(interval)
        {
        }

        TimerContext(const TimerContext& robj)
            : func(robj.func), userData(robj.userData), intervalMillsec(robj.intervalMillsec)
        {
        }
    };

    const TimerContext* findTimer(ThreadTask::TaskFunction f, int interval) const;

	void getAllTasks(std::deque<ThreadTask>& tasks);
    void doTasks();

    bool m_isStarted = false;

    std::thread* m_thCallback = nullptr;           // thread for callback tasks
    std::mutex m_mutexCBTasks;          // mutex for callback tasks
    std::condition_variable m_cvCBTasks;    // conditian variable for callback tasks
    std::deque<ThreadTask> m_cbTasks;   // list of callback tasks
    std::vector<TimerContext> m_timers; // 所有定时器对象

};//class ThreadTaskHandler

class ThreadPool
{
public:
    ThreadPool(size_t threadCount = 4);
    ~ThreadPool();
    void start();
    void stop();

    bool add(ThreadTask::TaskFunction f, void* ctx = nullptr);

private:
    void doTasks();
    ThreadTask fetchTask();

    size_t m_thCount = 4;
    std::vector<std::thread*> m_threads;
    std::mutex m_mutexCBTasks;          // mutex for callback tasks
    std::condition_variable m_cvCBTasks;    // conditian variable for callback tasks
    std::deque<ThreadTask> m_cbTasks;   // list of callback tasks
    std::atomic<bool> m_isRunning = false;
};//ThreadPool
}//namespace zUtils
#endif//_THREAD_TASK_DISPATCHER_H_
