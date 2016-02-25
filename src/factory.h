

/**
 * @file factory.h
 * @brief Header for the Factory class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef FACTORY_H
#define FACTORY_H

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <sstream> // std::istringstream

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class used to create entities used by the simulator.
 *
 * Factory automatically recognizes which entity to create and stores it
 * in the appropriate object.
 */
class Factory
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor. 
   * @param cell_state CellState object used to store new objects.
   */
  Factory (CellState& cell_state) : _cell_state (cell_state) {}
  
  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Factory (const Factory& other_factory);
  // /* @brief Assignment operator. */
  // Factory& operator= (const Factory& other_factory);
  /** @brief Destructor. */
  virtual ~Factory (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create entity from line.
   * @param line Text line to create entity from.
   * @return True if an entity was successfully created.
   */
  virtual bool handle (const std::string& line) = 0;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Store element in cell state.
   * @tparam Type of element to store.
   * @param element Heap element to store.
   * @param name Name of element (optional).
   */
  template <typename T>
    void store (T* element, const std::string& name = "");
  
  /**
   * @brief Read next input in stream as being of base type T.
   * @tparam Base type of next element in stream.
   * @param line_stream Stream to read from.
   * @return Element read. Throws a FormatException if next element is not
   *  of required type.
   */
  template <typename T>
    T read (std::istringstream& line_stream) const;

  /**
   * @brief Read next input in stream as a positive integer.
   * @param line_stream Stream to read from.
   * @return Quantity read. Throws a FormatException if next element is not
   *  an integer. Throws a ParserException if quantity is not positive.
   */
  int read_initial_quantity (std::istringstream& line_stream) const;
  
  /**
   * @brief Fetch element from cell state.
   * @tparam Type of element to fetch.
   * @param name Name of element.
   * @return Pointer to element corresponding to name. Throws a 
   *  DependencyException if required element is not found
   */
  template <typename T>
    T* fetch (const std::string& name) const;

  /**
   * @brief Fetch element from cell state corresponding to next input in stream.
   * @tparam Type of element to fetch.
   * @param line_stream Stream to read the name from.
   * @return Pointer to element corresponding to name in stream. Throws a 
   *  DependencyException if required element is not found. Throws a 
   *  FormatExcption if no name can be read in stream.
   */
  template <typename T>
    T* fetch (std::istringstream& line_stream) const;

  /**
   * @brief Fetch or create element from cell_state.
   * @tparam Type of element to fetch or create.
   * @param name Name of element.
   * @return Pointer to element corresponding to name in stream. If element is
   *  not found in current cell state, it is created.
   */
  template <typename T>
    T* fetch_or_create (const std::string& name);

  /**
   * @brief Fetch or create element correspondind to next input in stream.
   * @tparam Type of element to fetch or create.
   * @param line_stream Stream to read the name from.
   * @return Pointer to element corresponding to name in stream. If element is
   *  not found in current cell state, it is created. Throws a 
   *  FormatExcption if no name can be read in stream.
   */
  template <typename T>
    T* fetch_or_create (std::istringstream& line_stream);

  /**
   * @brief Check if next word in stream corresponds to some defined tag.
   * @param line_stream Stream to read next word from.
   * @param tag Word to check against.
   * @return True if the next word corresponds to tag.
   */
  bool check_tag (std::istringstream& line_stream, 
		  const std::string& tag) const;

  /**
   * @brief Accessor to cell state.
   * @return Reference to cell state.
   */
  const CellState& cell_state (void) const;


private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief CellState object used to store new units. */
  CellState& _cell_state;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
#include "cellstate.h"
#include "simulatorexception.h"

template <typename T>
inline void Factory::store (T* element, const std::string& name)
{
  _cell_state.store (element, name);
}

template <typename T>
inline T Factory::read (std::istringstream& line_stream) const
{
  T result;
  if (not (line_stream >> result)) { throw FormatException(); }
  return result;
}

inline 
int Factory::read_initial_quantity (std::istringstream& line_stream) const
{
  int result; 
  if (! (line_stream >> result)) { return 0; }
  if (result < 0)
    { throw ParserException ("Initial quantity must be positive"); }
  return result;
}

template <typename T>
inline T* Factory::fetch (const std::string& name) const
{
  T* result = _cell_state.find <T> (name);
  if (result == 0) { throw DependencyException (name); }
  return result;
}

template <typename T>
inline T* Factory::fetch (std::istringstream& line_stream) const
{
  return fetch <T> (read <std::string> (line_stream));
}

template <typename T>
inline T* Factory::fetch_or_create (const std::string& name)
{
  T* result = _cell_state.find <T> (name);
  if (result == 0) { result = new T; store (result, name); }
  return result;
}

template <typename T>
inline T* Factory::fetch_or_create (std::istringstream& line_stream)
{
  return fetch_or_create <T> (read <std::string> (line_stream));
}

inline bool Factory::check_tag (std::istringstream& line_stream, 
				 const std::string& tag) const
{
  std::string tag_read;
  return ((line_stream >> tag_read) && (tag_read == tag));
}

inline const CellState& Factory::cell_state (void) const
{ 
  return _cell_state; 
}

#endif // FACTORY_H
