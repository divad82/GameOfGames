#ifndef  GAME_H
#define  GAME_H

#include "board.h"
#include <string>
#include <vector>
class IPlayer;
/**
  Game driver class, stores the board and records each individual move.

  Using a state-machine the Game-object interacts with the players and
  updates the board state
  */
class  Game {
public:
    Game(const std::wstring &name, Board2D b,
         const std::vector<IPlayer*> &pl,
         const std::vector<Board2D::Move> &possible);
    IPlayer* getNextPlayer();
    IPlayer* getLastPlayer();
    std::vector<Board2D::Move> getBasicMoves() ;
    enum State {  eMove, eWinQ, eDrawQ, eSwitch, eValidate, eLoseQ, eUpdateState, eEnd};
    IPlayer * begin();
    State next(State state);
    Board2D getBoard() const;
    void    LogCurrent();
private:
    std::wstring m_name;
    std::vector<IPlayer*> m_players;
    IPlayer * m_winner;
    Board2D m_board;

    int m_index;
    bool m_draw;
    std::vector<Board2D::Move> m_available;
    std::vector<Board2D::Move> m_turns;
    std::vector<Board2D::Move> m_currentTurn;
};
#endif

