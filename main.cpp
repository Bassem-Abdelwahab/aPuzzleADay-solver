#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <algorithm> 
#include <time.h>
#include <stdlib.h>

#include "solutions_DB.h"

#define CAL_SIZE 59
#define NUM_PCS 9
#define CAL_ROW 8
#define CAL_COL 7
#define TOTAL_ELEMENTS 50
#define POSSIBILITIES TOTAL_ELEMENTS*(TOTAL_ELEMENTS-1)*(TOTAL_ELEMENTS-2)
static const std::string boardConfig[CAL_ROW]={ "...... ",
                                                "...... ",
                                                ".......",
                                                ".......",
                                                ".......",
                                                ".......",
                                                ".......",
                                                "...    "};
std::string decodedDate[CAL_SIZE] = {"Jan","Feb","Mar","Apr","May","Jun","   ","\n",
                                     "Jul","Aug","Sep","Oct","Nov","Dec","   ","\n",
                                     "Sun","Mon","Tue","Wed","Thu","Fri","Sat","\n",
                                     " 01"," 02"," 03"," 04"," 05"," 06"," 07","\n",
                                     " 08"," 09"," 10"," 11"," 12"," 13"," 14","\n",
                                     " 15"," 16"," 17"," 18"," 19"," 20"," 21","\n",
                                     " 22"," 23"," 24"," 25"," 26"," 27"," 28","\n",
                                     " 29"," 30"," 31"};
#define MAX_LOCATION 45
#define DEBUG   0
enum orientation_t {NOR_0=0 , NOR_90=1 , NOR_180=2 , NOR_270=3 , 
                    MIR_0=4 , MIR_90=5 , MIR_180=6 , MIR_270=7};
enum symmetry_t {NO_SYM = 0 , MIR_SYM=1 , ROT_SYM=2 , M_R_SYM = 3};
struct coord
{
    unsigned int row:4;
    unsigned int col:4;
};

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
    unsigned int get_cells_number();
    void set_currentOrientation(unsigned int index);
    unsigned int get_currentOrientation();
    coord get_dimensions();
    void reset_shape();

};

std::string decode_date(std::string boardRep)
{
    std::string date = "";
    unsigned int loc=boardRep.length();
    while(loc != std::string::npos)
    {
        loc = boardRep.rfind('.',loc);
        if(loc != std::string::npos)
        {
            date+=decodedDate[loc]+"-";
        }
    }
    date.erase(date.length()-1,1);
    return date;
}

unsigned int date_to_index(std::string input)
{
    std::string *ptrfound;
    unsigned int month = 12;
    unsigned int day = 0;
    if(input.length() == 6)
    {
        ptrfound = std::find(decodedDate,decodedDate+CAL_SIZE,input.substr(3,3));
        if(ptrfound < decodedDate+18)
        {
            month = ptrfound-decodedDate;
            month = ((month/8)*6) + (month%8);
        }
        ptrfound = std::find(decodedDate,decodedDate+CAL_SIZE,input.substr(0,2));
        if(ptrfound < decodedDate+CAL_SIZE)
        {
            day = ptrfound-decodedDate;
            day = (((day/8)-2)*7) + (day%8);
        }
    }
    return ((month)*31)+day;
}

class puzzle_board
{
    private:
    std::string m_board[CAL_ROW];
    public:
    puzzle_board();
    bool try_to_fit(puzzle_piece pieceToFit , coord pieceLocation);
    void remove_shape(puzzle_piece pieceToRem , coord pieceLocation);
    void reset_board();
    void board_to_array(std::string outBoard[]);
    std::string board_to_string();
};

inline unsigned int coord_to_loc(coord in);
inline coord loc_to_coord(unsigned int in);

struct backtrack
{
    unsigned int loc;
    puzzle_piece piece;
};

bool is_branch_viable( puzzle_board currentState , backtrack piecesArr[] , unsigned int indexPiece );


#define MAX_TIME 30*60*CLOCKS_PER_SEC

