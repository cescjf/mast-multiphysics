project (MAST)
enable_language (CXX Fortran)

set (MAST_VERSION_MAJOR 0)
set (MAST_VERSION_MINOR 9)

cmake_minimum_required(VERSION 2.8)

# flags for optional compilation of packages
option (ENABLE_GCMMA "Build with GCMMA interface" OFF)
option (ENABLE_DOT   "Build with DOT interface"   OFF)
option (ENABLE_NPSOL "Build with NPSOL interface" OFF)


#set default values
set (MAST_ENABLE_GCMMA 0)
set (MAST_ENABLE_DOT   0)
set (MAST_ENABLE_NPSOL 0)


# look for gcmma library

if (ENABLE_GCMMA)
   set (MAST_ENABLE_GCMMA 1)
   set (gcmma_lib_file    "gcmma-lib-file" CACHE STRING "GCMMA optimization library")
   set (gcmma_lib_dir     "gcmma-lib-dir" CACHE STRING "Directory containing GCMMA optimization library")
   find_library (gcmma_lib   ${gcmma_lib_file}    ${gcmma_lib_dir})
endif()


# look for dot library
if (ENABLE_DOT)
    set (MAST_ENABLE_DOT 1)
    set (dot_lib_file      "dot-lib-file" CACHE STRING "DOT optimization library")
    set (dot_lib_dir       "dot-lib-dir" CACHE STRING "Directory containing DOT optimization library")
    find_library (dot_lib     ${dot_lib_file}      ${dot_lib_dir})
endif()


# look for npsol library
if (ENABLE_NPSOL)
   set (MAST_ENABLE_NPSOL 1)
   set (npsol_lib_file    "npsol-lib-file" CACHE STRING "NPSOL optimization library")
   set (npsol_lib_dir     "npsol-lib-dir" CACHE STRING "Directory containing NPSOL optimization library")
   find_library (npsol_lib   ${npsol_lib_file}    ${npsol_lib_dir})
endif()


set (libmesh_dir       "libmesh-dir" CACHE STRING "Directory containing libMesh header and library")
set (eigen_include_dir "eigen-include-dir" CACHE STRING "Directory containing eigen header files")
set (boost_include_dir "boost-include-dir" CACHE STRING "Directory containing boost header files")
set (boost_test_lib    "boost-unit-test-lib" CACHE STRING "BOOST unit test library")
set (boost_lib_dir     "boost-lib-dir" CACHE STRING "Directory containing boost unit test library files")
set (mpi_include_dir   "mpi-include-dir" CACHE STRING "Directory containing MPI header files")
set (mpi_lib_dir       "mpi-lib-dir" CACHE STRING "Directory containing MPI library files")
set (fortran_lib_file  "fortran-lib-file" CACHE STRING "fortran library")
set (fortran_lib_dir   "fortran-lib-dir" CACHE STRING "Directory containing fortran library")
set (lapack_lib_file   "lapack" CACHE STRING "LAPACK library")
set (lapack_lib_dir    "lapack-lib-dir" CACHE STRING "Directory containing LAPACK library")
separate_arguments (lapack_lib_file)
set (blas_lib_file     "blas" CACHE STRING "BLAS library")
set (blas_lib_dir      "blas-lib-dir" CACHE STRING "Directory containing BLAS library")
separate_arguments (blas_lib_file)
set (petsc_dir         "petsc-dir" CACHE STRING "Directory containing PETSc installation")
set (slepc_dir         "slepc-dir" CACHE STRING "Directory containing PETSc installation")
set (petsc_arch         "petsc-arch" CACHE STRING "Directory containing PETSc installation")
set (CMAKE_Fortran_COMPILER "fortran-compiler" CACHE STRING "Fortran compiler")
set (CMAKE_CXX_COMPILER "cxx-compiler" CACHE STRING "CXX compiler")


include_directories (${PROJECT_SOURCE_DIR}/../src
                     ${PROJECT_SOURCE_DIR}/../examples
         		     ${PROJECT_SOURCE_DIR}/../tests
                     ${PROJECT_SOURCE_DIR}/../
                             ${eigen_include_dir}
		             ${boost_include_dir})

include_directories (${petsc_dir}/${petsc_arch}/include)
include_directories (${slepc_dir}/$ENV{petsc_arch}/include)
include_directories (SYSTEM ${mpi_include_dir})


find_library (mesh_dbg mesh_dbg ${libmesh_dir}/lib)
find_library (mesh_opt mesh_opt ${libmesh_dir}/lib)

