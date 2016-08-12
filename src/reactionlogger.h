
/**
 * @file reactionlogger.h
 * @brief Header for the ReactionLogger class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef REACTION_LOGGER_H
#define REACTION_LOGGER_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <string> // std::string

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "logger.h"

/**
 * @brief Class that logs most frequent reactions into a file.
 */
class ReactionLogger : public Logger
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param filename File where output should be written.
   * @param reaction Reactionss to log.
   * @param overwrite True if existing file should be overwritten 
   *  (true by default).
   */
  ReactionLogger (const std::string& filename, 
		  const std::vector <const Reaction*>& reactions, 
		  bool overwrite = true);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  ReactionLogger (const ReactionLogger& other_reaction_logger);
  /** @brief Assignment operator. */
  ReactionLogger& operator= (const ReactionLogger& other_reaction_logger);
 public:

  // /* @brief Destructor. */
  // ~ReactionLogger (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from Logger
  /**
   * @brief Log most frequent reactions since last log.
   * @param simulation_time Current simulation time.
   */
  void log (double simulation_time);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Reactions to log. */
  std::vector <const Reaction*> _reactions;

  /** @brief Number of reactions to log. */
  int _log_number;
  
  /** @brief Values to log. */
  std::vector <long long int> _values;
  /** @brief Values as of last log. */
  std::vector <long long int> _previous;
  /** @brief Indices of reactions sorted according to frequency. */
  std::vector <int> _indices;
};

// ======================
//  Inline declarations
// ======================
//

#endif // REACTION_LOGGER_H
