

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
#include "sitelocation.h"
#include "siteobserver.h"
#include "watchergroup.h"
#include "segment.h"

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
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void SequenceOccupation::add_element (const BoundChemical& element,
				      int first, int last)
{
  // update occupancy status
  for (int i = first; i <= last; i++) { _occupancy [i] += 1; }

  // notify change
  notify_change (first, last);
}

void SequenceOccupation::remove_element (const BoundChemical& element,
				    int first, int last)
{
  // update occupancy status
  for (int i = first; i <= last; i++) { _occupancy [i] -= 1; }  

  // notify change
  notify_change (first, last);
}

void SequenceOccupation::add_sequence (int quantity)
{
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
  _segments.push_front (Segment (position, position));
}

void SequenceOccupation::extend_segment (int position)
{
  /** @pre position must be within sequence bound. */
  REQUIRE ((position >= 0) && (position < _occupancy.size()));

  _occupancy [position] -= 1;
  
  // we look for the segment to be extended
  int previous = (position == 0) ? (_occupancy.size()-1) : (position-1);
  std::list <Segment>::iterator segment_it = _segments.begin();
  while (segment_it != _segments.end())
    {
      if (segment_it->last == previous)
	{
	  segment_it->last = position;
	  break;
	}
      ++segment_it;
    }

  // check whether segment has been completed
  // caution: segment can be circular
  int segment_length = segment_it->last - segment_it->first + 1;
  if ((segment_length == 0) || (segment_length == _occupancy.size()))
    {
      _segments.erase (segment_it);
      ++_number_sequences;
    }
  else
    {
      // we place the modified segment at the beginning of list
      _segments.splice (_segments.begin(), _segments, segment_it);
    }
}

void SequenceOccupation::add_watcher (int first, int last,
				      SiteObserver& observer)
{
  SiteAvailability* new_watcher;

  // look if watcher should be added to an existing group
  bool new_group = true;
  int ind_g = position_to_group (first);
  if (ind_g < _watcher_groups.size())
    {
      WatcherGroup* g = _watcher_groups [ind_g];
      if (((first <= g->first()) && (last >= g->first()))
	  || ((first <= g->first()) && (last >= g->last())))
	{
	  // add to group and look whether it overlaps with the following groups
	  new_watcher = g->add_watcher (first, last, observer);
	  fuse_groups (ind_g);
	  new_group = false;
	}
    }
  if (new_group == true)
    {
      // create and insert new group
      WatcherGroup* g = new WatcherGroup;
      new_watcher = g->add_watcher (first, last, observer);
      if (ind_g < _watcher_groups.size())
	_watcher_groups.insert (_watcher_groups.begin()+ind_g, g);
      else
	_watcher_groups.push_back (g);
    }

  // send first notification about site availability
  notify_site (new_watcher);
}

// ============================
//  Public Methods - Accessors
// ============================
//

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

int SequenceOccupation::position_to_group (int position)
{
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

void SequenceOccupation::notify_change (int a, int b)
{
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
	  if (is_site_affected (*watcher_it, a, b)) 
	    { notify_site (*watcher_it); }
	}
    }
}

void SequenceOccupation::notify_all_sites (void)
{
  for (int i = 0; i < _watcher_groups.size(); ++i)
    {
      const std::list <SiteAvailability*>&
	watchers = _watcher_groups[i]->watchers();
      for (std::list <SiteAvailability*>::const_iterator
	     watcher_it = watchers.begin();
	   watcher_it != watchers.end(); ++watcher_it)
	{
	  notify_site (*watcher_it);
	}
    }
}

void SequenceOccupation::notify_site (SiteAvailability* watcher)
{
  int first = watcher->first();
  int last = watcher->last();

  // check current occupancy
  int max_occupied = 0;
  for (int i = first; i < last; i++)
    {
      if (_occupancy [i] > max_occupied) { max_occupied = _occupancy [i]; }
    }

  // notify changes
  int number_sites_available = _number_sequences - max_occupied;
  if (number_sites_available < 0) { number_sites_available = 0; }
  watcher->notify (number_sites_available);
}
