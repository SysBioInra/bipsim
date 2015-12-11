

/**
 * @file rategroup.h
 * @brief Header for the RateGroup class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_GROUP_H
#define RATE_GROUP_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <cmath> // fabs

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "macros.h" // REQUIRE() ENSURE()
#include "randomhandler.h"
#include "ratetoken.h"

/**
 * @brief Class storing rates with similar values in a group.
 *
 * RateGroup stores rates dynamically in a group and picks random
 * rate indices according to the global rate distribution in the group.
 */
class RateGroup
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param min_rate Minimal value of rates contained in the group.
   * @param max_rate Maximal value of rates contained in the group.
   */
  RateGroup (double min_rate, double max_rate)
    : _min_rate (min_rate)
    , _max_rate (max_rate)
    , _total_rate (0)
    {
      /** @pre Min rate must be positive. */
      REQUIRE (min_rate >= 0);
      /** @pre Max rate must be strictly larger than min_rate. */
      REQUIRE (max_rate > min_rate);
    }

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // RateGroup (const RateGroup& other_rate_group);

  /**
   * @brief Destructor.
   */
  ~RateGroup (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Insert a new rate in the group.
   * @param token Token representing identity of rate to store.
   * @param rate Current value of the rate.
   */
  void insert (RateToken* token, double rate)
  {
    /** 
     * @pre rate must be greater than min_rate and smaller than
     *  max_rate provided at construction.
     */
    REQUIRE ((rate >= _min_rate) && (rate <= _max_rate));
    /** @pre rate must be strictly positive. */
    REQUIRE (rate > 0);

    token->set_position (RateKey(), _tokens.size());
    token->set_rate (RateKey(), rate);
    _tokens.push_back (token);
    _total_rate += rate;

    // total rate must be approximately correct
    ENSURE (fabs (_total_rate - _compute_total_rate()) <= 1e-10*_total_rate);
  }

  /**
   * @brief Remove a rate from the group.
   * @param token Token representing identity of rate to remove.
   */
  void remove (RateToken* token)
  {
     /** @pre Token must be inside group. */
    REQUIRE ((token->position() >= 0) && (token->position() < _tokens.size())
	     && (_tokens [token->position()] == token));

    if (_tokens.size () == 1) { _total_rate = 0; _tokens.pop_back(); return; }

    _total_rate -= token->rate();

    // overwrite element with current last element
    _tokens.back()->set_position (RateKey(), token->position());
    _tokens [token->position()] = _tokens.back();
    _tokens.pop_back();

    if (_total_rate < 0) { _total_rate = _compute_total_rate(); }

    /** @post Total rate must remain positive. */
    ENSURE (total_rate() >= 0);
    // total rate must remain approximately correct
    ENSURE (fabs (_total_rate - _compute_total_rate()) <= 1e-10*_total_rate);
  }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to total rate of elements contained in the group.
   * @return Sum of rates of elements contained in the group.
   */
  double total_rate (void) const { return _total_rate; }

  /**
   * @brief Draw random rate according to rate value distribution.
   * @return Index of the rate as marked on the RateToken provided at insertion.
   */
  int random_index (void) const
  {
    /** @pre Group size must be strictly positive. */
    REQUIRE (_tokens.size() > 0);
    /** @pre Total rate must be strictly positive. */
    REQUIRE (_total_rate > 0);
    // rejection method
    int i = RandomHandler::instance().draw_uniform (0, _tokens.size()-1);
    double u = RandomHandler::instance().draw_uniform (0.0, _max_rate);
    while (u >= _tokens [i]->rate())
      {
	i = RandomHandler::instance().draw_uniform (0, _tokens.size()-1);
	u = RandomHandler::instance().draw_uniform (0.0, _max_rate);
      }
    return _tokens [i]->index();
  }

  /**
   * @brief Group size accessor.
   * @return Current group size.
   */
  int size (void) const { return _tokens.size(); }
  
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
  //  * @brief Assignment operator.
  //  */
  // RateGroup& operator= (const RateGroup& other_rate_group);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
  //


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Minimal rate of elements contained in the group. */
  double _min_rate;

  /** @brief Maximal rate of element contained in the group. */
  double _max_rate;
  
  /** @brief Vector of tokens representing elements currently inside group. */
  std::vector <RateToken*> _tokens;

  /** @brief Estimation of the current total rate of group. */
  double _total_rate;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Sum all group rates from scratch.
   * @return Current total rate in group.
   */
  double _compute_total_rate (void) const
  {
    double result = 0;
    for (std::vector <RateToken*>::const_iterator token_it = _tokens.begin();
	 token_it != _tokens.end(); ++token_it)
      {	result += (*token_it)->rate(); }

    /** @post Result must be positive. */
    ENSURE (result >= 0);
    return result;
  }
  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // RATE_GROUP_H
