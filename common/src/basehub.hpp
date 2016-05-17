/*
 * =====================================================================================
 *
 *       Filename: basehub.hpp
 *        Created: 04/03/2016 03:49:00
 *  Last Modified: 05/16/2016 22:26:18
 *
 *    Description: 
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */
#pragma once

#include <thread>
#include <atomic>

template<typename Derived>
class BaseHub
{
    protected:
        // defind thread state of the hub
        //     0:   terminated
        //     1:   stopping
        //     2:   running
        //
        // won't make an enum for it since there is only three possibility
        std::atomic<int> m_ThreadState;
        std::thread      m_Thread;

    public:
        BaseHub()
            : m_ThreadState(0)
        {}

        virtual ~BaseHub() = default;

    public:
        void Launch()
        {
            State(2);
            m_Thread = std::thread([this](){ (static_cast<Derived*>(this))->MainLoop(); });
        }

        void Stop()
        {
            State(1);
        }

        void Join()
        {
            if(m_Thread.joinable()){ m_Thread.join(); }
        }

    public:
        int State()
        {
            return m_ThreadState.load(std::memory_order_relaxed);
        }

        void State(int nNewState)
        {
            m_ThreadState.store(nNewState, std::memory_order_relaxed);
        }

};
