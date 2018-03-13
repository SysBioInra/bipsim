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
 * @file boundunit.cpp
 * @brief Implementation of the BoundUnit class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout

// ==================
//  Project Includes
// ==================
//
#include "boundunit.h"
#include "bindingsite.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BoundUnit::BoundUnit (const BindingSite& site)
  : _initial_reading_frame (site.reading_frame())
  , _location (&site.location())
  , _binding_site (&site)
  , _first (site.first())
  , _last (site.last())
  , _reading_frame (site.reading_frame())
  , _strand (NO_STRAND)
{
}

BoundUnit::BoundUnit (ChemicalSequence& location, int first, 
		      int last, int reading_frame)
  : _initial_reading_frame (reading_frame)
  , _location (&location)
  , _binding_site (0)
  , _first (first)
  , _last (last)
  , _reading_frame (reading_frame)
  , _strand (NO_STRAND)
{
}

// BoundUnit::BoundUnit (const BoundUnit& other);
// BoundUnit& BoundUnit::operator= (const BoundUnit& other);
// BoundUnit::~BoundUnit (void)

// ===========================
//  Public Methods - Commands
// ===========================
//
void BoundUnit::rebind (const BindingSite& site)
{
  _first = site.first();
  _last = site.last();
  _reading_frame = site.reading_frame();
  _initial_reading_frame = site.reading_frame();
  _location = &site.location();
  _binding_site = &site;
  _strand = NO_STRAND;
}

void BoundUnit::rebind (ChemicalSequence& location, int first, 
			int last, int reading_frame)
{
  _first = first;
  _last = last;
  _reading_frame = reading_frame;
  _initial_reading_frame = reading_frame;
  _location = &location;
  _binding_site = 0;
  _strand = NO_STRAND;
}


// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