int main (int argc, char * argv[])
{
    using namespace std;
    unsigned int argsin[2*NUM_PCS+1] = {0};
    // string days[POSSIBILITIES];
    // int daysAgregate[POSSIBILITIES];
    map<string , string> days;
    long long solnAgregate = 0;
    
    clock_t begin = clock();
    clock_t end = clock();
    unsigned int tempIndex = 0;
    backtrack allPieces[NUM_PCS];
    puzzle_board calenderBoard;
    puzzle_piece tempPiece;
    unsigned int tempLocation;
    bool tempFlag = true;
    bool finished = false;
    coord dimBuffer;
    string pieceBuffer[3];
    ofstream outFile;
    

    if(argc == 2*NUM_PCS+2)
    {
        for (unsigned int i = 0; i < 2*NUM_PCS+1; i++)
        {
            argsin[i] = atol(argv[i+1]);
        }
        
    }
    else
    {
        for (unsigned int i = 0; i < 2*NUM_PCS+1; i++)
        {
            argsin[i] = 0;
        }
        
    }
    

    // for (unsigned int i = 0; i < POSSIBILITIES; i++)
    // {
    //     days[i] = "";
    //     daysAgregate[i] = 0;
    // }

    dimBuffer.row = 2;
    dimBuffer.col = 3;
    pieceBuffer[0] =  "###";
    pieceBuffer[1] =  "###";
    tempPiece = puzzle_piece('O',pieceBuffer,dimBuffer,M_R_SYM);
    tempPiece.set_currentOrientation(argsin[tempIndex*2]);
    allPieces[tempIndex] = {argsin[tempIndex*2+1],tempPiece};
    tempIndex++;
 
    dimBuffer.row = 3;
    dimBuffer.col = 3;
    pieceBuffer[0] =  "###";
    pieceBuffer[1] =  "## ";
    pieceBuffer[2] =  "#  ";
    tempPiece = puzzle_piece('M',pieceBuffer,dimBuffer,MIR_SYM);
    tempPiece.set_currentOrientation(argsin[tempIndex*2]);
    allPieces[tempIndex] = {argsin[tempIndex*2+1],tempPiece};
    tempIndex++;

    dimBuffer.row = 2;
    dimBuffer.col = 3;
    pieceBuffer[0] =  "# #";
    pieceBuffer[1] =  "###";
    tempPiece = puzzle_piece('U',pieceBuffer,dimBuffer,MIR_SYM);
    tempPiece.set_currentOrientation(argsin[tempIndex*2]);
    allPieces[tempIndex] = {argsin[tempIndex*2+1],tempPiece};
    tempIndex++;
 
    dimBuffer.row = 3;
    dimBuffer.col = 3;
    pieceBuffer[0] =  "###";
    pieceBuffer[1] =  "#  ";
    pieceBuffer[2] =  "#  ";
    tempPiece = puzzle_piece('V',pieceBuffer,dimBuffer,MIR_SYM);
    tempPiece.set_currentOrientation(argsin[tempIndex*2]);
    allPieces[tempIndex] = {argsin[tempIndex*2+1],tempPiece};
    tempIndex++;
   
    dimBuffer.row = 2;
    dimBuffer.col = 3;
    pieceBuffer[0] =  " ##";
    pieceBuffer[1] =  "###";
    tempPiece = puzzle_piece('P',pieceBuffer,dimBuffer,NO_SYM);
    tempPiece.set_currentOrientation(argsin[tempIndex*2]);
    allPieces[tempIndex] = {argsin[tempIndex*2+1],tempPiece};
    tempIndex++;
   
    dimBuffer.row = 2;
    dimBuffer.col = 4;
    pieceBuffer[0] = "##  ";
    pieceBuffer[1] = " ###"; 
    tempPiece = puzzle_piece('N',pieceBuffer,dimBuffer,NO_SYM);
    tempPiece.set_currentOrientation(argsin[tempIndex*2]);
    allPieces[tempIndex] = {argsin[tempIndex*2+1],tempPiece};
    tempIndex++;
   
    dimBuffer.row = 2;
    dimBuffer.col = 4;
    pieceBuffer[0] =  "####";
    pieceBuffer[1] =  " #  ";
    tempPiece = puzzle_piece('Y',pieceBuffer,dimBuffer,NO_SYM);
    tempPiece.set_currentOrientation(argsin[tempIndex*2]);
    allPieces[tempIndex] = {argsin[tempIndex*2+1],tempPiece};
    tempIndex++;
 
    dimBuffer.row = 2;
    dimBuffer.col = 4;
    pieceBuffer[0] =  "####";
    pieceBuffer[1] =  "#   ";
    tempPiece = puzzle_piece('L',pieceBuffer,dimBuffer,NO_SYM);
    tempPiece.set_currentOrientation(argsin[tempIndex*2]);
    allPieces[tempIndex] = {argsin[tempIndex*2+1],tempPiece};
 
    dimBuffer.row = 3;
    dimBuffer.col = 3;
    pieceBuffer[0] =  "## ";
    pieceBuffer[1] =  " # ";
    pieceBuffer[2] =  " ##";
    tempPiece = puzzle_piece('Z',pieceBuffer,dimBuffer,ROT_SYM);
    tempPiece.set_currentOrientation(argsin[tempIndex*2]);
    allPieces[tempIndex] = {argsin[tempIndex*2+1],tempPiece};
    tempIndex++;

    tempIndex = argsin[2*NUM_PCS];
    for (int i = 0; i < tempIndex; i++)
    {
        calenderBoard.try_to_fit(allPieces[i].piece,loc_to_coord(allPieces[i].loc));
    }
    while(DEBUG || end-begin < MAX_TIME && !finished)
    {
        
        if(tempIndex > NUM_PCS-1)   
        {      //all pieces used on board
            unsigned int tempInd = date_to_index(decode_date(calenderBoard.board_to_string()));
            if(days[tempInd] == "")
            {
                days[tempInd]+=decode_date(calenderBoard.board_to_string());
            }
            days[tempInd]+="\n";
            days[tempInd]+=calenderBoard.board_to_string();
            days[tempInd]+="\n\n";
            // outFile<<decode_date(calenderBoard.board_to_string())<<endl;
            // cout<<"found soln\n";
            // outFile<<calenderBoard.board_to_string()<<endl;
            //backtrack to latest piece can use
            calenderBoard.remove_shape(
                          allPieces[NUM_PCS-1].piece,
                          loc_to_coord(allPieces[NUM_PCS-1].loc));
            allPieces[NUM_PCS-1].piece.reset_shape();
            allPieces[NUM_PCS-1].loc = 0;
            tempIndex = NUM_PCS-2;
            tempFlag = false;
            
        }
        else //tempIndex is next piece to place
        {
            tempFlag =  calenderBoard.try_to_fit(
                        allPieces[tempIndex].piece,
                        loc_to_coord(allPieces[tempIndex].loc));
            if(tempFlag)
            {
                tempIndex++;
            }
        }
        while(!tempFlag) //get next orientation or location of next piece
        {
            calenderBoard.remove_shape(
                            allPieces[tempIndex].piece,
                            loc_to_coord(allPieces[tempIndex].loc));
            tempFlag = allPieces[tempIndex].piece.get_next_orientation();
            if(!tempFlag)
            {
                allPieces[tempIndex].piece.reset_shape();
                tempFlag = true;
                allPieces[tempIndex].loc++;
                if(allPieces[tempIndex].loc >= MAX_LOCATION)
                {
                    if(tempIndex == 0)//that is the last piece in the last location
                    {
                        finished = true;
                    }
                    else
                    {
                        allPieces[tempIndex].loc = 0;
                        tempFlag = false;
                        tempIndex--;
                    }
                }
            }
        }
        end = clock();
    }


    cout<<endl;
    for (int  i = 0; i < NUM_PCS; i++)
    {
        cout<<allPieces[i].piece.get_currentOrientation()<<' '<<allPieces[i].loc<<' ';
    }

    cout<<tempIndex<<endl;
    outFile.open("output.txt",ofstream::out|ofstream::app);

    for (unsigned int i = 0; i < POSSIBILITIES; i++)
    {
        outFile<<days[i];
    }
    

    outFile.flush();
    outFile.close();
    return 0;
}

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
        result.row = this->m_filled[this->m_filledCount-this->m_currentFilled-1].row;
        result.col = this->m_filled[this->m_filledCount-this->m_currentFilled-1].col;
        break;
        
        case NOR_90:
        result.row = this->m_filled[this->m_filledCount-this->m_currentFilled-1].col;
        result.col = this->m_dimensions.row-this->m_filled[this->m_filledCount-this->m_currentFilled-1].row-1;
        break;

        case NOR_180:
        result.row = this->m_dimensions.row-this->m_filled[this->m_filledCount-this->m_currentFilled-1].row-1;
        result.col = this->m_dimensions.col-this->m_filled[this->m_filledCount-this->m_currentFilled-1].col-1;
        break;
        
        case NOR_270:
        result.row = this->m_dimensions.col-this->m_filled[this->m_filledCount-this->m_currentFilled-1].col-1;
        result.col = this->m_filled[this->m_filledCount-this->m_currentFilled-1].row;
        break;

        case MIR_0 :
        result.row = this->m_filled[this->m_filledCount-this->m_currentFilled-1].row;
        result.col = this->m_dimensions.col-this->m_filled[this->m_filledCount-this->m_currentFilled-1].col-1;
        break;
        
        case MIR_90 :
        result.row = this->m_dimensions.col-this->m_filled[this->m_filledCount-this->m_currentFilled-1].col-1;
        result.col = this->m_dimensions.row-this->m_filled[this->m_filledCount-this->m_currentFilled-1].row-1;
        break;

        case MIR_180 :
        result.row = this->m_dimensions.row-this->m_filled[this->m_filledCount-this->m_currentFilled-1].row-1;
        result.col = this->m_filled[this->m_filledCount-this->m_currentFilled-1].col;
        break;

        case MIR_270 :
        result.row = this->m_filled[this->m_filledCount-this->m_currentFilled-1].col;
        result.col = this->m_filled[this->m_filledCount-this->m_currentFilled-1].row;
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

