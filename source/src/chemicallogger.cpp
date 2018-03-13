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
 * @file chemicallogger.cpp
 * @brief Implementation of the ChemicalLogger class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "chemicallogger.h"
#include "chemical.h"
#include "macros.h" // REQUIRE

// ==========================
//  Constructors/Destructors
// ==========================
//
ChemicalLogger::ChemicalLogger (const std::string& filename,
				const std::vector <const Chemical*>& chemicals,
				const std::vector <std::string>& names, 
				bool overwrite /*= true*/)
  : Logger (filename, overwrite) 
  , _chemicals (chemicals)
{
  /** @pre Vector of names must be as long as vector of references. */
  REQUIRE (chemicals.size() == names.size());
  
  // write header (if necessary)
  if (overwrite) { write_header (names); }
}

// Forbidden
// ChemicalLogger::ChemicalLogger (const ChemicalLogger& logger);
// ChemicalLogger& ChemicalLogger::operator= (const ChemicalLogger& other_logger);



// ===========================
//  Public Methods - Commands
// ===========================
//
void ChemicalLogger::log (double simulation_time)
{
  _output << simulation_time;
  for (std::vector <const Chemical*>::iterator chemical_it = _chemicals.begin();
       chemical_it != _chemicals.end(); ++chemical_it)
    {
      _output << "\t" << (*chemical_it)->number();
    }
  _output << "\n";
}


// =================
//  Private Methods
// =================
//
void ChemicalLogger::write_header (const std::vector <std::string>& names)
{
  _output << "time";
  for (std::vector <std::string>::const_iterator name_it = names.begin();
       name_it != names.end(); ++name_it)
    {
      _output << "\t" << *name_it;
    }
  _output << "\n";
}
