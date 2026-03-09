#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

#define Pi 3.1415926;
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


////////////////////////////////////////////////////////////////////////////Domain structure and function
struct Domain
{
    vector<int> cell;
    int type;
    double area;
};

vector <Domain> Doms1;
vector <Domain> Doms2;
vector <Domain> Domains;
Domain FindDom(Domain Dom, int Index, vector<Cell> mon);


////////////////////////////////////////////////////////////////////////////Domain structure and function

vector<Cell> Calcorrelation(vector<Cell> Cells);
vector<Cell> Cells;


int main(int argc, char* argv[])
{

    if(argc != 6)
    {
        cout << "usage: " << argv[0] << " Neighbor Orientation FrameNumber DensityNum DensityCoefficient" << endl;
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
    vector<double> Correlation;

    vector<double> corr11;
    vector<double> corr22;
    vector<double> corr12;
    vector<vector<int>> AngleCount;
    vector<double> AverageDomain;
    vector<double> AverageDomainArea;
    vector<double> AverageDomainNumber; // average number of polymer each domain.
    vector<vector<double>> DomainDistribution;
    int MaxDomainSize = 50;

    DomainDistribution.resize(densityNum);
    for(int i = 0; i < densityNum; i++)
    {
        AverageDomain.push_back(0);
        AverageDomainArea.push_back(0);
        AverageDomainNumber.push_back(0);
        DomainDistribution[i].resize(MaxDomainSize);
        for(int j = 0; j < MaxDomainSize; j++)
        {
            DomainDistribution[i][j] = 0;
        }
    }

    AngleCount.resize(TotalLoop);

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
            for(int i = 5; i < temparr.size(); i++)
            {
                Cells[tempcount].neighbor.push_back(temparr[i]);
            }
            temparr.clear();
            tempcount++;
        }




        //cout << "2" << endl;


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
            tempcount1++;
            temparr1.clear();
        }





        //cout << "loop" << lo<<endl;
        /*
        for(int i = 0; i < Cells.size(); i++)
        {
            cout << Cells[i].Index << " " << Cells[i].CenterX << " " << Cells[i].CenterY << " " << Cells[i].eigenX << " " << Cells[i].eigenY << " " << Cells[i].type << " ";
            for(int j = 0; j < Cells[i].neighbor.size(); j++)
            {
                cout << Cells[i].neighbor[j] << " ";
            }
            cout << endl;
        }
*/
//cout << "test" << endl;
        // Cal correlation
        Cells = Calcorrelation(Cells);
