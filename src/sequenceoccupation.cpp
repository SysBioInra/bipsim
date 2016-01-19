

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

// ==========================
//  Constructors/Destructors
// ==========================
//
SequenceOccupation::SequenceOccupation (int length, int number)
  : _number (number)
  , _occupancy_map (length, number)
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
  // add the reference and position to the chemicals map
  // _chemical_map [&element].push_back (SiteLocation(first, last));
  
  // update occupancy status
  for (int i = first; i <= last; i++) { _occupancy_map [i] += 1; }

  // notify change
  notify_change (first, last);
}

void SequenceOccupation::remove_element (const BoundChemical& element,
				    int first, int last)
{
  // remove the reference and position from the chemicals map
  // remove_reference_from_map (element, first, last);

  // update occupancy status
  for (int i = first; i <= last; i++) { _occupancy_map [i] -= 1; }  

  // notify change
  notify_change (first, last);
}

void SequenceOccupation::add_sequence (int quantity)
{
  _number += quantity;

  // notify change
  notify_all_sites();
}

void SequenceOccupation::remove_sequence (int quantity)
{
  _number -= quantity;

  // notify change
  notify_all_sites();
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
  new_watcher->notify (first, last, _number, _occupancy_map);
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
      if (_watcher_groups[i]->last() >= position) i_max = i-1;
      else i_min = i;
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
	  (*watcher_it)->notify (a, b, _number, _occupancy_map);
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
	  (*watcher_it)->notify (0, _occupancy_map.size()-1,
				 _number, _occupancy_map);
	}
    }
}

void 
SequenceOccupation::remove_reference_from_map (const BoundChemical& chemical,
					       int first, int last)
{
  SiteLocationList& location_list = _chemical_map [&chemical];
  /** @pre There must be a chemical of this type in the map. */
  REQUIRE (location_list.size() > 0); 

  if (location_list.size() == 1) // there is only one element in the list
    {
      // the list will be empty, we can remove it from the map
      // (no more chemical of the same type is bound to the sequence)
      _chemical_map.erase (&chemical);
    }
  else // at least one chemical of same type will be left on the sequence
    {
      // we erase a chemical at the right location
      SiteLocationList::iterator site_location = location_list.begin();
      while ( (( site_location->first() != first )
	       || ( site_location->last() != last ))
	      && (site_location != location_list.end()) )
	{ 
	  site_location++;
	}

      if (site_location != location_list.end())
	{
	  location_list.erase (site_location);
	}
      else 
	{
	  std::cerr << "ERROR: trying to delete bound chemical " << &chemical
		    << " that does not exist at position [" << first
		    << " , " << last << "]" << std::endl;
	  site_location = location_list.begin();
	  while (site_location != location_list.end())
	    { 
	      std::cout << site_location->first() << " "
			<< site_location->last() << std::endl;
	      site_location++;
	    }
	}
    }
}
