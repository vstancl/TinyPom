# Determine current version of conan client

set(CONAN_COMMAND "conan" CACHE STRING "Path to conan binary, used for determining current conan version")

execute_process(COMMAND ${CONAN_COMMAND} "--version" OUTPUT_VARIABLE CONAN_VERSION_RAW COMMAND_ERROR_IS_FATAL  ANY)
string(STRIP ${CONAN_VERSION_RAW} CONAN_VERSION_RAW_STRIPPED)
string(REPLACE "Conan version " "" CONAN_VERSION "${CONAN_VERSION_RAW_STRIPPED}")
string(REGEX MATCHALL <match-regex> <out-var> <input>...)
string(REPLACE "." ";" CONAN_VERSION_LIST ${CONAN_VERSION})
list(GET CONAN_VERSION_LIST 0 CONAN_VERSION_MAJOR)
list(GET CONAN_VERSION_LIST 1 CONAN_VERSION_MINOR)
list(GET CONAN_VERSION_LIST 2 CONAN_VERSION_PATCH)

message(STATUS "Detected Conan version: '${CONAN_VERSION}'")