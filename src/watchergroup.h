

/**
 * @file watchergroup.h
 * @brief Header for the ClassName class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef WATCHER_GROUP_H
#define WATCHER_GROUP_H

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
 * @brief Class storing/creating a group of site watchers on the same sequence.
 *
 * WatcherGroup can store and create SiteAvailability objects. It can also 
 * absorb another group. It computes the first and last base spanned by the
 * group on the sequence.
 */
class WatcherGroup
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  WatcherGroup (void);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  WatcherGroup (const WatcherGroup& other_watcher_group);
  /** @brief Assignment operator. */
  WatcherGroup& operator= (const WatcherGroup& other_watcher_group);

 public:
  /**
   * @brief Destructor.
   */
  ~WatcherGroup (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create and add a watcher for a specific site.
   * @param first First base of the site to watch.
   * @param last Last base of the site to watch.
   * @param observer SiteObserver to notify when site changes.
   */
  SiteAvailability* add_watcher (int first, int last, SiteObserver& observer);
  
  /**
   * @brief Absorb another group, leaving it empty.
   * @param other_group Other WatcherGroup whose watchers are taken over.
   */
  void absorb (WatcherGroup& other_group);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to watchers in the group.
   * @return List of watchers contained in the group.
   */
  const std::list <SiteAvailability*>& watchers (void) const;

  /**
   * @brief Accessor to first base occupied by a watcher in the group.
   * @return Position of first base spanned by the group.
   */
  int first (void) const;
  
  /**
   * @brief Accessor to last base occupied by a watcher in the group.
   * @return Position of last base spanned by the group.
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

  /** @brief Watchers contained in the group. */
  std::list <SiteAvailability*> _watchers;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
inline const std::list <SiteAvailability*>& WatcherGroup::watchers (void) const
{
  return _watchers;
}

inline int WatcherGroup::first (void) const
{
  return _first;
}

inline int WatcherGroup::last (void) const
{
  return _last;
}


#endif // WATCHER_GROUP_H
