#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm> 
#include <time.h>
#include <stdlib.h>

#include "puzzle_piece/puzzle_piece.hpp"
#include "helping_structure.hpp"

std::string decodedDate[51] = {"Jan","Feb","Mar","Apr","May","Jun", " ","\n",
                               "Jul","Aug","Sep","Oct","Nov","Dec", " ","\n",
                                "01", "02", "03", "04", "05", "06","07","\n",
                                "08", "09", "10", "11", "12", "13","14","\n",
                                "15", "16", "17", "18", "19", "20","21","\n",
                                "22", "23", "24", "25", "26", "27","28","\n",
                                "29", "30", "31"};
#define MAX_LOCATION 38
#define DEBUG   0

std::string decode_date(std::string boardRep)
{
    std::string date = "";
    unsigned int loc=0;
    loc = boardRep.rfind('.');
    if(loc != std::string::npos)
    {
        date+=decodedDate[loc]+"-";
        loc = boardRep.rfind('.',loc-1);
        if(loc != std::string::npos)
        {
            date+=decodedDate[loc];
        }
    }
    return date;
}

unsigned int date_to_index(std::string input)
{
    std::string *ptrfound;
    unsigned int month = 12;
    unsigned int day = 0;
    if(input.length() == 6)
    {
        ptrfound = std::find(decodedDate,decodedDate+51,input.substr(3,3));
        if(ptrfound < decodedDate+18)
        {
            month = ptrfound-decodedDate;
            month = ((month/8)*6) + (month%8);
        }
        ptrfound = std::find(decodedDate,decodedDate+51,input.substr(0,2));
        if(ptrfound < decodedDate+51)
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
    std::string m_board[7];
    public:
    puzzle_board();
    bool try_to_fit(puzzle_piece pieceToFit , coord pieceLocation);
    void remove_shape(puzzle_piece pieceToRem , coord pieceLocation);
    void reset_board();
    std::string board_to_string();
};

inline unsigned int coord_to_loc(coord in);
inline coord loc_to_coord(unsigned int in);

struct backtrack
{
    unsigned int loc;
    puzzle_piece piece;
};

#define MAX_TIME 1*60*CLOCKS_PER_SEC

int main (int argc, char * argv[])
{
    using namespace std;
    unsigned int argsin[17] = {1,26,3,0,3,4,3,28,
                               4,22,7,17,2,9,0,0,6};
    string days[12*31+1];
    
    clock_t begin = clock();
    clock_t end = clock();
    unsigned int tempIndex = 0;
    backtrack allPieces[8];
    puzzle_board calenderBoard;
    puzzle_piece tempPiece;
    unsigned int tempLocation;
    bool tempFlag = true;
    bool finished = false;
    coord dimBuffer;
    string pieceBuffer[3];
    ofstream outFile;
    outFile.open("output.txt",ofstream::out|ofstream::app);


    if(argc == 18)
    {
        for (unsigned int i = 0; i < 17; i++)
        {
            argsin[i] = atol(argv[i+1]);
        }
        
    }

    for (unsigned int i = 0; i < 12*31+1; i++)
    {
        days[i] = "";
    }

    dimBuffer.row = 2;
    dimBuffer.col = 3;
    pieceBuffer[0] =  "###";
    pieceBuffer[1] =  "###";
    tempPiece = puzzle_piece('O',pieceBuffer,dimBuffer,M_R_SYM);
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
   
    dimBuffer.row = 3;
    dimBuffer.col = 3;
    pieceBuffer[0] =  "## ";
    pieceBuffer[1] =  " # ";
    pieceBuffer[2] =  " ##";
    tempPiece = puzzle_piece('Z',pieceBuffer,dimBuffer,ROT_SYM);
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

    tempIndex = argsin[16];
    for (int i = 0; i < tempIndex; i++)
    {
        calenderBoard.try_to_fit(allPieces[i].piece,loc_to_coord(allPieces[i].loc));
    }
    while(DEBUG || end-begin < MAX_TIME && !finished)
    {
        //get Next piece to place
        if(tempIndex > 7)   
        {      //all pieces used on board
            unsigned int tempInd = date_to_index(decode_date(calenderBoard.board_to_string()));
            if(tempInd == 12*31 || days[tempInd] == "")
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
                          allPieces[7].piece,
                          loc_to_coord(allPieces[7].loc));
            allPieces[7].piece.reset_shape();
            allPieces[7].loc = 0;
            tempIndex = 6;
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
    cout<<allPieces[0].piece.get_currentOrientation()<<' '<<allPieces[0].loc<<' ';
    cout<<allPieces[1].piece.get_currentOrientation()<<' '<<allPieces[1].loc<<' ';
    cout<<allPieces[2].piece.get_currentOrientation()<<' '<<allPieces[2].loc<<' ';
    cout<<allPieces[3].piece.get_currentOrientation()<<' '<<allPieces[3].loc<<' ';
    cout<<allPieces[4].piece.get_currentOrientation()<<' '<<allPieces[4].loc<<' ';
    cout<<allPieces[5].piece.get_currentOrientation()<<' '<<allPieces[5].loc<<' ';
    cout<<allPieces[6].piece.get_currentOrientation()<<' '<<allPieces[6].loc<<' ';
    cout<<allPieces[7].piece.get_currentOrientation()<<' '<<allPieces[7].loc<<' ';
    cout<<tempIndex<<endl;

    for (unsigned int i = 0; i < 12*31+1; i++)
    {
        outFile<<days[i];
    }
    

    outFile.flush();
    outFile.close();
    return 0;
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
    while(canFit && pieceToFit.get_next_coord(parts[i]))
    {
        parts[i].row+= pieceLocation.row;
        parts[i].col+= pieceLocation.col;
        if( parts[i].row > 6 || parts[i].col > 6 || this->m_board[parts[i].row][parts[i].col]!='.')
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
        if( tempCoord.row < 7 && tempCoord.col < 7 && pieceName == this->m_board[tempCoord.row][tempCoord.col])
        {
           this->m_board[tempCoord.row][tempCoord.col] ='.';
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
        result+=this->m_board[i]+"\n";
    }
    return result;
}

inline unsigned int coord_to_loc(coord in)
{
    return in.row*7+in.col;
}

inline coord loc_to_coord(unsigned int in)
{
    return {in/7,in%7};
}
