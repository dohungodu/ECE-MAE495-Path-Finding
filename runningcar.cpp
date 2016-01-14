#include "runningcar.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>


using namespace std;
int BallsArray[ChessBoardWidth][ChessBoardWidth];
int VisitsArray[ChessBoardWidth][ChessBoardWidth];
int VisitsArrayTime[ChessBoardWidth][ChessBoardWidth];

int VisitTimeCount;

//osition BlockArray[6] = {{1,4},{2,3},{4,2},{6,1},{4,6},{5,5}};


Position FinishPos = {FinishX,FinishY};
Position StartPos;

bool checkFinishPos (Position v_Pos)
{
    //std::cout << "check finish" << ((v_Pos.horizontal == FinishPos.horizontal) && (v_Pos.vertical == FinishPos.vertical));
    return ((v_Pos.horizontal == FinishPos.horizontal) && (v_Pos.vertical == FinishPos.vertical));
}

int CalDistance(Position v_Pos,Position v_FinishPos)
{
    return abs(v_Pos.horizontal-v_FinishPos.horizontal)+abs(v_Pos.vertical-v_FinishPos.vertical);

}


void PrintBoard()
{
     int i, j;
        for (i=1;i<ChessBoardWidth+1;i++)
        {
            cout << endl;

            for (j=1;j<ChessBoardWidth+1;j++)
                {
                    if (BallsArray[i][j]<0) cout << "0 ";
                    else
                    cout << BallsArray[i][j] << " ";
                }
        }
        cout << endl;
}


void dataInitialize()
{
    int i, j;
    //Reset ball array
    VisitTimeCount =0;

    for (i=1;i<ChessBoardWidth+1;i++)
        for (j=1;j<ChessBoardWidth+1;j++)
        {
           BallsArray[i][j] = -1;
           VisitsArray[i][j] = 0;
           VisitsArrayTime [i][j] = 0;
        }


    StartPos.Direction = 'E';
    StartPos.horizontal = 1;
    StartPos.vertical = 1;



    BallsArray[StartPos.horizontal][StartPos.vertical] = 0; //Already visited

}

Position getPos(Position v_Pos,char v_side)
{
    string str;
    std::stringstream ss;

    Position tmpPos;
    tmpPos = v_Pos;

    ss << v_Pos.Direction << v_side;
    str = ss.str();
    //std::cout << str;
    if ((str == "EL") || (str == "WR") || (str == "SB") || (str == "NF"))
        tmpPos.horizontal = tmpPos.horizontal-1;

    if ((str == "ER") || (str == "WL") || (str == "SF") || (str == "NB"))
        tmpPos.horizontal = tmpPos.horizontal+1;

    if ((str == "EB") || (str == "WF") || (str == "SR") || (str == "NL"))
        tmpPos.vertical = tmpPos.vertical-1;

    if ((str == "EF") || (str == "WB") || (str == "SL") || (str == "NR"))
    {
        tmpPos.vertical = tmpPos.vertical+1;
    }


    return tmpPos;
}
bool checkBoundary(Position v_Pos)
{
    //Check if position is outside of the board. will return false
    if ((v_Pos.horizontal<=0) || (v_Pos.vertical <=0) || (v_Pos.horizontal>ChessBoardWidth) || (v_Pos.vertical>ChessBoardWidth))
        return false;

    return true;
}

