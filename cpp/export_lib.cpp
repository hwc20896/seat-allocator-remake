#include "pybind11/pybind11.h"
#include "pybind11/functional.h"
#include "pybind11/stl.h"
#include "backend.hpp"

PYBIND11_MODULE(grid_shuffler, m) {
    m.doc() = "Grid shuffler algorithm for Python";

    pybind11::class_<Backend>(m, "Backend")
        .def(pybind11::init())
        .def("set_data", &Backend::setData)
        .def("get_grid", &Backend::getGrid)
        .def("shuffle", &Backend::shuffle)
        .def("forbid_side_by_side_local", &Backend::forbidSideBySideLocal);

    auto sub_namespace = m.def_submodule("RuleFactory");

    sub_namespace.def("no_staying", &no_staying)
                 .def("forbid_digit_in_row", &forbid_digit_in_row);
}