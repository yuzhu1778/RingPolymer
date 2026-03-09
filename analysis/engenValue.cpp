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
    vector<int> mon;
    vector<double> x;
    vector<double> y;
    vector<double> xCopy;
    vector<double> yCopy;
    double CenterX;
    double CenterY;
    vector<vector<double>> matrix;
    vector<double> eigenValue;
    vector<double> eigenVector;
    vector<double> eigenV;
    vector<double> Vp;
    int type;
    double eigenRatio;
    double area;
    double orderPcosa;

};

struct Monomer
{
    double x;
    double y;
    int type;

};
struct Area
{
    double Area1;
    double Area2;
};
struct ratioAnalysis
{
    vector<double> ratio1;
    vector<int> CellIndex1;
    vector<double> ratio2;
    vector<int> CellIndex2;
    double AveRatio1;
    double AveRatio2;

};

    vector<double> AR1;
    vector<double> AR2;

vector<Cell> Cells;
vector<Monomer> mon;
vector<Area> AveArea;


Cell Calinertia(Cell cell);
Cell CaleigenValue(Cell cell);
Cell CalRatio(Cell cell);
Cell CaleigenVector(Cell Cell);
Cell CalVposition(Cell Cell);
Area CalAveArea(vector<Cell> Cells);
double reverseBoundaryCoordinate(double x, double AreaLength);


int main(int argc, char* argv[])
{

    if(argc != 7)
    {
        cout << "usage: " << argv[0] << " Index CellPosition MonPosition FrameNumber DensityNum DensityCoefficient" <<endl;
        return 0;
    }

      double AreaLength = 400;
      vector<double> area;
      area.push_back(AreaLength);
      int densityNum = atoi(argv[5]);
      double DensityCoefficient = atof(argv[6]);
      int TotalLoop = atoi(argv[4]);
      int CellNum;
      int MonNum;
        if(TotalLoop%densityNum != 0)
{
    TotalLoop -= TotalLoop%densityNum;
}

int FrameperDensity = TotalLoop/densityNum;

vector<double> orderParameter;

      ifstream GetCellPos;
    GetCellPos.open(argv[2], ios::in);

    ifstream GetMonPos;
    GetMonPos.open(argv[3], ios::in);



    vector<double> minValue;
    vector<double> maxValue;



 for(int lo = 0; lo < TotalLoop; lo++)
 {


    if(lo%FrameperDensity == 0&&lo != 0)
    {
        AreaLength *= DensityCoefficient;
        area.push_back(AreaLength);
    }
     //if(lo%1 == 0)
     if(lo != -1)
     {
    //cout << "Loop: " << lo << endl;
    char buf[1024];

    ratioAnalysis RatioAnalysis;





    /*
    for(int i = 0; i < Cells.size(); i++)
    {
        for(int j = 0; j < Cells[i].mon.size(); j++)
        {
            cout << Cells[i].mon[j] << " ";

        }
        cout << endl;
    }
*/

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
    /*
    for(int i = 0; i < CellNum; i++)
    {
        cout << Cells[i].Index << " " << Cells[i].CenterX << " " << Cells[i].CenterY << " " << Cells[i].type << endl;
    }

*/

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
    /*
    for(int i = 0; i < mon.size(); i++)
    {
        cout << mon[i].type << " " << mon[i].x << " " << mon[i].y << endl;
    }
    cout << mon.size() << endl;
*/

    //Read the Index of monomer for each cell
    ifstream GetIndex;
    string InfileName = argv[1];
    stringstream getIname;
    getIname << InfileName;
    getIname >> InfileName;
    GetIndex.open(InfileName);
    int contCell = 0;
    int temp;
    while(!GetIndex.eof())
    {
        GetIndex.getline(buf,1024);
        stringstream input(buf);
        //cout << buf << endl;
        while(input >> temp)
        {
            Cells[contCell].mon.push_back(temp);

        }
        contCell++;
        //cout << contCell << endl;
    }







    //connect monomer and cell
    for(int i = 0; i < Cells.size(); i++)
    {
        for(int j = 0; j < Cells[i].mon.size(); j++)
        {
            //cout << "test1" << endl;
            Cells[i].x.push_back(mon[Cells[i].mon[j]].x);
            Cells[i].y.push_back(mon[Cells[i].mon[j]].y);
            //cout << "test2" << endl;
            //cout << Cells[i].x[j] << " " << Cells[i].y[j] << endl;
        }
    }


    //fix boundary condition
    for(int i = 0; i < Cells.size(); i++)
    {
        for(int j = 0; j < Cells[i].x.size(); j++)
        {
            if(fabs(Cells[i].CenterX - Cells[i].x[j]) < 0.5*AreaLength)
            {
                Cells[i].xCopy.push_back(Cells[i].x[j]);
            }else if(fabs(Cells[i].CenterX - Cells[i].x[j]) >= 0.5*AreaLength)
            {
                Cells[i].xCopy.push_back(reverseBoundaryCoordinate(Cells[i].x[j],AreaLength));
            }

            if(fabs(Cells[i].CenterY - Cells[i].y[j]) < 0.5*AreaLength)
            {
                Cells[i].yCopy.push_back(Cells[i].y[j]);
            }else if(fabs(Cells[i].CenterY - Cells[i].y[j]) >= 0.5*AreaLength)
            {
                Cells[i].yCopy.push_back(reverseBoundaryCoordinate(Cells[i].y[j],AreaLength));
            }

        }
    }

    /*
    cout << "start" << endl;
    for(int i = 0; i < Cells.size(); i++)
    {
        for(int j = 0; j < Cells[i].xCopy.size();j++)
        {
            cout << Cells[i].xCopy[j] << " " << Cells[i].yCopy[j] << endl;
        }
    }
    cout << " end" << endl;

*/


////////////////////////////////////////////////////////////////////////////////////////////

// Calculate inertia and ratio;
for(int i = 0; i < CellNum; i++)
{
    Cells[i] = Calinertia(Cells[i]);
    Cells[i] = CaleigenValue(Cells[i]);
    //Cells[i] = CalRatio(Cells[i]);
    //Cells[i] = CaleigenVector(Cells[i]);
    //Cells[i] = CalVposition(Cells[i]);
}

/////////////////////////////////////////////////////////////////////////////////////////////////


double MAXV = 0;
double MINV = 0;
for(int i = 0; i < CellNum; i++)
{

    double maxV, minV;
    if(Cells[i].eigenValue[0] >= Cells[i].eigenValue[1])
    {
        maxV = Cells[i].eigenValue[0];
        minV = Cells[i].eigenValue[1];
    }
    else
    {
        maxV = Cells[i].eigenValue[1];
        minV = Cells[i].eigenValue[0];
    }
    MAXV += maxV;
    MINV += minV;

}

MAXV = MAXV/CellNum;
MINV = MINV/CellNum;

cout << lo << " " << MAXV << " " << MINV << endl;

maxValue.push_back(MAXV);
minValue.push_back(MINV);




    Cells.clear();
    mon.clear();
 }
 }



vector<double> aveMIN;
vector<double> aveMAX;

 for(int i = 0; i < densityNum; i++)
 {
    aveMAX.push_back(0);
    aveMIN.push_back(0);
 }



  int aveCon = 0;
 for(int i = 0; i < maxValue.size(); i++)
 {
     if(i % FrameperDensity == 0&&i != 0)
     {
         aveCon++;
     }
     aveMAX[aveCon] += maxValue[i];
     aveMIN[aveCon] += minValue[i];
 }


 for(int i = 0; i < densityNum; i++)
 {
     aveMAX[i] /= FrameperDensity;
     aveMIN[i] /= FrameperDensity;
     //cout << 484/(area[i]*area[i]) << " " << AR1Ave[i] << " " << AR2Ave[i] << endl;
     cout << CellNum/(area[i]*area[i]) << " " << aveMAX[i] << " " << aveMIN[i] << endl;
 }


    return 0;
}


