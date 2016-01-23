#include <iostream>
#include <string>
#include "board.h"
#include "cmdPlayer.h"
#include "game.h"
#include "piece.h"

/** Friendly game launcher
  Ideally more of this logic could be pushed into the player interface, however
  the player should be decoupled from the game.
  The system is still easily extendable only requiring an implementation of IPlayer
  with a GUI framework with and a tweaked main.
  */
void salutation(std::wstring &name, int &players);
Board2D setupBoard(const std::wstring &name);


//Greet the user and get the basic defintion of the game
void salutation(std::wstring &name, int &players){
    std::wcout<< "Lets play a Friendly Game" << std::endl;
    std::wcout<<"Where you and your friend agree on the game,"
        <<"just like real world counter parts"<<std::endl;
    std::wcout<<"In theory, future iterations could learn how to play by your example"<<std::endl;
    //TODO: store previous game types and prompt user with a list to choose
    std::wcout<<"What do you call this game(no whitespace please): ";
    std::wcin>>name;
    std::wcout<<"How many players:";
    std::wcin>>players;
}
Board2D setupBoard(const std::wstring &name) {
    std::wcout<<"What size grid does this game take"<<std::endl;

    Board2D::Pt size;
    do {
        std::wcout<<"W: ";
        std::wcin>>size.x;
    }while(size.x<1);
    do {
        std::wcout<<"H: ";
        std::wcin>>size.y;
    }while(size.y<1);

    return Board2D(name, size);
}
int main()
{
    std::wstring name;
    int players;
    salutation(name, players);
    Board2D myBoard = setupBoard(name);
    const std::wstring sentinel(L"~~");
    std::vector<IPlayer*> playerList;
    for (int i =0; i<players; ++i) {
        std::wcout<<"Player "<<i+1<<":"<<std::endl;
        std::wcout<<"Team name: ";
        std::wstring team;
        std::wcin>>team;
        IPlayer * nuPlayer = new CmdLinePlayer(team, &std::wcout, &std::wcin);
        playerList.push_back(nuPlayer);
        std::wcout<<"Setup your pieces, type \'"<<sentinel<<"\' to quit"<<std::endl;
        std::wcout<<"Piece Name: ";
        std::wstring pieceName;
        std::wcin>>pieceName;
        while (pieceName != sentinel) {
            Piece * nuPiece = new Piece(pieceName, nuPlayer);
            std::wcout<<"Where should this piece go:"<<std::endl;
            Board2D::Pt spot;
            do {
                std::wcout<<"X: ";
                std::wcin>>spot.x;
            }while(spot.x<-1);
            do {
                std::wcout<<"Y: ";
                std::wcin>>spot.y;
            }while(spot.y<-1);
            myBoard.placePiece(spot, nuPiece);
            //This would be nice, would require pulling this initialization into the player
//            std::wcout<<"View Board? (Y/N)"<<std::endl;
//            std::wstring ans = L"";
//            std::wcin>>ans;
//            if (ans == sentinel)
//                break;
//            else if(ans ==L"Y") {
//            }
            std::wcout<<"Piece Name: ";
            std::wcin>>pieceName;
        }
    }
    std::vector<Board2D::Move> moves;
    std::wcout<<L"What are the possible moves? (type \'"<<sentinel<<"\' when done)"<<std::endl;
    std::wstring m;
    do {
        std::wcout<<L">";
        std::wcin>>m;
        if (m != sentinel)
            moves.push_back(Board2D::Move(m));
    }while (m!=sentinel);

    Game theGame(name, myBoard, playerList, moves);
    theGame.begin();

    return 0;
}

