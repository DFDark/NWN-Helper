cmake_minimum_required(VERSION 3.0.2)
project(NWNHelper)

add_library(NWNHelper STATIC
    functions.hpp
    configuration-manager.hpp
    configuration-manager.cpp
    nwnhelper.hpp
    nwnhelper.cpp
    nwnhelper-main.hpp
    nwnhelper-main.cpp
    project.hpp
    project.cpp
    Components/spell-form.hpp
    Components/spell-form.cpp
    Components/feat-form.hpp
    Components/feat-form.cpp
    Components/first-time-configuration.hpp
    Components/first-time-configuration.cpp
    Components/export-form.hpp
    Components/export-form.cpp
    Components/master-feat-form.hpp
    Components/master-feat-form.cpp
    Components/skill-form.hpp
    Components/skill-form.cpp
    Components/import-form.hpp
    Components/import-form.cpp
    Components/ColumnForms/spell-column-form.hpp
    Components/ColumnForms/spell-column-form.cpp
    Components/ColumnForms/feat-column-form.hpp
    Components/ColumnForms/feat-column-form.cpp
    Components/ColumnForms/master-feat-column-form.hpp
    Components/ColumnForms/master-feat-column-form.cpp
    Components/ColumnForms/skill-column-form.hpp
    Components/ColumnForms/skill-column-form.cpp
    SelectionForms/spell-selection-form.hpp
    SelectionForms/spell-selection-form.cpp
    SelectionForms/feat-selection-form.hpp
    SelectionForms/feat-selection-form.cpp
    SelectionForms/master-feat-selection-form.hpp
    SelectionForms/master-feat-selection-form.cpp
    SelectionForms/skill-selection-form.hpp
    SelectionForms/skill-selection-form.cpp
    SelectionForms/class-selection-form.hpp
    SelectionForms/class-selection-form.cpp
    ListModels/spelllist-model.hpp
    ListModels/spelllist-model.cpp
    ListModels/featlist-model.hpp
    ListModels/featlist-model.cpp
    ListModels/masterfeatlist-model.hpp
    ListModels/masterfeatlist-model.cpp
    ListModels/skilllist-model.hpp
    ListModels/skilllist-model.cpp
    ListModels/classlist-model.hpp
    ListModels/classlist-model.cpp
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
target_link_libraries(main NWNHelper SimpleIni FileFormats Utility ${wxWidgets_LIBRARIES} stdc++fs)
