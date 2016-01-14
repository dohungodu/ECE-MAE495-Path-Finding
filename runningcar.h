#ifndef RUNNINGC-AR_H
#define RUNNINGCAR_H
#define  carWidth 30
#define  carLength  50
#define  chessWidth 90
#define ChessBoardWidth 6
#define SleepTime 50
#define FinishX 6
#define FinishY 6
#define StartX 1
#define StartY 1
#define StartDir 'E'
 struct Position
{
    int vertical;
    int horizontal;
    char Ver_Hor;
    char Direction;
};
 struct Direction
{
    //North, South, East, West
    char direction;

};

void PrintBoard();
void GoForward(Position *v_currPos);
char GetMinValue (Position v_currPos);

int CalDistance(Position v_Pos,Position v_FinishPos);
void setVisit (Position *v_Pos);
char GetPosNotChecked(Position v_currPos,Direction v_dir);
void TurnTheCar(Position *v_currPos, char v_turn); //turn_to = 'L', 'R','B'
bool checkBall(Position v_currPos, char v_sensor_data);
int checkNoBallinFrontForTest(Position v_currPos);
void blockPosition(Position v_Pos); //set lock the this position
void dataInitialize();
Position getPos(Position v_Pos,char v_side);
bool checkBoundary(Position v_Pos);
char GetPosNotChecked(Position v_currPos);
bool checkFinishPos (Position v_Pos);
bool checkBlockPosition(Position v_Pos);


#endif // RUNNINGCAR_H
