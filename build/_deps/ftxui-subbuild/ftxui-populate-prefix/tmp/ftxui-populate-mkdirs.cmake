# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/SpaceInvaders/build/_deps/ftxui-src")
  file(MAKE_DIRECTORY "C:/SpaceInvaders/build/_deps/ftxui-src")
endif()
file(MAKE_DIRECTORY
  "C:/SpaceInvaders/build/_deps/ftxui-build"
  "C:/SpaceInvaders/build/_deps/ftxui-subbuild/ftxui-populate-prefix"
  "C:/SpaceInvaders/build/_deps/ftxui-subbuild/ftxui-populate-prefix/tmp"
  "C:/SpaceInvaders/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp"
  "C:/SpaceInvaders/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src"
  "C:/SpaceInvaders/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/SpaceInvaders/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/SpaceInvaders/build/_deps/ftxui-subbuild/ftxui-populate-prefix/src/ftxui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
