#include "helper_functions.hpp"
#include <algorithm>

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
