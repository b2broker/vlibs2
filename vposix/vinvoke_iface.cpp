#include "vinvoke_iface.h"

#include <cassert>

//=======================================================================================
void vinvoke_iface::invoke( vinvoke_iface::task_type f )
{
    assert( f );
    _invoke( std::move(f) );
}
//=======================================================================================
vinvoke_iface::~vinvoke_iface()
{}
//=======================================================================================
