#include "impl_vposix/wrap_sys_epoll.h"

#include <sys/epoll.h>
#include "impl_vposix/linux_call.h"
#include "impl_vposix/wrap_unistd.h"
#include "vlog.h"

using namespace impl_vposix;


//=======================================================================================
//      epoll_receiver, throws not implemented message
//=======================================================================================
impl_vposix::epoll_receiver::~epoll_receiver()
{}
//=======================================================================================
void impl_vposix::epoll_receiver::on_ready_read()
{
    throw verror << "not implemented";
}
//=======================================================================================
void impl_vposix::epoll_receiver::on_ready_write()
{
    throw verror << "not implemented";
}
//=======================================================================================
void impl_vposix::epoll_receiver::on_hang_up()
{
    throw verror << "not implemented";
}
//=======================================================================================

//=======================================================================================
//      epoll as normal class
//=======================================================================================
epoll::epoll()
    : _efd( wrap_sys_epoll::create() )
{}
//=======================================================================================
epoll::~epoll()
{
    wrap_unistd::close( _efd );
}
//=======================================================================================
void epoll::add_read( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::add( _efd, fd, wrap_sys_epoll::In, receiver );
}
//=======================================================================================
void epoll::add_write( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::add( _efd, fd, wrap_sys_epoll::Out, receiver );
}
//=======================================================================================
void epoll::add_rw( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::add( _efd, fd, wrap_sys_epoll::InOut, receiver );
}
//=======================================================================================
void epoll::mod_read( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::mod( _efd, fd, wrap_sys_epoll::In, receiver );
}
//=======================================================================================
void epoll::mod_write( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::mod( _efd, fd, wrap_sys_epoll::Out, receiver );
}
//=======================================================================================
void epoll::mod_rw( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::mod( _efd, fd, wrap_sys_epoll::InOut, receiver );
}
//=======================================================================================
void epoll::remove( int fd )
{
    wrap_sys_epoll::del( _efd, fd );
}
//=======================================================================================
void epoll::wait_once()
{
    wrap_sys_epoll::wait_once( _efd );
}
//=======================================================================================

//=======================================================================================
//      wrap_sys_epoll, only wrappers for kernel calls
//=======================================================================================
//  http://man7.org/linux/man-pages/man2/epoll_create.2.html
int wrap_sys_epoll::create()
{
    return linux_call::check( ::epoll_create1, EPOLL_CLOEXEC );
}
//=======================================================================================
static void econtrol( int efd,
                      int fd,
                      int operation,
                      uint32_t events,
                      epoll_receiver* receiver )
{
    assert( receiver );
    epoll_event event;
    event.data.ptr = receiver;
    event.events   = events|EPOLLRDHUP|EPOLLPRI;
    linux_call::check( ::epoll_ctl, efd, operation, fd, &event );
}
//---------------------------------------------------------------------------------------
static uint32_t direction( wrap_sys_epoll::Direction d )
{
    switch (d)
    {
    case wrap_sys_epoll::In:    return EPOLLIN;
    case wrap_sys_epoll::Out:   return EPOLLOUT;
    case wrap_sys_epoll::InOut: return EPOLLIN|EPOLLOUT;
    }
    throw verror( "Direction not delected" );
}
//=======================================================================================
void wrap_sys_epoll::add( int efd, int fd, wrap_sys_epoll::Direction d,
                          epoll_receiver *receiver )
{
    econtrol( efd, fd, EPOLL_CTL_ADD, direction(d), receiver );
}
//=======================================================================================
void wrap_sys_epoll::mod( int efd, int fd, wrap_sys_epoll::Direction d,
                          epoll_receiver *receiver )
{
    econtrol( efd, fd, EPOLL_CTL_MOD, direction(d), receiver );
}
//=======================================================================================
void wrap_sys_epoll::del( int efd, int fd )
{
    econtrol( efd, fd, EPOLL_CTL_DEL, 0, nullptr );
}
//=======================================================================================
void wrap_sys_epoll::wait_once( int efd )
{
    enum { Waits_Count = 16 };

    struct epoll_event events[Waits_Count];
    int count = linux_call::check( ::epoll_wait, efd, events, Waits_Count, -1 );

    for ( int i = 0; i < count; ++i )
    {
        epoll_receiver *receiver = static_cast<epoll_receiver*>( events[i].data.ptr );
        uint32_t event = events[i].events;

        if ( event & EPOLLIN )
            receiver->on_ready_read();
        event &= ~EPOLLIN;

        if ( event & EPOLLOUT )
            receiver->on_ready_write();
        event &= ~EPOLLOUT;

        if ( event & EPOLLRDHUP )
            receiver->on_hang_up();
        event &= ~EPOLLRDHUP;

        if ( event )
            throw verror.hex()("Not all flags was recognized, leaved: ", event );
    }
}
//=======================================================================================
