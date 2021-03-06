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
 * @file chemicallogger.h
 * @brief Header for the ChemicalLogger class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef CHEMICAL_LOGGER_H
#define CHEMICAL_LOGGER_H

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
 * @brief Class that logs chemical concentrations into a file.
 * @details ChemicalLogger opens an output file and 
 * writes all concentration levels
 * to that file when asked to. Names and chemicals to write are read from 
 * a chemical handler at construction, the vector does not change afterwards.
 * User can choose whether an existing file should be overwritten or not.
 */
class ChemicalLogger : public Logger
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param filename File where output should be written.
   * @param chemicals Chemicals whose concentrations must be logged.
   * @param names Name of the chemicals (used for writing header).
   * @param overwrite True if existing file should be overwritten 
   *  (true by default).
   */
  ChemicalLogger (const std::string& filename, 
		  const std::vector <const Chemical*>& chemicals, 
		  const std::vector <std::string>& names, 
		  bool overwrite = true);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  ChemicalLogger (const ChemicalLogger& other_chemical_logger);
  /** @brief Assignment operator. */
  ChemicalLogger& operator= (const ChemicalLogger& other_chemical_logger);
 public:

  // /* @brief Destructor. */
  // ~ChemicalLogger (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from Logger
  /**
   * @brief Log current concentration levels.
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
  /**
   * @brief Write a header to outupt file.
   * @param names Names to write in the header.
   *
   * Header starts with simulation time then vectors all chemicals in
   * tab separated fields.
   */
  void write_header (const std::vector <std::string>& names);

  // ============
  //  Attributes
  // ============
  //
  /** @brief Chemicals to log. */
  std::vector <const Chemical*> _chemicals;
};

// ======================
//  Inline declarations
// ======================
//

#endif // CHEMICAL_LOGGER_H
