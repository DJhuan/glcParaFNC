cmake_minimum_required(VERSION 3.30)
project(glcParaFNC)

# Define os arquivos fonte
set(SOURCE_FILES main.cpp
                 Glc.cpp
                 ArranjoBin.cpp)

# Define o diretório de saída do binário
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Adiciona o executável
add_executable(glcParaFNC ${SOURCE_FILES})

# Inclui os diretórios de cabeçalhos (se houver)
target_include_directories(glcParaFNC PRIVATE ${CMAKE_SOURCE_DIR})

