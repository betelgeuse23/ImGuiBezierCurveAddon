set(VCPKG_ROOT "$ENV{VCPKG_ROOT}" CACHE PATH "Path to an existing vcpkg installation")

if(NOT EXISTS "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    message(STATUS "Existing vcpkg installation not found. Attempting to install vcpkg into the build directory...")

    find_package(Git REQUIRED)
    if(NOT GIT_FOUND)
        message(FATAL_ERROR "Git not found. Cannot proceed with vcpkg installation.")
    endif()

    set(VCPKG_INSTALLATION_DIR "${CMAKE_BINARY_DIR}/vcpkg" CACHE PATH "Path to the local vcpkg installation")

    if(NOT EXISTS "${VCPKG_INSTALLATION_DIR}/.git")
        message(STATUS "Cloning vcpkg into the build directory...")
        execute_process(COMMAND ${GIT_EXECUTABLE} clone https://github.com/microsoft/vcpkg.git "${VCPKG_INSTALLATION_DIR}")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} -E chdir "${VCPKG_INSTALLATION_DIR}" ./bootstrap-vcpkg.sh -disableMetrics)

    set(VCPKG_ROOT "${VCPKG_INSTALLATION_DIR}" CACHE PATH "Path to the local vcpkg installation" FORCE)
endif()

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ImGuiBezierCurveAddon VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

file(GLOB ADDON_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB ADDON_HEADERS "${PROJECT_SOURCE_DIR}/src/*.h")

include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(ImGuiBezierCurveAddon SHARED ${ADDON_SOURCES} ${ADDON_HEADERS})
add_library(ImGuiBezierCurveAddon_s STATIC ${ADDON_SOURCES} ${ADDON_HEADERS})
set_target_properties(ImGuiBezierCurveAddon PROPERTIES OUTPUT_NAME "ImGuiBezierCurveAddon")
set_target_properties(ImGuiBezierCurveAddon_s PROPERTIES OUTPUT_NAME "ImGuiBezierCurveAddon")

find_package(imgui CONFIG REQUIRED)
target_link_libraries(ImGuiBezierCurveAddon PRIVATE imgui::imgui)
target_link_libraries(ImGuiBezierCurveAddon_s PRIVATE imgui::imgui)

install(TARGETS ImGuiBezierCurveAddon ImGuiBezierCurveAddon_s LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/ DESTINATION include FILES_MATCHING PATTERN "*.h")

option(BUILD_EXAMPLE "Build the example program" ON)
if(BUILD_EXAMPLE)
    add_executable(ImGuiBezierCurveExample example/main.cpp)
    target_include_directories(ImGuiBezierCurveExample PRIVATE ${PROJECT_SOURCE_DIR}/src)
    
    find_package(glfw3 CONFIG REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(OpenGL REQUIRED)
    target_link_libraries(ImGuiBezierCurveExample PRIVATE ImGuiBezierCurveAddon imgui::imgui glfw GLEW::GLEW OpenGL::GL)

    add_custom_command(TARGET ImGuiBezierCurveExample POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:ImGuiBezierCurveExample> $<TARGET_FILE_DIR:ImGuiBezierCurveExample> COMMAND_EXPAND_LISTS)
    
    install(TARGETS ImGuiBezierCurveExample DESTINATION example)
    
    file(GLOB EXAMPLE_DLLS "${CMAKE_CURRENT_BINARY_DIR}/*/glew32.dll" "${CMAKE_CURRENT_BINARY_DIR}/*/glew32d.dll" "${CMAKE_CURRENT_BINARY_DIR}/*/glfw3.dll")
    install(FILES ${EXAMPLE_DLLS} DESTINATION example)
endif()

option(BUILD_DOCUMENTATION "Build the documentation with Doxygen" ON)
if(BUILD_DOCUMENTATION)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/Doxyfile.in)
        set(DOXYGEN_OUT ${PROJECT_SOURCE_DIR}/Doxyfile)
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        add_custom_target(doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION doc)
    else()
        message("Doxygen needs to be installed to generate documentation")
    endif()
endif()
