/**
 * @file /mm_vision_msgs/include/mm_vision_msgs/image.hpp
 * 
 * @brief Convert to our simple image format.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_vision_msgs_IMAGE_PAIR_HPP_
#define mm_vision_msgs_IMAGE_PAIR_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/time/timestamp.hpp>
#include <mm_messages/message.hpp>
#include <memory>
#include <vector>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_vision_msgs {

/*****************************************************************************
** Image Definitions
*****************************************************************************/
/**
 * @brief An image structure that makes sense for non-opencv platforms.
 *
 * On our embedded boards we usually dont run with opencv, but we need a
 * structure which we can fling around. This is it.
 */
struct ImagePair {
  ImagePair() {};
  /**
   * @brief Create a pair from two incoming character arrays (usually from the grabber).
   * @param left
   * @param right
   * @param size : size of a single one of the image buffers.
   */
  ImagePair(const unsigned char* left_buffer,
            const unsigned char *right_buffer,
            const unsigned int& size);
  std::vector<unsigned char> left, right;
  bool colour;
  unsigned int width, height, size; // calculate from incoming buffer sizes
  ecl::TimeStamp timestamp;
};

typedef std::shared_ptr<ImagePair> ImagePairPtr;

} // namespace mm_vision_msgs

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** Serialisation
*****************************************************************************/

template<>
struct Message<mm_vision_msgs::ImagePairPtr> {
  static void encode(const mm_vision_msgs::ImagePairPtr& pair, ByteArray& buffer);
  static mm_vision_msgs::ImagePairPtr decode(const unsigned char* buffer, const unsigned int& size);
};

} // namespace mm_messages

#endif /* mm_vision_msgs_IMAGE_PAIR_HPP_ */
