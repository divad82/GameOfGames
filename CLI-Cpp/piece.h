#ifndef PIECE_H
#define PIECE_H
#include <string>
#include "IPlayer.h"
#include "board.h"
//class MoveStrategy;

typedef Board2D::Pt Size;
/**
  Piece object curerntly just manages its
  owner, size, and name.

  In the future a strategy or policy could be used to enforce a piece's
  movement. For example, chess' Knight piece would require a definition of
  its L-move style

  */
class Piece {
public:
    Piece(std::wstring name, IPlayer * owner, Size sz = Size(1,1)); //, MoveStrategy *ms = NULL);
    virtual std::wstring getName() const;
    //virtual MoveStrategy * getMoves() const;
    virtual Board2D::Pt getSize() const;
    virtual IPlayer * getOwner() const;

private:
    std::wstring m_name;
    IPlayer* m_owner;
    Size m_size;
    //MoveStrategy * m_moves;
};

#endif
