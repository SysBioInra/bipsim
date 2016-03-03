

/**
 * @file doublestrandlogger.h
 * @brief Header for the DoubleStrandLogger class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef DOUBLE_STRAND_LOGGER_H
#define DOUBLE_STRAND_LOGGER_H

// ==================
//  General Includes
// ==================
//
#include <fstream> // std::ofstream
#include <list> // std::list
#include <string> // std::string

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Class that logs double strand extensions into file.
 *
 * DoubleStrandLogger opens an output file and writes the state of a
 * double strand replication into a file.
 */
class DoubleStrandLogger
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param filename File where output should be written.
   * @param double_strand DoubleStrand whose state must be logged.
   * @param overwrite True if existing file should be overwritten 
   *  (true by default).
   */
  DoubleStrandLogger (const std::string& filename, 
		      const DoubleStrand& double_strand, 
		      bool overwrite = true);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  DoubleStrandLogger (const DoubleStrandLogger& other);
  /** @brief Assignment operator. */
  DoubleStrandLogger& operator= (const DoubleStrandLogger& other);
 public:

  /**
   * @brief Destructor.
   */
  ~DoubleStrandLogger (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Log current state.
   * @param simulation_time Current simulation time.
   */
  void log (double simulation_time);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief File where output is written. */
  std::ofstream _output;

  /** @brief Double strand whose state should be written. */
  const DoubleStrand& _double_strand;
      
  // =================
  //  Private Methods
  // =================
  //
  void write_sites (const ChemicalSequence& sequence);
};

// ======================
//  Inline declarations
// ======================
//

#endif // DOUBLE_STRAND_LOGGER_H
