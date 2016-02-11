

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

// ==================
//  Project Includes
// ==================
//
#include "sequenceoccupation.h"
#include "siteavailability.h"
#include "bindingsite.h"
#include "watchergroup.h"
#include "partialstrand.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
SequenceOccupation::SequenceOccupation (int length, int number)
  : _number_sequences (number)
  , _occupancy (length, 0)
{
}

// Forbidden
// SequenceOccupation::SequenceOccupation (const SequenceOccupation& other_sequence_occupation);
// SequenceOccupation& SequenceOccupation::operator= (const SequenceOccupation& other_sequence_occupation);

SequenceOccupation::~SequenceOccupation (void)
{
  for (int i = 0; i < _watcher_groups.size(); ++i) 
    { 
      delete _watcher_groups[i];
    }
  for (std::list <SiteAvailability*>::iterator 
	 watcher_it = _moving_sites.begin();
       watcher_it != _moving_sites.end(); ++watcher_it) 
    { 
      delete *watcher_it;
    }
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

void SequenceOccupation::start_new_segment (int position)
{
  /** @pre position must be within sequence bound. */
  REQUIRE ((position >= 0) && (position < _occupancy.size()));

  _occupancy [position] -= 1;
  std::list <PartialStrand>::iterator strand_it = _partials.begin();
  while (strand_it != _partials.end())
    {
      if (strand_it->start_new_segment (position)) { break; }
      ++strand_it;
    }
  if (strand_it == _partials.end()) 
    { 
      _partials.push_back (_occupancy.size()); --strand_it;
      strand_it->start_new_segment (position);
    }
  _check_completion (strand_it);
  notify_change (position, position);
}

void SequenceOccupation::extend_segment (int position)
{
  /** @pre position must be within sequence bound. */
  REQUIRE ((position >= 0) && (position < _occupancy.size()));

  _occupancy [position] -= 1;
  std::list <PartialStrand>::iterator strand_it = _partials.begin();
  while (strand_it != _partials.end())
    {
      if (strand_it->extend_segment (position)) { break; }
      ++strand_it;
    }
  /** @pre There must be a segment to extend. */
  REQUIRE (strand_it != _partials.end()); 
  _check_completion (strand_it);
  notify_change (position, position);
}

void SequenceOccupation::watch_site (BindingSite& site)
{
  SiteAvailability* new_watcher;
  int first = site.relative_first();
  int last = site.relative_last();

  // look if watcher should be added to an existing group
  bool new_group = true;
  int ind_g = position_to_group (first);
  if (ind_g < _watcher_groups.size())
    {
      WatcherGroup* g = _watcher_groups [ind_g];
      if (((first <= g->first()) && (last >= g->first()))
	  || ((first <= g->last()) && (last >= g->last())))
	{
	  // add to group and look whether it overlaps with the following groups
	  new_watcher = g->add_watcher (site);
	  fuse_groups (ind_g);
	  new_group = false;
	}
    }
  if (new_group == true)
    {
      // create and insert new group
      WatcherGroup* g = new WatcherGroup;
      new_watcher = g->add_watcher (site);
      if (ind_g < _watcher_groups.size())
	_watcher_groups.insert (_watcher_groups.begin()+ind_g, g);
      else
	_watcher_groups.push_back (g);
    }

  // send first notification about site availability
  new_watcher->notify (number_available_sites 
		       (new_watcher->site().relative_first(),
			new_watcher->site().relative_last()));
}

void SequenceOccupation::watch_moving_site (BindingSite& site)
{
  _moving_sites.push_back (new SiteAvailability (site));
}

void SequenceOccupation::unwatch_moving_site (const BindingSite& site)
{
  std::list <SiteAvailability*>::iterator watcher_it = _moving_sites.begin();
  while (watcher_it != _moving_sites.end()) 
    { 
      if (&((*watcher_it)->site()) == &site) { delete *watcher_it; break; }
      ++watcher_it;
    }
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

std::list <int> SequenceOccupation::left_ends (void) const
{
  std::list <int> result;
  for (std::list <PartialStrand>::const_iterator strand_it = _partials.begin();
       strand_it != _partials.end (); ++strand_it)
    {
      std::list <int> tmp = strand_it->left_ends();
      result.insert (result.end(), tmp.begin(), tmp.end());
    }
  return result;
}

std::list <int> SequenceOccupation::right_ends (void) const
{
  std::list <int> result;
  for (std::list <PartialStrand>::const_iterator strand_it = _partials.begin();
       strand_it != _partials.end (); ++strand_it)
    {
      std::list <int> tmp = strand_it->right_ends();
      result.insert (result.end(), tmp.begin(), tmp.end());
    }
  return result;
}

// =================
//  Private Methods
// =================
//
void SequenceOccupation::fuse_groups (int index)
{
  if (index >= _watcher_groups.size()-1) return;
  if (_watcher_groups[index]->last() < _watcher_groups[index+1]->first())
    return;
  _watcher_groups[index]->absorb (*_watcher_groups[index+1]);
  delete _watcher_groups[index+1];
  _watcher_groups.erase (_watcher_groups.begin() + index+1);
  fuse_groups (index);
}

int SequenceOccupation::position_to_group (int position) const
{
  if (_watcher_groups.size() == 0) { return 1; }
  if (_watcher_groups[0]->last() >= position) { return 0; }

  // we look for the largest i such that
  //  _watcher_groups[i].last() < position
  int i_min = 0; int i_max = _watcher_groups.size()-1;
  int i;
  while (i_min < i_max)
    {
      i = (i_min + i_max + 1)/2;
      if (_watcher_groups[i]->last() >= position) { i_max = i-1; }
      else { i_min = i; }
    }

  // we return the next group that may contain position, i.e. i+1
  return i_min+1;
}

void SequenceOccupation::notify_change (int a, int b) const
{
  if (_watcher_groups.size() == 0) return;
  int last_group = position_to_group (b);
  if (last_group == _watcher_groups.size()) { --last_group; }
  for (int i = position_to_group (a); i <= last_group; ++i)
    {
      const std::list <SiteAvailability*>&
	watchers = _watcher_groups[i]->watchers();
      for (std::list <SiteAvailability*>::const_iterator
	     watcher_it = watchers.begin();
	   watcher_it != watchers.end(); ++watcher_it)
	{
	  if ((*watcher_it)->is_affected (a, b)) 
	    {
	      (*watcher_it)->notify (number_available_sites 
				     ((*watcher_it)->site().relative_first(),
				      (*watcher_it)->site().relative_last()));
	    }

	}
    }
  for (std::list <SiteAvailability*>::const_iterator
	 watcher_it = _moving_sites.begin();
       watcher_it != _moving_sites.end(); ++watcher_it) 
    {
      if ((*watcher_it)->is_affected (a, b))
	{
	  (*watcher_it)->notify (number_available_sites 
				 ((*watcher_it)->site().relative_first(),
				  (*watcher_it)->site().relative_last()));
	}
    }
}

void SequenceOccupation::notify_all_sites (void) const
{
  for (int i = 0; i < _watcher_groups.size(); ++i)
    {
      const std::list <SiteAvailability*>&
	watchers = _watcher_groups[i]->watchers();
      for (std::list <SiteAvailability*>::const_iterator
	     watcher_it = watchers.begin();
	   watcher_it != watchers.end(); ++watcher_it)
	{
	  (*watcher_it)->notify (number_available_sites 
				 ((*watcher_it)->site().relative_first(),
				  (*watcher_it)->site().relative_last()));
	}
    }
  for (std::list <SiteAvailability*>::const_iterator 
	 watcher_it = _moving_sites.begin();
       watcher_it != _moving_sites.end(); ++watcher_it) 
    {
      (*watcher_it)->notify (number_available_sites 
			     ((*watcher_it)->site().relative_first(),
			      (*watcher_it)->site().relative_last()));
    }
}

void SequenceOccupation::
_check_completion (std::list <PartialStrand>::iterator& strand_it)
{
  if (strand_it->completed()) 
    {
      _partials.erase (strand_it);
      ++_number_sequences;
      for (int i = 0; i < _occupancy.size(); ++i) { ++_occupancy [i]; }
    }
}
