#include "poll_context.h"

#include <queue>
#include <mutex>
#include <cassert>
#include <thread>

#include "impl_vposix/wrap_sys_epoll.h"
#include "impl_vposix/wrap_sys_eventfd.h"

#include "vlog.h"

using namespace impl_vpoll;



//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
class poll_context::pimpl final : public impl_vposix::epoll_receiver
{
public:
    pimpl();
    ~pimpl() override;

    std::queue<task_type> tasks;
    std::mutex tasks_mutex;

    std::thread::id my_id;

    impl_vposix::eventfd semaphore;

    impl_vposix::epoll poll;
    bool in_poll  { false };
    bool let_stop { false };

    void on_ready_read() override;
};
#pragma GCC diagnostic pop
//=======================================================================================
poll_context::pimpl::pimpl()
{
    my_id = std::this_thread::get_id();

    poll.add_read( semaphore.handle(), this );
}
//=======================================================================================
poll_context::pimpl::~pimpl()
{
    poll.del( semaphore.handle() );
}
//=======================================================================================
void poll_context::pimpl::on_ready_read()
{
    task_type task;
    while ( semaphore.read() )
    {
        {
            std::lock_guard<std::mutex> lock( tasks_mutex );
            assert( !tasks.empty() );
            task = std::move( tasks.front() );
            tasks.pop();
        }

        if ( task )
            task();
        else
            let_stop = true;

    } // while has task.
}
//=======================================================================================
poll_context::poll_context()
    : p( new pimpl )
{}
//=======================================================================================
poll_context::~poll_context()
{}
//=======================================================================================
void poll_context::poll()
{
    if( p->in_poll )
        throw verror << "Recurse poll detected.";

    if( p->my_id != std::this_thread::get_id() )
        throw verror << "Poll in other thread";

    p->in_poll  = true;

    p->let_stop = false;
    try
    {
        while ( !p->let_stop )
            p->poll.wait_once();
    }
    catch (...)
    {
        p->in_poll = false;
        throw;
    }

    p->in_poll = false;
}
//=======================================================================================
void poll_context::stop()
{
    push( nullptr );
}
//=======================================================================================
void poll_context::push( task_type && task )
{
    {
        std::lock_guard<std::mutex> lock( p->tasks_mutex );
        p->tasks.push( std::move(task) );
    }
    p->semaphore.write();
}
//=======================================================================================
