#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

struct Cell
{
    int Index;
    double CenterX;
    double CenterY;
    int type;
    vector<int> neighbor;
    double area;
    double eigenX;
    double eigenY;
    vector<double> cosa;
    vector<double> angle;
};

vector<Cell> Cells;

int main(int argc, char* argv[])
{

    if(argc != 6)
    {
        cout << "usage: " << argv[0] << " CellsCenter Orientation FrameNumber DensityNum DensityCoefficient" << endl;
        return 0;
    }

    int CellPoints;
    int TotalLoop = atoi(argv[3]);
    int densityNum = atoi(argv[4]);
    if(TotalLoop%densityNum != 0)
{
    TotalLoop -= TotalLoop%densityNum;
}

    double DensityCofficient = atof(argv[5]);
    int FrameperDensity = TotalLoop/densityNum;
    vector <double> density;


    double bonL = 400;

        ifstream Rfile;
        Rfile.open(argv[1], ios::in);

        ifstream Rfile1;
        Rfile1.open(argv[2], ios::in);




    for(int lo = 0; lo < TotalLoop; lo++)
    {

        char buf[1024];
        double temp;


        double tempcount = 0;
        Rfile.getline(buf,1024);
        if(!atoi(buf))
        {
            Rfile.close();
            return 0;
        }
        CellPoints = atoi(buf);
        Cells.resize(CellPoints);

        if(lo%FrameperDensity == 0 &&lo != 0)
        {
            bonL *= DensityCofficient;
            density.push_back(CellPoints/(bonL*bonL));
        }else if(lo == 0)
        {
            density.push_back(CellPoints/(bonL*bonL));
        }
        cout << CellPoints << " " << bonL << endl;



        for(int j = 0; j < CellPoints;j++)
        //while(!Rfile.eof())
        {
            Rfile.getline(buf,1024);
            stringstream input(buf);
            //cout << buf << endl;
            vector<double> temparr;
            while(input >> temp)
            {
                temparr.push_back(temp);
            }
            input.str("");
            Cells[tempcount].Index = int(temparr[0]);
            Cells[tempcount].CenterX = temparr[1];
            Cells[tempcount].CenterY = temparr[2];
            Cells[tempcount].type = int(temparr[3]);
            Cells[tempcount].area = temparr[4];
            temparr.clear();
            tempcount++;
        }


  /*
        //Read the position of Cells
    vector<double> temparr;
    double temppos;
    GetCellPos.getline(buf,1024);
    if(!atoi(buf))
    {
        GetCellPos.close();
        return 0;
    }
    CellNum = atoi(buf);
    Cells.resize(CellNum);
    for(int i = 0; i < CellNum; i++)
    {
        if(GetCellPos.eof())
        {
            cout << "File format not recognized!" << endl;
            GetCellPos.close();
            return 0;
        }
        GetCellPos.getline(buf,1024);
        stringstream input(buf);
        while(input >> temppos)
        {
            temparr.push_back(temppos);
        }
        Cells[i].Index = temparr[0];
        Cells[i].CenterX = temparr[1];
        Cells[i].CenterY = temparr[2];
        Cells[i].type = temparr[3];
        Cells[i].area = temparr[4];
        temparr.clear();
    }
*/


        char buf1[1024];
        double temp1;
        //cout << 3 << endl;
        Rfile1.getline(buf,1024);
        if(!atoi(buf))
        {
            Rfile1.close();
            return 0;
        }
        CellPoints = atoi(buf);
        int tempcount1 = 0;
        for(int j = 0; j < CellPoints; j++)
        //while(!Rfile1.eof())
        {
            Rfile1.getline(buf1, 1024);
            //cout << buf1;
            stringstream Input(buf1);
            vector<double> temparr1;
            while(Input >> temp1)
            {   //cout << temp1 << endl;
                temparr1.push_back(temp1);
                //cout << temp1 << endl;
            }
            //cout << "5 " << endl;
            Input.str("");
            if(Cells.size() > 0)
            //if(Cells[tempcount].Index == int(temparr[0]))
            {
                Cells[tempcount1].eigenX = temparr1[1];
                Cells[tempcount1].eigenY = temparr1[2];
            }else
            {
                return -1;
            }
            cout << Cells[tempcount1].CenterX << " " << Cells[tempcount1].CenterY << " " << Cells[tempcount1].eigenX << " " << Cells[tempcount1].eigenY << endl;
            tempcount1++;
            temparr1.clear();
        }



}



    return 0;
}

