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
 * @file sequenceoccupation.cpp
 * @brief Implementation of the SequenceOccupation class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <algorithm> // std::find

// ==================
//  Project Includes
// ==================
//
#include "sequenceoccupation.h"
#include "bindingsite.h"
#include "sitegroup.h"
#include "partialstrand.h"
#include "macros.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
SequenceOccupation::SequenceOccupation (int length)
  : _number_sequences (0)
  , _number_segments (length, 0)
  , _occupancy (length, 0)
{
}

// Forbidden
// SequenceOccupation::SequenceOccupation (const SequenceOccupation& other);
// SequenceOccupation& SequenceOccupation::operator= (const SequenceOccupation& other);

SequenceOccupation::~SequenceOccupation (void)
{
  for (std::size_t i = 0; i < _site_groups.size(); ++i)
    { delete _site_groups[i]; }
  for (std::vector <PartialStrand*>::iterator strand_it 
	 = _partials.begin();
       strand_it != _partials.end(); ++strand_it)
    { delete *strand_it; }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void SequenceOccupation::add_element (int first, int last)
{
  /** @pre first must be within sequence bound. */
  REQUIRE ((first >= 0) && (first < _occupancy.size()));
  /** @pre last must be within sequence bound. */
  REQUIRE ((last >= 0) && (last < _occupancy.size()));
  /** @pre first must be smaller or equal to last. */
  REQUIRE (first <= last);

  // update occupancy status
  for (int i = first; i <= last; i++) { _occupancy [i] += 1; }

  // notify change
  notify_change (first, last);
}

void SequenceOccupation::remove_element (int first, int last)
{
  /** @pre first must be within sequence bound. */
  REQUIRE ((first >= 0) && (first < _occupancy.size()));
  /** @pre last must be within sequence bound. */
  REQUIRE ((last >= 0) && (last < _occupancy.size()));
  /** @pre first must be smaller or equal to last. */
  REQUIRE (first <= last);

  // update occupancy status
  for (int i = first; i <= last; i++) { _occupancy [i] -= 1; }  

  // notify change
  notify_change (first, last);
}

void SequenceOccupation::add_sequence (int quantity)
{
  /** @pre quantity must be positive. */
  REQUIRE (quantity >= 0);

  _number_sequences += quantity;
  notify_all_sites();
}

void SequenceOccupation::remove_sequence (int quantity)
{
  /** @pre quantity must be smaller than the current number of sequences. */
  REQUIRE (quantity <= _number_sequences);

  _number_sequences -= quantity;
  notify_all_sites();
}

bool SequenceOccupation::extend_strand (int strand_id, int position)
{
  /** @pre position must be within sequence bound. */
  REQUIRE ((position >= 0) && (position < _occupancy.size()));
  /** @pre strand_id must be in valid range. */
  REQUIRE (strand_id >= 0);
  // create new partial strand if necessary
  for (int i = _partials.size(); i <= strand_id; ++i)
    { 
      _partials.push_back (new PartialStrand (_occupancy.size()));
      _partial_creation_order.push_back (i);
    }

  if (_partials [strand_id]->occupied (position)) { return false; }
  _partials [strand_id]->occupy (position);
  _occupancy [position] -= 1;
  _number_segments [position] += 1;
  notify_change (position, position);
  return true;
}

void SequenceOccupation::release_strand_id (int strand_id)
{
  _partials [strand_id]->reset();
  _partial_creation_order.remove (strand_id);
  _unused_partials.push (strand_id);
  ++_number_sequences;
  for (std::size_t i = 0; i < _occupancy.size(); ++i) 
    { ++_occupancy [i]; _number_segments [i] -= 1; }
}

void SequenceOccupation::watch_site (BindingSite& site)
{
  // look if site should be added to an existing group
  bool new_group = true;
  std::size_t ind_g = position_to_group (site.first());
  if (ind_g < _site_groups.size())
    {
      SiteGroup* g = _site_groups [ind_g];
      if (site.overlaps (g->first(), g->last()))
	{
	  // add to group and look whether it overlaps with the following groups
	  g->add_site (site);
	  fuse_groups (ind_g);
	  new_group = false;
	}
    }
  if (new_group == true)
    {
      // create and insert new group
      SiteGroup* g = new SiteGroup;
      g->add_site (site);
      if (ind_g < _site_groups.size())
	{ _site_groups.insert (_site_groups.begin()+ind_g, g); }
      else
	{ _site_groups.push_back (g); }
    }

  // send first notification about site availability
  site.update();
}

// ============================
//  Public Methods - Accessors
// ============================
//
int SequenceOccupation::number_sites (int first, int last) const
{
  /** @pre first must be within sequence bound. */
  REQUIRE ((first >= 0) && (first < _occupancy.size()));
  /** @pre last must be within sequence bound. */
  REQUIRE ((last >= 0) && (last < _occupancy.size()));
  /** @pre first must be smaller or equal to last. */
  REQUIRE (first <= last);

  int min = _number_segments [first];
  for (int i = first+1; i <= last; ++i)
    {
      if (_number_segments [i] < min) { min = _number_segments [i]; }
    }
  return _number_sequences + min;
}

int SequenceOccupation::number_available_sites (int first, int last) const
{
  /** @pre first must be within sequence bound. */
  REQUIRE ((first >= 0) && (first < _occupancy.size()));
  /** @pre last must be within sequence bound. */
  REQUIRE ((last >= 0) && (last < _occupancy.size()));
  /** @pre first must be smaller or equal to last. */
  REQUIRE (first <= last);

  int max_occupied = _occupancy [first];
  for (int i = first+1; i <= last; ++i)
    {
      if (_occupancy [i] > max_occupied) { max_occupied = _occupancy [i]; }
    }
  int result = _number_sequences - max_occupied;
  if (result < 0) { return 0; }
  return result;
}

int SequenceOccupation::partial_strand_id (int position) const
{
  /** @pre position must be within sequence bound. */
  REQUIRE ((position >= 0) && (position < _occupancy.size()));

  // test existing strands
  for (std::list <int>::const_iterator it = _partial_creation_order.begin();
       it != _partial_creation_order.end(); ++it)
    {
      if (_partials [*it]->occupied (position) == false) { return *it; }
    }

  // segment could not be created on existing strands, give index of
  // new strand
  // recycle an existing strand:
  if (!_unused_partials.empty()) 
    { 
      int result = _unused_partials.top();
      _unused_partials.pop();
      _partial_creation_order.push_back (result);
      return result; 
    }
  
  // else return index of next new strand:
  return _partials.size();
}

std::list <std::vector <int> > SequenceOccupation::partial_strands (void) const
{
  std::list <std::vector <int> > result;
  for (std::list <int>::const_iterator it = _partial_creation_order.begin();
       it != _partial_creation_order.end(); ++it)
    {
      result.push_back (_partials [*it]->segments());
    }
  return result;
}

// =================
//  Private Methods
// =================
//
void SequenceOccupation::fuse_groups (std::size_t index)
{
  // if group is last in vector no fusion with a follower is possible
  if (index >= _site_groups.size()-1) return;

  // fuse if group overlaps with the following one
  if (_site_groups [index]->last() < _site_groups [index+1]->first())
    { return; }
  _site_groups [index]->absorb (*_site_groups [index+1]);
  delete _site_groups [index+1];
  _site_groups.erase (_site_groups.begin() + index+1);
  fuse_groups (index);
}

std::size_t SequenceOccupation::position_to_group (int position) const
{
  if (_site_groups.size() == 0) { return 1; }
  if (_site_groups[0]->last() >= position) { return 0; }

  // we look for the largest i such that
  //  _site_groups[i].last() < position
  std::size_t i_min = 0;
  std::size_t i_max = _site_groups.size()-1;
  std::size_t i;
  while (i_min < i_max)
    {
      i = (i_min + i_max + 1)/2;
      if (_site_groups[i]->last() >= position) { i_max = i-1; }
      else { i_min = i; }
    }

  // we return the next group that may contain position, i.e. i+1
  return i_min+1;
}

void SequenceOccupation::notify_change (int a, int b) const
{
  if (_site_groups.size() > 0)
    {
      std::size_t last_group = position_to_group (b);
      if (last_group == _site_groups.size()) { --last_group; }
      for (std::size_t i = position_to_group (a); i <= last_group; ++i)
	{ _site_groups [i]->update (a, b); }
    }
}

void SequenceOccupation::notify_all_sites (void) const
{
  for (std::size_t i = 0; i < _site_groups.size(); ++i)
    { _site_groups [i]->update_all(); }
}
