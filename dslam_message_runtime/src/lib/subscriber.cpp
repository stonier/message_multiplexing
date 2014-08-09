/**
 * @file /dslam_message_runtime/src/lib/subscriber.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <nanomsg/nn.h>
#include "../../include/dslam_message_runtime/mux.hpp"
#include "../../include/dslam_message_runtime/subscriber.hpp"

// for debugging
//#include <cstring>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Implementation
*****************************************************************************/

Subscriber::Subscriber(const std::string &name) :
    name(name)
{}

Subscriber::~Subscriber() {
}

} // dslam
