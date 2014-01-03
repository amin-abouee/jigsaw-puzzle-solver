# relase binary folder
set(CMAKE_RELEASE_DIR "../build/release")
# debug binary folder
set(CMAKE_DEBUG_DIR "../build/debug")

SET(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake/ )
find_package(OpenCV REQUIRED)

include_directories(../include)

add_executable (jigsaw_solver
                main.cpp
                Chromosome.cpp
                GeneticAlgorithm.cpp
                Piece.cpp
                )

target_link_libraries (jigsaw_solver ${OpenCV_LIBS})

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	set_target_properties(jigsaw_solver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_DEBUG_DIR})
	set_target_properties(jigsaw_solver PROPERTIES OUTPUT_NAME jigsaw_solver${BUILD_PREFIX})
else()
	set_target_properties(jigsaw_solver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RELEASE_DIR})
endif()