//Get the appropriate position to go
char GetPosNotChecked(Position v_currPos) //F:front;L:left;R:right;B:back
{
    Position leftPos, rightPos, backPos,frontPos;

    leftPos = getPos(v_currPos,'L');
    rightPos = getPos(v_currPos,'R');
    backPos = getPos(v_currPos,'B');
    frontPos = getPos(v_currPos,'F');

    //std::cout << BallsArray[2][4]<<frontPos.vertical<<frontPos.horizontal;

    //Set priority to check: first-->front, second-->left, third-->right, last-->back
    if ((checkBoundary(frontPos)) && (BallsArray[frontPos.horizontal][frontPos.vertical] ==-1))
            return 'F';

    //Decision between two equal visits at finish point
    if ((checkBoundary(leftPos)) && (BallsArray[leftPos.horizontal][leftPos.vertical]==-1) &&
        (checkBoundary(rightPos)) && (BallsArray[rightPos.horizontal][rightPos.vertical] ==-1))
                {
                    if (CalDistance(leftPos,FinishPos) < CalDistance(rightPos,FinishPos))
                        return 'L';
                    else
                        return 'R';
                }

    if ((checkBoundary(leftPos)) && (BallsArray[leftPos.horizontal][leftPos.vertical] ==-1))
            return 'L';
    if ((checkBoundary(rightPos)) && (BallsArray[rightPos.horizontal][rightPos.vertical] ==-1))
            return 'R';
    if ((checkBoundary(backPos)) && (BallsArray[backPos.horizontal][backPos.vertical] ==-1))
            return 'B';

    return 'O'; //error;
}


bool checkBall(Position v_currPos, char v_sensor_data) //-1 not check;1 block; 0 free to go
{
    Position frontPos;
    frontPos = getPos(v_currPos,'F');



    //std::cout << "sensor data" << v_sensor_data << std::endl;

    //int sensor_value =  (int)(v_sensor_data.at(0)); //
    //int right_sensor = (int)(v_sensor_data.at(3)); //use data format "[x;y]"

    if (v_sensor_data == '1')
    {

        //std::cout << "ball in front" << std::endl;

        return true;
    }
    //has a ball in front
    if (BallsArray [frontPos.horizontal][frontPos.vertical]==1)
        return true;

    //use sensor to check ball
    //if (v_sensor_data > 0)
    //    return true;

    //Free to go
    return false;

}

int checkNoBallinFrontForTest(Position v_currPos) //-1 not check;1 block; 0 free to go
{
    Position frontPos;
    frontPos = getPos(v_currPos,'F');
    if (BallsArray [frontPos.horizontal][frontPos.vertical] == -1)
    {
        BallsArray [frontPos.horizontal][frontPos.vertical] = 0; //testing purpose
    }
    return BallsArray [frontPos.horizontal][frontPos.vertical];
}

void blockPosition(Position v_Pos)
{
    BallsArray[v_Pos.horizontal][v_Pos.vertical] = 1;
}

bool checkBlockPosition(Position v_Pos)
{
    return BallsArray[v_Pos.horizontal][v_Pos.vertical] == 1;
}

