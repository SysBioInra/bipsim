

/**
 * @file release.cpp
 * @brief Implementation of the DoubleStrandRecruitment class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cerr

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // REQUIRE
#include "doublestrandrecruitment.h"
#include "freechemical.h"
#include "boundchemical.h"
#include "chemicalsequence.h"
#include "boundunit.h"
#include "boundunitfactory.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
DoubleStrandRecruitment::DoubleStrandRecruitment (BoundChemical& recruiter,
						  FreeChemical& recruit,
						  BoundChemical& bound_recruit,
						  double rate)
  : _recruiter (recruiter)
  , _recruit (recruit)
  , _bound_recruit (bound_recruit)
  , _rate (rate)
  , _volume_constant (rate)
{
  /** @pre Rate must be strictly positive. */
  REQUIRE (rate > 0);

  _reactants.push_back (&_recruiter);
  _reactants.push_back (&_recruit);

  _products.push_back (&_bound_recruit);
}

// Not needed for this class (use of compiler generated versions)
// DoubleStrandRecruitment::DoubleStrandRecruitment (const DoubleStrandRecruitment& other);
// DoubleStrandRecruitment& DoubleStrandRecruitment::operator= (const DoubleStrandRecruitment& other);
// DoubleStrandRecruitment::~DoubleStrandRecruitment (void);

// ===========================
//  Public Methods - Commands
// ===========================
//

// ============================
//  Public Methods - Accessors
// ============================
//
bool DoubleStrandRecruitment::is_reaction_possible (void) const
{
  return ((_recruiter.number() > 0) && (_recruit.number() > 0));
}

// ===================
//  Protected Methods
// ===================
//
void DoubleStrandRecruitment::do_reaction (void)

{
  /** @pre There must be enough reactants to perform reaction. */
  REQUIRE (is_reaction_possible());

  BoundUnit& unit = _recruiter.random_unit();
  /** @pre Chemical performing reaction must be bound to an extending strand. */
  REQUIRE (unit.strand() != BoundUnit::NO_STRAND);
  ChemicalSequence& appariated_strand = *(unit.location().appariated_strand());
  int first = unit.location().complementary (unit.last());
  int last = unit.location().complementary (unit.first());
  if (appariated_strand.number_available_sites (first, last) > 0)
    {
      _recruit.remove (1);
      int reading_frame = first + (unit.reading_frame() - unit.first());
      BoundUnit& recruited_unit = BoundUnitFactory::instance().
	create (appariated_strand, first, last, reading_frame);
      recruited_unit.set_strand (unit.strand());
      _bound_recruit.add (recruited_unit);
      appariated_strand.bind_unit (first, last);
    }
}

double DoubleStrandRecruitment::compute_rate (void) const
{
  return _volume_constant * _recruiter.number() * _recruit.number();
}

void DoubleStrandRecruitment::print (std::ostream& output) const
{
  output << "DoubleStrandRecruitment reaction.";
}


// =================
//  Private Methods
// =================
//
