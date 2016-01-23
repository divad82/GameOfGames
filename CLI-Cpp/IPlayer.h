#ifndef IPLAYER_H
#define IPLAYER_H
#include "game.h"
#include <string>
#include "board.h"
/**
  Abstract interface for interaction with the game and player

  Supporting the "friendly game" concept it's the player's responiblity
  to evaluate their opponents' moves, the state of the game, and submit their moves
  to be evaluated in turn
  */
class IPlayer {
public:
    virtual bool            isDone() =0;
    virtual bool            isDraw() =0;
    virtual bool            setGame(Game * inG)=0;
    virtual bool            isValid(std::vector<Board2D::Move> m) =0;
    virtual IPlayer *       hasWon(IPlayer * player = NULL) = 0;
    virtual std::wstring    getName() =0;
    virtual Board2D::Move   go()=0;
    virtual void            congratulate(const std::vector<Board2D::Move> &turns)=0;
};

#endif // IPLAYER_H
