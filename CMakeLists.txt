cmake_minimum_required(VERSION 3.14)

# 프로젝트 이름 설정
project(4Q_3DProject LANGUAGES CXX CUDA)

# vcpkg 사용 설정
set(CMAKE_TOOLCHAIN_FILE "C:/Users/user/Documents/GitHub/vcpkg/scripts/buildsystems/vcpkg.cmake")

# CUDA 설정
enable_language(CUDA)

# PhysX 라이브러리 디렉토리 및 설정
set(PHYSX_INCLUDE_DIR "C:/Users/user/Documents/GitHub/vcpkg/installed/x64-windows/include")
set(PHYSX_LIB_DIR "C:/Users/user/Documents/GitHub/vcpkg/installed/x64-windows/lib")

# PhysX 라이브러리 포함 경로 추가
include_directories(${PHYSX_INCLUDE_DIR})

# PhysX 설정
set(PhysX_DIR "C:/Users/user/Documents/GitHub/vcpkg/installed/x64-windows/share/vcpkg-cmake-config")
list(APPEND CMAKE_PREFIX_PATH "${PhysX_DIR}")

find_package(PhysX REQUIRED CONFIG)
include_directories(${PHYSX_INCLUDE_DIRS})

# 프로젝트 소스 파일 추가
set(SOURCES
    02. PhysXEngine/PhysX.cpp
    TestApp/TestApp.cpp
    DemoApp/Demo.cpp
)

# 실행 파일 생성
add_executable(4Q_3DProject ${SOURCES})

# PhysX 라이브러리 링크
target_link_directories(4Q_3DProject PRIVATE ${PHYSX_LIB_DIR})
set(PHYSX_LIBRARIES
    PhysX_64.lib
    PhysXCharacterKinematic_static_64.lib
    PhysXCommon_64.lib
    PhysXCooking_64.lib
    PhysXExtensions_static_64.lib
    PhysXFoundation_64.lib
    PhysXPvdSDK_static_64.lib
    PhysXTask_static_64.lib
    PhysXVehicle_static_64.lib
    PhysXVehicle2_static_64.lib
    poly2tri.lib
    pugixml.lib
    SceneQuery_static_64.lib
    SimulationController_static_64.lib
)
target_link_libraries(4Q_3DProject PRIVATE ${PHYSX_LIBRARIES})