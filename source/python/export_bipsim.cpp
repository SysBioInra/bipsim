
#include <boost/python.hpp>
using namespace boost::python;

#include "bipsim.hpp"


BOOST_PYTHON_MODULE(bipsim)
{
    class_<BipSim, boost::noncopyable>(
      "BipSim", init<const std::string&>()
    )
      .def("perform_next_reaction", &BipSim::perform_next_reaction)
      .def("skip", &BipSim::skip)
      .def("set_chemicals", &BipSim::set_chemicals)
      .def("set_chemical_number", &BipSim::set_chemical_number)
      .def("reset_ignored_variation", &BipSim::reset_ignored_variation)
      .def("simulation_time", &BipSim::simulation_time)
      .def("next_reaction_time", &BipSim::next_reaction_time)
      .def("volume", &BipSim::volume)
      .def("chemical_number", &BipSim::chemical_number)
      .def("chemical_ignored_variation", &BipSim::chemical_ignored_variation)
    ;
}
