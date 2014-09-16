/**
 * @file /mm_radio/include/mm_radio/publisher.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_radio_PUBLISHER_HPP_
#define mm_radio_PUBLISHER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/time.hpp>
#include <iostream>
#include <mm_messages.hpp>
#include <sstream>
#include <string>
#include <vector>
#include "radio.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_radio {

typedef mm_messages::Publisher<Radio> Publisher;

} // namespace mm_radio

#endif /* mm_radio_PUBLISHER_HPP_ */
