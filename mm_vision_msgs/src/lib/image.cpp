/**
 * @file /mm_vision_msgs/src/lib/image.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../../include/mm_vision_msgs/image.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** Implementation
*****************************************************************************/
/**
 * Serialise it onto a buffer.
 *
 * We're doing more copies than we need to here. Be good to be able to pass
 * the image's buffer directly to the underlying send command. However that
 * probably isn't possible as it requires a prefixed header and contiguous
 * memory across header and data.
 *
 * @param image
 * @param buffer
 */
void Message<mm_vision_msgs::ImagePtr>::encode(const mm_vision_msgs::ImagePtr& image, ByteArray& buffer) {
  // we might like to do something more tricky once we can use c++11 here (like std::move) to avoid
  // copying buffers.
  buffer = image->data;
}

/**
 * This deserialises everything into image, filling in all fields except the semantic id (which would
 * be applied externally)
 * @param buffer : the buffer directly from socket land.
 * @param size : size of the allocated buffer
 * @return ImagePtr
 */
mm_vision_msgs::ImagePtr Message<mm_vision_msgs::ImagePtr>::decode(const unsigned char* buffer, const unsigned int& size) {
  mm_vision_msgs::ImagePtr image = std::make_shared<mm_vision_msgs::Image>();
  image->data.assign(buffer, buffer + size); // c++0x buffer+size -> std::end(buffer) instead of size?
  if (size == 76800) { // QVGA
    image->width = 320;
    image->height = 240;
    image->colour = false;
  } else if (size == 153600) { // CQVGA
    image->width = 320;
    image->height = 240;
    image->colour = true;
  } else if (size == 307200) { // VGA
    image->width = 640;
    image->height = 480;
    image->colour = false;
  } else if (size == 614400) { // CVGA
    image->width = 640;
    image->height = 480;
    image->colour = true;
  }
  image->size = size;
  return image;
}


} // namespace mm_messages
