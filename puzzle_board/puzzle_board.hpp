#ifndef PUZZLE_BOARD_HPP_
#define PUZZLE_BOARD_HPP_
#include <string>
#include "../puzzle_piece/puzzle_piece.hpp"
#include "../helper_functions.hpp"

class puzzle_board
{
private:
    std::string m_board[7];

public:
    puzzle_board();
    bool try_to_fit(puzzle_piece pieceToFit, coord pieceLocation);
    void remove_shape(puzzle_piece pieceToRem, coord pieceLocation);
    void reset_board();
    std::string board_to_string();
};

#endif // !PUZZLE_BOARD_HPP_