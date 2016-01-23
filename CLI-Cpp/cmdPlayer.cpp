#include "cmdPlayer.h"
#include "piece.h"


CmdLinePlayer::CmdLinePlayer(const std::wstring &name
                             , std::wostream *out
                             , std::wistream *in)
    : m_name(name)
    , m_output(out)
    , m_input(in)
    , m_game(NULL)
{

}
/*
CmdLinePlayer::CmdLinePlayer(const CmdLinePlayer & other) {
    this->m_name = other.m_name;
    this->m_input =other.m_input;
    this->m_output =other.m_output;
}
*/
bool CmdLinePlayer::isDone() {
    *m_output<<m_name<<L"- Are you done?(Y/N)"<<std::endl;
    return yesNoQuestion();
}

IPlayer * CmdLinePlayer::hasWon(IPlayer * player /*= NULL*/) {

    if (player) {
        *m_output<<m_name<<L"- Did "<<player->getName()<<" win?(Y/N)"<<std::endl;
        if (yesNoQuestion()){
            return player;
        }
    }
    *m_output<<m_name<<L"- Did you win?(Y/N)"<<std::endl;
    bool ans = yesNoQuestion();
    if (ans)
        return this;

    return NULL;
}
void CmdLinePlayer::congratulate(const std::vector<Board2D::Move> &turns) {
    *m_output<<"Hurray! "<<m_name<<" won!"<<std::endl;
    int size = (int)turns.size();
    *m_output<<"In only "<<size<<" move(s)"<<std::endl;
    for (int x =0; x<size; ++x) {
        Board2D::Move move = turns[x];
        *m_output<<L"Turn "<<x+1<<": "<<std::endl;
        *m_output<<move.name<<L" went from: ("<<move.from.x<<L", "<<move.from.y<<L")"<<std::endl;
        *m_output<<L"to ("<<move.to.x<<L", "<<move.to.y<<L")"<<std::endl;
    }
    asciiBoardState();
}

bool    CmdLinePlayer::isDraw() {
    *m_output<<m_name<<L"- Is it a draw(Y/N)"<<std::endl;
    return yesNoQuestion();
}

bool    CmdLinePlayer::isValid(std::vector<Board2D::Move> m) {
  //should probably let the player request the board state again
    *m_output<<m_name<<L"- Are the above moves valid?(Y/N)"<<std::endl;
    return yesNoQuestion();

}

std::wstring CmdLinePlayer::getName() {
    return m_name;
}

