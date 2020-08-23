set(PROJECT_NAME "BattleFireEditor")
GroupDirectories("" "" ${PROJECT_SOURCE_DIR}/Runtime common_runtime_sources)
GroupDirectories("" "" ${PROJECT_SOURCE_DIR}/Platform/Windows/Editor win_editor_sources)
GroupDirectories("" "" ${PROJECT_SOURCE_DIR}/Platform/Windows/Common win_common_sources)

set(WinEditorSources ${common_runtime_sources} ${win_editor_sources} ${win_common_sources})

add_compile_definitions(ALICE_WIN_PLAYER _DEBUG UNICODE _UNICODE _SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS)

set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1")
set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /GR /EHsc")
set(CMAKE_C_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1")
set(CMAKE_C_FLAGS "/DWIN32 /D_WINDOWS /W3")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

link_directories(${PROJECT_SOURCE_DIR}/External/PrebuiltLibs/x86)
add_executable(${PROJECT_NAME} ${WinEditorSources})
target_link_libraries(${PROJECT_NAME} gdiplus libprotobuf Imm32)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Editor")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/Editor")