unsigned int puzzle_piece::get_cells_number()
{
    return this->m_filledCount;
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

puzzle_board::puzzle_board()
{
    this->reset_board();
}

bool puzzle_board::try_to_fit(puzzle_piece pieceToFit , coord pieceLocation)
{
    char toFill = pieceToFit.get_pieceName();
    coord parts[6];
    unsigned int i = 0;
    bool canFit = true;
    coord checKfit = pieceToFit.get_dimensions();
    checKfit.col+=pieceLocation.col;
    checKfit.row+=pieceLocation.row;
    if(checKfit.col > CAL_COL || checKfit.row > CAL_ROW)
    {
        canFit = false;
    }
    while(canFit && pieceToFit.get_next_coord(parts[i]))
    {
        parts[i].row+= pieceLocation.row;
        parts[i].col+= pieceLocation.col;
        if( parts[i].row > CAL_ROW-1 || parts[i].col > CAL_COL-1 || this->m_board[parts[i].row][parts[i].col]!='.')
        {
            canFit=false;
        }
        i++;
    }
    if(canFit)
    {
        while(i>0)
        {
            i--;
            this->m_board[parts[i].row][parts[i].col] = toFill;
        }
    }
    return canFit;
}

void puzzle_board::remove_shape(puzzle_piece pieceToRem , coord pieceLocation)
{
    char pieceName = pieceToRem.get_pieceName();
    coord tempCoord = {0,0};
    unsigned int i = 0;
    bool canFit = true;
    while(pieceToRem.get_next_coord(tempCoord))
    {
        tempCoord.row+= pieceLocation.row;
        tempCoord.col+= pieceLocation.col;
        if( tempCoord.row < CAL_ROW && tempCoord.col < CAL_COL && pieceName == this->m_board[tempCoord.row][tempCoord.col])
        {
           this->m_board[tempCoord.row][tempCoord.col] ='.';
        }
    }
}

void puzzle_board::reset_board()
{
    for (int i = 0; i < CAL_ROW; i++)
    {
        this->m_board[i] = boardConfig[i];
    }
}

void puzzle_board::board_to_array(std::string outBoard[])
{
    for (int i = 0; i < CAL_ROW; i++)
    {
        outBoard[i]=this->m_board[i];
    }
}

std::string puzzle_board::board_to_string()
{
    std::string result = "";
    for (int i = 0; i < CAL_ROW; i++)
    {
        result+=this->m_board[i]+"\n";
    }
    return result;
}

inline unsigned int coord_to_loc(coord in)
{
    return in.row*CAL_COL+in.col;
}

inline coord loc_to_coord(unsigned int in)
{
    return {in/CAL_COL,in%CAL_COL};
}

bool is_branch_viable( puzzle_board currentState , backtrack piecesArr[] , unsigned int indexPiece )
{
    bool result = true;
    std::string currentBoard[CAL_ROW];
    currentState.board_to_array(currentBoard);
    const unsigned int spacesMaxSize = NUM_PCS+3;
    unsigned int pool [spacesMaxSize] = {0};
    unsigned int poolSize = 3;
    unsigned int spaces[spacesMaxSize] = {0};
    unsigned int spacesSize = 0;
    for (unsigned int i = 0; i < 3; i++)
    {
        pool[i] = 1;
    }
    for (unsigned int  i = indexPiece+1; i < NUM_PCS; i++)
    {
        pool[poolSize] = piecesArr[i].piece.get_cells_number();
        poolSize++;
    }
    std::sort(pool,pool+poolSize);
    
    


    /* get*/





}
