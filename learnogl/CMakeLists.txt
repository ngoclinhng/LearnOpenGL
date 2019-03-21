# Helper function to build example
#
#   TARGET_NAME: the name of target to be build, e.g hello_world
#
#   SECTION: The section contains the file ${TARGET_NAME}.cc,
#            e.g getting-started
function (BUILD_EXAMPLE TARGET_NAME SECTION)

  set(SECTION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SECTION})
  set(MAIN_TARGET_FILE ${SECTION_DIR}/${TARGET_NAME}.cc)

  # Make sure that the SECTION exists.
  if (NOT EXISTS "${SECTION_DIR}")
    message(FATAL_ERROR "Directory does not exist: ${SECTION_DIR}")
  endif()
  if (EXISTS "${SECTION_DIR}" AND
    NOT IS_DIRECTORY "${SECTION_DIR}")
    message(FATAL_ERROR "${SECTION_DIR} exists, but not a directory")
  endif()

  # Make sure that the main target file exists.
  if (NOT EXISTS "${MAIN_TARGET_FILE}")
    message(FATAL_ERROR "File does not exist: ${MAIN_TARGET_FILE}")
  endif()

  add_executable(${TARGET_NAME}
    # The main example file
    ${SECTION}/${TARGET_NAME}.cc

    # Utilities
    utility/read_file_content.cc
    utility/build_program_from_shaders.cc
    utility/create_window.cc
    utility/compile_shader_from_single_source_string.cc
    utility/utility.h)

  # Include directories
  target_include_directories(${TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLOG_INCLUDE_DIRS})
  target_link_libraries(${TARGET_NAME} glfw glad ${GLOG_LIBRARIES})

  # Custom output directory for this TARGET
  set_target_properties( ${TARGET_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SECTION})

  # IMPORTTANT.
  # This shit is a bit tricky because we need absolute path to each shader file
  # in order to properly load them into our program.
  target_compile_definitions(${TARGET_NAME}
    PUBLIC TARGET_OUTPUT_BINARY_DIR=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SECTION})

  # Copy shader files
  ################################################################

  set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SECTION})
  set(DESTINATION_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SECTION})

  file(GLOB DATA_FILES
    # Insert more file extensions here
    ${SOURCE_DIR}/*.vertex
    ${SOURCE_DIR}/*.fragment)

  foreach (FULL_FILE_PATH ${DATA_FILES})
    message(STATUS "Copying file ${FULL_FILE_NAME} to ${DESTINATION_DIR}")
    get_filename_component(FILE_NAME ${FULL_FILE_PATH} NAME)
    add_custom_command(
      TARGET ${TARGET_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E
      copy ${FULL_FILE_PATH} ${DESTINATION_DIR}/${FILE_NAME})
  endforeach()
endfunction (BUILD_EXAMPLE)

# Getting started
################################################################################
build_example(hello_world getting-started)

