#include <pybind11/pybind11.h>
#include "../include/uchronia_pb_exports.h"

int add(int i, int j) {
    return i + j;
}

namespace py = pybind11;

PYBIND11_MODULE(uchronia_pb, m) {
    m.doc() = R"pbdoc(
        Pybind11 uchronia bindings plugin
        -----------------------

        .. currentmodule:: uchronia_pb

        .. autosummary::
           :toctree: _generate

           CreateSingleTimeSeriesDataFromStruct
           etc.
    )pbdoc";
	m.def("GetEnsembleDatasetDataIdentifiers", &GetEnsembleDatasetDataIdentifiers, R"pbdoc(GetEnsembleDatasetDataIdentifiers)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
