# --------------------------------------------------------------------------------
#                         Documentation (no change needed).
# --------------------------------------------------------------------------------
# Add a make target 'doc' to generate API documentation with Doxygen.
# You should set options to your liking in the file 'Doxyfile.in'.
find_package(Doxygen)

# if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    add_custom_target(doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT} &> doxygen.log
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "${BoldMagenta}Generating API documentation with Doxygen. open (html/index.html) to see the docs.${ColourReset}"
        VERBATIM)
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif(DOXYGEN_FOUND)
# endif()

# find_program(SPHINX_EXECUTABLE
#   NAMES sphinx-build
#   DOC "Path to sphinx-build executable")

# include(FindPackageHandleStandardArgs)

# # Handle standard arguments to find_package like REQUIRED and QUIET
# find_package_handle_standard_args(Sphinx
#   "Failed to find sphinx-build executable"
#   SPHINX_EXECUTABLE)


# set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/docs)
# set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/docs)

# add_custom_target(Sphinx ALL
#                   COMMAND ${SPHINX_EXECUTABLE} -b html -Dbreathe_projects.CatCutifier=${DOXYGEN_OUTPUT_DIR}/xml
#                   ${SPHINX_SOURCE} ${SPHINX_BUILD}
#                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
#                   ${DOXYGEN_INDEX_FILE}
#                   MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
#                   COMMENT "Generating documentation with Sphinx")
