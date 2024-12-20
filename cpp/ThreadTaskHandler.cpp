/*
 * @Date: 2023-02
 * @Description: 实现将操作调度到特定的线程
 */

#include "ThreadTaskHandler.h"
#include <chrono>
#include <vector>

using namespace zUtils;

#define TASK_COUNT_MAX 300

ThreadTaskHandler::ThreadTaskHandler()
{

}

ThreadTaskHandler::~ThreadTaskHandler()
{
    stop();
}

bool ThreadTaskHandler::start()
{
    if(m_thCallback!=nullptr)
        return false;
    //logi("ThreadTaskHandler start this=%p", this);
    m_isStarted = true;
    m_thCallback = new std::thread(&ThreadTaskHandler::doTasks, this);
    return true;
}

void ThreadTaskHandler::stop()
{
    if(m_thCallback==nullptr) return;

    m_isStarted = false;

    m_cvCBTasks.notify_all();
    if(m_thCallback->joinable()) 
    {
        m_thCallback->join();
    }
    delete m_thCallback;
    m_thCallback = nullptr;
    //logi("ThreadTaskHandler stop done this=%p", this);
}

bool ThreadTaskHandler::add(ThreadTask::TaskFunction f, void* ctx /*= nullptr*/)
{
    if(!m_isStarted) return false;
    {
        std::unique_lock<std::mutex> lock(m_mutexCBTasks);
        m_cbTasks.push_back(std::move(ThreadTask(f, ctx)));
    }
    m_cvCBTasks.notify_one();
        //printf("push task success \n");
    return true;
}

uint64_t ThreadTaskHandler::addTimer(int intervalMillsec, ThreadTask::TaskFunction f, void* ctx /*= nullptr*/)
{
    std::unique_lock<std::mutex> lock(m_mutexCBTasks);
    int index = m_timers.size();
    m_timers.push_back(TimerContext(f, ctx, intervalMillsec));
    return (uint64_t)&m_timers[index];
}

bool ThreadTaskHandler::removeTimer(uint64_t timerId)
{
    std::unique_lock<std::mutex> lock(m_mutexCBTasks);
    for(std::vector<TimerContext>::iterator iter = m_timers.begin();
            iter!=m_timers.end();
            iter++)
    {
        uint64_t id = (uint64_t)&(*iter);
        if(timerId==id)
        {
            m_timers.erase(iter);
            return true;
        }
    }
    return false;
}
void ThreadTaskHandler::getAllTasks(std::deque<ThreadTask>& tasks)
{
	std::unique_lock<std::mutex> lock(m_mutexCBTasks);
	tasks.swap(m_cbTasks);
}

void ThreadTaskHandler::doTasks()
{
    do{
        std::deque<ThreadTask> tasks;
        std::vector<TimerContext> timerNeedTriggle;
        {
            std::unique_lock<std::mutex> lock(m_mutexCBTasks);
            uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            int minInterval = -1;
            uint64_t nearestTriggleTime = 0;
            // 如果设置了timer，则获取timer列表中最小的定时间隔值以及距离当前时间最早触发的时间点
            for(auto& iter : m_timers)
            {
                if(-1==minInterval || iter.intervalMillsec < minInterval)
                    minInterval = iter.intervalMillsec;
                if(iter.lastTriggle==0) 
                {
                    // 如果定时器一次都没有触发过，则直接触发一次
                    nearestTriggleTime = now;
                }
                else
                {
                    if(0==nearestTriggleTime || iter.lastTriggle + iter.intervalMillsec < nearestTriggleTime)
                        nearestTriggleTime = iter.lastTriggle + iter.intervalMillsec;
                }
            }
            if(minInterval!=-1)
            {
                minInterval /= 2;
                minInterval = minInterval <= 0 ? 1 : minInterval < 5 ? minInterval : 5;
            }
            while(m_cbTasks.empty() && m_isStarted)
            {
                //printf("start wait\n");
                // wait for data
                if(minInterval<0)
                    m_cvCBTasks.wait_for(lock, std::chrono::milliseconds(100));
                else
                {
                    m_cvCBTasks.wait_for(lock, std::chrono::milliseconds(minInterval));
                    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    if(now >= nearestTriggleTime)   // 如果当前时间超过了最早需要触发的timer的时间点就退出while
                        break;
                }
            }
            //printf("wait success size=%lu\n", m_cbTasks.size());
            tasks.swap(m_cbTasks);

            // 收集已经达到触发时间的timer
            now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            for(auto& iter : m_timers)
            {
                if(iter.lastTriggle==0 || now > (iter.lastTriggle + iter.intervalMillsec))
                {
					if (iter.lastTriggle == 0)
						iter.lastTriggle = now;
					else
						iter.lastTriggle += iter.intervalMillsec;
                    timerNeedTriggle.push_back(iter);
                }
            }
        }
        
        for (auto iter : tasks)
        {
            iter();
            if (tasks.size() > TASK_COUNT_MAX && !m_isStarted)
            {
                //loge("TaskHandler drop %u task because this handler stopped.this=%p", tasks.size(), this);
                break;
            }
        }
        if(!m_isStarted)
            break;
        for( auto iter : timerNeedTriggle )
        {
            if(iter.func!=nullptr)
            {
                iter.func(iter.userData);
            }
        }
    }while(m_isStarted);
	// 线程在结束前，做完剩余的Task
	std::deque<ThreadTask> tasks;
	getAllTasks(tasks);
    if (tasks.size() > TASK_COUNT_MAX)
    {
        //loge("TaskHandler too much tasks. drop %u task when stopped.this=%p", tasks.size(), this);
        return;
    }
	for (auto iter : tasks)
	{
		iter();
	}
}

zUtils::ThreadPool::ThreadPool(size_t threadCount)
: m_thCount(threadCount)
{
}

zUtils::ThreadPool::~ThreadPool()
{
    stop();
}

void zUtils::ThreadPool::start()
{
    if(m_isRunning) return;
    m_isRunning = true;
    for(size_t i = 0; i < m_thCount; ++i)
    {
        m_threads.emplace_back(new std::thread(&ThreadPool::doTasks, this));
    }
}

void zUtils::ThreadPool::stop()
{
    m_isRunning = false;
    m_cvCBTasks.notify_all();
    for(auto th : m_threads)
    {
        if(th && th->joinable())
        {
            th->join();
        }
        delete th;
    }
    m_threads.clear();
}

bool zUtils::ThreadPool::add(ThreadTask::TaskFunction f, void *ctx)
{
    if(!m_isRunning) return false;
    {
        std::unique_lock<std::mutex> lock(m_mutexCBTasks);
        m_cbTasks.push_back(std::move(ThreadTask(f, ctx)));
    }
    m_cvCBTasks.notify_one();
        //printf("push task success \n");
    return true;
}

void zUtils::ThreadPool::doTasks()
{
    while(m_isRunning)
    {
        ThreadTask task = fetchTask();
        task();
    }
}

ThreadTask zUtils::ThreadPool::fetchTask()
{
    std::unique_lock<std::mutex> lock(m_mutexCBTasks);
    m_cvCBTasks.wait(lock, [&](){return m_isRunning && !m_cbTasks.empty();});
    if(m_isRunning && !m_cbTasks.empty())
    {
        ThreadTask task = std::move(m_cbTasks.front());
        m_cbTasks.pop_front();
        return task;
    }
    return ThreadTask(nullptr);
}
