/**
 * @file /dslam_message_runtime/include/dslam_message_runtime/impl/header.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef dslam_message_runtime_HEADER_HPP_
#define dslam_message_runtime_HEADER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Interfaces
*****************************************************************************/

struct PacketHeader {
  static const unsigned int size;
  PacketHeader(const unsigned int& id, const unsigned int& length);

  unsigned int const& signature() const { return header0; }
  unsigned int const& id()        const { return header1; }
  unsigned int const& reserved()  const { return header2; }
  unsigned int const& length()    const { return header3; }

  unsigned int header0, header1, header2, header3;
};

} // namespace dslam

#endif /* dslam_message_runtime_HEADER_HPP_ */
