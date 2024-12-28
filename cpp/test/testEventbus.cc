/*
 * @Author: zhuqingquan
 * @Date: 2024-12-22 
 * @FilePath: /EventBus/include/testEventbus.cc
 * @Description: simple test code for eventbus. for example.
 */
#include <iostream>
#include "eventbus.h"

using namespace std;
using namespace zutils;

class TestEvent : public Event
{
public:
    TestEvent(const std::string& msg)
        : Event("TestEvent")
        , m_msg(msg)
    {

    }

    std::string m_msg;
};//class TestEvent

class TestEventHandler : public EventHandler<TestEvent>
{
public:
    virtual int onEvent(const TestEvent& e) 
    { 
        cout << "TestEvent is fired. msg=" << e.m_msg << endl;
        return 0; 
    }
};//class TestEventHandler

int main(int argc, char* argv[])
{
    cout << "start event bus" << endl;
    EventBus eb;
    TestEventHandler* handler = new TestEventHandler();
    eb.registEventHandler("TestEvent", handler);
    eb.emitSync(std::make_shared<TestEvent>("hello TestEvent"));
    eb.unregistEventHandler("TestEvent",handler);
    delete handler;
    return 0;
}