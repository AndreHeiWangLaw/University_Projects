# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/_deps/thirdparty-glew-src"
  "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/_deps/thirdparty-glew-build"
  "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/_deps/thirdparty-glew-subbuild/thirdparty-glew-populate-prefix"
  "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/_deps/thirdparty-glew-subbuild/thirdparty-glew-populate-prefix/tmp"
  "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/_deps/thirdparty-glew-subbuild/thirdparty-glew-populate-prefix/src/thirdparty-glew-populate-stamp"
  "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/_deps/thirdparty-glew-subbuild/thirdparty-glew-populate-prefix/src"
  "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/_deps/thirdparty-glew-subbuild/thirdparty-glew-populate-prefix/src/thirdparty-glew-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/_deps/thirdparty-glew-subbuild/thirdparty-glew-populate-prefix/src/thirdparty-glew-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Concordia/2023 Summer/comp371/project/github/project_cmake/project_cmake/_deps/thirdparty-glew-subbuild/thirdparty-glew-populate-prefix/src/thirdparty-glew-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
