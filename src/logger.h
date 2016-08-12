
/**
 * @file logger.h
 * @brief Header for the Logger class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef LOGGER_H
#define LOGGER_H

// ==================
//  General Includes
// ==================
//
#include <fstream> // std::ofstream
#include <string> // std::string

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract Class for logging elements into a file.
 */
class Logger
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param filename File where output should be written.
   * @param overwrite True if existing file should be overwritten 
   *  (true by default).
   */
  Logger (const std::string& filename, bool overwrite = true)
    {
      // open file
      if (overwrite) { _output.open (filename.c_str()); }
      else { _output.open (filename.c_str(), std::ofstream::app); }
    }

 private:
  // Forbidden
  /** @brief Copy constructor. */
  Logger (const Logger& other_chemical_logger);
  /** @brief Assignment operator. */
  Logger& operator= (const Logger& other_chemical_logger);
 public:

  /**
   * @brief Destructor.
   */
  virtual ~Logger (void) { _output.close(); }

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Log elements.
   * @param simulation_time Current simulation time.
   */
  virtual void log (double simulation_time) = 0;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

protected:      
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief File where output is written. */
  std::ofstream _output;
};

// ======================
//  Inline declarations
// ======================
//

#endif // LOGGER_H