//cout << "test" << endl;



 ///////////////////////////////////////////////////////////////////////Find Domain
 //Find Domains
 if(lo != -1)
{

    vector<int> Coundex;
    for(int i = 0; i < CellPoints; i++)
    {
        Coundex.push_back(i);
    }

//cout << "Loops " << lo << endl;

    while(Coundex.size()>0)
    {
        //cout << Coundex.size()<< endl;
        Domain tempDom;
        tempDom.type = Cells[Coundex[0]].type;
        tempDom.cell.push_back(Coundex[0]);
        vector<int>::iterator it1 = Coundex.begin();
        it1 = Coundex.erase(it1);
        tempDom = FindDom(tempDom, tempDom.cell[0], Cells);

       //if(tempDom.cell.size() > 1)
       {
       Domains.push_back(tempDom);
       }
        /*
        if(tempDom.type == 1)
        {
            Doms1.push_back(tempDom);
        }
        else if(tempDom.type == 2)
        {
            Doms2.push_back(tempDom);

        }
        */
        //cout << "test" << endl;
        for(int i = 0; i < tempDom.cell.size(); i++)
        {
            for(vector<int>::iterator it2 = Coundex.begin(); it2 != Coundex.end();)
                if(*it2 == tempDom.cell[i])
            {
                it2 = Coundex.erase(it2);
            }
            else
            {
                it2++;
            }

        }

/*
if(lo%100 == -1)
{
        for(int i = 0; i < tempDom.cell.size(); i++)
        {
            cout << tempDom.cell[i] << " ";
        }
        cout << endl;
}
*/
    }

    double tempDomainArea = 0;
    double tempNumPoylmer = 0;
    vector<int> tempDomDistribution;
    for(int i = 0; i < MaxDomainSize; i++)
    {
        tempDomDistribution.push_back(0);
    }

    for(int i = 0; i < Domains.size(); i++)
    {
        int temIndex = Domains[i].cell.size() - 1;
        if(Domains[i].cell.size() > MaxDomainSize)
        {
            temIndex = MaxDomainSize - 1;
        }
        tempDomDistribution[temIndex]++;
    }


    int DomainCondition = 3;
    int DomainCount = 0;
    for(int i = 0; i < Domains.size(); i++)
    {
        if(Domains[i].cell.size() >= DomainCondition)
        {
        DomainCount++;
        for(int j = 0; j < Domains[i].cell.size(); j++)
        {
            tempDomainArea += Cells[Domains[i].cell[j]].area;
        }
        tempNumPoylmer += Domains[i].cell.size();
        }
    }
    tempDomainArea /= DomainCount;
    tempNumPoylmer /= DomainCount;



    AverageDomain[lo/FrameperDensity] += DomainCount;


    AverageDomainArea[lo/FrameperDensity] += double(tempDomainArea);
    AverageDomainNumber[lo/FrameperDensity] += double(tempNumPoylmer);

    for(int i = 0; i < MaxDomainSize; i++)
    {
        DomainDistribution[lo/FrameperDensity][i] += tempDomDistribution[i];
    }

    if(lo%100 == 0)
    {
    cout << lo <<" "<< DomainCount << " " << tempDomainArea << " " << tempNumPoylmer << " ";
    for(int i = 0; i < MaxDomainSize; i++)
    {
        cout << tempDomDistribution[i] << " ";
    }
    cout << endl;
    }

    Domains.clear();

    }

   // cout << "Find domain" << endl;



 ///////////////////////////////////////////////////////////////////////Find Domain



 /*

        double tempCorre = 0;
        int totalKcount = 0;
        for(int i = 0; i < Cells.size();i++)
        {
            for(int j = 0; j < Cells[i].angle.size();j++)
            {
            tempCorre += Cells[i].angle[j];
            //cout.width(10);

            //cout << tempCorre << " " << totalKcount << " " << Cells[i].angle[j] << " " <<Cells[i].cosa[j] <<endl;
            totalKcount++;
            }
        }
        //cout << tempCorre << " " << totalKcount << endl;
        tempCorre /= totalKcount;
    //cout << tempCorre << " " << totalKcount << endl;
        Correlation.push_back(tempCorre);




    double tem11 = 0, tem22 = 0, tem12 = 0;
    double count11 = 0, count22 = 0, count12 = 0;
    for(int i = 0; i < Cells.size(); i++)
    {
        for(int j = 0; j < Cells[i].neighbor.size(); j++)
        {
            double temI = Cells[i].neighbor[j];
            if(Cells[i].type == 1 && Cells[temI].type == 1)
            {
                tem11 += Cells[i].angle[j];
                count11++;
            }else if(Cells[i].type == 2 && Cells[temI].type == 2)
            {
                tem22 += Cells[i].angle[j];
                count22++;
            }else if((Cells[i].type == 1&& Cells[temI].type == 2)||(Cells[i].type == 2 && Cells[temI].type == 1))
            {
                tem12 += Cells[i].angle[j];
                count12++;
            }
        }
    }
    //cout << count11 << " " << count22 << " " << count12 << endl;
    tem11 /= count11;
    corr11.push_back(tem11);
    tem22 /= count22;
    corr22.push_back(tem22);
    tem12 /= count12;
    corr12.push_back(tem12);



        if(lo%FrameperDensity == (FrameperDensity-1) &&lo != 0)
        {
            ofstream coutAngle, cout11A,cout22A,cout12A;
            stringstream WAname;
            string Aname;
            WAname << "Angle" << lo;
            WAname >> Aname;
            coutAngle.open(Aname,ios::app);
            WAname.clear();
            WAname << "ARR" << lo;
            WAname >> Aname;
            cout11A.open(Aname, ios::app);
            WAname.clear();
            WAname << "ASS" << lo;
            WAname >> Aname;
            cout22A.open(Aname, ios::app);
            WAname.clear();
            WAname<< "ARS" << lo;
            WAname >> Aname;
            cout12A.open(Aname, ios::app);
            WAname.clear();

            //cout << "loop" << lo << endl;

            for(int i = 0; i < Cells.size(); i++)
            {
                for(int j = 0 ; j < Cells[i].angle.size();j++)
                {
                coutAngle << Cells[i].angle[j] << endl;
                if(Cells[i].type != Cells[Cells[i].neighbor[j]].type)
                {
                    cout12A << Cells[i].angle[j] << endl;
                }else if(Cells[i].type == 1 && Cells[Cells[i].neighbor[j]].type == 1)
                {
                    cout11A << Cells[i].angle[j] << endl;
                }else if(Cells[i].type == 2 && Cells[Cells[i].neighbor[j]].type == 2)
                {
                    cout22A << Cells[i].angle[j] << endl;
                }
                }
            }

        }

     //Angle Distribution Analysis
     int AngleCountInterve = 5;
    for(int i = 0; i < 90/AngleCountInterve; i++)
     {
         AngleCount[lo].push_back(0);
     }


     for(int i = 0; i < Cells.size(); i++)
     {
         for(int j = 0; j < Cells[i].angle.size(); j ++)
         {

             if(Cells[i].angle[j] < 90)
             {
             int tempIndex = Cells[i].angle[j]/AngleCountInterve;
             AngleCount[lo][tempIndex]++;
             }else if(Cells[i].angle[j] >= 90)
             {
                 AngleCount[lo][int(90/AngleCountInterve)]++;
             }

         }
     }

        /*
        cout << "loop" << lo << "  ";
        for(int i = 0; i < AngleCount[lo].size();i++)
        {

            cout << AngleCount[lo][i] << " ";
        }
        cout << endl;

*/




        ///////////////////////////////////////////////////

        Cells.clear();


    }

    Rfile.close();
    Rfile1.close();
    /////////END LOOP HERE


