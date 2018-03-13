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
 * @file simulatorexception.h
 * @brief Header for the DependencyException and FormatException classes.
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
 * @details DependencyException should be raised whenever a dependency 
 * (to another unit
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
    : _msg (unknown_reference + " (unknown reference)")
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
  virtual const char* what() const throw() { return _msg.c_str(); }

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Reference that was not found. */
  std::string _msg;

  // =================
  //  Private Methods
  // =================
  //
};


/**
 * @brief Exception class for parsing related errors.
 * @details ParserException defines generic exceptions for parsing issues.
 * User can specify any message he wants as the message error.
 */
class ParserException : public std::exception
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param error_message Message to display as error message.
   */
  ParserException (const std::string& error_message)
    : _message (error_message)
    { 
    }

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // ParserException (const ParserException& other_exception);
  // /* @brief Assignment operator. */
  // ParserException& operator= (const ParserException& other_exception);
  // EXCEPTION: Destructor is empty so I should define none of the 3 methods,
  //  but compiler forgets the `throw()` part of the definition...
  /**
   * @brief Destructor.
   */
  virtual ~ParserException (void) throw() {}

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
   * @return Exceptions's error message.
   */
  virtual const char* what() const throw()
  {
    return _message.c_str();
  }

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Error message to display when what() is called. */
  std::string _message;

  // =================
  //  Private Methods
  // =================
  //
};


/**
 * @brief Exception class for incorrectly formatted input lines.
 *
 * FormatException inherits ParserException and sets "invalid format" as the
 * standard error message to display.
 */
class FormatException : public ParserException
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  FormatException (void)
    : ParserException ("Invalid format")
    { 
    }

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // FormatException (const FormatException& other_exception);
  // /* @brief Assignment operator. */
  // FormatException& operator= (const FormatException& other_exception);
  // EXCEPTION: Destructor is empty so I should define none of the 3 methods,
  //  but compiler forgets the `throw()` part of the definition...
  /**
   * @brief Destructor.
   */
  ~FormatException (void) throw() {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //


  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // ============
  //  Attributes
  // ============
  //

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
