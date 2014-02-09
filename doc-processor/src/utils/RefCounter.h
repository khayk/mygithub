#pragma once

#include <boost\intrusive_ptr.hpp>
#include <Unknwn.h>

// ref counting function for COM. required when using boost::intrusive_ptr
inline void intrusive_ptr_add_ref( IUnknown* p_interface )
{
    if ( p_interface )
        p_interface->AddRef();
}

inline void intrusive_ptr_release( IUnknown* p_interface )
{
    if ( p_interface )
        p_interface->Release();
}

// here can be other ref counting classes