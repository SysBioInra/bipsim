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
 * @file doublestrandlogger.cpp
 * @brief Implementation of the DoubleStrandLogger class.
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
#include "doublestrandlogger.h"
#include "doublestrand.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
DoubleStrandLogger::DoubleStrandLogger (const std::string& filename,
					const DoubleStrand& double_strand, 
					bool overwrite /*= true*/)
  : Logger (filename, overwrite)
  , _double_strand (double_strand)
{}

// Forbidden
// DoubleStrandLogger::DoubleStrandLogger (const DoubleStrandLogger& other);
// DoubleStrandLogger& DoubleStrandLogger::operator= (const DoubleStrandLogger& other);
// DoubleStrandLogger::~DoubleStrandLogger (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void DoubleStrandLogger::log (double simulation_time)
{
  _output << simulation_time;
  _output << " Sense\n";
  write_sites (_double_strand.sense());
  _output << " Antisense\n";
  write_sites (_double_strand.antisense());
  _output << "\n";
}

// =================
//  Private Methods
// =================
//
void DoubleStrandLogger::write_sites (const ChemicalSequence& sequence)
{
  std::list <std::vector <int> > strands = sequence.partial_strands();
  int id = 1;
  for (std::list <std::vector <int> >::iterator it = strands.begin();
       it != strands.end(); ++it, ++id)
    {
      _output << "Strand " << id << ":";
      for (std::size_t i = 0; i < it->size(); i+=2)
	{ _output << "[" << (*it)[i] << " " << (*it)[i+1] << "]"; }
      _output << "\n";
    }
}
