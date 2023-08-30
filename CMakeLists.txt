set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(kailcc LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(kaicc
    ./source/main.c
    ./source/compiler.c    
)