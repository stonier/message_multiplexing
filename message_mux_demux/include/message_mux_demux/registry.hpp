/**
 * @file /message_mux_demux/include/message_mux_demux/registry.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef message_mux_demux_REGISTRY_HPP_
#define message_mux_demux_REGISTRY_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#ifndef NDEBUG
  #include <iostream>
#endif
#include <map>
#include <set>
#include <sstream>
#include <string>
#include "exceptions.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace message_mux_demux {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class MessageRegistry {
public:
  /**
   * @grief Register a packet id with this typed registry.
   * @return bool : false if a packet was already registered.
   */
  template<int ID, typename T>
  static bool add(const std::string &description="") {
    if (ids().insert( std::pair<int, std::string>(ID, description)).second) {
      add_by_type<T>(ID);
      #ifdef NDEBUG
        std::cout << "Registering message packet.............[" << ID << " : '" << description << "']" << std::endl;
      #endif
      return true;
    } else {
      #ifdef NDEBUG
        std::cout << "Registering message packet failed......[" << ID << " : '" << description << "']" << std::endl;
      #endif
      return false;
    }
  }
  static bool isRegistered(const int& id) {
    return (ids().find(id) != ids().end());
  }

  template <typename T>
  static bool isRegisteredWithType(const int& id) {
    return (ids_by_type<T>().find(id) != ids_by_type<T>().end());
  }

private:
  template <typename T>
  static void add_by_type(const int& id) {
    ids_by_type<T>().insert(id);
  }

  /****************************************
   ** Hidden Globals
   ****************************************/
  template <typename T>
  static std::set<int>& ids_by_type() {
    static std::set<int> ids;
    return ids;
  }
  static std::map<int, std::string>& ids() {
    static std::map<int, std::string> complete_id_set;
    return complete_id_set;
  }
};

/*****************************************************************************
 ** Registry Enablers
 *****************************************************************************/

/**
 * @brief Library instantiable variable for registration purposes.
 *
 * This is used in conjunction with the macro below to load packet
 * information into the registry. You do not need to use this variable
 * directly.
 */
template<int ID, typename T>
struct RegistryEntry {
  RegistryEntry(const std::string& description="") {
    bool result = MessageRegistry::add<ID, T>(description);
    if ( !result ) {
      std::stringstream ss;
      ss << "registry already holds id '" << ID << "' (our packet desc: '" << description << "')" << std::endl;
      throw MessageRegistrationFailed(ss.str());
    }
  }
};

} // message_mux_demux

/*****************************************************************************
 ** Registry Macros
 *****************************************************************************/

// Macro for loading id-type pairs into the registry. If multiple macros
// are called for the same id, with different types, you will get
// 1) a compile time error if they are in the same translation unit OR
// 2) a runtime error (exception) if they are in different, but linked translation units
#define MESSAGE_MULTIPLEXING_REGISTER_PACKET_INFO(id, T, Description) \
  namespace message_mux_demos { \
    const message_mux_demux::RegistryEntry<id, T> unused_packet_info_##id(Description); \
  }


#endif /* message_mux_demux_REGISTRY_HPP_ */
