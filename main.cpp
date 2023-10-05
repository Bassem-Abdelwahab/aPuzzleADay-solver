#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm> 
#include <time.h>
#include <stdlib.h>

#include "puzzle_piece/puzzle_piece.hpp"
#include "puzzle_board/puzzle_board.hpp"
#include "constants/constants.hpp"
#include "helper_functions.hpp"
#include "helping_structure.hpp"

#define MAX_LOCATION 38
#define DEBUG   0
#define MAX_TIME 1*60*CLOCKS_PER_SEC

inline unsigned int coord_to_loc(coord in);
inline coord loc_to_coord(unsigned int in);

struct backtrack
{
    unsigned int loc;
    puzzle_piece piece;
};


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


inline unsigned int coord_to_loc(coord in)
{
    return in.row*7+in.col;
}

inline coord loc_to_coord(unsigned int in)
{
    return {in/7,in%7};
}