Cell Calinertia(Cell cell)
{
    vector<vector<double>> temp;
    temp.resize(2);
    temp[0].resize(2);
    temp[1].resize(2);
    for(int i = 0; i < temp.size();i++)
    {
        for(int j = 0; j < temp[i].size(); j++)
        {
            temp[i][j] = 0;
        }
    }
    for(int i = 0; i < cell.mon.size(); i++)
        {

            temp[0][0] += pow((cell.yCopy[i] - cell.CenterY),2);
            temp[0][1] += (cell.xCopy[i] - cell.CenterX)*(cell.yCopy[i] - cell.CenterY);
            temp[1][1] += pow((cell.xCopy[i] - cell.CenterX),2);
        }
        temp[1][0] = temp[0][1];

        for(int i = 0; i <2; i++)
        {
            for(int j = 0; j < 2; j++)
            {
                if(fabs(temp[i][j]) < 0.000001)
                {
                    temp[i][j] = 0;
                }
            }
        }

        cell.matrix = temp;



        return cell;

}

Cell CaleigenValue(Cell cell)
{
    double a, b, c;
    a = 1;
    b = -(cell.matrix[0][0] + cell.matrix[1][1]);
    c =  cell.matrix[0][0] * cell.matrix[1][1] - cell.matrix[0][1]*cell.matrix[1][0];
    vector<double> temp;
    double x1 = (-b + sqrt(pow(b,2) - 4*a*c))/(2*a);
    double x2 = (-b - sqrt(pow(b,2) - 4*a*c))/(2*a);

    temp.push_back(x1);
    temp.push_back(x2);
    cell.eigenValue = temp;
    return cell;
}



