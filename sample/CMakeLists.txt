
add_library(libSample Sample.cxx SampleParameters.cxx)

find_package(ALIROOT COMPONENTS PHOS)
if(ALIROOT_FOUND)
  include_directories(SYSTEM ${ALIROOT_INCLUDES})
  message(${ALIROOT_INCLUDES})
  set(LIBS ${LIBS} ${ALIROOT_LIBRARIES})
  add_library(libExtractor ExtractorTask.cxx SampleCandidate.cxx )
  add_subdirectory(plugin)
endif(ALIROOT_FOUND)



add_subdirectory(local)