find_library (petsc_lib petsc ${petsc_dir}/lib ${petsc_dir}/${petsc_arch}/lib NO_CMAKE_FIND_ROOT_PATH)
find_library (slepc_lib slepc ${slepc_dir}/lib ${slepc_dir}/${petsc_arch}/lib NO_CMAKE_FIND_ROOT_PATH)

find_library (mpicxx_lib    mpi_cxx              ${mpi_lib_dir} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
find_library (mpi_lib       mpi                  ${mpi_lib_dir} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

find_library (boost_unit_test_lib   ${boost_test_lib}    ${boost_lib_dir})

find_library (fortran_lib   ${fortran_lib_file}    ${fortran_lib_dir})

foreach (f ${lapack_lib_file})
   find_library (lapack_lib    ${f}   ${lapack_lib_dir})
endforeach (f)

foreach (f ${blas_lib_file})
   find_library (blas_lib      ${f}     ${blas_lib_dir})
endforeach (f)


#
# write the configuration file
#
configure_file ( ${PROJECT_SOURCE_DIR}/../src/base/mast_config.h.in
                 ${PROJECT_SOURCE_DIR}/../src/base/mast_config.h )


####################################################################
#  add files for MAST library and add it to cmake's build list
####################################################################
file (GLOB_RECURSE mast_source_files
      ${PROJECT_SOURCE_DIR}/../src/*.cpp
      ${PROJECT_SOURCE_DIR}/../src/*.h)
add_library (mast ${mast_source_files})

target_link_libraries (mast
		               debug ${mesh_dbg}
		               optimized ${mesh_opt}
		               ${petsc_lib}
		               ${slepc_lib}
		               ${lapack_lib}
                       ${blas_lib}
		               ${gcmma_lib}
		               ${dot_lib}
		               ${npsol_lib}
		               ${fortran_lib}
		               ${mpicxx_lib}
		               ${mpi_lib})

set_property (TARGET mast APPEND
              PROPERTY INCLUDE_DIRECTORIES
	          ${libmesh_dir}/include)
set_property (TARGET mast APPEND
	          PROPERTY INCLUDE_DIRECTORIES
	          ${petsc_dir}/include
              ${petsc_dir}/${petsc_arch}/include)
set_property (TARGET mast APPEND
	          PROPERTY INCLUDE_DIRECTORIES
	          ${slepc_dir}/include
              ${slepc_dir}/${petsc_arch}/include)

####################################################################
#  tell cmake to link example driver with mast library
####################################################################
file (GLOB_RECURSE example_driver_source_files
      ${PROJECT_SOURCE_DIR}/../examples/*.cpp
      ${PROJECT_SOURCE_DIR}/../examples/*.h)
add_executable (example_driver ${example_driver_source_files})

find_package (Threads REQUIRED)
target_link_libraries (example_driver mast -lpthread ${CMAKE_THREAD_LIBS_INIT})
set_property (TARGET example_driver APPEND
              PROPERTY INCLUDE_DIRECTORIES
	          ${libmesh_dir}/include)
set_property (TARGET example_driver APPEND
              PROPERTY INCLUDE_DIRECTORIES
              ${petsc_dir}/include
              ${petsc_dir}/${petsc_arch}/include)
set_property (TARGET example_driver APPEND
              PROPERTY INCLUDE_DIRECTORIES
              ${slepc_dir}/include
              ${slepc_dir}/${petsc_arch}/include)

####################################################################
#  tell cmake to link the tests
####################################################################
file (GLOB_RECURSE mast_test_source_files
      ${PROJECT_SOURCE_DIR}/../examples/*.cpp
      ${PROJECT_SOURCE_DIR}/../examples/*.h
      ${PROJECT_SOURCE_DIR}/../tests/*.cpp
      ${PROJECT_SOURCE_DIR}/../tests/*.h)
list (REMOVE_ITEM mast_test_source_files ${PROJECT_SOURCE_DIR}/../examples/base/examples_driver.cpp)
add_executable (mast_tests ${mast_test_source_files})

target_link_libraries (mast_tests
                       mast
			           ${boost_unit_test_lib})
set_property (TARGET mast_tests APPEND
	          PROPERTY INCLUDE_DIRECTORIES
	          ${libmesh_dir}/include
	          ${})
set_property (TARGET mast_tests APPEND
              PROPERTY INCLUDE_DIRECTORIES
              ${petsc_dir}/include
              ${petsc_dir}/${petsc_arch}/include)
set_property (TARGET mast_tests APPEND
              PROPERTY INCLUDE_DIRECTORIES
              ${slepc_dir}/include
              ${slepc_dir}/${petsc_arch}/include)
