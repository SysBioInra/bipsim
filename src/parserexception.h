

/**
 * @file parserexception.h
 * @brief Header for the ParserException class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef PARSER_EXCEPTION_H
#define PARSER_EXCEPTION_H

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
 * @brief Exception class for parsing related errors.
 *
 * ParserException defines generic exceptions for parsing issues. User can
 * specify any message he wants as the message error.
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

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // ParserException (const ParserException& other_parser_exception);

  /**
   * @brief Destructor.
   */
  ~ParserException (void) throw() {}

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
  // ParserException& operator= (const ParserException& other_parser_exception);

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
  /** @brief Error message to display when what() is called. */
  std::string _message;

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

#endif // PARSER_EXCEPTION_H
