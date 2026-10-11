#minimum version of cmake required
cmake_minimum_required(VERSION 3.16...3.27)

#name of the projct
project(FirstProject)

#ensuring we have Geant4 installed
find_package(Geant4 REQUIRED ui_all vis_all)

# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add the executable, use our local headers, and link it to the Geant4 libraries
add_executable(FirstProject FirstProject.cc ${sources} ${headers})
target_include_directories(FirstProject PRIVATE include)
target_link_libraries(FirstProject PRIVATE ${Geant4_LIBRARIES})

# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
set(FirstProject_SCRIPTS
  init_vis.mac
  vis.mac
  )

foreach(_script ${FirstProject_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
