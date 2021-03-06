cmake_minimum_required(VERSION 3.0)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

project(bunsan_pm)

if(BunsanCMakeBuiltin)
    BunsanCMakeLoad()
else()
    find_package(BunsanCMake CONFIG REQUIRED)
endif()

bunsan_add_library(${PROJECT_NAME}
    src/lib/index.cpp
    src/lib/entry.cpp
    src/lib/checksum.cpp
    src/lib/repository.cpp
    src/lib/snapshot.cpp

    src/lib/repository/builder.cpp
    src/lib/repository/cache.cpp
    src/lib/repository/distributor.cpp
    src/lib/repository/extractor.cpp
    src/lib/repository/local_system.cpp

    src/lib/cache.cpp
)
bunsan_use_bunsan_package(${PROJECT_NAME} bunsan_crypto bunsan_crypto)
bunsan_use_bunsan_package(${PROJECT_NAME} bunsan_utility bunsan_utility)

bunsan_add_library(${PROJECT_NAME}_compatibility
    src/compatibility/repository_cpp.cpp
    src/compatibility/repository_c.cpp
)
bunsan_use_target(${PROJECT_NAME}_compatibility ${PROJECT_NAME})

bunsan_add_library(${PROJECT_NAME}_python
    src/compatibility/repository_python.cpp
)
bunsan_use_target(${PROJECT_NAME}_python ${PROJECT_NAME}_compatibility)
bunsan_use_boost(${PROJECT_NAME}_python python3)
bunsan_use_python3(${PROJECT_NAME}_python)
bunsan_install_python_module_target(
    TARGET ${PROJECT_NAME}_python
    MODULE bunsan.pm._pm
)
bunsan_install_python_module_init(
    INIT src/compatibility/__init__.py
    MODULE bunsan.pm
)

bunsan_add_executable(${PROJECT_NAME}_cli
    src/bin/cli.cpp
)
bunsan_use_target(${PROJECT_NAME}_cli ${PROJECT_NAME})

bunsan_install_headers()
bunsan_install_targets(
    ${PROJECT_NAME}
    ${PROJECT_NAME}_compatibility
    ${PROJECT_NAME}_cli
)
bunsan_install_project()

bunsan_include_tests()
