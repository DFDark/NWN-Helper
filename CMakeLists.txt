cmake_minimum_required(VERSION 3.0.2)
project(NWNHelper)

add_library(NWNHelper STATIC
    configuration-manager.hpp
    configuration-manager.cpp
    nwnhelper.hpp
    nwnhelper.cpp
    nwnhelper-main.hpp
    nwnhelper-main.cpp
    Components/spell-form.hpp
    Components/spell-form.cpp
    Components/feat-form.hpp
    Components/feat-form.cpp
    Components/first-time-configuration.hpp
    Components/first-time-configuration.cpp
    ListModels/spelllist-model.hpp
    ListModels/spelllist-model.cpp
    ListModels/featlist-model.hpp
    ListModels/featlist-model.cpp
)

add_library(SimpleIni STATIC
    simpleini/SimpleIni.h
    simpleini/ConvertUTF.h
    simpleini/ConvertUTF.c
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include_directories(".")

add_subdirectory(FileFormats)
add_subdirectory(Utility)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include( "${wxWidgets_USE_FILE}" )

add_executable(main nwnhelper.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(main NWNHelper SimpleIni FileFormats Utility ${wxWidgets_LIBRARIES})
