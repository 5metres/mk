#include "PyCaller.h"
#include <pybind11/embed.h>
#include <iostream>
namespace py = pybind11;

std::string process_image(const std::string &imagePath) {
    std::cout << "Entering process_image with path: " << imagePath << std::endl;

    try {
        py::module image_processor = py::module::import("image_processor");
        py::object my_function = image_processor.attr("process_image");
        py::object result = my_function(imagePath);
        std::cout << "Python processing completed." << std::endl;
        return result.cast<std::string>();
    } catch (const py::error_already_set &e) {
        std::cerr << "Python error: " << e.what() << std::endl;
        return "error";
    }
}
