/**
 * @file /mm_mux_demux/include/mm_radio/radio.hpp
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_radio_RADIO_HPP_
#define mm_radio_RADIO_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/utilities/function_objects.hpp>
#include <ecl/threads/mutex.hpp>
#include <ecl/threads/thread.hpp>
#include <map>
#include <memory>
#include <mm_messages.hpp>
#include <string>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_radio {
namespace impl {

/*****************************************************************************
 ** Interfaces
 *****************************************************************************/

class RadioMuxDemux {
public:
  RadioMuxDemux(const std::string& name,
             const std::string& url,
             const mm_messages::Verbosity::Level& verbosity=mm_messages::Verbosity::QUIET);
  RadioMuxDemux(const RadioMuxDemux& other);
  ~RadioMuxDemux();

  /********************
   ** Start/Stop
   ********************/
  void spin();
  void shutdown();

  /********************
   ** Transmission
   ********************/
  int send(const unsigned int& id, const mm_messages::ByteArray& msg_buffer);

  /********************
   ** Subscribers
   ********************/
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
  typedef ecl::BinaryFunction<const unsigned char*,const unsigned int&,void>* BufferCallbackFunction;
  typedef std::map<unsigned int, BufferCallbackFunction> SubscriberMap;
  typedef std::map<unsigned int, BufferCallbackFunction>::iterator SubscriberMapIterator;
  typedef std::map<unsigned int, BufferCallbackFunction>::const_iterator SubscriberMapConstIterator;
  typedef std::pair<unsigned int, BufferCallbackFunction> SubscriberMapPair;
  typedef std::pair<std::map<unsigned int, BufferCallbackFunction>::iterator,bool> SubscriberMapResultPair;

  std::string name, url;
  int socket;
  mm_messages::Verbosity::Level verbosity;
  bool shutdown_requested;
  ecl::Thread thread;
  SubscriberMap subscribers;
  ecl::Mutex mutex;
};

} // namespace impl
} // mm_radio

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_radio {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class RadioMuxDemux {
public:
  typedef std::map<std::string, std::shared_ptr<impl::RadioMuxDemux>> RadioMap;
  typedef std::pair<std::string, std::shared_ptr<impl::RadioMuxDemux>> RadioMapPair;
  typedef std::map<std::string, std::shared_ptr<impl::RadioMuxDemux>>::iterator RadioMapIterator;
  typedef std::map<std::string, std::shared_ptr<impl::RadioMuxDemux>>::const_iterator RadioMapConstIterator;

  enum Errors {
    NotAvailable = -1   // when you try to connect to <name>, but <name> has not yet been started
  };

  /********************
   ** Registration
   ********************/
  static void start(const std::string& name,
                    const std::string& url,
                    const mm_messages::Verbosity::Level& verbosity=mm_messages::Verbosity::QUIET);
  static RadioMap& radios();

  /********************
   ** Shutdown
   ********************/
  static void shutdown();  // shutdown all radios
  static void shutdown(const std::string& name);  // shutdown only this radio

  /********************
   ** Transmissions
   ********************/
  static int send(const std::string& name, const unsigned int& id, const mm_messages::ByteArray& msg_buffer);

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
   RadioMapIterator iter = RadioMuxDemux::radios().find(name);
   if ( iter != RadioMuxDemux::radios().end() ) {
     (iter->second)->registerSubscriber(id, &C::processPacket, s);
   } else {
     std::cout << "Radio : no radio by that name found (while registering subscriber)"<< std::endl;
   }
 }

 static void unregisterSubscriber(const std::string& name, const unsigned int& id);
};

} // namespace mm_radio

#endif /* mm_radio_RADIO_HPP_ */
