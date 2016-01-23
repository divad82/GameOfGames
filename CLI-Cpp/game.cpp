#include "game.h"
#include "piece.h"
#include "IPlayer.h"
Game::Game(const std::wstring &name, Board2D b,
           const std::vector<IPlayer*> &pl,
           const std::vector<Board2D::Move> &possible)
    : m_name(name), m_board(b)
    , m_players(pl), m_winner(NULL)
    , m_index(0), m_draw(false)
    , m_available(possible)
{
    if (m_available.empty())
        m_available.push_back(Board2D::Move(L"Move"));
}
IPlayer* Game::getNextPlayer() {
    m_index = (++m_index) % m_players.size();
    return m_players[m_index];
}
IPlayer* Game::getLastPlayer() {
    --m_index;
    int sz =m_players.size();
    if (m_index < 0)
        m_index =  sz-1;
    m_index %= sz;
    return m_players[m_index];
}
std::vector<Board2D::Move> Game::getBasicMoves() {
    return m_available;
}
IPlayer * Game::begin() {
    for (int x =0; x<(int)m_players.size(); ++x) {
        m_players[x]->setGame(this);
    }
    next(eMove);
    return m_winner;
}
void    Game::LogCurrent() {
   //XM L or JSON
//can't do it this way
    //m_turns.insert(m_turns.end(), m_currentTurn.begin(), m_currentTurn.end());
}

Board2D Game::getBoard() const{
    return m_board;
}
Game::State Game::next(Game::State state) {
    while(true) { //break out at the end
        IPlayer * currentPlayer = m_players[m_index];
        IPlayer * lastPlayer=NULL;
        switch(state) {
            case eEnd: {
            m_winner->congratulate(m_turns);
            return eEnd; break;
        }
            case eMove:
            {
                Board2D::Move m = currentPlayer->go();
                m_currentTurn.push_back(m);
                if(currentPlayer->isDone()) {
                    state = eWinQ;
                }
                break;
            }
            case eWinQ:
                m_winner = (currentPlayer->hasWon()) ? currentPlayer : NULL;
                if (m_winner)
                    state = eSwitch;
                else
                    state = eDrawQ;
                break;
            case eDrawQ:
                m_draw = currentPlayer->isDraw();
                state = eSwitch;
                break;
            case eSwitch:
                lastPlayer = currentPlayer;
                getNextPlayer();
                state = eValidate;
                break;
            case eValidate:
                if (currentPlayer->isValid(m_currentTurn)) {
                    state = eLoseQ;
                } else {
                    state = eMove;
                    //TODO argue?
                    m_currentTurn.clear();
                    getLastPlayer();
                }
                break;
            case eLoseQ:
                //this is where the other player
                //agrees or disagrees with the players assement
                state =eUpdateState;
                if (currentPlayer->hasWon(m_winner)==m_winner) {
                    if(m_winner){
                        state =eEnd;
                    } else {//(winner == NULL) {
                        if (m_draw == currentPlayer->isDraw()){
                            if (m_draw){
                                state =eEnd;
                                //winner is null.. and yet we ended.. ok
                                //cat's game..
                            }
                        }else {
                           ; //state = argue(current, last)
                        }
                    }
                }else {
                    ;//state = argue(current, last);
                }
                break;
            case eUpdateState:
            {
                state = eMove;
                for (int x =0; x<(int)m_currentTurn.size(); ++x) {
                    Board2D::Move target = m_currentTurn[x];
                    if (m_board.movePiece(target.from, target.to)){
                        m_turns.push_back(target);
                    }else if (lastPlayer && m_board.placePiece(target.to, new Piece(lastPlayer->getName(), lastPlayer))) {
                        m_turns.push_back(target);
                    }else {
                        break;//yellow card - argue with ref
                    }
                }
                m_currentTurn.clear();
            }
        }
    }
    return state;
}
/*
  need to migrate from GAME object into the refree concept
    need to change a CLpayer to do "Moves" instead of pick ups and downs

  need a better graveyard /place unit concept..
  checkers might be a better test../tic tac toe
  serialize board
then hope this all compiles
  */
