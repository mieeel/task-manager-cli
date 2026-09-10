cmake_minimum_required(VERSION 3.12)

# Nome do projeto e versão
project(TaskManagerCLI VERSION 1.1.0 LANGUAGES CXX)

# Exigir C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Lista de arquivos fonte e cabeçalhos
set(SOURCES
    main.cpp
    Colors.hpp
    Task.hpp
    TaskManager.hpp
    StorageManager.hpp
)

# Definir o executável gerado
add_executable(task ${SOURCES})

# Configurações do compilador para alertas (Clean Code)
if(MSVC)
    target_compile_options(task PRIVATE /W4)
else()
    target_compile_options(task PRIVATE -Wall -Wextra -Wpedantic)
endif()