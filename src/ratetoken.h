

/**
 * @file ratetoken.h
 * @brief Header for the RateToken class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_TOKEN_H
#define RATE_TOKEN_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class enabling restrited access to RateToken.
 *
 * GroupKey uses the pass-key idiom to allow other objects access to some of
 * RateToken's members.
 * @sa RateKey, RateToken.
 */
class GroupKey
{
 private:
  friend class HybridRateContainer;
  /**
   * @brief Default constructor (can only be created by friends).
   */
  GroupKey (void) {}
};

/**
 * @brief Class enabling restrited access to RateToken.
 *
 * GroupKey uses the pass-key idiom to allow other objects access to some of
 * RateToken's members.
 * @sa RateKey, RateToken.
 */
class RateKey
{
 private:
  friend class RateGroup;
  /**
   * @brief Default constructor (can only be created by friends).
   */
  RateKey (void) {}
};

/**
 * @brief Class representing current position within a HybridRateContainer.
 *
 * RateToken bears all information associated with a rate in a
 * HybridRateContainer (its original index, its rate, its position in the
 * container). The token can be passed from the main container to the groups.
 * it is composed of, each of these classes having only access to some of the
 * members of the token.
 * @sa RateGroup, HybridRateContainer.
 */
class RateToken
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param index Unique index associated with the reaction rate.
   */
  RateToken (int index): _index (index), _group (0), _position (0), _rate (0)
  {}

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // RateToken (const RateToken& other_class_name);
  // /* @brief Assignment operator. */
  // RateToken& operator= (const RateToken& other_rate_token);
  // /* @brief Destructor. */
  // ~RateToken (void);


  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Group index setter.
   * @param key Key restraining access to this method (only the friends of 
   *  GroupKey can access this setter).
   * @param group Group index where the token currently is.
   */
  void set_group (const GroupKey& key, int group) { _group = group; }
  
  /**
   * @brief Position setter.
   * @param key Key restraining access to this method (only the friends of 
   *  RateKey can access this setter).
   * @param position Current position of token within the group.
   */
  void set_position (const RateKey& key, int position) { _position = position; }
  
  /**
   * @brief Rate value setter.
   * @param key Key restraining access to this method (only the friends of 
   *  RateKey can access this setter).
   * @param rate Current rate value.
   */
  void set_rate (const RateKey& key, double rate) { _rate = rate; }


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to rate index.
   */
  int index (void) const { return _index; }

  /**
   * @brief Accessor to current group index.
   */
  int group (void) const { return _group; }

  /**
   * @brief Accessor to current position within group.
   */
  int position (void) const { return _position; }

  /**
   * @brief Accessor to current rate value.
   */
  double rate (void) const { return _rate; }

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Unique index originally attributed to reaction rate. */
  int _index;
  
  /** @brief Index of the group the rate is currently in. */
  int _group;

  /** @brief Current position of the rate within the group. */
  int _position;

  /** @brief Current rate value. */
  double _rate;
  
  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//


#endif // RATE_TOKEN_H
