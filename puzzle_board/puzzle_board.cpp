#include "puzzle_board.hpp"

puzzle_board::puzzle_board()
{
    this->reset_board();
}

bool puzzle_board::try_to_fit(puzzle_piece pieceToFit, coord pieceLocation)
{
    char toFill = pieceToFit.get_pieceName();
    coord parts[6];
    unsigned int i = 0;
    bool canFit = true;
    while (canFit && pieceToFit.get_next_coord(parts[i]))
    {
        parts[i].row += pieceLocation.row;
        parts[i].col += pieceLocation.col;
        if (parts[i].row > 6 || parts[i].col > 6 || this->m_board[parts[i].row][parts[i].col] != '.')
        {
            canFit = false;
        }
        i++;
    }
    if (canFit)
    {
        while (i > 0)
        {
            i--;
            this->m_board[parts[i].row][parts[i].col] = toFill;
        }
    }
    return canFit;
}

void puzzle_board::remove_shape(puzzle_piece pieceToRem, coord pieceLocation)
{
    char pieceName = pieceToRem.get_pieceName();
    coord tempCoord = {0, 0};
    unsigned int i = 0;
    bool canFit = true;
    while (pieceToRem.get_next_coord(tempCoord))
    {
        tempCoord.row += pieceLocation.row;
        tempCoord.col += pieceLocation.col;
        if (tempCoord.row < 7 && tempCoord.col < 7 && pieceName == this->m_board[tempCoord.row][tempCoord.col])
        {
            this->m_board[tempCoord.row][tempCoord.col] = '.';
        }
    }
}

void puzzle_board::reset_board()
{
    this->m_board[0] = "...... ";
    this->m_board[1] = "...... ";
    this->m_board[2] = ".......";
    this->m_board[3] = ".......";
    this->m_board[4] = ".......";
    this->m_board[5] = ".......";
    this->m_board[6] = "...    ";
}

std::string puzzle_board::board_to_string()
{
    std::string result = "";
    for (int i = 0; i < 7; i++)
    {
        result += this->m_board[i] + "\n";
    }
    return result;
}