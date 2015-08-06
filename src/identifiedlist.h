

/**
 * @file identifiedlist.h
 * @brief Header for the IdentifiedList class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef IDENTIFIEDLIST_H
#define IDENTIFIEDLIST_H

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <map> // std::map

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief A map that associates a name with an integer identifier.
 */
typedef std::map<std::string,int> IdMap;

/**
 * @brief Associates unique integer identifers to string elements.
 *
 */
class IdentifiedList
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  IdentifiedList ( void )
    : _last_attributed_id ( 0 ) {}
   

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // IdentifiedList (IdentifiedList& other_identified_lit);

  /**
   * @brief Destructor
   */
  virtual ~IdentifiedList (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create new integer identifier for the given element if not already
   *  listed.
   * @param element_name Name of the element.
   */
  void create_id ( std::string element_name );

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return identifier corresponding to element name.
   * @return Integer element identifier (IdentifiedList::NOT_FOUND if element is
   *  unknown).
   * @param element_name Name of the element.   
   */
  int id ( std::string element_name ) const;

  /**
   * @brief Check whether an element is identified in the list.
   * @return True if the element is already listed.
   * @param element_name Name of the element to look for.   
   */
  bool exists ( std::string element_name ) const;

  /**
   * @brief Return name corresponding to element identifier.
   * @return String containing element name (empty if id is unknown).
   * @param element_id Identifier of the element to look for.   
   */
  std::string name (int element_id) const;
  

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
  // IdentifiedList& operator= (IdentifiedList& other_identified_list);


  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;

  // ==================
  //  Public Constants
  // ==================
  //
  /**
   * @brief Identifier associated with unknown elements.
   */
  static const int NOT_FOUND = 0;

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief The map that associates names with integer identifiers. */
  IdMap _identifiers;
  /** @brief The last integer identifiers attributed to an element. */
  int _last_attributed_id;

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//


#endif // IDENTIFIEDLIST_H
