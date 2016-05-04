
/**
 * @file builder.h
 * @brief Header for the Builder classes.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef BUILDER_H
#define BUILDER_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <list> // std::list

// ==================
//  Project Includes
// ==================
//
#include "interpreter.h"

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class for builders creating entities from text input.
 */
class Builder
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  Builder (CellState& cell_state);
  /** @brief Default destructor. */
  virtual ~Builder (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Match current input with format used by builder and create entity.
   * @param text_input Current InputLine read.
   * @return True if input matched builder format, false otherwise.
   */
  virtual bool match (InputLine& text_input) = 0;
  
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
   * @brief Fetch element from cell state.
   * @tparam Type of element to fetch.
   * @param name Name of element.
   * @return Reference to element corresponding to name. Throws a 
   *  DependencyException if required element is not found
   */
  template <typename T>
    T& fetch (const std::string& name) const;

  /**
   * @brief Find element in cell state.
   * @tparam Type of element to find.
   * @param name Name of element.
   * @return Pointer to element corresponding to name. 0 if not found.
   */
  template <typename T>
    T* find (const std::string& name) const;

  /**
   * @brief Fetch or create element from cell_state.
   * @tparam Type of element to fetch or create.
   * @param name Name of element.
   * @return Reference to element corresponding to name. If element is
   *  not found in current cell state, it is created.
   */
  template <typename T>
    T& fetch_or_create (const std::string& name);

  /**
   * @brief Convert next token in line to positive integer.
   * @param text_input InputLine being read.
   * @throw ParserException Integer read is not positive.
   */
  int read_initial_quantity (InputLine& text_input) const;

 private:
  // =================
  //  Private Methods
  // =================
  //
  /** @brief CellState used to fetch and store simulator entities. */
  CellState& _cell_state;

  // ============
  //  Attributes
  // ============
  //  
};

// ======================
//  Inline declarations
// ======================
//
#include "cellstate.h"
#include "simulatorexception.h"
#include "interpreter.h"

inline Builder::Builder (CellState& cell_state)
  : _cell_state (cell_state)
{
}

template <typename T>
inline void Builder::store (T* element, const std::string& name)
{
  _cell_state.store (element, name);
}

template <typename T>
inline T* Builder::find (const std::string& name) const
{
  return _cell_state.find <T> (name);
}

template <typename T>
inline T& Builder::fetch (const std::string& name) const
{
  return _cell_state.fetch <T> (name);
}

template <typename T>
inline T& Builder::fetch_or_create (const std::string& name)
{
  T* result = _cell_state.find <T> (name);
  if (result == 0) { result = new T; store (result, name); }
  return *result;
}

inline 
int Builder::read_initial_quantity (InputLine& text_input) const
{
  int result = 0;
  IntToken init_format (result);
  if (init_format.match (text_input))
    {
      if (result < 0)
	{ throw ParserException ("Initial quantity must be positive"); }
    }
  return result;
}

#endif // BUILDER_H
