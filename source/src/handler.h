// 
// Copyright 2017 INRA
// Authors: M. Dinh, S. Fischer
// Last modification: 2017-09-19
// 
// 
// Licensed under the GNU General Public License.
// You should have received a copy of the GNU General Public License
// along with BiPSim.  If not, see <http://www.gnu.org/licenses/>.
// 


/**
 * @file handler.h
 * @brief Header for the Handler class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef HANDLER_H
#define HANDLER_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <string> // std::string
#include <map> // std::map

// ==================
//  Project Includes
// ==================
//
#include "simulatorexception.h"
#include "forwarddeclarations.h"
#include "macros.h"

/**
 * @brief Generic class for storing elements.
 * @tparam T Type of objects to store (must derive from SimulatorInput).
 * @details Handler stores elements created on the heap with their
 * user defined name (optional). They are automatically destroyed
 * when the handler is destroyed. Elements can be accessed by name
 * or with an integer identifier that is automatically assigned at
 * storage.
 */
template <class T>
class Handler
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  Handler (void) {}

 private:
  // Forbidden
  /** @brief Copy constructor. */
  Handler (const Handler& other_handler);
  /** @brief Assignment operator. */
  Handler& operator= (const Handler& other_handler);
 public:

  /** @brief Destructor. */
  ~Handler (void)
    {
      for (typename std::vector <T*>::iterator ref_it = _references.begin();
	   ref_it != _references.end(); ++ref_it)
	{
	  delete *ref_it;
	}
    }

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Store pointer to element and (optionnaly) its name.
   * @param element Pointer to store.
   * @param name Name of element. Must be unique if specified. Empty by default.
   * @return True if element was successfully stored by handler.
   */
  bool store (SimulatorInput* element, const std::string& name = "")
  {
    if (_name_to_id.find (name) != _name_to_id.end())
      {
	std::string msg = "identifier " + name + " already used";
	throw ParserException(msg);
      }
    
    // see if this type can be handled
    T* T_element = dynamic_cast <T*> (element);
    if (T_element == 0) return false;

    _references.push_back (T_element);
    if (name != "") { _name_to_id [name] = _references.size()-1; }
    return true;
  }


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Find pointer to element from name.
   * @return Pointer to element corresponding to name or 0 if name not found.
   * @param name Name given to element at storage.
   */
  T* find (const std::string& name) const
  {
    std::map <std::string,int>::const_iterator id_it = _name_to_id.find (name);
    if (id_it != _name_to_id.end())
      { return _references [id_it->second]; }
    else
      { return 0; } 
  }

  /**
   * @brief Find identifier associated with name.
   * @return Integer identifier or Handler::NOT_FOUND if name not found.
   * @param name Name given to element at storage.
   */
  int find_id (const std::string& name) const
  {
    std::map <std::string,int>::const_iterator id_it = _name_to_id.find (name);
    if (id_it != _name_to_id.end())
      { return id_it->second; }
    else
      { return NOT_FOUND; } 
  }

  /**
   * @brief Return reference to element from identifier.
   * @return Reference to element corresponding to id.
   * @param id Identifier attributed to element at storage.
   */
  T& reference (int id) const
    {
      /** @pre id must exist. */
      REQUIRE ((id >= 0) && (id < _references.size())); 
      return _references [id];
    }

  /**
   * @brief Return name associated with identifier.
   * @return Name corresponding to id. Empty string if id is unknown.
   * @param id Identifier attributed to element at storage.
   */
  std::string name (int id) const
    {
      std::string result = "";
      
      // Look for element_id in the map
      bool found = false;
      typename std::vector <T*>::const_iterator it = _name_to_id.begin ();
      while ((result == "") &&  (it != _name_to_id.end()))
	{
	  if ( it->second == id ) { result = it->first; }
	  ++it;
	}
      
      return result;
    }

  /**
   * @brief Accessor to existing identifiers.
   * @return Identifiers that have been created so far.
   */
  std::list<int> existing_identifiers (void) const
    {
      std::list <int> ids (_references.size());
      int i = 0;
      for (std::list <int>::iterator id_it = ids.begin();
	   id_it != ids.end(); ++id_it, ++i)
	{ *id_it = i; }
      return ids;
    }

  /**
   * @brief Accessor to all references.
   * @return Vector containing all stored references.
   */
  const std::vector<T*>& references (void) const
    {
      return _references;
    }

  // ==================
  //  Public Constants
  // ==================
  //
  /** @brief Constant returned if an identifier was not found. */
  static const int NOT_FOUND = -1;

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief The map that associates names with integer identifiers. */
  std::map <std::string, int> _name_to_id;

  /** @brief Vector of pointers. */
  std::vector <T*> _references;  

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//

#endif // HANDLER_H
