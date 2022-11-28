project(cgal_viewer)
cmake_minimum_required(VERSION 3.1...3.20)

find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)

if(CGAL_Qt5_FOUND)
    add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)
endif()

find_package(Boost REQUIRED)

if(NOT Boost_FOUND)
    message(STATUS "This project requires the Boost library.")
    return()
endif()

create_single_source_cgal_program("cgal_viewer.cpp")
create_single_source_cgal_program("cgal_grid_simplifier.cpp")
create_single_source_cgal_program("cgal_wlop_simplifier.cpp")

if(CGAL_Qt5_FOUND)
    target_link_libraries(cgal_viewer PUBLIC CGAL::CGAL_Qt5)
    target_link_libraries(cgal_grid_simplifier PUBLIC CGAL::CGAL_Qt5)
    target_link_libraries(cgal_wlop_simplifier PUBLIC CGAL::CGAL_Qt5)
endif()
