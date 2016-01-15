

/**
 * @file dependencyexception.h
 * @brief Header for the DependencyException class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef DEPENDENCY_EXCEPTION_H
#define DEPENDENCY_EXCEPTION_H

// ==================
//  General Includes
// ==================
//
#include <exception>
#include <string>
#include <sstream>

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Exception class used to indicate dependency issues.
 *
 * DependencyException should be raised whenever a dependency (to another unit
 * or reaction) cannot be resolved, as the unit or reaction does not seem to
 * exist.
 */
class DependencyException : public std::exception
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param unknown_reference Name of the reference that could not be found.
   */
  DependencyException (const std::string& unknown_reference)
    : _reference (unknown_reference)
    { 
    }

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // DependencyException (const DependencyException& other_exception);
  // /* @brief Assignment operator. */
  // DependencyException& operator= (const DependencyException& other_exception);
  // EXCEPTION: Destructor is empty so I should define none of the 3 methods,
  //  but compiler forgets the `throw()` part of the definition...
  /**
   * @brief Destructor.
   */
  ~DependencyException (void) throw() {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to error message.
   * @return Error message indicating reference that could not be found.
   */
  virtual const char* what() const throw()
  {
    std::string msg = _reference + " (unknown reference)";
    return msg.c_str();
  }

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Reference that was not found. */
  std::string _reference;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//

#endif // DEPENDENCY_EXCEPTION_H
