#ifndef BOARD2D_H
#define BOARD2D_H
#include <map>
#include <vector>
#include <string>
#include <math.h>
class Piece;
/**
  Abstract two dimensional game board.  Should be able to describe the state
  for any grid based game: Tic Tac Toe, Chess, Checkers, Go, Othello, e.g.
  Stern-Halma (aka "Chinese Checkers" http://en.wikipedia.org/wiki/Chinese_checkers)
  might be possible, but it would be an imperfect fit.

  Since we don't programatically validate the moves and state, we do not need
  to store empty spaces.  Instead of using 2 dimensional arrays we can store the
  board state by creating a key value pair with the location and the piece.
  */
class Board2D {
public:
    //STRUCTURES and TYPES
    //Light weight Pt coordinate class
    struct Pt {
        Pt(int a=-1,int b=-1) : x(a), y(b) {}
        int x;
        int y;
        bool operator==(const Pt &rhs) {
            return rhs.x ==x && rhs.y == y;
        }
    };
    //Comparsion struct to order the Map
    struct ptLT {
      bool operator() (const Pt& lhs, const Pt& rhs) const
      { //pt closest to the orgin is smaller
          //if they are the same distance, LHS is smaller
          //we can save some clocks by comparing their non square rooted values
          //          return pow(lhs.x, 2) + pow(lhs.y,2) <pow(rhs.x,2) + pow(rhs.y,2);
          if (lhs.x==rhs.x) {
              return lhs.y < rhs.y;
          } else {
              return lhs.x < rhs.x;
          }
      }
    };

    //A move defintion all encapsulated in one struct
    struct Move {
        Move(const std::wstring & n)
            :name(n)
        {}
        Pt from;
        Pt to;
        std::wstring name;
    };
    typedef std::pair<Pt, Piece*> BoardSpot;
    typedef std::map<Pt, Piece*, Board2D::ptLT> BoardState;
    typedef std::vector<Piece*> Directory;

public://METHODS
    //Constructor, we insist on a name  for future serialization
    //Dimension of the board is equivilent to the bottom right point
    //location
    Board2D(const std::wstring &in_name, Pt dimensions);
    //Copy constructor
    Board2D(const Board2D &that);
    //Destructor
    ~Board2D();
    //Accessor methods
	std::wstring getName() const;
    Pt getSize() const;    
    Piece* whatAt(Pt spot) const;
    //Board state is a map of points to pieces, if there isn't a
    //entry at that Pt then that point is empty
    BoardState getState() const;
    //Grave Yard is a term in many games for pieces that are off board
    //Chess and checkers require it to be accurately recorded for
    //pawn redemption and "king-ing"
    Directory getGraveYard() const;
    //Modifiers
    bool    placePiece  (Pt spot, Piece * newPiece);
    bool    removePiece (Pt Spot);
    bool    movePiece   (Pt from, Board2D::Pt to);


private:
    std::wstring m_name;
    Pt	m_size;
    BoardState m_state;
    Directory m_graveYard;
    const Pt m_grave;       //sentinel defined as address for "graveyard" point location
};



#endif
