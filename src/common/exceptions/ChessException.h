#ifndef CHESS_CHESSEXCEPTION_H
#define CHESS_CHESSEXCEPTION_H

#include <string>

class ChessException : public std::exception {
private:
    std::string _msg;
public:
    explicit ChessException(const std::string& message) : _msg(message) { };

    const char* what() const noexcept override {
        return _msg.c_str();
    }
};

#endif //CHESS_CHESSEXCEPTION_H
