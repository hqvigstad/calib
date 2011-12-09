
  
# AliRoot
find_file(ALIROOT_INCLUDES NAMES AliRun.h PATHS $ENV{ALICE_ROOT}/include/)
if(NOT ALIROOT_INCLUDES)
  message(FATAL_ERROR "could not find AliRun.h in $ENV{ALICE_ROOT}/include/")
  endif(NOT ALIROOT_INCLUDES)
include_directories(SYSTEM $ENV{ALICE_ROOT}/include/)

#AliRoot-PHOS
find_file(ALIROOT_PHOS_INCLUDES NAMES AliPHOSReconstructor.h  PATHS $ENV{ALICE_ROOT}/PHOS/)
if(NOT ALIROOT_PHOS_INCLUDES)
  message(FATAL_ERROR "could not find AliPHOSReconstructor.h in $ENV{ALICE_ROOT}/PHOS/")
  endif(NOT ALIROOT_PHOS_INCLUDES)
include_directories(SYSTEM $ENV{ALICE_ROOT}/PHOS/)
add_library(libExtractor SampleParameters.cxx ExtractorTask.cxx)

add_library(libSampleParameters Sample.cpp SampleParameters.cxx)

# find_package(ALIROOT COMPONENTS STEER PHOS)
# if(ALIROOT_FOUND)
#   add_library(libExtractor SampleParameters.cxx ExtractorTask.cxx)
# endif(ALIROOT_FOUND)

add_subdirectory(plugin)
