

/**
 * @file complexationimp.h
 * @brief Header for the ComplexationImp, FreeComplexationImp, BoundComplexationImp classes.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef COMPLEXATION_IMP_H
#define COMPLEXATION_IMP_H

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
 * @brief Abstract class defining interface of Complexation implementations.
 *
 * ComplexationImp defines an interface for varying implementations of the
 * Complexation reaction.
 */
class ComplexationImp 
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param component_a Reference to the first chemical involved.
   * @param component_b Reference to the second chemical involved.
   * @param complex Reference to the complex.
   */
  ComplexationImp (Chemical& component_a, Chemical& component_b,
		   Chemical& complex)
    : _component_a (component_a)
    , _component_b (component_b)
    , _complex (complex) {}

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // ComplexationImp (const ComplexationImp& other_class_name);
  // /* @brief Assignment operator. */
  // ComplexationImp& operator= (const ComplexationImp& other_class_name);
  // /* @brief Destructor. */
  // ~ComplexationImp (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform forward Complexation reaction.
   */
  virtual void perform_forward (void) = 0;

  /**
   * @brief Perform backward Complexation reaction.
   */
  virtual void perform_backward (void) = 0;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /** 
   * @brief Accessor to first component of reaction. 
   * @return Reference to first component of reaction.
   */
  Chemical& component_a (void) const { return _component_a; }

  /** 
   * @brief Accessor to second component of reaction. 
   * @return Reference to second component of reaction.
   */
  Chemical& component_b (void) const { return _component_b; }

  /** 
   * @brief Accessor to complex of reaction. 
   * @return Reference to complex of reaction.
   */
  Chemical& complex (void) const { return _complex; }

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Reference to first component of the complex. */
  Chemical& _component_a;

  /** @brief Reference to second component of the complex. */
  Chemical& _component_b;

  /** @brief Reference to the complex. */
  Chemical& _complex;

  // =================
  //  Private Methods
  // =================
  //
};

/**
 * @brief Class implementing Complexation for free chemicals.
 *
 * FreeComplexationImp implements forward and backward reactions involving
 * only free chemicals or chemical sequences.
 */
class FreeComplexationImp : public ComplexationImp
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param component_a Reference to a free component.
   * @param component_b Reference to a free component.
   * @param complex Reference to a free complex.
   */
  FreeComplexationImp (Chemical& component_a, Chemical& component_b,
		       Chemical& complex);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // ComplexationImp (const ComplexationImp& other_class_name);
  // /* @brief Assignment operator. */
  // ComplexationImp& operator= (const ComplexationImp& other_class_name);
  // /* @brief Destructor. */
  // ~ComplexationImp (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from ComplexationImp
  void perform_forward (void);
  void perform_backward (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // ============
  //  Attributes
  // ============
  //

  // =================
  //  Private Methods
  // =================
  //
};

/**
 * @brief Class implementing Complexation for bound chemicals.
 *
 * BoundComplexationImp implements forward and backward reactions involving
 * two bound chemicals and other chemicals.
 */
class BoundComplexationImp : public ComplexationImp
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param component_a Reference to a bound component.
   * @param component_b Reference to a free component.
   * @param complex Reference to a bound complox.
   */
  BoundComplexationImp (BoundChemical& component_a, Chemical& component_b,
			BoundChemical& complex);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // ComplexationImp (const ComplexationImp& other_class_name);
  // /* @brief Assignment operator. */
  // ComplexationImp& operator= (const ComplexationImp& other_class_name);
  // /* @brief Destructor. */
  // ~ComplexationImp (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from ComplexationImp
  void perform_forward (void);
  void perform_backward (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // ============
  //  Attributes
  // ============
  //

  // =================
  //  Private Methods
  // =================
  //
  /** 
   * @brief Accessor to free component of reaction. 
   * @return Reference to free component of reaction.
   */
  Chemical& _free_component (void) const;

  /** 
   * @brief Accessor to bound component of reaction. 
   * @return Reference to bound component of reaction.
   */
  BoundChemical& _bound_component (void) const;

  /** 
   * @brief Accessor to bound complex. 
   * @return Reference to complex as a BoundChemical.
   */
  BoundChemical& _bound_complex (void) const;
};


// ======================
//  Inline declarations
// ======================
//
#include "boundchemical.h"

inline Chemical& BoundComplexationImp::_free_component (void) const 
{ 
  return component_b();
}

inline BoundChemical& BoundComplexationImp::_bound_component (void) const
{ 
  return static_cast <BoundChemical&> (component_a()); 
}

inline BoundChemical& BoundComplexationImp::_bound_complex (void) const
{ 
  return static_cast <BoundChemical&> (complex()); 
}

#endif // COMPLEXATION_IMP_H
