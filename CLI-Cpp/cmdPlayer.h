#ifndef CMDLINEPLAYER_H
#define CMDLINEPLAYER_H
#include "IPlayer.h"
#include <iostream>
/**
  Implementation of the IPlayer interface.

  Using ascii art and command line prompts, the player interacts with the overall game
  */
class CmdLinePlayer : public IPlayer {
public:

    CmdLinePlayer(const std::wstring &name
                  , std::wostream *out
                  , std::wistream *in);

    bool    isDone();
    bool    isDraw();
    bool    isValid(std::vector<Board2D::Move> m) ;
    IPlayer *       hasWon  (IPlayer * player = NULL);
    std::wstring    getName ();
    bool    setGame(Game * inG);
    Board2D::Move   go      ();
    void            congratulate(const std::vector<Board2D::Move> &turns);
private:
    void asciiBoardState();
    std::wstring m_name;
    std::wostream * m_output;
    std::wistream * m_input;
    Game *m_game;
    bool yesNoQuestion();

};

#endif

