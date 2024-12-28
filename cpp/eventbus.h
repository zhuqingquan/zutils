/*
 * @Author: zhuqingquan
 * @Date: 2024-12-22
 * @FilePath: include/eventbus.h
 * @Description: interface to EventBus lib
 */
#pragma once
#ifndef _ZUTILS_EVENT_BUS_H_
#define _ZUTILS_EVENT_BUS_H_

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <functional>

namespace zutils
{
    class Event
    {
    public:
        Event(const std::string& _t)
            : m_type(_t)
        {

        }

        Event(const std::string& _t, void* sender)
            : m_type(_t)
        {

        }

        virtual ~Event() {}

        const std::string& type() const { return m_type; }
    private:
        std::string m_type;
        void* sender;
    };//class Event

    template<typename EvtType>
    class EventHandler
    {
    public:
        EventHandler() {};
        virtual ~EventHandler() {}

        virtual int onEvent(const EvtType& e) { return 0; }
    };//EventHandler

    class EventBus
    {
    public:
        EventBus();
        virtual ~EventBus();

        virtual bool start();
        virtual bool stop();

        template<typename EvtType>
        bool registEventHandler(const std::string& type, EventHandler<EvtType>* handler);

        template<typename EvtType>
        bool unregistEventHandler(const std::string& type, EventHandler<EvtType>* handler);

        bool emitSync(const std::shared_ptr<Event>& evt);

        typedef std::function<void (const Event& e)> FireEventFunc;

    protected:
        struct slot
        {
            //std::string evtType;
            void* handler = nullptr;
            FireEventFunc fFireEvt;
            slot() {};
            slot(void* _h, FireEventFunc _f)
                : handler(_h), fFireEvt(_f)
            {

            }
        };//struct slot
        typedef std::vector<slot> EventHandlerList; 
        typedef std::map<std::string, EventHandlerList*> HandlersMap; 
        HandlersMap m_handlers;
    };//class EventBus

    EventBus::EventBus()
    {

    }

    EventBus::~EventBus()
    {
        stop();
    }

    bool EventBus::start()
    {
        return false;
    }

    bool EventBus::stop()
    {
        return false;
    }

    template<typename EvtType>
    bool EventBus::registEventHandler(const std::string& type, EventHandler<EvtType>* handler)
    {
        const auto& iter = m_handlers.find(type);
        EventHandlerList* handlerList = nullptr;
        if(iter==m_handlers.end())
        {
            handlerList = new EventHandlerList();
            m_handlers[type] = handlerList;
        }
        else
        {
            handlerList = iter->second;
        }
        for(auto s = handlerList->begin() ; s!=handlerList->end() ; s++)
        {
            if(s->handler==handler) // 如果同一种type，同一个handler对象已经注册过则不再注册，直接返回失败
                return false;
        }
        FireEventFunc f = std::bind([](const Event& e, EventHandler<EvtType>* handler){
            handler->onEvent((const EvtType&)e);
        }, std::placeholders::_1, handler);
        handlerList->emplace_back(slot(handler, f));
        return true;
    }

    template<typename EvtType>
    bool EventBus::unregistEventHandler(const std::string& type, EventHandler<EvtType>* handler)
    {
        return false;
    }

    bool EventBus::emitSync(const std::shared_ptr<Event>& evt)
    {
        const auto& iter = m_handlers.find(evt->type());
        if(iter==m_handlers.end())
            return false;
        EventHandlerList* handlers = iter->second;
        for(auto s = handlers->begin() ; s!=handlers->end() ; s++)
        {
            s->fFireEvt(*evt);
        }
        return true;
    }
}
#endif//_ZUTILS_EVENT_BUS_H_