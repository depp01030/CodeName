cmake_minimum_required(VERSION 3.16)

# 讓使用者可以透過指令切換編譯 Server 還是 Client
option(BUILD_SERVER "Build the server version" OFF)

if(BUILD_SERVER)
    set(PROJECT_NAME "ChatRoom-Server")
    add_definitions(-D BUILD_SERVER)
    set(DIR_NAME "Server")
else()
    set(PROJECT_NAME "ChatRoom-Client")
    set(DIR_NAME "Client")
endif()

project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)

# --- 關鍵修正區 ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON) # 開啟自動 UI 處理

# 設定 Mac 的 Qt 路徑
if(APPLE)
    set(CMAKE_PREFIX_PATH "/Users/depp/Qt/6.10.2/macos")
endif()

# 尋找組件 (確保你有裝 HttpServer 模組)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Network HttpServer)
# -----------------
message(STATUS "Qt6 components found")

# 添加源文件
message(STATUS "Adding source files")



# 设置包含目录
set(MAINFILE 
    main.cpp 
)
set(MAINWINDOW
    modules/mainwindow/include/mainwindow.h
    modules/mainwindow/src/mainwindow.cpp
)

set(CHAT_ROOM
    modules/chatroom/include/chatroom.h
    modules/chatroom/src/chatroom.cpp
    modules/chatroom/include/chattextedit.h
    modules/chatroom/src/chattextedit.cpp
)

set(GAME_CONTROLLER 
    modules/game_controller/include/game_controller.h
    modules/game_controller/src/game_controller.cpp
)
set(GAME_SCREEN
    modules/game_screen/include/game_screen.h
    modules/game_screen/src/game_screen.cpp
    modules/game_screen/include/code_card.h
    modules/game_screen/src/code_card.cpp
)
set(GAME_CORE
    modules/game_core/include/game_core.h
    modules/game_core/src/game_core.cpp 
    modules/game_core/include/game_state.h
    modules/game_core/src/game_state.cpp
    modules/game_core/include/candidate_words.h
    modules/game_core/include/card_color.h
)

# 添加 utils 模块的源文件
set(NETWORK_UTILS
    modules/network_utils/include/user.h
    modules/network_utils/src/user.cpp

)
set(UTILS
    modules/utils/include/color.h
    modules/utils/src/color.cpp
)
# MessageSysytem
set(MESSAGE_SYSTEM
    modules/message_system/include/message_processer.h
    modules/message_system/src/message_processer.cpp
    modules/message_system/include/chat_message.h
    modules/message_system/src/chat_message.cpp
    modules/message_system/include/game_message.h
    modules/message_system/src/game_message.cpp
    modules/message_system/include/system_message.h
    modules/message_system/src/system_message.cpp
)
# 添加 UI 文件
set(UI_FILES
    modules/mainwindow/ui/mainwindow.ui
    modules/chatroom/ui/chatroom.ui
    modules/game_screen/ui/game_screen.ui
)
set(RESOURCE_FILES
    resources/resources.qrc
)
set(INETWORK_DATA_MANAGER
    modules/network_data_manager/include/network_data_manager.h
    modules/network_data_manager/src/network_data_manager.cpp
    modules/network_data_manager/include/network_data_factory.h
)
if(BUILD_SERVER)
    set(NETWORK_DATA_MANAGER
        modules/network_data_manager_server/include/network_data_manager_server.h
        modules/network_data_manager_server/src/network_data_manager_server.cpp
    ) 
else()
    set(NETWORK_DATA_MANAGER
        modules/network_data_manager_client/include/network_data_manager_client.h
        modules/network_data_manager_client/src/network_data_manager_client.cpp
    )
endif()

# 明确指定 UI 文件的处理

# 告訴 CMake 到這些資料夾去找 .ui 檔案
set(CMAKE_AUTOUIC_SEARCH_PATHS
        "${CMAKE_CURRENT_SOURCE_DIR}/modules/mainwindow/ui"
        "${CMAKE_CURRENT_SOURCE_DIR}/modules/chatroom/ui"
        "${CMAKE_CURRENT_SOURCE_DIR}/modules/game_screen/ui"
)
# qt6_wrap_ui(UI_HEADERS ${UI_FILES})

message(STATUS "UI files: ${UI_FILES}")
message(STATUS "Generated UI headers: ${UI_HEADERS}")



# 创建可执行文件
add_executable(${PROJECT_NAME}
    ${MAINFILE}
    ${MAINWINDOW}
    ${CHAT_ROOM}
    ${INETWORK_DATA_MANAGER}
    ${NETWORK_DATA_MANAGER}
    ${RESOURCE_FILES} 
    ${GAME_CORE}
    ${GAME_SCREEN}
    ${GAME_CONTROLLER}
    ${NETWORK_UTILS}
    ${UTILS}
    ${MESSAGE_SYSTEM}
    ${UI_FILES}
    #${UI_HEADERS}  # 确保包含生成的 UI 头文件
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Widgets 
    Qt6::Network 
    Qt6::HttpServer
) 
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/mainwindow/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/game_core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/game_screen/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/game_controller/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/chatroom/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/network_utils/include
    ${CMAKE_CURRENT_BINARY_DIR}  # 包含生成的 UI 头文件
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/utils/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/message_system/include
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/network_data_manager/include
)

if(BUILD_SERVER)
    target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/network_data_manager_server/include
    )
else()
    target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/network_data_manager_client/include
    )
endif()

# 设置可执行文件的输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    # RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/${DIR_NAME}/${CMAKE_BUILD_TYPE}"
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/${DIR_NAME}/${CMAKE_BUILD_TYPE}")
# 确保 UI 头文件被正确生成
# add_custom_target(generate_ui_headers DEPENDS ${UI_HEADERS})
# add_dependencies(${PROJECT_NAME} generate_ui_headers)
# 根据编译模式设置不同的编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE "-g")  # 启用调试符号
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE "-O3") # 启用优化
endif()

# 复制 Qt DLL 和平台插件到输出目录 (仅在 Windows 上)
# if(WIN32)
#     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_PREFIX_PATH}/bin/windeployqt.exe
#             --qmldir ${CMAKE_SOURCE_DIR}
#             $<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_NAME:${PROJECT_NAME}>
#     )
# endif()

message(STATUS "CMake configuration completed")
