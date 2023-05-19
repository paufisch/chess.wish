#ifndef LAMAUI_GAMECONTROLLER_H
#define LAMAUI_GAMECONTROLLER_H

#include "windows/GameWindow.h"
#include "panels/ConnectionPanel.h"
#include "panels/MainGamePanel.h"
#include "network/ResponseListenerThread.h"
#include "../common/game_state/game_state.h"


class GameController {

public:
    static void init(GameWindow* gameWindow);
    static void connectToServer();
    static void updateGameState(game_state* newGameState);
    static void startGame();
    static void resign();
    static wxEvtHandler* getMainThreadEventHandler();
    static void showError(const std::string& title, const std::string& message);
    static void showStatus(const std::string& message);
    static void showGameOverMessage();
    static void selectPiece(int i, int j);
    static void movePiece(int from_i, int from_j, int i, int j);

private:
    static GameWindow* _gameWindow;
    static ConnectionPanel* _connectionPanel;
    static MainGamePanel* _mainGamePanel;
    static player* _me;
    static game_state* _currentGameState;

};


#endif //LAMAUI_GAMECONTROLLER_H