Cell CalRatio(Cell cell)
{
    cell.eigenRatio = (cell.eigenValue[0] > cell.eigenValue[1]) ? cell.eigenValue[0]/cell.eigenValue[1] : cell.eigenValue[1]/cell.eigenValue[0];
    return cell;
}


double reverseBoundaryCoordinate(double x, double AreaLength)
{
	if(x > 0.5*AreaLength)
	{
		x = x - AreaLength;
	}
	else if(x < 0.5*AreaLength)
	{
		x = x + AreaLength;
	}
	return x;
}


Cell CaleigenVector(Cell Cell)
{
    Cell.eigenVector.resize(2);
   double a,b,c,d,x1,x2;

        if(Cell.eigenValue[0] >= Cell.eigenValue[1])
        {
        a = Cell.matrix[0][0] - Cell.eigenValue[0];
        b = Cell.matrix[0][1];
        c = Cell.matrix[1][0];
        d = Cell.matrix[1][1] - Cell.eigenValue[0];
        }else if(Cell.eigenValue[0] < Cell.eigenValue[1])
        {
        a = Cell.matrix[0][0] - Cell.eigenValue[1];
        b = Cell.matrix[0][1];
        c = Cell.matrix[1][0];
        d = Cell.matrix[1][1] - Cell.eigenValue[1];
        }
        if(fabs(a) < 0.000001)
        {
            a = 0;
        }
        if(fabs(b) < 0.000001)
        {
            b = 0;
        }
        if(fabs(c) < 0.000001)
        {
            c = 0;
        }
        if(fabs(d) < 0.000001)
        {
            d = 0;
        }
        //cout << a << " " << b << endl;
        //cout << c << " " << d << endl;
        if(a!=0)
        {
            x2 = a/sqrt(a*a+b*b);
            x1 = -b/a*x2;
        }else if(c != 0)
        {
            x2 = c/sqrt(c*c + d*d);
            x1 = -d/c*x2;
        }else if(b != 0)
        {
            x1 = b/sqrt(a*a + b*b);
            x2 = -a/b*x1;
        }else if(d != 0)
        {
            x1 = d/sqrt(c*c + d*d);
            x2 = -c/d*x1;
        }

        if((a == 0&& d == 0&&c != 0&&d != 0)||(c == 0 && b == 0&&a!=0&&d!=0))
        {
            x1 = 0;
            x2 = 0;
        }
        if(a == 0 && b == 0 && c == 0 && d == 0)
        {
            x1 = 1;
            x2 = 0;
        }
        //cout << "x12" << endl;
        //cout << x1 << x2 << endl;
        //cout << "test2" << endl;
        Cell.eigenVector.push_back(x1);
        Cell.eigenVector.push_back(x2);

        //cout << "test3" << endl;
    double temp1,temp2;
    //temp1 = Cell.eigenVector[0][0] + Cell.eigenValue[1]/Cell.eigenValue[0]*Cell.eigenVector[1][0];
    //temp2 = Cell.eigenVector[0][1] + Cell.eigenValue[1]/Cell.eigenValue[0]*Cell.eigenVector[1][1];
    temp1 = x1;
    temp2 = x2;
    Cell.eigenV.push_back(temp1);
    Cell.eigenV.push_back(temp2);// x and y should switch in the real situation
    //cout << " pos" << endl;
    //cout << Cell.eigenV[0] << " " << Cell.eigenV[1] << endl;
    return Cell;
}


Cell CalVposition(Cell Cell)
{
    //cout << " pos" << endl;
    //cout << Cell.eigenV[0] << " " << Cell.eigenV[1] << endl;
    vector<double> temp;
    double a;
    a = Cell.CenterX - 0.1*0.5*Cell.eigenRatio*Cell.eigenV[1];
    temp.push_back(a);
    a = Cell.CenterY - 0.1*0.5*Cell.eigenRatio*Cell.eigenV[0];
    temp.push_back(a);
    a = Cell.CenterX + 0.1*0.5*Cell.eigenRatio*Cell.eigenV[1];
    temp.push_back(a);
    a = Cell.CenterY + 0.1*0.5*Cell.eigenRatio*Cell.eigenV[0];
    temp.push_back(a);

    Cell.Vp = temp;
    return Cell;
}

Area CalAveArea(vector<Cell> Cells)
{
    Area temp;
    temp.Area1 = 0;
    temp.Area2 = 0;
     int nth1 = 0;
     int nth2 = 0;
    for(int i = 0; i < Cells.size(); i++)
    {
        if(Cells[i].type == 1)
        {
            temp.Area1 += Cells[i].area;
            nth1++;
        }else if(Cells[i].type == 2)
        {
            temp.Area2 += Cells[i].area;
            nth2++;
        }
    }

    temp.Area1 /= nth1;
    temp.Area2 /= nth2;
    return temp;
}



