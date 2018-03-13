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
 * @file loading.cpp
 * @brief Implementation of the Loading class.
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
#include "macros.h" // REQUIRE()
#include "loading.h"
#include "freechemical.h"
#include "boundchemical.h"
#include "boundunit.h"
#include "loadingtable.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Loading::Loading (BoundChemical& loader, const LoadingTable& table)
  : _loader (loader)
  , _table (table)
  , _template_filter (table)
  , _volume_constant (1)
{
  // ask loader to classify units according to templates read
  _loader.add_filter (_template_filter);

  _reactants.push_back (&loader);
  const std::set<FreeChemical*>& bases = table.chemicals_loaded();
  _reactants.insert (_reactants.end(), bases.begin(), bases.end());
  const std::set<BoundChemical*>& occupied = table.occupied_states();
  _products.insert (_products.end(), occupied.begin(), occupied.end());
}

DoubleStrandLoading::DoubleStrandLoading (BoundChemical& loader, 
					  const LoadingTable& table,
					  BoundChemical& stalled_form)
  : Loading (loader, table)
  , _stalled_form (stalled_form)
{}
 
// Forbidden
// Loading::Loading (Loading& other_loading);
// Loading& Loading::operator= (Loading& other_loading);

Loading::~Loading (void)
{
  _loader.remove_filter (_template_filter);
}

// ===========================
//  Public Methods - Commands
// ===========================
//


// ============================
//  Public Methods - Accessors
// ============================
//
bool Loading::is_reaction_possible (void) const
{
  return (_template_filter.loading_rate() > 0);
}

// =================
//  Private Methods
// =================
//
double Loading::compute_rate (void) const
{
  /**
   * Loading rate is generally defined by r = sum ( k_on_i x [A_i] x [B_i]),
   * where [A_i] is the concentration of Loader on template i and [B_i] the
   * concentration of chemical to load i. k_on_i may vary from a
   * template to another. All this information is stored within the
   * LoadingTable associated with the Loader. The concentration of units to
   * bind is easy to get, the loader can compute the rest of the formula for
   * us as it holds information about templates and base concentrations.
   */
  return _volume_constant * _template_filter.loading_rate();
}

void Loading::do_reaction (void)
{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_reaction_possible());
  load_chemical (random_unit());
  // for the moment nothing is done at the sequence level,
  // Release handles product creation.
}

void DoubleStrandLoading::do_reaction (void)
{
  // select unit weighted by the template read
  BoundUnit& unit = random_unit();
  /** @pre Bound unit must be bound to a double strand sequence. */
  REQUIRE (unit.location().appariated_strand() != 0);

  // bind to existing partial strand if necessary
  if (unit.strand() == BoundUnit::NO_STRAND)
    { 
      unit.set_strand (unit.location().appariated_strand()->partial_strand_id 
		       (unit.location().complementary (unit.reading_frame())));
    }
     
  if (unit.location().appariated_strand()->extend_strand 
      (unit.strand(), unit.location().complementary (unit.reading_frame())))
    { 
      load_chemical (unit); 
    }
  else // go to stalled form
    {
      _loader.remove (unit);
      _stalled_form.add (unit);
    }
}

void Loading::load_chemical (BoundUnit& unit)
{
  int template_index =  _table.template_index 
    (unit.location().sequence().substr 
     (unit.reading_frame(), _table.template_length()));
  _loader.remove (unit);
  _table.chemical_to_load (template_index).remove (1);
  _table.occupied_state (template_index).add (unit);
}

void Loading::print (std::ostream& output) const
{
  output << "Loading";
}
