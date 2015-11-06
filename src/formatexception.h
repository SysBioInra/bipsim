

/**
 * @file formatexception.h
 * @brief Header for the FormatException class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef FORMAT_EXCEPTION_H
#define FORMAT_EXCEPTION_H

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
#include "parserexception.h"

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

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // FormatException (const FormatException& other_format_exception);

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

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator.
  //  */
  // FormatException& operator= (const FormatException& other_format_exception);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
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

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // FORMAT_EXCEPTION_H
