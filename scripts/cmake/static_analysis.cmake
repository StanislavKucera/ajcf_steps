
if(AJCF_ENABLE_CPPCHECK)
    message(STATUS "AJCF - Enabling cppcheck analyzer")
    find_program(CPPCHECK cppcheck)
    if(CPPCHECK)
        set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress=missingInclude --enable=all --inconclusive -i ${CMAKE_SOURCE_DIR}/imgui/lib)
    else()
        message(SEND_ERROR "cppcheck requested but executable not found")
    endif()
endif()

if(AJCF_ENABLE_CLANG_TIDY)
    message(STATUS "AJCF - Enabling clang-tidy analyzer")
    find_program(CLANGTIDY clang-tidy)
    if(CLANGTIDY)
        set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY} -extra-arg=-Wno-unknown-warning-option)
    else()
        message(SEND_ERROR "clang-tidy requested but executable not found")
    endif()
endif()

if(AJCF_ENABLE_INCLUDE_WHAT_YOU_USE)
    message(STATUS "AJCF - Enabling include-what-you-use analyzer")
    find_program(INCLUDE_WHAT_YOU_USE include-what-you-use)
    if(INCLUDE_WHAT_YOU_USE)
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE})
    else()
        message(SEND_ERROR "include-what-you-use requested but executable not found")
    endif()
endif()
