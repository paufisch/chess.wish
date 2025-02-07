#include "GameController.h"
#include "../common/network/requests/JoinGameRequest.h"
#include "../common/network/requests/StartGameRequest.h"
#include "../common/network/requests/ResignRequest.h"
#include "../common/network/requests/MovePieceRequest.h"
#include "network/ClientNetworkManager.h"


// initialize static members
GameWindow* GameController::_gameWindow = nullptr;
ConnectionPanel* GameController::_connectionPanel = nullptr;
MainGamePanel* GameController::_mainGamePanel = nullptr;

Player* GameController::_me = nullptr;
GameState* GameController::_currentGameState = nullptr;



void GameController::init(GameWindow* gameWindow) {

    GameController::_gameWindow = gameWindow;

    // Set up main panels
    GameController::_connectionPanel = new ConnectionPanel(gameWindow);
    GameController::_mainGamePanel = new MainGamePanel(gameWindow);

    // Hide all panels
    GameController::_connectionPanel->Show(false);
    GameController::_mainGamePanel->Show(false);

    // Only show connection panel at the start of the game
    GameController::_gameWindow->showPanel(GameController::_connectionPanel);

    // Set status bar
    GameController::showStatus("Not connected");
}


void GameController::connectToServer() {

    // get values form UI input fields
    wxString inputServerAddress = GameController::_connectionPanel->getServerAddress().Trim();
    wxString inputServerPort = GameController::_connectionPanel->getServerPort().Trim();
    wxString inputPlayerName = GameController::_connectionPanel->getPlayerName().Trim();

    // check that all values were provided
    if(inputServerAddress.IsEmpty()) {
        GameController::showError("Input error", "Please provide the server's address");
        return;
    }
    if(inputServerPort.IsEmpty()) {
        GameController::showError("Input error", "Please provide the server's port number");
        return;
    }
    if(inputPlayerName.IsEmpty()) {
        GameController::showError("Input error", "Please enter your desired Player name");
        return;
    }

    // convert host from wxString to std::string
    std::string host = inputServerAddress.ToStdString();

    // convert port from wxString to uint16_t
    unsigned long portAsLong;
    if(!inputServerPort.ToULong(&portAsLong) || portAsLong > 65535) {
        GameController::showError("Connection error", "Invalid port");
        return;
    }
    uint16_t port = (uint16_t) portAsLong;

    // convert Player name from wxString to std::string
    std::string playerName = inputPlayerName.ToStdString();

    // connect to network
    ClientNetworkManager::init(host, port);

    // send request to join game
    GameController::_me = new Player(playerName);
    JoinGameRequest request = JoinGameRequest(GameController::_me->get_id(), GameController::_me->get_player_name());
    ClientNetworkManager::sendRequest(request);

}


void GameController::updateGameState(GameState* newGameState) {

    // save the new game state as our current game state
    GameController::_currentGameState = newGameState;


    // make sure we are showing the main game panel in the window (if we are already showing it, nothing will happen)
    GameController::_gameWindow->showPanel(GameController::_mainGamePanel);


    if (GameController::_currentGameState->get_players().size() == 2) {
        // command the main game panel to rebuild itself, based on the new game state
        GameController::_mainGamePanel->buildGameState(GameController::_currentGameState, GameController::_me);
    }

    
    if(GameController::_currentGameState->is_finished()) {
        GameController::showGameOverMessage();
    }
}


void GameController::startGame() {
    StartGameRequest request = StartGameRequest(GameController::_currentGameState->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}


void GameController::resign() {
    ResignRequest request = ResignRequest(GameController::_currentGameState->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}


void GameController::movePiece(int from_i, int from_j, int i, int j) {
    //i,j are indices of the destination panel
    //i,j are the indices of the selected piece
    MovePieceRequest request = MovePieceRequest(GameController::_currentGameState->get_id(), GameController::_me->get_id(), from_i, from_j, i, j);
    ClientNetworkManager::sendRequest(request);
}


wxEvtHandler* GameController::getMainThreadEventHandler() {
    return GameController::_gameWindow->GetEventHandler();
}


void GameController::showError(const std::string& title, const std::string& message) {
    wxMessageBox(message, title, wxICON_ERROR);
}


void GameController::showStatus(const std::string& message) {
    GameController::_gameWindow->setStatus(message);
}


void GameController::showGameOverMessage() {
    std::string title;
    std::string message;
    if (_currentGameState->get_loser()->get_id() == _me->get_id()) {
        title = "You lost!";
        if (_currentGameState->is_resigned()) {
            message = "you resigned";
        } else if (_currentGameState->get_round_number() == _currentGameState->get_max_number_rounds()){
            message = "your king died of old age";
        } else {
            message = "your opponent murdered your king";
        }
    } else {
        title = "You won!";
        if (_currentGameState->is_resigned()) {
            message = "your opponent resigned";
        } else if (_currentGameState->get_round_number() == _currentGameState->get_max_number_rounds()) {
            message = "your opponents king died of old age";
        } else if (_currentGameState->get_players().size() == 1) {
            message = "your opponent left the game or lost connection";
        } else {
            message = "you murdered your opponents king";
        }
    }
/*
    wxMessageDialog dialogBox = wxMessageDialog(nullptr, message, title, wxICON_NONE);
    dialogBox.SetOKLabel(wxMessageDialog::ButtonLabel("Close Game"));
    int buttonClicked = dialogBox.ShowModal();
    if(buttonClicked == wxID_OK) {
        GameController::_gameWindow->Close();
    }
    */


    wxMessageDialog dialogBox = wxMessageDialog(nullptr, message, title, wxICON_NONE|wxYES_NO);
    dialogBox.SetYesNoLabels(wxMessageDialog::ButtonLabel("Start new Game"), wxMessageDialog::ButtonLabel("Close Game"));
    int buttonClicked = dialogBox.ShowModal();
    if(buttonClicked == wxID_NO) {
        GameController::_gameWindow->Close();
    } else if (buttonClicked == wxID_YES) {
        GameController::startGame();
    }

}