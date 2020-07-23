cmake_minimum_required(VERSION 3.14)
project(M3C2)

# Windows specific defines
if(WIN32)
  add_definitions(-DNOMINMAX)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  if(MSVC)
    add_definitions(/openmp)
  endif()
endif()


# Declare library
add_library(${PROJECT_NAME} STATIC src/points.hpp
			    src/svd.hpp
			    src/m3c2.cpp
			    src/m3c2.h)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

install (TARGETS ${PROJECT_NAME}
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin)

install(FILES src/m3c2.h DESTINATION include)