/*
///////////////////////////////////////////////////////////////////////////

cout << "Angle distribution with different density: " <<endl;
vector<vector<double>> AngleVsDensity;
AngleVsDensity.resize(density.size());
for(int i = 0; i < AngleVsDensity.size(); i++)
{
    AngleVsDensity[i].resize(AngleCount[0].size());
    for(int j = 0; j < AngleVsDensity[i].size(); j++)
    {
        AngleVsDensity[i][j] = 0;
    }
}

int AngIndex = 0;
for(int i = 0; i < AngleCount.size(); i++)
{

   if(i%(TotalLoop/density.size()) == 0&& i != 0)
        {
            AngIndex++;
        }
    for(int j = 0; j < AngleCount[i].size(); j++)
    {
        AngleVsDensity[AngIndex][j] += AngleCount[i][j];
    }
}


for(int i = 0; i < AngleVsDensity.size(); i++)
{
    cout << density[i] << " ";
    for(int j = 0; j < AngleVsDensity[i].size(); j++)
    {
        AngleVsDensity[i][j]/= (TotalLoop/density.size());
        cout << AngleVsDensity[i][j] << " ";
    }
    cout << endl;
}
*/

////////////////////////////////////////////////////////////////////////





/*




    cout << "time correlation:" << endl;
    for(int i = 0; i < Correlation.size(); i++)
    {
        cout<< i << " " << Correlation[i] << " "<< corr11[i] << " " << corr22[i] << " " << corr12[i]  << endl;
        //cout<< i << " " << Correlation[i]   << endl;
    }


    int corrCount = 0;
    vector<double> tempcorrelation;
    vector<double> tempcorr11;
    vector<double> tempcorr22;
    vector<double> tempcorr12;
    for(int i = 0; i < density.size();i++)
    {
        tempcorrelation.push_back(0);
        tempcorr11.push_back(0);
        tempcorr12.push_back(0);
        tempcorr22.push_back(0);
    }
    for(int i = 0; i < Correlation.size(); i++)
    {
        if(i%FrameperDensity == 0&& i != 0)
        {
            corrCount++;
        }
        tempcorrelation[corrCount] += Correlation[i];
        tempcorr11[corrCount] += corr11[i];
        tempcorr12[corrCount] += corr12[i];
        tempcorr22[corrCount] += corr22[i];
    }

    cout << "density correlation" << endl;
    for(int i = 0; i < density.size(); i++)
    {
        tempcorrelation[i]/= (Correlation.size()/density.size());
        tempcorr11[i] /= (corr11.size()/density.size());
        tempcorr12[i] /= (corr12.size()/density.size());
        tempcorr22[i] /= (corr22.size()/density.size());
        cout << density[i] << " " << tempcorrelation[i] << " " << tempcorr11[i] << " " << tempcorr22[i] << " " << tempcorr12[i] << endl;
    }
    */

    cout << "density Average_number_of_domains" << endl;
    for(int i = 0; i < density.size(); i++)
    {
        AverageDomain[i] /= FrameperDensity;
        cout << density[i] << " " << AverageDomain[i] << endl;
    }

     cout << "density Average_area_of_domains" << endl;
    for(int i = 0; i < density.size(); i++)
    {
        AverageDomainArea[i] /= FrameperDensity;
        cout << density[i] << " " << AverageDomainArea[i] << endl;
    }
    cout << "density Average_number_of_polymer per Domain" << endl;
    for(int i = 0; i < density.size(); i++)
    {
        AverageDomainNumber[i] /= FrameperDensity;
        cout << density[i] << " " << AverageDomainNumber[i] << endl;
    }

    cout << "density Domain Distribution" << endl;
    for(int i = 0; i < density.size(); i++)
    {
        cout << density[i] << " ";
        for(int j = 0; j < MaxDomainSize; j++)
        {
            DomainDistribution[i][j] /= FrameperDensity;
            cout << DomainDistribution[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}

vector<Cell> Calcorrelation(vector<Cell> Cells)
{
    double cosa;
    for(int i = 0; i < Cells.size(); i++)
    {
      //cout << "ce1" << endl;
      for(int j = 0; j < Cells[i].neighbor.size(); j++)
       {
           int tempI;
           tempI = Cells[i].neighbor[j];
           cosa = (Cells[i].eigenX*Cells[tempI].eigenX + Cells[i].eigenY*Cells[tempI].eigenY)/(1*1);
           if(cosa < 0)
           {
               cosa = -cosa;
           }
           if(cosa > 1)
           {
               cosa = 1;
           }
           double angle = acos(cosa)*180/3.1415;
            /*if(abs(cosa - 1)< 0.0001)
            {
                angle = 0;
            }
            */
           Cells[i].cosa.push_back(cosa);
           Cells[i].angle.push_back(angle);
           //cout << Cells[i].cosa[j] << " " << Cells[i].angle[j] << endl;
           //if(Cells[i].angle[j] > 90 ||Cells[i].angle[j] < 0)
           //cout << "angle: " << Cells[i].angle[j] << endl;
           //cout << j << endl;
       }
    }
    return Cells;
}


////////////////////////////////////////Find Domain

Domain FindDom(Domain Dom, int Index, vector<Cell> mon)
{
  //int Flag = Dom.type;
  for(int i = 0; i < mon[Index].neighbor.size(); i++)
  {
    if((mon[Index].angle[i] <= 5)&&count(Dom.cell.begin(),Dom.cell.end(),mon[Index].neighbor[i])==0)
    //if(mon[mon[Index].neighbor[i]].type == Dom.type&&count(Dom.cell.begin(),Dom.cell.end(),mon[Index].neighbor[i])==0)
    {

        Dom.cell.push_back(mon[Index].neighbor[i]);
        Dom = FindDom(Dom, mon[Index].neighbor[i], mon);

    }
  }
  return Dom;
}



