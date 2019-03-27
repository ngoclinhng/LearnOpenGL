set(UTILITY_SOURCE
  utility/create_window.cc
  utility/vertex_buffer.cc
  utility/shader.cc
)

set(PUBLIC_DEPENDENCIES "")
set(PUBLIC_DEPENDENCIES_INCLUDE_DIRS "")

set(PRIVATE_DEPENDENCIES "")
set(PRIVATE_DEPENDENCIES_INCLUDE_DIRS "")

list(APPEND PUBLIC_DEPENDENCIES glad glfw ${GLOG_LIBRARIES})
list(APPEND PRIVATE_DEPENDENCIES_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
)

include(BuildExample)

# Getting started
################################################################################
set(SECTION_NAME getting-started)

build_example(hello_world ${SECTION_NAME})
build_example(colored_triangle ${SECTION_NAME})
build_example(index_buffer ${SECTION_NAME})
build_example(uniform ${SECTION_NAME})
