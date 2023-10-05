

map_lines = [' ',' ',' ','└',' ','│','┌','├',' ','┘','─','┴','┐','┤','┬','┼']
           

def representPuzzle(inputPuzzle):
    UP_PATH = 1
    RT_PATH = 2
    DN_PATH = 4
    LT_PATH = 8
    outputPuzzle =["","","","","","","",""]
    iTer = 0
    hasRight = False
    pieceTL = ' '
    pieceTR = ' '
    pieceBL = ' '
    pieceBR = ' '
    paths = 0
    for row in range(8):
        for col in range(8):
            pieceTL = ' '
            pieceTR = ' '
            pieceBL = ' '
            pieceBR = ' '
            paths = 0
            hasRight = False
            if row > 0 and col > 0: 
                pieceTL = inputPuzzle[row-1][col-1]
            if row > 0 and col < 7: 
                pieceTR = inputPuzzle[row-1][col]
            if row < 7 and col > 0: 
                pieceBL = inputPuzzle[row][col-1]
            if row < 7 and col < 7: 
                pieceBR = inputPuzzle[row][col]
            if pieceTL != pieceTR:
                paths += UP_PATH
            if pieceBR != pieceTR:
                paths += RT_PATH
                hasRight = True
            if pieceBR != pieceBL:
                paths += DN_PATH
            if pieceTL != pieceBL:
                paths += LT_PATH
            outputPuzzle[iTer] += map_lines[paths]
            if col < 7 and hasRight: 
                outputPuzzle[iTer]+= '─'
            else :
                outputPuzzle[iTer]+= ' '
        iTer+= 1
    return outputPuzzle

# ┌─────┬─┬───┐  
# │   ┌─┼─┤   │
# ├───┘ │ │   ├─┐
# │ ┌───┤ └───┤ │
# ├─┴─┐ └───┬─┘ │
# ├─┐ ├─────┴─┐ │
# ├─┘ │ ┌─────┴─┘
# └───┴─┘
# test = []
# test.append("PPP.OO ")      
# test.append("PPZVOO ")      
# test.append("ZZZVOOY")      
# test.append("ZNNVVVY")      
# test.append("UUNNNYY")      
# test.append(".ULLLLY")      
# test.append("UUL    ")
puzzle = []
tempOutputRep = []
outputRep = ["","","","","","","",""]
finalWriteToFile = []
forbidenDays = ["30-Feb" , "31-Feb" , "31-Apr" , "31-Jun" , "31-Sep" , "31-Nov"]
# monthNames = ["Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"]
# month = 0
day = 0
# print(representPuzzle(test))
otfile = open("Calender Answers.txt","w",-1,"utf-8")
frstBatchFile = open("FirstBatchAnswers.txt","r")
scndBatchFile = open("SecondBatchAnswers.txt","r")
frstBatchList = []
scndBatchList = []
frstBatchIgnoreFlag = False
scndBatchIgnoreFlag = False
for line in frstBatchFile:
    frstBatchList.append(line)
for line in scndBatchFile:
    scndBatchList.append(line)
frstBatchFile.close()
scndBatchFile.close()
dayText = frstBatchList[0][:-1]
solns = 0
totalSolns = 0
frstBatchIter = 1
scndBatchIter = 1
while day < 12*31:
    tempChar = frstBatchList[frstBatchIter][0]
    while not(tempChar >= '0' and tempChar <='9') :
        if not frstBatchIgnoreFlag and frstBatchList[frstBatchIter] != "\n" :
            puzzle = []
            for i in range(7):
                puzzle.append(frstBatchList[frstBatchIter+i][:-1])
            frstBatchIter+= 7
            tempOutputRep = representPuzzle(puzzle)
            for i in range(8):
                outputRep[i]+= tempOutputRep[i]
            solns+= 1 
            if solns%15 == 0:
                for i in range(8):
                    finalWriteToFile.append(outputRep[i]+"\n")
                    outputRep[i] = ""
        frstBatchIter+= 1
        tempChar = frstBatchList[frstBatchIter][0]
    
    tempChar = scndBatchList[scndBatchIter][0]
    while not(tempChar >= '0' and tempChar <='9') :
        if not scndBatchIgnoreFlag and scndBatchList[scndBatchIter] != "\n" :
            puzzle = []
            for i in range(7):
                puzzle.append(scndBatchList[scndBatchIter+i][:-1])
            scndBatchIter+= 7
            tempOutputRep = representPuzzle(puzzle)
            for i in range(8):
                outputRep[i]+= tempOutputRep[i] 
            solns+= 1
            if solns%15 == 0:
                for i in range(8):
                    finalWriteToFile.append(outputRep[i]+"\n")
                    outputRep[i] = ""
        scndBatchIter+= 1
        tempChar = scndBatchList[scndBatchIter][0]
    
    if not frstBatchIgnoreFlag:
        otfile.write(dayText+" : "+str(solns)+" solutions\n") 
        for line in finalWriteToFile:
            otfile.write(line) 
        for i in range(8):
            otfile.write(outputRep[i]+"\n")
    
    
    if frstBatchList[frstBatchIter][:-1] in forbidenDays:
        frstBatchIgnoreFlag = True
    else  :
        frstBatchIgnoreFlag = False
    if scndBatchList[scndBatchIter][:-1] in forbidenDays:
        scndBatchIgnoreFlag = True
    else :
        scndBatchIgnoreFlag = False
    if not frstBatchIgnoreFlag:
        dayText =  frstBatchList[frstBatchIter][:-1]
    outputRep = ["","","","","","","",""]
    finalWriteToFile = []
    totalSolns+= solns
    solns = 0
    frstBatchIter+= 1
    scndBatchIter+= 1
    day+= 1
print(totalSolns)
otfile.close()