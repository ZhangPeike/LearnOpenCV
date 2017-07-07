# homest CMake file; see http://www.cmake.org and 
#                        http://www.insightsoftwareconsortium.org/wiki/index.php/CMake_Tutorial

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(HOMEST)

IF(CMAKE_GENERATOR MATCHES "^Visual Studio *")
  # get rid of CRT warnings
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF(CMAKE_GENERATOR MATCHES "^Visual Studio *")


# levmar library
SET(LEVMAR_INCDIR "/usr/local/levmar-2.5" CACHE PATH "Path to LEVMAR library header")
SET(LEVMAR_LIBDIR "/usr/local/levmar-2.5" CACHE PATH "Path to LEVMAR library")

# lapack/blas
SET(HAVE_LAPACK 1 CACHE BOOL "Do we have LAPACK/BLAS?")
# the directory where the lapack/blas/f2c libraries reside
SET(LAPACKBLAS_DIR "/usr/lib" CACHE PATH "Path to lapack/blas libraries")
#SET(LAPACKBLAS_DIR "C:\Program Files\Intel\MKL\10.2.2.025\ia32\lib" CACHE PATH "Path to lapack/blas libraries") # MKL
SET(NEED_F2C 1 CACHE BOOL "Do we need either f2c or F77/I77?")
SET(HAVE_PLASMA 0 CACHE BOOL "Do we have PLASMA parallel linear algebra library?")
IF(HAVE_PLASMA)
 SET(PLASMA_DIR "/usr/local/PLASMA" CACHE PATH "Path to PLASMA root")
ENDIF(HAVE_PLASMA)
OPTION(BUILD_DEMO "Build demo program?" TRUE)

# actual names for the lapack/blas/f2c libraries
SET(LAPACKBLAS_LIB_NAMES "lapack;blas" CACHE STRING "The name of the lapack & blas libraries")
#SET(LAPACKBLAS_LIB_NAMES "mkl_solver_sequential;mkl_intel_c;mkl_sequential;mkl_core" CACHE STRING "The name of the lapack libraries") # MKL
IF(NEED_F2C)
  SET(F2C_LIB_NAME f2c CACHE STRING "The name of the f2c or F77/I77 library")
  # f2c is sometimes equivalent to libF77 & libI77
  #SET(F2C_LIB_NAME "libF77;libI77" CACHE STRING "The name of the f2c or F77/I77 library")
ELSE(NEED_F2C)
  SET(F2C_LIB_NAME "" CACHE STRING "The name of the f2c or F77/I77 library")
ENDIF(NEED_F2C)

# actual names for the PLASMA libraries
IF(HAVE_PLASMA)
  SET(PLASMA_LIB_NAMES "plasma;coreblas;quark;lapacke" CACHE STRING "The names of the PLASMA libraries")
ENDIF(HAVE_PLASMA)


########################## NO CHANGES BEYOND THIS POINT ##########################

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${LEVMAR_INCDIR})
IF(HAVE_PLASMA)
 INCLUDE_DIRECTORIES(${PLASMA_DIR}/include ${PLASMA_DIR}/quark)
ENDIF(HAVE_PLASMA)

# homest library source files
ADD_LIBRARY(homest STATIC
  calc_2Dhomog_coeffs.c homest.c lqs.c linalg.c norm.c buckets.c
  compiler.h homest.h lqs.h maplefuncs.h util.h
)

# demo program
IF(BUILD_DEMO)
  SET(LIBS homest levmar)

  LINK_DIRECTORIES(${CMAKE_BINARY_DIR}) # location of the homest library
  LINK_DIRECTORIES(${LAPACKBLAS_DIR})
  LINK_DIRECTORIES(${LEVMAR_LIBDIR})

# libraries the demo depends on
  IF(HAVE_PLASMA)
    LINK_DIRECTORIES(${PLASMA_DIR}/lib)
    SET(LIBS ${LIBS} ${PLASMA_LIB_NAMES})
  ENDIF(HAVE_PLASMA)

  IF(HAVE_LAPACK)
    IF(NEED_F2C)
      SET(LIBS ${LIBS} ${LAPACKBLAS_LIB_NAMES} ${F2C_LIB_NAME})
    ELSE(NEED_F2C)
      SET(LIBS ${LIBS} ${LAPACKBLAS_LIB_NAMES})
    ENDIF(NEED_F2C)
  ENDIF(HAVE_LAPACK)

  ADD_EXECUTABLE(homest_demo homest_demo.c homest.h)
  TARGET_LINK_LIBRARIES(homest_demo ${LIBS})
  MESSAGE(STATUS "homest_demo will be linked against ${LIBS}")

# make sure that the library is built before the demo
  ADD_DEPENDENCIES(homest_demo homest)
ENDIF(BUILD_DEMO)

#ADD_SUBDIRECTORY(matlab)
