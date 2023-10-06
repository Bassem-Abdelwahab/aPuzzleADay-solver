#include "puzzle_piece.hpp"

void puzzle_piece::set_pieceName(char pieceName)
{
    this->m_pieceName = pieceName;
}

char puzzle_piece::get_pieceName()
{
    return this->m_pieceName;
}

void puzzle_piece::set_pieceShape(std::string * strPtr, coord dimensions)
{
    this->m_dimensions = dimensions;
    this->m_filledCount =0;
    if(strPtr != nullptr)
    {
        this->m_currentFilled = 0;
        for (unsigned int i = 0; i < dimensions.row; i++)
        {
            for (unsigned int j = 0; j < dimensions.col; j++)
            {
                if(strPtr[i][j] !=' ')
                {
                    this->m_filled[this->m_currentFilled].row=i;
                    this->m_filled[this->m_currentFilled].col=j;
                    this->m_currentFilled++;
                }
            }
        }
        this->m_filledCount = this->m_currentFilled;
    }
    this->m_currentFilled = 0;
}

void puzzle_piece::set_shapeSymmetry(symmetry_t shapeSymmetry)
{
    this->m_currentOrientation = 2;
    this->m_orientations[0] = NOR_0;
    this->m_orientations[1] = NOR_90;
    if(shapeSymmetry == NO_SYM || shapeSymmetry == MIR_SYM)
    {
        this->m_orientations[this->m_currentOrientation] = NOR_180;
        this->m_orientations[this->m_currentOrientation+1] = NOR_270;
        this->m_currentOrientation += 2;
    }
    if(shapeSymmetry == NO_SYM || shapeSymmetry == ROT_SYM)
    {
        for (unsigned int i = 0; i < this->m_currentOrientation; i++)
        {
            this->m_orientations[this->m_currentOrientation+i] = (orientation_t)(MIR_0 + this->m_orientations[i]);
        }
        this->m_currentOrientation *= 2;
    }
    this->m_orientationsCount = this->m_currentOrientation;
    this->m_currentOrientation = 0;
}

bool puzzle_piece::get_next_orientation()
{
    bool result = false;
    this->m_currentFilled = 0;
    if(this->m_currentOrientation < this->m_orientationsCount-1)
    {
        this->m_currentOrientation++;
        result = true;
    }
    return result;
}

void puzzle_piece::set_currentOrientation(unsigned int index)
{
    if( index < this->m_orientationsCount)
    {
        this->m_currentFilled = 0;
        this->m_currentOrientation = index;
    }
}

unsigned int puzzle_piece::get_currentOrientation()
{
    return this->m_currentOrientation;
}

coord puzzle_piece::transformed_point()
{
    coord result = {0,0};
    switch(this->m_orientations[m_currentOrientation])
    {
        case NOR_0:
        result.row = this->m_filled[this->m_currentFilled].row;
        result.col = this->m_filled[this->m_currentFilled].col;
        break;
        
        case NOR_90:
        result.row = this->m_filled[this->m_currentFilled].col;
        result.col = this->m_dimensions.row-this->m_filled[this->m_currentFilled].row-1;
        break;

        case NOR_180:
        result.row = this->m_dimensions.row-this->m_filled[this->m_currentFilled].row-1;
        result.col = this->m_dimensions.col-this->m_filled[this->m_currentFilled].col-1;
        break;
        
        case NOR_270:
        result.row = this->m_dimensions.col-this->m_filled[this->m_currentFilled].col-1;
        result.col = this->m_filled[this->m_currentFilled].row;
        break;

        case MIR_0 :
        result.row = this->m_filled[this->m_currentFilled].row;
        result.col = this->m_dimensions.col-this->m_filled[this->m_currentFilled].col-1;
        break;
        
        case MIR_90 :
        result.row = this->m_dimensions.col-this->m_filled[this->m_currentFilled].col-1;
        result.col = this->m_dimensions.row-this->m_filled[this->m_currentFilled].row-1;
        break;

        case MIR_180 :
        result.row = this->m_dimensions.row-this->m_filled[this->m_currentFilled].row-1;
        result.col = this->m_filled[this->m_currentFilled].col;
        break;

        case MIR_270 :
        result.row = this->m_filled[this->m_currentFilled].col;
        result.col = this->m_filled[this->m_currentFilled].row;
        break;
    }
    return result;
}

bool puzzle_piece::get_next_coord(coord &outCoord)
{
     bool result = false;
    if(this->m_currentFilled < this->m_filledCount)
    {
        outCoord = this->transformed_point();
        this->m_currentFilled++;
        result = true;
    }
    return result;
}

coord puzzle_piece::get_dimensions()
{   
    coord result = {0,0};
    if(this->m_orientations[this->m_currentOrientation] & 1)//odd
    {
        result.row = this->m_dimensions.col;
        result.col = this->m_dimensions.row;
    }
    else
    {
        result.row = this->m_dimensions.row;
        result.col = this->m_dimensions.col;
    }
    return result;
}

void puzzle_piece::reset_shape()
{
    this->m_currentFilled = 0;
    this->m_currentOrientation = 0;
}

puzzle_piece::puzzle_piece(char pieceName ,std::string * strPtr , coord dimensions , symmetry_t shapeSymmetry)
{
    this->set_pieceName(pieceName);
    this->set_pieceShape(strPtr,dimensions);
    this->set_shapeSymmetry(shapeSymmetry);
    this->reset_shape();
}

puzzle_piece::puzzle_piece()
{
    puzzle_piece('a',nullptr,{0,0},NO_SYM);
}