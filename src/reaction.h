

/**
 * @file reaction.h
 * @brief Header for the Reaction class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef REACTION_H
#define REACTION_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class that represent all possible reactions. 
 *
 * All reactions that happen in the cell inherit this class. It can be used to
 * access reaction rates and perform reactions.
 */
class Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  Reaction (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Reaction (Reaction& other_reaction);

  /**
   * @brief Destructor
   */
  virtual ~Reaction ( void );

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update chemical quantities according to the forward reaction.
   */
  virtual void perform_forward ( void ) = 0;

  /**
   * @brief Update chemical quantities according to the backward reaction.
   */
  virtual void perform_backward ( void ) = 0;

  /**
   * @return Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const = 0;


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns the forward reaction rate.
   * @return The forward reaction rate.
   */
  virtual double forward_rate ( void ) const = 0;


  /**
   * @brief Returns the backward reaction rate.
   * @return The backward reaction rate.
   */
  virtual double backward_rate ( void ) const = 0;


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
  // Reaction& operator= (Reaction& other_reaction);
  
  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param reaction Reference to the reaction whose information should be written.
   */
  friend std::ostream& operator<< (std::ostream& output, const Reaction& reaction);
  
  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant( void ) const;


protected:

  // ============
  //  Attributes
  // ============
  //

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // REACTION_H