bool  CmdLinePlayer::setGame(Game *inGame) {
    if(m_game ==NULL) {
        if (inGame) {
            m_game = inGame;
            return true;
        }
    }
    return false;
}
Board2D::Move CmdLinePlayer::go() {
    if (!m_game)
        throw std::exception(); //should probably define an exception for "What game am I playing"
    std::vector<Board2D::Move> mlist = m_game->getBasicMoves();
    *m_output<<m_name<<L"- your turn, choose an option"<<std::endl;
    int size =mlist.size();
    for (int x =0; x<size; ++x) {
        *m_output<<L"\t"<<x+1<<": "<<mlist[x].name<<std::endl;
    }
    *m_output<<L"\t"<<0<<": Display board"<<std::endl;
    *m_output<<L"\t"<<-9<<": Quit"<<std::endl;
    int choice;
    *m_input>>choice;
    while (choice<1 || choice>size) {
        if (choice== 0) {
            asciiBoardState();
        }else if (choice == -9) {
            *m_output<<"QUITTER!"<<std::endl;
            return Board2D::Move(L"Quit");
        }else {
            *m_output<<L"Invalid option, please try again"<<std::endl;
        }
        *m_input>>choice;
    }
    Board2D::Move move = mlist[choice];
    //move name is only for serialization
    Board2D::Pt dim = m_game->getBoard().getSize();
    *m_output<<L"The board is "<<dim.x<<L" by "<<dim.y<<std::endl;
    *m_output<<L"From: (use -1 for off board)"<<std::endl;
    do {
        *m_output<<L"X:"<<std::endl;
        *m_input>>move.from.x;
    }while(move.from.x > dim.x || move.from.x<-1);
    do {
        *m_output<<L"Y:"<<std::endl;
        *m_input>>move.from.y;
    }while(move.from.y > dim.y || move.from.y<-1);

    *m_output<<L"To: (use -1 for off board)"<<std::endl;
    do {
        *m_output<<L"X:"<<std::endl;
        *m_input>>move.to.x;
    }while(move.to.x > dim.x || move.to.x<-1);
    do {
        *m_output<<L"Y:"<<std::endl;
        *m_input>>move.to.y;
    }while(move.to.y > dim.x || move.to.y<-1);


    return move;
}
#define DEBUG_ASCII_ART 0
void CmdLinePlayer::asciiBoardState() {
    if (!m_game)
        return;
    Board2D b =m_game->getBoard();
    Board2D::BoardState bs = b.getState();
    Board2D::Pt dim =b.getSize();
    //quick dump
    Board2D::BoardState::const_iterator bsIter = bs.begin();
#if DEBUG_ASCII_ART
    for (;bsIter != bs.end(); ++bsIter) {
        *m_output<<"("<<(*bsIter).first.x<<", "<<(*bsIter).first.y<<") is "<<(*bsIter).second->getName()<<std::endl;

    }
#endif
    /*****
      x-.-0--|-1--|-2--.
      0 |____|____|____|
      1 |____|____|____|
      2 |____|____|____|
      Legend:?
    ****/
    std::wstring used =L"";
    typedef std::map<std::wstring, wchar_t> Legend;
    Legend key;
    for (int y =0; y<dim.y; ++y) {
        for(int x =0; x<dim.x; ++x) {
            if (x==0 && y==0) {
                //print header
                *m_output<<L"x-.";
                for (int i=0; i<dim.x; ++i) {
                    if (i) *m_output<<L"|";
                    *m_output<<L"-"<<i<<L"--";
                }
                *m_output<<L".\n";
            }
            //Row
#if DEBUG_ASCII_ART
            std::cout<<"Looking for"<<x<<"-"<<y<<std::endl;
#endif
            Board2D::BoardState::const_iterator res = bs.find(Board2D::Pt(x,y));
            wchar_t team = L'_';
            wchar_t sym = L'_';
            if (res != bs.end()) {
#if DEBUG_ASCII_ART
                 std::cout<<"found!"<<std::endl;
#endif
                Piece * p = (*res).second;
                if(p) {
#if DEBUG_ASCII_ART
                    std::cout<<"PIECE\n";
#endif
                    IPlayer * ply = p->getOwner();
                    if (ply) {

#if DEBUG_ASCII_ART
                         std::cout<<"ply\n";
#endif
                        std::wstring nm = ply->getName();

                        team = nm[0];
                    }
                    sym = p->getName()[0]; //need similar unique logic
                }
            }
            if (x == 0)
                *m_output<<y<<" ";
            *m_output<<L"|_"<<team<<sym<<L"_";
        }
        *m_output<<L"|\n";
    }
    //todo graveyard
    *m_output<<L"Pieces Off Board:"<<std::endl;
    //GY
    Board2D::Directory dir = b.getGraveYard();
    for (int z=0; z<(int)dir.size(); ++z) {
        wchar_t team = L'_';
        wchar_t sym = L'_';
        IPlayer * ply = dir[z]->getOwner();
        if (ply) {
            std::wstring nm = ply->getName();
            team = nm[0];
        }
        sym = dir[z]->getName()[0];
        if (z% 5==0)
            *m_output<<std::endl;
        else if(z)
            *m_output<<"-";
        *m_output<<team<<sym;
    }
    *m_output<<"---------------------------------------------"<<std::endl;


}

bool CmdLinePlayer::yesNoQuestion() {
    wchar_t ans = L'N';
    *m_input>>ans;
    ans = towupper(ans);
    return ans == L'Y';
}
