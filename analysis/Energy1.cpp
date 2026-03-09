#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>
#include <cstdlib>


using namespace std;

struct Monomer
{
    double x;
    double y;
    double xCopy;
    double yCopy;
    int type;
    int RightNeighbor;
    int LeftNeighbor;
    double SpringL;
    double BendE;
    double BondE;

};
struct Cell
{
    vector<int> monomers;
};


double TwoPointsDistance(double x1, double y1, double x2, double y2)
{
	double Res;
	Res = sqrt(pow((x1-x2),2)+pow((y1-y2),2));

	return Res;
}


int main(int argc, char* argv[])
{
     if(argc != 3)
    {
        cout << "usage: " << argv[0] << " monPosition kp" <<endl;
        return 0;
    }

vector<Cell> Cells;
vector<Monomer> mon;

    double kp = atof(argv[2]);
    double AreaLength = 400;
    double HalfAreaLength = 0.5*AreaLength;
    int TotalLoop = 60000;
    double ReduceFactor = 0.9;
    int densityNum = 12;
    int framePerDensity = TotalLoop/densityNum;
    int PolymerLength = 40;
    int CellNum = 484;

//cout << "1 " << endl;
    ifstream GetMonPos;
    GetMonPos.open(argv[1], ios::in);



    for(int lo = 0; lo < TotalLoop; lo++)
    {
       Cells.resize(CellNum);
        if(lo%framePerDensity ==0 && lo != 0)
        {
            AreaLength *= ReduceFactor;
            HalfAreaLength = 0.5*AreaLength;
        }

        char buf[1024];
        vector<double> temparr;
        double temppos;
        int MonNum;

         //Read the position of Monomers
    GetMonPos.getline(buf,1024);
    if(!atoi(buf))
    {
        GetMonPos.close();
        return 0;
    }
    MonNum = atoi(buf);
    mon.resize(MonNum);
    GetMonPos.getline(buf,1024);
    for(int i = 0; i < MonNum; i++)
    {
        GetMonPos.getline(buf,1024);
        stringstream input(buf);
        while(input >> temppos)
        {
            temparr.push_back(temppos);
        }
        mon[i].type = temparr[0];
        mon[i].x = temparr[1];
        mon[i].y = temparr[2];
        temparr.clear();
    }

    //cout << "2 " << endl;
    for (int i = 0; i < mon.size();i++)
    {
       // cout << i<<" "<< mon[i].x << " " << mon[i].y << endl;
    }


    int NN = 0;
    //cout << mon.size() << " " << Cells.size()<< endl;
    for(int i = 0; i < CellNum; i++)
    {
        for(int j = 0; j < PolymerLength; j++)
        {

             if(j > 0 && j < PolymerLength -1)
            {
                mon[NN].LeftNeighbor = NN -1;
                mon[NN].RightNeighbor = NN + 1;
            }
            else if(j == 0)
            {
                mon[NN].LeftNeighbor = NN + PolymerLength - 1;
                mon[NN].RightNeighbor = NN + 1;
            }
            else if(j == PolymerLength - 1)
            {
                mon[NN].LeftNeighbor = NN - 1;
                mon[NN].RightNeighbor = NN - PolymerLength + 1;
            }
                    Cells[i].monomers.push_back(NN);
                    NN++;

        }


    }
   // cout << "3 " << endl;

/*
for(int i = 0; i < Cells.size(); i++)
{
    for(int j = 0 ; j < Cells[i].monomers.size();j++)
    {
        cout << Cells[i].monomers[j] << " ";
    }
    cout << endl;
}
*/




for(int i = 0; i < Cells.size(); i++)
    {
        double X_flag = mon[Cells[i].monomers[0]].x;
        double Y_flag = mon[Cells[i].monomers[0]].y;
        mon[Cells[i].monomers[0]].xCopy = mon[Cells[i].monomers[0]].x;
        mon[Cells[i].monomers[0]].yCopy = mon[Cells[i].monomers[0]].y;
        for(int j = 1; j < Cells[i].monomers.size(); j++)
        {
            mon[Cells[i].monomers[j]].xCopy = mon[Cells[i].monomers[j]].x;
            mon[Cells[i].monomers[j]].yCopy = mon[Cells[i].monomers[j]].y;

            if(abs(mon[Cells[i].monomers[j]].x - X_flag) > HalfAreaLength)
            {
                if(X_flag < HalfAreaLength)
                {
                    mon[Cells[i].monomers[j]].xCopy = mon[Cells[i].monomers[j]].x - AreaLength;
                }else if(X_flag >= HalfAreaLength)
                {
                    mon[Cells[i].monomers[j]].xCopy = mon[Cells[i].monomers[j]].x + AreaLength;
                }
            }
            if(abs(mon[Cells[i].monomers[j]].y - Y_flag) > HalfAreaLength)
            {
                if(Y_flag < HalfAreaLength)
                {
                    mon[Cells[i].monomers[j]].yCopy = mon[Cells[i].monomers[j]].y - AreaLength;
                }else if(Y_flag >= HalfAreaLength)
                {
                    mon[Cells[i].monomers[j]].yCopy = mon[Cells[i].monomers[j]].y + AreaLength;
                }

            }


        }
    }
    //cout << "4 " << endl;

    for (int i = 0; i < mon.size();i++)
    {
        //cout << i<<" "<< mon[i].xCopy << " " << mon[i].yCopy << endl;
    }

     for(int i = 0; i < mon.size(); i++)

        {
        mon[i].SpringL = TwoPointsDistance(mon[i].xCopy, mon[i].yCopy, mon[mon[i].RightNeighbor].xCopy, mon[mon[i].RightNeighbor].yCopy);
        }

//cout << "5 " << endl;
    double BendE = 0;
    double BondE = 0;
    for(int n = 0; n < mon.size(); n++)
    {
         int j1, j3;
            j1 = mon[n].RightNeighbor;
            j3 = mon[n].LeftNeighbor;
            mon[n].BondE = 0.5*kp*pow((mon[n].SpringL - a),2);
            double cosn = ((mon[j1].xCopy-mon[n].xCopy)*(mon[j3].xCopy-mon[n].xCopy)+(mon[j1].yCopy-mon[n].yCopy)*(mon[j3].yCopy-mon[n].yCopy))/(mon[n].SpringL*mon[j3].SpringL);
            mon[n].BendE = kp*(cosn + 1.0);
            BendE += mon[n].BendE;
            BondE += mon[n].BondE;






    }



    cout << lo << " " << BendE << " " << BondE << endl;

    Cells.clear();
    mon.clear();
//cout << "6 " << endl;
    }







    return 0;





}
