

/**
 * @file sitegroup.cpp
 * @brief Implementation of the SiteGroup class.
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
#include "sitegroup.h"
#include "bindingsite.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
SiteGroup::SiteGroup (void)
  : _first (std::numeric_limits <int>::max())
  , _last (-1)
{
}

// SiteGroup::SiteGroup (const SiteGroup& other);
// SiteGroup& SiteGroup::operator= (const SiteGroup& other;
// SiteGroup::~SiteGroup (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void SiteGroup::add_site (BindingSite& site)
{
  _sites.push_back (&site);
  if (site.first() < _first) _first = site.first();
  if (site.last() > _last) _last = site.last();

  if (_sites.size() > 5)
    {
      std::cerr << "WARNING: created a site watcher group containing "
		<< _sites.size() << " elements.\n";
    }
}

void SiteGroup::absorb (SiteGroup& other_group)
{
  _sites.splice (_sites.end(), other_group._sites);
  if (other_group._first < _first) _first = other_group._first;
  if (other_group._last > _last) _last = other_group._last;
  other_group._first = std::numeric_limits<int>::max();
  other_group._last = -1;

  if (_sites.size() > 5)
    {
      std::cerr << "WARNING: created a site watcher group containing "
		<< _sites.size() << " elements.\n";
    }
}


// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
