

/**
 * @file reactionclassification.h
 * @brief Header for the ReactionClassification class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef REACTION_CLASSIFICATION_H
#define REACTION_CLASSIFICATION_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <list> // std::list

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // REQUIRE()
#include "forwarddeclarations.h"

/**
 * @brief Split up reactions according to integration time step.
 *
 * ReactionClassification is a helper class that allows to split up reactions
 * in classes that should be integrated with the same time step. Classes can
 * be created for any specific time step or specifiying that reactions should
 * be always updated (ReactionClassification::ALWAYS_UPDATED) and reactions added
 * to existing classes.
 */
class ReactionClassification
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  ReactionClassification (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ReactionClassification ( const ReactionClassification& other_reaction_classification );

  /**
   * @brief Destructor
   */
  virtual ~ReactionClassification (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create a new class for reactions integrated at a specific time step.
   * @param time_step Time step at which reactions belonging to this class should be integrated. Must
   *  be a strictly positive value or ReactionClassification::ALWAYS_UPDATED.
   * @return Identifier attributed to the class. Corresponds to the order of creations of classes,
   *  starting with 0.
   */
  int create_new_class (double time_step);

  /**
   * @brief Add reaction to an existing class.
   * @param class_id Integer identifier of the class.
   * @param reaction Reaction to add to the class.
   */
  void add_reaction_to_class (int class_id, Reaction& reaction);

  /**
   * @brief Add several reactions to an existing class.
   * @param class_id Integer identifier of the class.
   * @param reactions List of reactions to add to the class.
   */
  void add_reaction_list_to_class (int class_id, const std::list<Reaction*>& reactions);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return number of classes.
   * @return Number of reaction classes currently created. Class identifiers can be deduced
   *  from this value as attributed identifiers span 0 to number_classes()-1;
   */
  int number_classes (void) const;

  /**
   * @brief Return integration time_step of given class.
   * @param class_id Integer identifier of the class.
   * @return Positive integration time_step or ReactionClassification::ALWAYS_UPDATED for reactions
   *  needing to be updated after every reaction.
   */
  double time_step (int class_id) const;

  /**
   * @brief Return reactions belonging to a given class.
   * @param class_id Integer identifier of the class.
   * @return reactions List of reactions belonging to the class.
   */
  const std::list<Reaction*>& reactions (int class_id) const;

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator
  //  */
  // ReactionClassification& operator= ( const ReactionClassification& other_reaction_classification );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  virtual bool check_invariant (void) const;

  // ==================
  //  Public Constants
  // ==================
  //
  /**
   * @brief Time step value for reactions whose rate should always be updated.
   *
   * ReactionClassification::ALWAYS_UPDATED should be used to specify the time_step of
   * classes containing reaction rates that need to be updated after evry reaction. The
   * corresponding numerical value should never be used, as it could change over time.
   */
  static const double ALWAYS_UPDATED = -1;

private:

  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief Current number of classes.
   */
  int _current_size;

  /**
   * @brief List of reactions (one vector index = one class).
   */
  std::vector< std::list<Reaction*> > _reaction_classification;

  /**
   * @brief Time steps associated with every class.
   */
  std::vector<double> _time_steps;

  // =================
  //  Private Methods
  // =================
  //

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline void ReactionClassification::add_reaction_to_class (int class_id, Reaction& reaction)
{
  /** @pre class_id must be within existing identifier range. */
  REQUIRE ((class_id >= 0) && (class_id < _current_size));

  _reaction_classification [class_id].push_back (&reaction);
}

inline void ReactionClassification::add_reaction_list_to_class (int class_id, const std::list<Reaction*>& reactions)
{
  /** @pre class_id must be within existing identifier range. */
  REQUIRE ((class_id >= 0) && (class_id < _current_size));

  // copy input list at the end of existing list
  _reaction_classification [class_id].insert (_reaction_classification [class_id].end(), reactions.begin(), reactions.end());
}


inline int ReactionClassification::number_classes (void) const
{
  return _current_size;
}

inline const std::list <Reaction*>& ReactionClassification::reactions (int class_id) const
{
  /** @pre class_id must be within existing identifier range. */
  REQUIRE ((class_id >= 0) && (class_id < _current_size));

  return _reaction_classification [class_id];
}

inline double ReactionClassification::time_step (int class_id) const
{
  /** @pre class_id must be within existing identifier range. */
  REQUIRE ((class_id >= 0) && (class_id < _current_size));

  return _time_steps [class_id];
}


#endif // REACTION_CLASSIFICATION_H
