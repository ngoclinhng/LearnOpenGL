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

  set(VERTEX_SHADER_FILE_PATH ${SOURCE_DIR}/${TARGET_NAME}.vertex)
  set(FRAGMENT_SHADER_FILE_PATH ${SOURCE_DIR}/${TARGET_NAME}.fragment)

  get_filename_component(VERTEX_SHADER_FILE_NAME
    ${VERTEX_SHADER_FILE_PATH} NAME)
  get_filename_component(FRAGMENT_SHADER_FILE_NAME
    ${FRAGMENT_SHADER_FILE_PATH} NAME)

  add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy ${VERTEX_SHADER_FILE_PATH} $<TARGET_FILE_DIR:${TARGET_NAME}>
    COMMENT "Copying file ${VERTEX_SHADER_FILE_PATH} to ${DESTINATION_DIR}"
    VERBATIM)
    
  add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy ${FRAGMENT_SHADER_FILE_PATH} $<TARGET_FILE_DIR:${TARGET_NAME}>
    COMMENT "Copying file ${FRAGMENT_SHADER_FILE_PATH} to ${DESTINATION_DIR}"
    VERBATIM)
endfunction (BUILD_EXAMPLE)

# Getting started
################################################################################
set(SECTION_NAME getting-started)

build_example(hello_world ${SECTION_NAME})
build_example(colored_triangle ${SECTION_NAME})
build_example(index_buffer ${SECTION_NAME})
build_example(uniform ${SECTION_NAME})
