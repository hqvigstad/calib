# -*- mode: cmake -*-
# author: Henrik Qvigstad <henrik.qvigstad@cern.ch>
#
# apptional components
#
# If ALIROOT is found, the following is defined:
#  ALIROOT_FOUND - true if AliROOT is found
#  ALIROOT_INCLUDES - The AliROOT include directories (you may use the alias ALIROOT_INCLUDE_DIRS)
#  ALIROOT_LIBS - the AliROOT library statements (you may use the alias ALIROOT_LIBRARIES)
#  ( ALIROOT_DEFINITION - not functional!  )



# find root-config
Find_program(ROOT_CONFIG root-config )

if (${ROOT_CONFIG} MATCHES "ROOT_CONFIG-NOTFOUND")
  if(ALIROOT_FIND_REQUIRED)
    message(FATAL_ERROR "Install Root and make sure it (root-config) is in PATH")
  elseif(ALIROOT_FIND_REQUIRED)
    message(WARNING "Install Root and make sure it (root-config) is in PATH")
  endif(ROOT_FIND_REQUIRED)
endif (${ROOT_CONFIG} MATCHES "ROOT_CONFIG-NOTFOUND")


# determin target
execute_process(
    COMMAND root-config --arch
    OUTPUT_VARIABLE ALICE_TARGET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
# libs should be found in
set(ALICE_LIB_DIR $ENV{ALICE_ROOT}/lib/tgt_${ALICE_TARGET})


# check to see if libESD.so is built
find_library(ALIROOT_FOUND ESD ${ALICE_LIB_DIR})
if(NOT ALIROOT_FOUND)
  message(STATUS "AliROOT not found")
  if(ALIROOT_FIND_REQUIRED)
    message(FATAL_ERROR "AliROOT Required!")
  endif(ALIROOT_FIND_REQUIRED)
elseif(NOT ALIROOT_FOUND)
  message(STATUS "libESD found, assuming that complete aliroot build is found in \$ALICE_ROOT")
endif(NOT ALIROOT_FOUND)




# Main AliROOT (should maybe be split into sub-components like the detector modules)
set(ALIROOT_INCLUDE "$ENV{ALICE_ROOT}/include/")
set(ALIROOT_LIB "-L${ALICE_LIB_DIR} -lmicrocern -llhapdf -lpythia6")
set(ALIROOT_LIB "${ALIROOT_LIBS} -lSTEERBase -lESD -lCDB -lRAWDatabase -lRAWDatarec -lAOD -lANALYSIS")
set(ALIROOT_LIB "${ALIROOT_LIBS} -lSTEER -lRAWDatasim -lFASTSIM -lEVGEN -lAliPythia6 -lSTAT")
set(ALIROOT_LIB "${ALIROOT_LIBS} -lhijing -lTHijing -lSTRUCT")


# *** Components ***

# set ALIROOT_USE_*COMPONENT* variables
foreach( component ${ALIROOT_FIND_COMPONENTS} )
  set( ALIROOT_USE_${component} TRUE )
endforeach( component )


# PHOS
if(ALIROOT_USE_PHOS)
  find_file(ALIROOT_PHOS_FOUND NAMES AliPHOSReconstructor.h  PATHS $ENV{ALICE_ROOT}/PHOS/)
  if(ALIROOT_PHOS_FOUND)
    # includes
    set(ALIROOT_PHOS_INCLUDE $ENV{ALICE_ROOT}/PHOS/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_PHOS_INCLUDE})
    # libs
    set(ALIROOT_PHOS_LIB "-L${ALIROOT_LIB_DIR} -lPHOSUtils -lPHOSbase -lPHOSsim -lPHOSrec")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_PHOS_LIB})
  elseif(ALIROOT_PHOS_FOUND)
    message(STATUS "ALIROOT_PHOS not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliPHOSReconstructor.h in $ENV{ALICE_ROOT}/PHOS/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_PHOS_FOUND)
endif(ALIROOT_USE_PHOS)


set(ALIROOT_INCLUDES ${ALIROOT_INCLUDE})
set(ALIROOT_INCLUDE_DIRS ${ALIROOT_INCLUDE})
set(ALIROOT_LIBS ${ALIROOT_LIB})
set(ALIROOT_LIBRARIES ${ALIROOT_LIB})



# macro(FINDLIBS RESULTBOOL PATHTOLOOK ) # ARGN (libnames)
#   set(LIBNAMELIST ${ARGN})
#   foreach(libname ${LIBNAMELIST})
#     find_library(FOUND_${libname} ${libname} ${PATHTOLOOK} )
# #      message("finding  ${libname} in ${PATHTOLOOK}")
# #      message("result: " ${FOUND_${LIBPATH}})
#     set(LIBPATH ${libname})
#     if(NOT FOUND_${libname})
#       message(STATUS "WARNING: lib ${libname} not found in ${PATHTOLOOK}")
#       return()
#     endif(NOT FOUND_${libname})
#   endforeach(libname ${LIBNAMELIST})
#   set(${RESULTBOOL} TRUE)
# endmacro(FINDLIBS)
