

/**
 * @file watchergroup.cpp
 * @brief Implementation of the WatcherGroup class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <limits> // std::numeric_limits

// ==================
//  Project Includes
// ==================
//
#include "watchergroup.h"
#include "siteavailability.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
WatcherGroup::WatcherGroup (void)
  : _first (std::numeric_limits <int>::max())
  , _last (-1)
{
}

// Not needed for this class (use of default copy constructor) !
// WatcherGroup::WatcherGroup (const WatcherGroup& other_watcher_group);

WatcherGroup::~WatcherGroup (void)
{
  for (std::list <SiteAvailability*>::iterator
	 watcher_it = _watchers.begin();
       watcher_it != _watchers.end(); ++watcher_it)
    {
      delete *watcher_it;
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
SiteAvailability* WatcherGroup::add_watcher (int first, int last,
					     SiteObserver& observer)
{
  SiteAvailability* new_watcher = new SiteAvailability (first, last, observer);
  _watchers.push_back (new_watcher);
  if (first < _first) _first = first;
  if (last > _last) _last = last;

  if (_watchers.size() > 5)
    {
      std::cerr << "WARNING: created a site watcher group containing "
		<< _watchers.size() << " elements.\n";
    }
  return new_watcher;
}

void WatcherGroup::absorb (WatcherGroup& other_group)
{
  for (std::list <SiteAvailability*>::iterator
	 watcher_it = other_group._watchers.begin();
       watcher_it != other_group._watchers.end(); ++ watcher_it)
    {
      _watchers.push_back (*watcher_it);
    }
  other_group._watchers.clear();
  if (other_group._first < _first) _first = other_group._first;
  if (other_group._last > _last) _last = other_group._last;
  other_group._first = std::numeric_limits<int>::max();
  other_group._last = -1;

  if (_watchers.size() > 5)
    {
      std::cerr << "WARNING: created a site watcher group containing "
		<< _watchers.size() << " elements.\n";
    }
}


// ============================
//  Public Methods - Accessors
// ============================
//


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// WatcherGroup& WatcherGroup::operator= (const WatcherGroup& other_watcher_group);


// =================
//  Private Methods
// =================
//
