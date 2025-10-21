#include "pybind11/pybind11.h"
#include "pybind11/functional.h"
#include "pybind11/stl.h"
#include "backend.hpp"

PYBIND11_MODULE(grid_shuffler, m) {
    m.doc() = "Grid shuffler algorithm for Python";

    pybind11::class_<Backend>(m, "Backend")
        .def(pybind11::init())
        .def("set_data", &Backend::setData)
        .def("get_grid", &Backend::getGrid);
}