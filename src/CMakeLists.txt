cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(ARCH "x86_64")

if(CMAKE_BUILD_TYPE MATCHES Release)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s") # strip debug symbols from release build
	set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
	add_definitions(-DRELEASE)
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
	set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0")
	add_definitions(-DDEBUG)
endif()

project(RenderAPI)
set(CMAKE_CXX_STANDARD 17)
include_directories(../include)
set(SRC
		renderapi/enums.hh
		renderapi/textureGL45.hh
		renderapi/shaderGL45.hh
		renderapi/vertexBufferGL45.hh
		renderapi/indexBufferGL45.hh
		renderapi/framebufferGL45.hh
		renderapi/renderPassGL45.hh)

add_library(${PROJECT_NAME} STATIC ${SRC} dummy.cc)
