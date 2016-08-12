cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.
									# Если версия установленой программы
									# старее указаной, произайдёт аварийный выход.

project(lz4wrapper)				# Название проекта

set(SOURCE_LIB LZ4_Wrapper.cpp)			# Установка переменной со списком исходников

include_directories(lz4)			# Расположение заголовочных файлов

set(EXECUTABLE_OUTPUT_PATH "lib")

add_library(lz4wraper STATIC ${SOURCE_LIB})	# Создает исполняемый файл с именем main

add_subdirectory(lz4)				# Добавление подпроекта, указывается имя дирректории
 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

target_link_libraries(lz4wraper  lz4)		# Линковка программы с библиотекой
