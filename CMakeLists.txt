cmake_minimum_required(VERSION 3.9)
project(DOGM LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 14)

option(BUILD_TESTS "Build unit tests" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
find_package(OpenGL REQUIRED)

find_package(GLM)
if (NOT GLM_FOUND)
	set(GLM_INCLUDE_DIR "" CACHE PATH "")
endif()

find_package(GLFW3)
if (NOT GLFW3_FOUND)
	set(GLFW3_INCLUDE_DIR "" CACHE PATH "")
	set(GLFW3_LIBRARY "" CACHE FILEPATH "" )
endif()

find_package(GLEW)
if (NOT GLEW_FOUND)
	set(GLEW_INCLUDE_DIRS "" CACHE PATH "")
	set(GLEW_LIBRARIES "" CACHE FILEPATH "" )
endif()

include_directories(
	SYSTEM
	${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
	${GLFW3_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
	${GLM_INCLUDE_DIR}
)

set(HEADER_FILES
	include/dogm/init.h
	include/dogm/init_new_particles.h
	include/dogm/mass_update.h
	include/dogm/particle_to_grid.h
	include/dogm/predict.h
	include/dogm/resampling.h
	include/dogm/statistical_moments.h
	include/dogm/update_persistent_particles.h
	include/dogm/ego_motion_compensation.h
	include/common.h
	include/cuda_utils.h
	include/dogm.h
	include/dogm_types.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES})

set(SRC_FILES
	src/dogm/init.cu
	src/dogm/init_new_particles.cu
	src/dogm/mass_update.cu
	src/dogm/particle_to_grid.cu
	src/dogm/predict.cu
	src/dogm/resampling.cu
	src/dogm/statistical_moments.cu
	src/dogm/update_persistent_particles.cu
	src/dogm/ego_motion_compensation.cu
	src/dogm.cu
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_FILES})

add_library(dogm STATIC
    ${HEADER_FILES}
	${SRC_FILES}
)

target_include_directories(dogm PUBLIC include)

# support for gpu lambdas
string(APPEND CMAKE_CUDA_FLAGS " --expt-extended-lambda")
# supress glm warnings
#string(APPEND CMAKE_CUDA_FLAGS " -Xcudafe --diag_suppress=esa_on_defaulted_function_ignored")

#set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --expt-extended-lambda -Xcudafe --diag_suppress=esa_on_defaulted_function_ignored")
set(CUDA_PROPAGATE_HOST_FLAGS ON)

set_target_properties(dogm PROPERTIES PUBLIC_HEADER "include/dogm.h;include/dogm_types.h")

target_link_libraries(dogm
	PRIVATE ${OPENGL_LIBRARY}
	PRIVATE ${GLFW3_LIBRARY}
	PRIVATE ${GLEW_LIBRARIES}
)

#INSTALL(TARGETS dogm
#		ARCHIVE DESTINATION lib/dogm
#        LIBRARY DESTINATION lib/dogm
#        PUBLIC_HEADER DESTINATION include/dogm
#)

add_subdirectory(demo)
