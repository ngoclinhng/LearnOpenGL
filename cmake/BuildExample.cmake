# Helper function to copy shaders associated with each target
function (COPY_SHADERS_FOR_TARGET TARGET_NAME SECTION_NAME)
  # Shaders source directory
  set(SHADERS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SECTION_NAME})
  if (NOT EXISTS "${SHADERS_SOURCE_DIR}")
    message(FATAL_ERROR "Bug: directory does not exist: ${SHADERS_SOURCE_DIR}")
  endif()
  if (EXISTS "${SHADERS_SOURCE_DIR}" AND
      NOT IS_DIRECTORY "${SHADERS_SOURCE_DIR}")
    message(FATAL_ERROR "Bug: ${SHADERS_SOURCE_DIR} exists, 
            but not a directory")
  endif()

  # Shaders destination directory
  set(SHADERS_DESTINATION_DIR
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SECTION_NAME}/shaders
  )
  if (NOT EXISTS "${SHADERS_DESTINATION_DIR}")
    message(STATUS "Creating shaders directory for target ${TARGET_NAME}")
    file(MAKE_DIRECTORY "${SHADERS_DESTINATION_DIR}")
  endif()
  if (EXISTS "${SHADERS_DESTINATION_DIR}" AND
     NOT IS_DIRECTORY "${SHADERS_DESTINATION_DIR}")
    message(FATAL_ERROR "Bug: ${SHADERS_DESTINATION_DIR} exists, but not
      a directory"
    )
  endif()
  
  set(VERTEX_SHADER_FILE_PATH ${SHADERS_SOURCE_DIR}/${TARGET_NAME}.vertex)
  set(FRAGMENT_SHADER_FILE_PATH ${SHADERS_SOURCE_DIR}/${TARGET_NAME}.fragment)

  if (NOT EXISTS "${VERTEX_SHADER_FILE_PATH}")
    message(FATAL_ERROR "Bug: File does not exist: ${VERTEX_SHADER_FILE_PATH}")
  endif()
  if (NOT EXISTS "${FRAGMENT_SHADER_FILE_PATH}")
    message(FATAL_ERROR "Bug: File does not exist: ${FRAGMENT_SHADER_FILE_PATH}")
  endif()

  get_filename_component(VERTEX_SHADER_FILE_NAME
    ${VERTEX_SHADER_FILE_PATH} NAME)
  get_filename_component(FRAGMENT_SHADER_FILE_NAME
    ${FRAGMENT_SHADER_FILE_PATH} NAME)

  add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy ${VERTEX_SHADER_FILE_PATH} ${SHADERS_DESTINATION_DIR}
    COMMENT "Copying file ${VERTEX_SHADER_FILE_PATH} to ${SHADERS_DESTINATION_DIR}"
    VERBATIM)
    
  add_custom_command( TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy ${FRAGMENT_SHADER_FILE_PATH} ${SHADERS_DESTINATION_DIR}
    COMMENT "Copying file ${FRAGMENT_SHADER_FILE_PATH} to ${SHADERS_DESTINATION_DIR}"
    VERBATIM)
endfunction()

# Helper function to build example
function (BUILD_EXAMPLE TARGET_NAME SECTION_NAME)
  set(SECTION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${SECTION_NAME})

  # Make sure that the section does actually exist.
  if (NOT EXISTS "${SECTION_DIR}")
    message(FATAL_ERROR "Bug: directory does not exist: ${SECTION_DIR}")
  endif()
  if (EXISTS "${SECTION_DIR}" AND
      NOT IS_DIRECTORY "${SECTION_DIR}")
    message(FATAL_ERROR "Bug: ${SECTION_DIR} exists, but not a directory")
  endif()

  # Make sure that target name actually exists.
  set(FULL_PATH_TO_TARGET_FILE ${SECTION_DIR}/${TARGET_NAME}.cc)
  if (NOT EXISTS "${FULL_PATH_TO_TARGET_FILE}")
    message(FATAL_ERROR "Bug: File does not exist: ${FULL_PATH_TO_TARGET_FILE}")
  endif()

  add_executable(${TARGET_NAME}
    ${SECTION_NAME}/${TARGET_NAME}.cc
    ${UTILITY_SOURCE}
  )
  target_link_libraries(${TARGET_NAME}
    PUBLIC ${PUBLIC_DEPENDENCIES}
    PRIVATE ${PRIVATE_DEPENDENCIES}
  )
  target_include_directories(${TARGET_NAME}
    PUBLIC ${PUBLIC_DEPENDENCIES_INCLUDE_DIRS}
    PRIVATE ${PRIVATE_DEPENDENCIES_INCLUDE_DIRS}
  )

  # Custom output directory for this target
  set_target_properties(${TARGET_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SECTION_NAME}
  )

  # We need this in our target source code in order to load shaders program
  # properly. This is a bit odd but for now i don't know any better way to
  # handle this.
  target_compile_definitions(${TARGET_NAME}
    PUBLIC
    SHADERS_DESTINATION_DIR=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SECTION_NAME}/shaders/
  )

  # Copy shader files.
  copy_shaders_for_target(${TARGET_NAME} ${SECTION_NAME})
  
endfunction()