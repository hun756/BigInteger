# Safely remove build artifacts
file(GLOB build_files
    "${CMAKE_BINARY_DIR}/CMakeCache.txt"
    "${CMAKE_BINARY_DIR}/CMakeFiles/*"
)

foreach(file ${build_files})
    if(EXISTS ${file})
        file(REMOVE_RECURSE ${file})
    endif()
endforeach()