char GetMinValue (Position v_currPos)
{
    int v_min = 36000; //number of visits
    int v_min_time = 36000;
    char v_dir;
    Position leftPos, rightPos, backPos,frontPos;

    leftPos = getPos(v_currPos,'L');
    rightPos = getPos(v_currPos,'R');
    backPos = getPos(v_currPos,'B');
    frontPos = getPos(v_currPos,'F');

    //std::cout << "back block: "<< checkBlockPosition(backPos)<< std::endl;
    //std::cout << "back boundary: "<< checkBoundary(backPos)<< std::endl;
    // std::cout << "value: "<< VisitsArray[backPos.horizontal][backPos.vertical]<< std::endl;

    if ((!checkBlockPosition(backPos)) && (checkBoundary(backPos)))
    {
        if (VisitsArray[backPos.horizontal][backPos.vertical] < v_min)
            {

            v_min_time = VisitsArrayTime[backPos.horizontal][backPos.vertical];
            v_min = VisitsArray[backPos.horizontal][backPos.vertical];
            v_dir = 'B';
            }
        else
            if
             ((VisitsArray[backPos.horizontal][backPos.vertical] == v_min)
             && (VisitsArrayTime[backPos.horizontal][backPos.vertical] < v_min_time))
             {
                    v_min = VisitsArray[backPos.horizontal][backPos.vertical];
                    v_min_time = VisitsArrayTime[backPos.horizontal][backPos.vertical];
                    v_dir = 'B';
             }
    }

    if ((!checkBlockPosition(rightPos)) &&(checkBoundary(rightPos)))
            {
                if (VisitsArray[rightPos.horizontal][rightPos.vertical] < v_min)
                {
                        v_min_time = VisitsArrayTime[rightPos.horizontal][rightPos.vertical];
                        v_min = VisitsArray[rightPos.horizontal][rightPos.vertical];
                        v_dir = 'R';
                }
                else
                    if
                     ((VisitsArray[rightPos.horizontal][rightPos.vertical] == v_min)
                    && (VisitsArrayTime[rightPos.horizontal][rightPos.vertical] < v_min_time))
                {
                        v_min_time = VisitsArrayTime[rightPos.horizontal][rightPos.vertical];
                        v_min = VisitsArray[rightPos.horizontal][rightPos.vertical];
                        v_dir = 'R';
                }
            }

    if ((!checkBlockPosition(leftPos)) && (checkBoundary(leftPos)))

            {
                if (VisitsArray[leftPos.horizontal][leftPos.vertical] < v_min)
                {
                        v_min_time = VisitsArrayTime[leftPos.horizontal][leftPos.vertical];
                        v_min = VisitsArray[leftPos.horizontal][leftPos.vertical];
                        v_dir = 'L';
                }
                else
                    if
                     ((VisitsArray[leftPos.horizontal][leftPos.vertical] == v_min)
                    && (VisitsArrayTime[leftPos.horizontal][leftPos.vertical] < v_min_time))
                {
                        v_min_time = VisitsArrayTime[leftPos.horizontal][leftPos.vertical];
                        v_min = VisitsArray[leftPos.horizontal][leftPos.vertical];
                        v_dir = 'L';
                }

            }

    if ((!checkBlockPosition(frontPos)) &&(checkBoundary(frontPos)))
        {

          if (VisitsArray[frontPos.horizontal][frontPos.vertical] < v_min)
                {
                        v_min_time = VisitsArrayTime[frontPos.horizontal][frontPos.vertical];
                        v_min = VisitsArray[frontPos.horizontal][frontPos.vertical];
                        v_dir = 'F';
                }
                else
                    if
                     ((VisitsArray[frontPos.horizontal][frontPos.vertical] == v_min)
                    && (VisitsArrayTime[frontPos.horizontal][frontPos.vertical] < v_min_time))
                {
                        v_min_time = VisitsArrayTime[frontPos.horizontal][frontPos.vertical];
                        v_min = VisitsArray[frontPos.horizontal][frontPos.vertical];
                        v_dir = 'F';
                }
        }

    return v_dir;
}

void setVisit (Position *v_Pos)
{
 BallsArray[v_Pos->horizontal][v_Pos->vertical] = 0;
}

void GoForward(Position *v_currPos)
{
    VisitsArray[v_currPos->horizontal][v_currPos->vertical]++;
    BallsArray[v_currPos->horizontal][v_currPos->vertical] = 0;
    VisitTimeCount ++;
    VisitsArrayTime[v_currPos->horizontal][v_currPos->vertical] = VisitTimeCount;
    Position tmpPos;

    tmpPos.Direction = v_currPos->Direction;
    tmpPos.vertical = v_currPos->vertical;
    tmpPos.horizontal = v_currPos->horizontal;

    tmpPos = getPos(tmpPos,'F');

    v_currPos->Direction = tmpPos.Direction;
    v_currPos->horizontal = tmpPos.horizontal;
    v_currPos->vertical = tmpPos.vertical;
}
void TurnTheCar(Position *v_currPos, char v_turn)
{
    std::string str;
    std::stringstream ss;

    ss << v_currPos->Direction << v_turn;
    str = ss.str();
    //std::cout << "turn to direction" << str <<std::endl;
    if ((str == "EL") || (str == "WR") || (str == "SB") || (str == "NF"))
        v_currPos->Direction ='N';

    if ((str == "ER") || (str == "WL") || (str == "SF") || (str == "NB"))
        v_currPos->Direction ='S';

    if ((str == "EB") || (str == "WF") || (str == "SR") || (str == "NL"))
        v_currPos->Direction ='W';

    if ((str == "EF") || (str == "WB") || (str == "SL") || (str == "NR"))
    {
        v_currPos->Direction ='E';
    }
    //std::cout << "turn to " << v_currPos->Direction <<std::endl;
}




