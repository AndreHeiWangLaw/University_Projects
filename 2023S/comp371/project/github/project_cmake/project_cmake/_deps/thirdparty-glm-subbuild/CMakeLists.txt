# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.27.0-rc5)

# Reject any attempt to use a toolchain file. We must not use one because
# we could be downloading it here. If the CMAKE_TOOLCHAIN_FILE environment
# variable is set, the cache variable will have been initialized from it.
unset(CMAKE_TOOLCHAIN_FILE CACHE)
unset(ENV{CMAKE_TOOLCHAIN_FILE})

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(thirdparty-glm-populate NONE)



include(ExternalProject)
ExternalProject_Add(thirdparty-glm-populate
                     "UPDATE_DISCONNECTED" "True" "DOWNLOAD_EXTRACT_TIMESTAMP" "TRUE" "EXTERNALPROJECT_INTERNAL_ARGUMENT_SEPARATOR" "URL" "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/code/thirdparty/glm-0.9.9.8.zip"
                    SOURCE_DIR          "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/_deps/thirdparty-glm-src"
                    BINARY_DIR          "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/_deps/thirdparty-glm-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


