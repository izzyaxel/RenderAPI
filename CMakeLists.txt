cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(RenderAPI)
set(CMAKE_CXX_STANDARD 17)
include_directories(include) #include GLAD for development (autocompletion) purposes, GLAD includes should be removed when you stop working on a file/before committing
set(SRC
		src/renderapi/textureGL45.hh
		src/renderapi/shaderGL45.hh
		src/renderapi/meshGL45.hh
		src/renderapi/framebufferGL45.hh
		src/renderapi/renderPassGL45.hh)
add_executable(${PROJECT_NAME} ${SRC} src/dummy.cc) #Dummy file is present to make IDEs like CLion recognize this as a C++ project
