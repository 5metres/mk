cmake_minimum_required(VERSION 3.10)
project(ImageBackend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find pybind11
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

include_directories(${PYTHON_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src)

# Add executable
add_executable(backend
    src/main.cpp 
    src/Server.cpp 
    src/PyCaller.cpp
)

# Link libraries
if(WIN32)
    target_link_libraries(backend PRIVATE pybind11::embed ${Boost_LIBRARIES} ${PYTHON_LIBRARY} Threads::Threads Ws2_32)
else()
    target_link_libraries(backend PRIVATE pybind11::embed ${Boost_LIBRARIES} ${PYTHON_LIBRARY} Threads::Threads)
endif()

set(PYINSTALLER_OUTPUT ${CMAKE_SOURCE_DIR}/dist/image_processor.exe)
file(COPY ${PYINSTALLER_OUTPUT} DESTINATION ${CMAKE_BINARY_DIR})