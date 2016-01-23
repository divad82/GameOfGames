#include "piece.h"
/*class MoveStrategy {
public:
    MoveStrategy();
    //Eventually this should be a container with  potential
    //moves

};//*/

Piece::Piece(std::wstring name,IPlayer * owner,
      Size sz/* = Size(1,1)*/)//,
      //MoveStrategy *ms/* = NULL*/)m_moves(ms),
    : m_name(name), m_size(sz),  m_owner(owner)
{
}

std::wstring Piece::getName() const {
    return m_name;
}
//MoveStrategy * Piece::getMoves() const{
//    return m_moves;
//}
Size Piece::getSize() const {
    return m_size;
}
IPlayer * Piece::getOwner() const {
    return m_owner;
}
