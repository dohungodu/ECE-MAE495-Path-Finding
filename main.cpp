#include <iostream>
#include <tchar.h>

#include "runningcar.h"
#include <string>
#include <sstream>
#include "Serial.h"
using namespace std;

Serial* SP = new Serial("\\\\.\\COM4");    // adjust as needed

char sensor_info;
int step_count =0;
int count_to_exit = 0;
void sendDatatoCar(char ch)
{
    char outcomingData[256] = "";			// don't forget to pre-allocate memory
    int dataLength = 255;
	int readResult = 0;

        outcomingData[0] = ch;
        bool ii = SP->WriteData(outcomingData,1);
}

char* readDatafromCar()
{
    char incomingData[256] = "";			// don't forget to pre-allocate memory
	int dataLength = 255;
	int readResult = 1;
    int count_read = 0;

        while(SP->IsConnected() && (readResult>0) )
        {
            readResult = SP->ReadData(incomingData,dataLength);
            //incomingData[readResult] = 0;
            Sleep(100);
        }
    return incomingData;
}

char Command_Data (char ch)
{

    Sleep(1000);
    sendDatatoCar(ch);
    switch(ch)
        {
              case 'B':  Sleep(2500);
              break;
              case 'S':  Sleep(2000);
              break;
              default:   Sleep(2500);
              break;
        }

    char* rd = readDatafromCar();

    char aa[256];
    strcpy(aa,rd);
    if (strlen(aa) == 0)
        count_to_exit++;

    if (count_to_exit > 3)
        {
            cout << "Please check connection. Program is about to terminate ...";
            Sleep(2000);
            exit(0);
        }
    return aa[0];
}

//Init the connection
bool xbee_init()
{



    return true;
}

int main(int argc, char**argv)
{

    dataInitialize();
	if (SP->IsConnected())
		cout << "We're connected";
    sensor_info = Command_Data('S');

//    char outcomingData[256] = "";			// don't forget to pre-allocate memory
//
//    //wwrite data
//    int iii =0;
//    while (iii <100)
//    {
//        iii++;
//        if ( (iii % 2) ==1)
//            outcomingData[0] = 'S';
//        else
//            outcomingData[0] = 'B';
//
//        char sensor_info = Command_Data(outcomingData[0]);
//
//        if (sensor_info == '0')
//            cout << "free to go" << sensor_info << endl;
//        else
//            cout << "iam running"<<sensor_info << endl;
//
//    }
//	return 0;



    //*************************Main control block***************************
    char v_PosNotChecked, v_MinPos;
    Position CurrPosCar, fromPos;

    //set a start position
    CurrPosCar.Direction = StartDir;
    CurrPosCar.horizontal = StartX;
    CurrPosCar.vertical = StartY;
    //set the first position as a visited
    setVisit(&CurrPosCar);

        while (!checkFinishPos(CurrPosCar))
        {
            std::cout<<"hor: " <<CurrPosCar.horizontal <<" ver:" <<CurrPosCar.vertical << endl;
            PrintBoard();
            //Get a position that has not been checked before value = -1
            v_PosNotChecked = GetPosNotChecked(CurrPosCar); //F,L, R, B,
            if (v_PosNotChecked != 'O') //there is a position near-by have not been visited
            {
                fromPos = CurrPosCar;
                TurnTheCar(&CurrPosCar,v_PosNotChecked);

                //**************Car control************************
                //If not 'Forward' command we have to turn the car in {R, or L, or B}
                if (v_PosNotChecked != 'F')
                    {
                        //Physical turn the car
                        Command_Data(v_PosNotChecked);
                    }

                //Send a sensor signal to the car. Return value is a char with 0 or 1
                sensor_info = Command_Data('S');

                //**************Car control************************
                if (!checkBall(CurrPosCar,sensor_info)) //False, no ball, free go
                    {
                        GoForward(&CurrPosCar);
                        //**************Car control************************
                        //Physical: go forward
                        Command_Data('F');
                        step_count++;
                        cout << "done go forward for " << step_count << " steps." << endl;
                        //**************end Car control************************
                    }
                else //has a ball in front
                    {
                        //tmpPos = getPos(CurrPosCar,'F');
                        blockPosition(getPos(CurrPosCar,'F')); //set blocked to the front position
                    }
            }
        else //near-by positions have been visited. Now select a lowest visit
            {

                    v_MinPos = GetMinValue (CurrPosCar); //L, R, F, B
                    fromPos = CurrPosCar;
                    TurnTheCar(&CurrPosCar,v_MinPos);
                    //**************Car control************************
                    //Physical: Turn the car if needed
                    if (v_MinPos != 'F')
                        Command_Data(v_MinPos);  //turn the car

                    GoForward(&CurrPosCar);

                    //Physical: Go forward
                    Command_Data('F');
                    step_count++;
                    cout << "done go forward for " << step_count << " steps." << endl;
            }
        }

        std::cout << "Reach destination.";
    //**************end Car control************************

    return 0;
}
