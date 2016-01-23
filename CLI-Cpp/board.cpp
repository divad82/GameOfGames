
#include "board.h"
#include "piece.h"
#include <iostream>

Board2D::Board2D(const std::wstring &in_name, Board2D::Pt dimensions) :
    m_name(in_name), m_size(dimensions), m_grave(-1,-1)
{
}
Board2D::Board2D(const Board2D &that) {
  m_name = that.m_name;
  m_size = that.m_size;
  m_state.clear();
  if (!that.m_state.empty()) {
      BoardState::const_iterator iter = that.m_state.begin();
      BoardState::const_iterator end = that.m_state.end();
      for (; iter != end; ++iter) {
          if ((*iter).second) {
              m_state.insert(BoardSpot((*iter).first, new Piece(*(*iter).second)));

          }
      }
  }
  m_graveYard.clear();
  if (!m_graveYard.empty()) {
      Directory::const_iterator diter = that.m_graveYard.begin();
      Directory::const_iterator dend = that.m_graveYard.end();
      for (; diter != dend; ++diter) {
          if (*diter) {
              m_graveYard.push_back(new Piece(*(*diter)));
          }
      }
  }

}

Board2D::~Board2D() {
    if (!m_state.empty()) {
        BoardState::iterator iter = m_state.begin();
        BoardState::iterator end = m_state.end();
        for (; iter != end; ++iter) {
            if ((*iter).second) {
                delete (*iter).second;
                (*iter).second =NULL;
            }
        }
        m_state.clear();
    }
    if (!m_graveYard.empty()) {
        Directory::iterator diter = m_graveYard.begin();
        Directory::iterator dend = m_graveYard.end();
        for (; diter != dend; ++diter) {
            if (*diter) {
                delete *diter;
                *diter =NULL;
            }
        }
        m_graveYard.clear();
    }
}
std::wstring Board2D::getName() const {
    return m_name;
}
Board2D::Pt Board2D::getSize() const {
    return m_size;
}
Board2D::BoardState Board2D::getState() const {
    return m_state;
}
Piece* Board2D::whatAt(Board2D::Pt spot) const {
    return NULL; //return m_state[spot];
}
#define DEBUG_CHANGES 0
bool    Board2D::placePiece(Board2D::Pt spot, Piece * newPiece) {
    if (spot == m_grave) {
        m_graveYard.push_back(newPiece); //insert(Plot(newPiece.name, newPiece));
        return true;
    }
    BoardState::iterator res = m_state.find(spot);
    if (res == m_state.end()) { //nothing there ok, Board2D::ptComp
        m_state.insert(std::pair<Board2D::Pt, Piece*>(spot, newPiece));
#if DEBUG_CHANGES
        std::cout<<"placed"<<std::endl;
#endif
        return true;//d@h WHY AREN"T THESE UNIQUE?
    }
#if DEBUG_CHANGES
    std::cout<<"failed place"<<std::endl;
#endif
    return false;
}
bool    Board2D::removePiece(Board2D::Pt spot) {
    BoardState::iterator res = m_state.find(spot);
    if (res != m_state.end()) { //yay found something
        m_graveYard.push_back((*res).second);
        m_state.erase(res);
#if DEBUG_CHANGES
        std::cout<<"removed"<<std::endl;
#endif
        return true;
    }
#if DEBUG_CHANGES
    std::cout<<"failed remove"<<std::endl;
#endif
    return false;
}
bool    Board2D::movePiece(Board2D::Pt from, Board2D::Pt to) {
    BoardState::iterator org = m_state.find(from);
    if (org != m_state.end()) { //yay found something
        BoardState::iterator dst = m_state.find(to);
        if (dst == m_state.end()) {//nothing there ok
            m_state.erase(dst);
            m_state.insert(std::pair<Board2D::Pt, Piece*>(to, (*org).second));
#if DEBUG_CHANGES
            std::cout<<"moved"<<std::endl;
#endif
            return true;
        }
    }
#if DEBUG_CHANGES
    std::cout<<"failed move"<<std::endl;
#endif
    return false;
}
Board2D::Directory Board2D::getGraveYard() const {
    return m_graveYard;
}
