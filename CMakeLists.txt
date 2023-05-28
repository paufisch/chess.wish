cmake_minimum_required(VERSION 3.15)
project(Chess) # your project name

set(CMAKE_CXX_STANDARD 20)

include_directories(sockpp/include)

find_package(wxWidgets COMPONENTS core base net REQUIRED)
include(${wxWidgets_USE_FILE})

# define a variable CLIENT_SOURCE_FILES that contains the paths to all source files required to compile the client executable
set(CLIENT_SOURCE_FILES
        src/client/main.cpp
        src/client/app/Chess.cpp src/client/app/Chess.h
        src/client/GameController.cpp src/client/GameController.h
        # UI
        src/client/windows/GameWindow.cpp src/client/windows/GameWindow.h
        src/client/uiElements/ImagePanel.cpp src/client/uiElements/ImagePanel.h
        src/client/panels/ConnectionPanel.cpp src/client/panels/ConnectionPanel.h
        src/client/panels/MainGamePanel.cpp src/client/panels/MainGamePanel.h
        src/client/uiElements/InputField.cpp src/client/uiElements/InputField.h
        src/client/uiElements/ImagePanel.cpp src/client/uiElements/ImagePanel.h
        src/client/panels/MyPanel.h src/client/panels/MyPanel.cpp
        # network
        src/client/network/ClientNetworkManager.cpp src/client/network/ClientNetworkManager.h
        src/client/network/ResponseListenerThread.cpp src/client/network/ResponseListenerThread.h
        # game state
        src/common/game_state/GameState.cpp src/common/game_state/GameState.h
        src/common/game_state/player/Player.cpp src/common/game_state/player/Player.h
        src/common/game_state/Board.cpp src/common/game_state/Board.h
        src/common/game_state/Color.h
        src/common/game_state/piece/Piece.cpp src/common/game_state/piece/Piece.h
        # client requests
        src/common/network/requests/ClientRequest.cpp src/common/network/requests/ClientRequest.h
        src/common/network/requests/JoinGameRequest.cpp src/common/network/requests/JoinGameRequest.h
        src/common/network/requests/StartGameRequest.cpp src/common/network/requests/StartGameRequest.h
        src/common/network/requests/MovePieceRequest.cpp src/common/network/requests/MovePieceRequest.h
        src/common/network/requests/ResignRequest.cpp src/common/network/requests/ResignRequest.h
        # server responses
        src/common/network/responses/ServerResponse.cpp src/common/network/responses/ServerResponse.h
        src/common/network/responses/RequestResponse.cpp src/common/network/responses/RequestResponse.h
        src/common/network/responses/FullStateResponse.cpp src/common/network/responses/FullStateResponse.h
        # serialization
        src/common/serialization/Serializable.h
        src/common/serialization/value_type_helpers.h
        src/common/serialization/vector_utils.h
        src/common/serialization/SerializableValue.h
        src/common/serialization/JsonUtils.h
        src/common/serialization/UuidGenerator.h
        src/common/serialization/UniqueSerializable.cpp src/common/serialization/UniqueSerializable.h)
# define a variable SERVER_SOURCE_FILES that contains the paths to all source files required to compile the server executable
set(SERVER_SOURCE_FILES
        src/server/main.cpp
        src/server/GameInstance.cpp src/server/GameInstance.h
        src/server/GameInstanceManager.cpp src/server/GameInstanceManager.h
        src/server/PlayerManager.cpp src/server/PlayerManager.h
        src/server/ServerNetworkManager.cpp src/server/ServerNetworkManager.h
        src/server/RequestHandler.cpp src/server/RequestHandler.h
        # game state
        src/common/game_state/GameState.cpp src/common/game_state/GameState.h
        src/common/game_state/player/Player.cpp src/common/game_state/player/Player.h
        src/common/game_state/Board.cpp src/common/game_state/Board.h
        src/common/game_state/Color.h
        src/common/game_state/piece/Piece.cpp src/common/game_state/piece/Piece.h
        # client requests
        src/common/network/requests/ClientRequest.cpp src/common/network/requests/ClientRequest.h
        src/common/network/requests/JoinGameRequest.cpp src/common/network/requests/JoinGameRequest.h
        src/common/network/requests/StartGameRequest.cpp src/common/network/requests/StartGameRequest.h
        src/common/network/requests/MovePieceRequest.cpp src/common/network/requests/MovePieceRequest.h
        src/common/network/requests/ResignRequest.cpp src/common/network/requests/ResignRequest.h
        # server responses
        src/common/network/responses/ServerResponse.cpp src/common/network/responses/ServerResponse.h
        src/common/network/responses/RequestResponse.cpp src/common/network/responses/RequestResponse.h
        src/common/network/responses/FullStateResponse.cpp src/common/network/responses/FullStateResponse.h
        # serialization
        src/common/serialization/Serializable.h
        src/common/serialization/value_type_helpers.h
        src/common/serialization/vector_utils.h
        src/common/serialization/SerializableValue.h
        src/common/serialization/JsonUtils.h
        src/common/serialization/UuidGenerator.h
        src/common/serialization/UniqueSerializable.cpp src/common/serialization/UniqueSerializable.h)
# set source files for client-executable
add_executable(Chess-client ${CLIENT_SOURCE_FILES})
# set compile directives for client-executable
target_compile_definitions(Chess-client PRIVATE CHESS_CLIENT=1 RAPIDJSON_HAS_STDSTRING=1)
# link with wxWidgets
target_link_libraries(Chess-client ${wxWidgets_LIBRARIES})
# Comment out if you don't want to print network-related messages into the console
target_compile_definitions(Chess-client PRIVATE PRINT_NETWORK_MESSAGES=1)

# set source files for server-executable
add_executable(Chess-server ${SERVER_SOURCE_FILES})
# set compile directives for server-executable
target_compile_definitions(Chess-server PRIVATE CHESS_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)
# Comment out if you don't want to print network-related messages into the console
target_compile_definitions(Chess-server PRIVATE PRINT_NETWORK_MESSAGES=1)


# linking to sockpp
if(WIN32)
    message("Detected Win32")

    target_link_libraries(Chess-client ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/sockpp-static.lib)
    target_link_libraries(Chess-server ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/sockpp-static.lib)

    # Necessary to get sockets working under Windows (with MingW)
    target_link_libraries(Chess-client wsock32 ws2_32)
else()
    message("Not Win32, so probably a Linux") # We assume it's Linux in this case

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    target_link_libraries(Chess-client ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/libsockpp.so Threads::Threads)
    target_link_libraries(Chess-server ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/libsockpp.so Threads::Threads)
endif()

# copy assets (images) to binary directory
file(INSTALL assets DESTINATION ${CMAKE_BINARY_DIR})


set(CMAKE_CXX_FLAGS "--coverage")
# set source files for server-library
add_library(Chess-lib ${SERVER_SOURCE_FILES})
# set compile directives for server-library
target_compile_definitions(Chess-lib PRIVATE CHESS_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)

add_subdirectory(googletest)
add_subdirectory(unit-tests)

