cmake_minimum_required(VERSION 3.22)
project(Disposable_Cams LANGUAGES C CXX)

if(NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(FATAL_ERROR "Digital Cam targets Windows x64.")
endif()

enable_language(RC)

find_package(Python3 REQUIRED)

set(AFTEREFFECTS_SDK "${CMAKE_CURRENT_SOURCE_DIR}/extern/AfterEffectsSDK" CACHE PATH "Path to the After Effects SDK root (contains Examples/Headers)")
if(NOT EXISTS "${AFTEREFFECTS_SDK}/Examples/Headers/AEConfig.h")
  message(FATAL_ERROR "AFTEREFFECTS_SDK is invalid. Expected ${AFTEREFFECTS_SDK}/Examples/Headers/AEConfig.h")
endif()

set(AE_HEADERS "${AFTEREFFECTS_SDK}/Examples/Headers")
set(AE_UTIL    "${AFTEREFFECTS_SDK}/Examples/Util")
set(PIPL_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/tools/pipltool.py")

unset(PIPL_TOOL CACHE)
find_file(PIPL_TOOL
  NAMES PiPLtool.exe PiPLTool.exe
  PATHS "${AFTEREFFECTS_SDK}"
  PATH_SUFFIXES
    Examples/Resources
    Resources
    Examples/Util
    Util
    Tools
    bin
  NO_DEFAULT_PATH
)

if(NOT PIPL_TOOL)
  file(GLOB_RECURSE _SDK_PIPL_CANDIDATES
    LIST_DIRECTORIES false
    "${AFTEREFFECTS_SDK}/*PiPLtool.exe"
    "${AFTEREFFECTS_SDK}/*PiPLTool.exe"
  )
  if(_SDK_PIPL_CANDIDATES)
    list(GET _SDK_PIPL_CANDIDATES 0 PIPL_TOOL)
    set(PIPL_TOOL "${PIPL_TOOL}" CACHE FILEPATH "Path to Adobe After Effects SDK PiPLtool.exe" FORCE)
  endif()
endif()

if(NOT PIPL_TOOL OR NOT EXISTS "${PIPL_TOOL}")
  message(FATAL_ERROR
    "PiPLtool.exe was not found anywhere inside AFTEREFFECTS_SDK (${AFTEREFFECTS_SDK}). "
    "Ensure the bundled Adobe After Effects SDK in extern/AfterEffectsSDK is complete and includes PiPLtool.exe."
  )
endif()

message(STATUS "Found PiPLtool.exe: ${PIPL_TOOL}")

set(SRC
  src/DigitalCam.cpp
  src/CameraProfiles.cpp
  src/CameraProfiles.h
  src/NoiseEngine.h
  src/ColorEngine.h
  src/DigitalCam.h
)

set(PIPL_R "${CMAKE_CURRENT_SOURCE_DIR}/src/DigitalCamPiPL.r")
set(PIPL_RR "${CMAKE_CURRENT_BINARY_DIR}/DigitalCamPiPL.rr")
set(PIPL_RRC "${CMAKE_CURRENT_BINARY_DIR}/DigitalCamPiPL.rrc")
set(PIPL_RC "${CMAKE_CURRENT_BINARY_DIR}/DigitalCamPiPL.rc")

if(MSVC)
  set(PIPL_R_WIN "${PIPL_R}")
  set(PIPL_RR_WIN "${PIPL_RR}")
  set(PIPL_RRC_WIN "${PIPL_RRC}")
  set(PIPL_RC_WIN "${PIPL_RC}")
  set(AE_HEADERS_WIN "${AE_HEADERS}")
  set(AE_HEADERS_SP_WIN "${AE_HEADERS}/SP")
  set(PIPL_TOOL_WIN "${PIPL_TOOL}")

  file(TO_NATIVE_PATH "${PIPL_R}" PIPL_R_WIN)
  file(TO_NATIVE_PATH "${PIPL_RR}" PIPL_RR_WIN)
  file(TO_NATIVE_PATH "${PIPL_RRC}" PIPL_RRC_WIN)
  file(TO_NATIVE_PATH "${PIPL_RC}" PIPL_RC_WIN)
  file(TO_NATIVE_PATH "${AE_HEADERS}" AE_HEADERS_WIN)
  file(TO_NATIVE_PATH "${AE_HEADERS}/SP" AE_HEADERS_SP_WIN)
  file(TO_NATIVE_PATH "${PIPL_TOOL}" PIPL_TOOL_WIN)

  add_custom_command(
    OUTPUT "${PIPL_RR}"
    COMMAND "${CMAKE_CXX_COMPILER}" /nologo /I "${AE_HEADERS_WIN}" /I "${AE_HEADERS_SP_WIN}" /EP /P "/Fi${PIPL_RR_WIN}" /Tc "${PIPL_R_WIN}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS "${PIPL_R}"
    COMMENT "PiPL: preprocessing .r to .rr"
    VERBATIM
  )

  add_custom_command(
    OUTPUT "${PIPL_RRC}"
    COMMAND "${PIPL_TOOL_WIN}" "${PIPL_RR_WIN}" "${PIPL_RRC_WIN}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS "${PIPL_RR}" "${PIPL_TOOL}"
    COMMENT "PiPL: running PiPLtool"
    VERBATIM
  )

  add_custom_command(
    OUTPUT "${PIPL_RC}"
    COMMAND "${CMAKE_CXX_COMPILER}" /nologo /D MSWindows /EP /P "/Fi${PIPL_RC_WIN}" /Tc "${PIPL_RRC_WIN}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS "${PIPL_RRC}"
    COMMENT "PiPL: generating .rc"
    VERBATIM
  )
else()
  add_custom_command(
    OUTPUT "${PIPL_RC}"
    COMMAND "${Python3_EXECUTABLE}" "${PIPL_SCRIPT}" "${PIPL_R}" "${PIPL_RC}"
    DEPENDS "${PIPL_R}" "${PIPL_SCRIPT}" "${PIPL_TOOL}"
    COMMENT "PiPL: generating .rc with pipltool.py"
    VERBATIM
  )
endif()

add_custom_target(DigitalCam_PiPL ALL DEPENDS "${PIPL_RC}")

add_library(Digital_Cam SHARED ${SRC} "${PIPL_RC}")
add_dependencies(Digital_Cam DigitalCam_PiPL)

target_include_directories(Digital_Cam PRIVATE
  "${AE_HEADERS}"
  "${AE_HEADERS}/SP"
  "${AE_UTIL}"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_features(Digital_Cam PRIVATE cxx_std_17)
target_compile_definitions(Digital_Cam PRIVATE WIN32 _WINDOWS NOMINMAX MSWindows)

if(MSVC)
  target_compile_options(Digital_Cam PRIVATE /O2 /EHsc /W4 /permissive- /utf-8)
  target_link_options(Digital_Cam PRIVATE /SUBSYSTEM:WINDOWS)
else()
  target_compile_options(Digital_Cam PRIVATE -O3 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers)
  target_link_options(Digital_Cam PRIVATE -static-libgcc -static-libstdc++ -Wl,--subsystem,windows)
endif()

set_target_properties(Digital_Cam PROPERTIES
  SUFFIX ".aex"
  OUTPUT_NAME "Disposable Cams"
  PREFIX ""
)

add_custom_command(TARGET Digital_Cam POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:Digital_Cam>"
    "${CMAKE_CURRENT_SOURCE_DIR}/Disposable Cams.aex"
  COMMENT "Deploying production Disposable Cams.aex"
)
