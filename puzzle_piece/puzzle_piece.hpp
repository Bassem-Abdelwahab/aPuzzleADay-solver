#ifndef PUZZLE_PIECE_HPP_
#define PUZZLE_PIECE_HPP_

#include <string>

#include "../helping_structure.hpp"

enum orientation_t {NOR_0=0 , NOR_90=1 , NOR_180=2 , NOR_270=3 , 
                    MIR_0=4 , MIR_90=5 , MIR_180=6 , MIR_270=7};
enum symmetry_t {NO_SYM = 0 , MIR_SYM=1 , ROT_SYM=2 , M_R_SYM = 3};

class puzzle_piece
{
    private:
    char m_pieceName;
    coord m_filled[6];
    coord m_dimensions;
    orientation_t m_orientations[8];
    unsigned int m_currentFilled:4;
    unsigned int m_filledCount:4;
    unsigned int m_currentOrientation:4;
    unsigned int m_orientationsCount:4;
    coord transformed_point();
    public:

    puzzle_piece();
    puzzle_piece(char pieceName , std::string * strPtr , coord dimensions , symmetry_t shapeSymmetry);
    void set_pieceName(char pieceName);
    char get_pieceName();
    void set_pieceShape(std::string * strPtr, coord dimensions);
    void set_shapeSymmetry(symmetry_t shapeSymmetry);
    bool get_next_orientation();
    bool get_next_coord(coord &outCoord);
    void set_currentOrientation(unsigned int index);
    unsigned int get_currentOrientation();
    coord get_dimensions();
    void reset_shape();

};

#endif // !PUZZLE_PIECE_HPP_
