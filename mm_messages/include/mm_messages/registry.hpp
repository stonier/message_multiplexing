/**
 * @file /mm_messages/include/mm_messages/registry.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_messages_REGISTRY_HPP_
#define mm_messages_REGISTRY_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include "exceptions.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

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
      std::cout << "Registering message packet.............[" << ID << " : '" << description << "']" << std::endl;
      return true;
    } else {
      std::cout << "Registering message packet failed......[" << ID << " : '" << description << "']" << std::endl;
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

} // mm_messages

/*****************************************************************************
 ** Registry Macros
 *****************************************************************************/

// Macro for loading id-type pairs into the registry. If multiple macros
// are called for the same id, with different types, you will get
// 1) a compile time error if they are in the same translation unit OR
// 2) a runtime error (exception) if they are in different, but linked translation units

// This requires some heavy macro expansions to work with __COUNTER__ so that
// __COUNTER__ gets expanded *before* concatenation happens (this is what goes
// wrong in a naive one line implementation). See this url for details:
//
//    http://blog.aaronballman.com/2011/12/stupid-compiler-tricks/
//
#define MM_REGISTER_IMP_2(uuid) unused_packet_info_##uuid
#define MM_REGISTER_IMP_1(id, T, Description, uuid) \
    namespace mm_messages_registry_entries { \
      const mm_messages::RegistryEntry<id, T> MM_REGISTER_IMP_2(uuid)(Description); \
    }
#define MM_REGISTER_PACKET_INFO(id, T, Description) MM_REGISTER_IMP_1(id, T, Description, __COUNTER__)

#endif /* mm_messages_REGISTRY_HPP_ */
