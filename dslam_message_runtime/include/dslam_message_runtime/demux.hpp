/**
 * @file /dslam_message_runtime/include/dslam_message_runtime/demux.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef dslam_message_runtime_DEMUX_HPP_
#define dslam_message_runtime_DEMUX_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/utilities/function_objects.hpp>
#include <ecl/threads/mutex.hpp>
#include <map>
#include <string>
#include <thread>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {
namespace impl {

class MessageDemux {
public:
  MessageDemux(const std::string& name, const std::string& url);
  MessageDemux(const MessageDemux& other);
  ~MessageDemux();
  void spin();

  template<typename C>
  void registerSubscriber(
      const unsigned int& id,
      void (C::*processPacket)(const unsigned char*, const unsigned int&),
      C &s) {
    SubscriberMapResultPair ret;
    mutex.lock();
    BufferCallbackFunction function = new ecl::PartiallyBoundBinaryMemberFunction<C,const unsigned char*,const unsigned int&, void>(processPacket,s);
    ret = subscribers.insert(SubscriberMapPair(id, function));
    mutex.unlock();
    if ( !ret.second )  {
      // some error handling
    }
  }

  void unregisterSubscriber(const unsigned int& id);

private:
//  typedef ecl::UnaryFunction<const unsigned char*,void>* BufferCallbackFunction;
  typedef ecl::BinaryFunction<const unsigned char*,const unsigned int&,void>* BufferCallbackFunction;
  typedef std::map<unsigned int, BufferCallbackFunction> SubscriberMap;
  typedef std::map<unsigned int, BufferCallbackFunction>::iterator SubscriberMapIterator;
  typedef std::map<unsigned int, BufferCallbackFunction>::const_iterator SubscriberMapConstIterator;
  typedef std::pair<unsigned int, BufferCallbackFunction> SubscriberMapPair;
  typedef std::pair<std::map<unsigned int, BufferCallbackFunction>::iterator,bool> SubscriberMapResultPair;

  std::string name;
  int socket;
  std::thread thread;
  SubscriberMap subscribers;
  ecl::Mutex mutex;
};

} // namespace impl
} // namespace dslam

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class MessageDemux {
public:
  typedef std::map<std::string, std::shared_ptr<impl::MessageDemux>> DemuxMap;
  typedef std::pair<std::string, std::shared_ptr<impl::MessageDemux>> DemuxMapPair;
  typedef std::map<std::string, std::shared_ptr<impl::MessageDemux>>::iterator DemuxMapIterator;
  typedef std::map<std::string, std::shared_ptr<impl::MessageDemux>>::const_iterator DemuxMapConstIterator;

  static void registerDemux(const std::string& name, const std::string& url);
  static void unregisterDemux(const std::string& name);
  /**
   * @brief Store demux's in an invisible storage location.
   *
   * This makes storage transparent for the user. Note that it will only allocate
   * when first called.
   * @return
   */
  static DemuxMap& demultiplexers();

  /**
   * @brief Register a callback with the specified demux.
   *
   * Basically we just need a function that can handle const unsigned char* buffers.
   *
   * Could make this more specific and specify 'template<typename DataType>' with
   * 'Subscriber<DataType>' instead of 'C', but this keeps it open for creating new
   * subscriber types.
   *
   * @param name
   * @param id
   * @param processPacket
   * @param s
   */
  template<typename C>
  static void registerSubscriber(
      const std::string& name,
      const unsigned int& id,
      void (C::*processPacket)(const unsigned char*, const unsigned int&),
      C &s)
  {
    DemuxMapIterator iter = demultiplexers().find(name);
    if ( iter != demultiplexers().end() ) {
      (iter->second)->registerSubscriber(id, &C::processPacket, s);
    } else {
      std::cout << "Demux : no demux by that name found (while registering subscriber)"<< std::endl;
    }
  }

  static void unregisterSubscriber(const std::string& name, const unsigned int& id);
};

typedef MessageDemux MessageClient;

} // dslam

#endif /* dslam_message_runtime_DEMUX_HPP_ */
