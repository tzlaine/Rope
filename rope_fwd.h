// -*- C++ -*-
#ifndef _ROPE_FWD_H_
#define _ROPE_FWD_H_

#include <boost/smart_ptr/intrusive_ptr.hpp>


class rope;

namespace rope_detail {
    struct rope_node;
    void intrusive_ptr_add_ref(rope_node*);
    void intrusive_ptr_release(rope_node*);
    typedef boost::intrusive_ptr<rope_node> rope_node_ptr;
}


#endif // _ROPE_FWD_H_
