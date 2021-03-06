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
 * @file producttable.h
 * @brief Header for the ProductTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef PRODUCT_TABLE_H
#define PRODUCT_TABLE_H

// ==================
//  General Includes
// ==================
//
#include <map> // std::map
#include <set> // std::set

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "simulatorinput.h"

/**
 * @brief Class storing position to product associations.
 *
 * ProductTable stores (start,end) to ChemicalSequence associations.
 * The idea is that a sequence can be build from a template starting
 * at start position and ending at end.
 */
class ProductTable : public SimulatorInput
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param transformation_table Table used to generate products from parent.
   */
  ProductTable (const TransformationTable& transformation_table);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // ProductTable (const ProductTable& other_product_table);
  // /* @brief Assignment operator. */
  // ProductTable& operator= (const ProductTable& other_product_table);
  // /* @brief Destructor. */
  // ~ProductTable (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add a new (start,end) to Chemicalsequence association.
   * @param parent Template sequence.
   * @param first Starting position on template.
   * @param last Ending position on template.
   * @param product ChemicalSequence generated by synthesizing
   *  from template from start to end.
   */
  void add (const ChemicalSequence& parent, int first, int last,
	    ChemicalSequence& product);

  /**
   * @brief Generate child sequence from parent.
   * @param parent Template sequence.
   * @param first Starting position on template.
   * @param last Ending position on template.
   * @return Sequence generated from template.
   */
  std::string generate_child_sequence (ChemicalSequence& parent,
				       int first, int last) const;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to product given start and end position.
   * @param parent Template sequence.
   * @param first Starting position on template.
   * @param last Ending position on template.
   * @return ChemicalSequence generated by synthesizing
   *  from template from start to end.
   */
  ChemicalSequence* product (const ChemicalSequence& parent,
			     int first, int last) const;

  /**
   * @brief Accessor to products stored in the table.
   * @return Set of products stored in the table.
   */
  const std::set <ChemicalSequence*>& products (void) const;

 private:
  // ============
  //  Attributes
  // ============
  //  
  /** @brief TransformationTable giving child sequence from parent sequence. */
  const TransformationTable& _transformation_table;

  typedef std::map <int, ChemicalSequence*> EndMap;
  typedef std::map <int, EndMap> StartMap;
  typedef std::map <const ChemicalSequence*, StartMap> ParentMap;

  /** @brief Map yielding product given parent, start and end positions.*/
  ParentMap _product_map;
  /** @brief Set of products stored. */
  std::set <ChemicalSequence*> _products;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
#include "transformationtable.h"
#include "chemicalsequence.h"

inline 
ProductTable::ProductTable (const TransformationTable& transformation_table)
	     : _transformation_table (transformation_table)
{
}

inline void ProductTable::add (const ChemicalSequence& parent,
			       int first, int last, ChemicalSequence& product)
{
  _product_map [&parent][first][last] = &product;
  _products.insert (&product);
}

inline std::string
ProductTable::generate_child_sequence (ChemicalSequence& parent,
				       int first, int last) const
{
  return _transformation_table.transform 
    (parent.sequence().substr (first, last-first+1));
}

inline
ChemicalSequence* ProductTable::product (const ChemicalSequence& parent,
					 int first, int last) const
{
  ParentMap::const_iterator p_it = _product_map.find (&parent);
  if (p_it == _product_map.end()) return 0;

  StartMap::const_iterator s_it = (p_it->second).find (first);
  if (s_it == (p_it->second).end()) return 0;

  EndMap::const_iterator e_it = (s_it->second).find (last);
  if (e_it == (s_it->second).end()) return 0;

  return e_it->second;
}

inline
const std::set <ChemicalSequence*>& ProductTable::products (void) const
{
  return _products;
}

#endif // PRODUCT_TABLE_H
