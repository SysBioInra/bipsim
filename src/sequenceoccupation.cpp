
/**
 * @file sequenceoccupation.cpp
 * @brief Implementation of the SequenceOccupation class.
 * 
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

// ==========================
//  Constructors/Destructors
// ==========================
//
SequenceOccupation::SequenceOccupation (int length, int number)
  : _number_sequences (number)
  , _occupancy (length, 0)
  , _free_end_factory (0)
{
}

// Forbidden
// SequenceOccupation::SequenceOccupation (const SequenceOccupation& other);
// SequenceOccupation& SequenceOccupation::operator= (const SequenceOccupation& other);

SequenceOccupation::~SequenceOccupation (void)
{
  for (int i = 0; i < _site_groups.size(); ++i)  { delete _site_groups[i]; }
  for (std::list <PartialStrand*>::iterator strand_it = _partials.begin();
       strand_it != _partials.end(); ++strand_it)
    { delete *strand_it; }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void SequenceOccupation::add_element (const BoundChemical& element,
				      int first, int last)
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

void SequenceOccupation::remove_element (const BoundChemical& element,
					 int first, int last)
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

  // notify change
  notify_all_sites();
}

void SequenceOccupation::remove_sequence (int quantity)
{
  /** @pre quantity must be smaller than the current number of sequences. */
  REQUIRE (quantity <= _number_sequences);

  _number_sequences -= quantity;

  // notify change
  notify_all_sites();
}

void SequenceOccupation::start_segment (int position)
{
  /** @pre position must be within sequence bound. */
  REQUIRE ((position >= 0) && (position < _occupancy.size()));
  /** @pre Free end factory must have been defined. */
  REQUIRE (_free_end_factory != 0);

  _occupancy [position] -= 1;
  std::list <PartialStrand*>::iterator strand_it = _partials.begin();
  while (strand_it != _partials.end())
    {
      if ((*strand_it)->start_segment (position)) { break; }
      ++strand_it;
    }
  if (strand_it == _partials.end()) 
    { 
      _partials.push_back 
	(new PartialStrand (_occupancy.size(), *_free_end_factory)); 
      --strand_it;
      (*strand_it)->start_segment (position);
    }
  _check_completion (strand_it);
  notify_change (position, position);
}

void SequenceOccupation::extend_segment (int position)
{
  /** @pre position must be within sequence bound. */
  REQUIRE ((position >= 0) && (position < _occupancy.size()));
  /** @pre Free end factory must have been defined. */
  REQUIRE (_free_end_factory != 0);

  _occupancy [position] -= 1;
  std::list <PartialStrand*>::iterator strand_it = _partials.begin();
  while (strand_it != _partials.end())
    {
      if ((*strand_it)->extend_segment (position)) { break; }
      ++strand_it;
    }
  /** @pre There must be a segment to extend. */
  REQUIRE (strand_it != _partials.end()); 
  _check_completion (strand_it);
  notify_change (position, position);
}

void SequenceOccupation::register_site (BindingSite& site)
{
  // look if site should be added to an existing group
  bool new_group = true;
  int ind_g = position_to_group (site.first());
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

void SequenceOccupation::register_moving_site (BindingSite& site)
{
  _moving_sites.push_back (&site);
  site.update();
}

void SequenceOccupation::deregister_moving_site (BindingSite& site)
{
  _moving_sites.remove (&site);
}

// ============================
//  Public Methods - Accessors
// ============================
//
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

// =================
//  Private Methods
// =================
//
void SequenceOccupation::fuse_groups (int index)
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

int SequenceOccupation::position_to_group (int position) const
{
  if (_site_groups.size() == 0) { return 1; }
  if (_site_groups[0]->last() >= position) { return 0; }

  // we look for the largest i such that
  //  _site_groups[i].last() < position
  int i_min = 0; int i_max = _site_groups.size()-1;
  int i;
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
  // update static sites
  if (_site_groups.size() > 0)
    {
      int last_group = position_to_group (b);
      if (last_group == _site_groups.size()) { --last_group; }
      for (int i = position_to_group (a); i <= last_group; ++i)
	{ _site_groups [i]->update (a, b); }
    }

  // update moving sites
  for (std::list <BindingSite*>::const_iterator site_it = _moving_sites.begin();
       site_it != _moving_sites.end(); ++site_it) 
    {
      if ((*site_it)->overlaps (a, b)) { (*site_it)->update(); }
    }
}

void SequenceOccupation::notify_all_sites (void) const
{
  // update static sites
  for (int i = 0; i < _site_groups.size(); ++i)
    { _site_groups [i]->update_all(); }

  // update moving sites
  for (std::list <BindingSite*>::const_iterator site_it = _moving_sites.begin();
       site_it != _moving_sites.end(); ++site_it) 
    { (*site_it)->update(); }
}

void SequenceOccupation::
_check_completion (std::list <PartialStrand*>::iterator& strand_it)
{
  if ((*strand_it)->completed()) 
    {
      delete *strand_it;
      _partials.erase (strand_it);
      ++_number_sequences;
      for (int i = 0; i < _occupancy.size(); ++i) { ++_occupancy [i]; }
    }
}
