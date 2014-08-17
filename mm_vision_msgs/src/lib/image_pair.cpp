/**
 * @file /mm_vision_msgs/src/lib/image_pair.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../../include/mm_vision_msgs/image_pair.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_vision_msgs {

ImagePair::ImagePair(const unsigned char* left_buffer,
                     const unsigned char *right_buffer,
                     const unsigned int& size)
: left(left_buffer, left_buffer + size),
  right(right_buffer, right_buffer + size),
  size(size)
{
  // this is very similar to the decoder
  if (size == 76800) { // QVGA
    width = 320;
    height = 240;
    colour = false;
  } else if (size == 153600) { // CQVGA
    width = 320;
    height = 240;
    colour = true;
  } else if (size == 307200) { // VGA
    width = 640;
    height = 480;
    colour = false;
  } else if (size == 614400) { // CVGA
    width = 640;
    height = 480;
    colour = true;
  }
}


} // namespace mm_vision_msgs

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
void Message<mm_vision_msgs::ImagePairPtr>::encode(const mm_vision_msgs::ImagePairPtr& pair, ByteArray& buffer) {
  // we might like to do something more tricky once we can use c++11 here (like std::move) to avoid
  // copying buffers.
  buffer = pair->left;
  buffer.insert(buffer.end(), pair->right.begin(), pair->right.end());
}

/**
 * This deserialises everything into image pair, filling in all fields except the semantic id (which would
 * be applied externally)
 * @param buffer : the buffer directly from socket land.
 * @param size : size of the allocated buffer
 * @return ImagePairPtr
 */
mm_vision_msgs::ImagePairPtr Message<mm_vision_msgs::ImagePairPtr>::decode(const unsigned char* buffer, const unsigned int& size) {
  mm_vision_msgs::ImagePairPtr pair = std::make_shared<mm_vision_msgs::ImagePair>();
  pair->left.assign(buffer, buffer + size/2); // c++0x buffer+size -> std::end(buffer) instead of size?
  pair->right.assign(buffer + size/2, buffer + size); // c++0x buffer+size -> std::end(buffer) instead of size?
  pair->size = size/2;  // single image size
  if (pair->size == 76800) { // QVGA
    pair->width = 320;
    pair->height = 240;
    pair->colour = false;
  } else if (pair->size == 153600) { // CQVGA
    pair->width = 320;
    pair->height = 240;
    pair->colour = true;
  } else if (pair->size == 307200) { // VGA
    pair->width = 640;
    pair->height = 480;
    pair->colour = false;
  } else if (pair->size == 614400) { // CVGA
    pair->width = 640;
    pair->height = 480;
    pair->colour = true;
  }
  return pair;
}


} // namespace mm_messages
