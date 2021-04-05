

class Species(object):
    """
    Class used to store simple chemical species.

    Attributes
    ----------
    id: 
        species id.
    concentration:
        initial concentration.
    """

    def __init__(self, id_, concentration):
        """
        Build from ID and concentration.

        Parameters
        ----------
        id_: str
            species id.
        concentration: int
            initial concentration

        """
        self.id = id_
        self.concentration = concentration
        
    def to_bipsim(self):
        result = "FreeChemical {} {}".format(self.id, self.concentration)
        return result
    
        
class Reaction(object):
    """
    Class used to store simple chemical reactions.

    Attributes
    ----------
    reactants: dict 
        List of reactants with stoichiometry.
    products: dict 
        List of products with stoichiometry.
    kon: float
        Forward kinetic constant.
    koff: float 
        Backward kinetic constant (0 if irreversible).
    """

    def __init__(self):
        """
        Empty constructor.
        """
        self.reactants = {}
        self.products = {}
        self.kon = 1
        self.koff = 0
        
    def add_reactant(self, species_id, stoichiometry):
        """
        Add reactant.

        Parameters
        ----------
        species_id: str
            Species ID.
        stoichiometry: int
            Stoichiometry.
        """
        if species_id in self.reactants:
            self.reactants[species_id] += stoichiometry
        else:
            self.reactants[species_id] = stoichiometry
        
    def add_product(self, species_id, stoichiometry):
        """
        Add product.

        Parameters
        ----------
        species_id: str
            Species ID.
        stoichiometry: int
            Stoichiometry.
        """
        if species_id in self.products:
            self.products[species_id] += stoichiometry
        else:
            self.products[species_id] = stoichiometry
        
    def to_bipsim(self):
        reactants = " ".join("{} -{}".format(k, v) for k,v in self.reactants.items())
        products = " ".join("{} {}".format(k, v) for k,v in self.products.items())
        rates = "rates {} {}".format(self.kon, self.koff)
        result = " ".join(("ChemicalReaction", reactants, products, rates))
        return result