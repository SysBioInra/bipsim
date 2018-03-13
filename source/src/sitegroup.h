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
 * @file sitegroup.h
 * @brief Header for the ClassName class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SITE_GROUP_H
#define SITE_GROUP_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class storing a group of sites on the same sequence.
 *
 * SiteGroup stores BindingSite objects. It can also 
 * absorb another group. It computes the first and last base spanned by the
 * group on the sequence.
 */
class SiteGroup
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  SiteGroup (void);

  // /* @brief Copy constructor. */
  // SiteGroup (const SiteGroup& other);
  // /* @brief Assignment operator. */
  // SiteGroup& operator= (const SiteGroup& other);
  // /* @brief Destructor. */
  // ~SiteGroup (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add site to group.
   * @param site BindingSite.
   */
  void add_site (BindingSite& site);
  
  /**
   * @brief Absorb another group, leaving it empty.
   * @param other_group Other SiteGroup whose sites are taken over.
   */
  void absorb (SiteGroup& other_group);

  /**
   * @brief Update all sites contained within a given segment.
   * @param a Start of segment within wich sites should be updated.
   * @param b End of segment within wich sites should be updated.
   */
  void update (int a, int b);
  
  /**
   * @brief Update all sites contained in group.
   */
  void update_all (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to first base occupied by a site in the group.
   * @return Position of first base spanned by the group 
   *  (<tt>std::numeric_limits \<int\>:: max()</tt> if group is empty).
   */
  int first (void) const;
  
  /**
   * @brief Accessor to last base occupied by a site in the group.
   * @return Position of last base spanned by the group (-1 if group is empty).
   */
  int last (void) const;

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief First base spanned by the group. */
  int _first;

  /** @brief Last base spanned by the group. */
  int _last;

  /** @brief Sites contained in the group. */
  std::list <BindingSite*> _sites;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
#include "bindingsite.h"

inline int SiteGroup::first (void) const
{
  return _first;
}

inline int SiteGroup::last (void) const
{
  return _last;
}

inline void SiteGroup::update (int a, int b)
{
  for (std::list <BindingSite*>::iterator site_it = _sites.begin();
       site_it != _sites.end(); ++site_it)
    { 
      if ((*site_it)->overlaps (a, b)) { (*site_it)->update(); }
    }
}

inline void SiteGroup::update_all (void)
{
  for (std::list <BindingSite*>::iterator site_it = _sites.begin();
       site_it != _sites.end(); ++site_it)
    { (*site_it)->update(); }
}

#endif // SITE_GROUP_H
