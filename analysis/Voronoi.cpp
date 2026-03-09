#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Periodic_2_Delaunay_triangulation_2.h>
#include <CGAL/Periodic_2_Delaunay_triangulation_traits_2.h>

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Periodic_2_Delaunay_triangulation_traits_2<K> PK;
typedef CGAL::Periodic_2_Delaunay_triangulation_2<PK>       P2DT2;

typedef PK::Point_2        Point;
typedef PK::Triangle_2     Triangle;

typedef P2DT2::Periodic_triangle           Periodic_triangle;
typedef P2DT2::Periodic_triangle_iterator  Periodic_triangle_iterator;
typedef P2DT2::Iterator_type               Iterator_type;

using namespace std;


//g++ Voronoi.cpp -lgmp 
struct Pointstruct{float x; float y;};
struct monomer
{
    int Index;
    float x;
    float y;
    int type;
    vector<int> neighbor;
    double area;
};
struct Domain
{
    vector<int> cell;
    int type;
    double area;
};

vector <Domain> Doms1;
vector <Domain> Doms2;

Domain FindDom(Domain Dom, int Index, vector<monomer>mon);


int main(int argc, char* argv[])
{


    if(argc != 5)
    {
        cout << "usage: " << argv[0] << " CellPositionName FrameNumber DensityNumber DensityCoefficient" << endl;
        return 0;
    }


vector<float> AveDomain;
vector<int> numDomain;
vector<int> FnumDomain;
vector<int> SnumDomain;
vector <int> TLength;
vector <double> neighborsize;   // Find the average numbers of each monomer for each frame
vector<vector<int>> neinum;       // total neighbor number
vector<vector<int>> rigidneinum; //rigid neighbor number
vector<vector<int>> softneinum; // soft neighbor number
neinum.resize(6);
rigidneinum.resize(6);
softneinum.resize(6);
vector<double> aveArea1;
vector<double> aveArea2;
vector<double> aveCellArea1;
vector<double> aveCellArea2;
vector <double> density;

int CellNum;
int TotalLoop = atoi(argv[2]);
int densityNum = atoi(argv[3]);
int numpoints;
double DensityCoefficient = atof(argv[4]);
float tempb = 400;

if(TotalLoop%densityNum != 0)
{
    TotalLoop -= TotalLoop%densityNum;
}
int FrameperDensity = TotalLoop/densityNum;

ifstream GetCellPosition;
GetCellPosition.open(argv[1], ios::in);
if(GetCellPosition.good()){
		//cout <<"reading file" <<endl;
	}
 		else {
			cout <<"FAIL " << endl;
			if(GetCellPosition.rdstate()&std::ifstream::failbit){
				cout << "failBit" << endl;
			}
		}



    ofstream outNeighbors;
    stringstream NeiName;
    NeiName << "neighbor";
    string neighborname;
    NeiName >> neighborname;
    outNeighbors.open(neighborname, ios::app);


for(int lo = 0; lo < TotalLoop; lo++)
{

        Pointstruct tp [5000];

if(lo%1 == 0)
{
    //cout << "Loop" << lo << endl;
    string filename= "CC";

vector<monomer> mon;










char buf1[128];
GetCellPosition >> buf1;
if(!atoi(buf1))
{
    GetCellPosition.close();
    return 0;
}
numpoints = atoi(buf1);
CellNum = atoi(buf1);

    mon.resize(numpoints);
    float xcom[numpoints];
    float ycom[numpoints];

        if(lo%FrameperDensity == 0 && lo != 0)
        {
            tempb *= DensityCoefficient;
            density.push_back(numpoints/(tempb*tempb));
        }else if(lo == 0)
        {
            density.push_back(numpoints/(tempb*tempb));
        }

        float bx=tempb, by=tempb;
        P2DT2 T(P2DT2::Iso_rectangle(0,0,bx,by));
        float coord;


  for(int i=0; i<numpoints; i++){
		for (int j=0; j<5; j++){

                if(GetCellPosition.eof())
                {
                    cout << "File format not recognized!" << endl;
                    GetCellPosition.close();
                    return 0;
                }
			char buf[128];
			GetCellPosition >> buf;
			coord = atof(buf);
			if(j == 0){mon[i].Index = coord;}
			if(j == 1){xcom[i] = coord;mon[i].x = coord;}
			if(j == 2){ycom[i] = coord;mon[i].y = coord;}
            if(j == 3){mon[i].type = coord;}
			if(j == 4){mon[i].area = coord;}
		}
  }

	//endloop:
//cout << "start" << endl;
int errorCount = 0;
  for(int i=0; i<numpoints; i++){
    //cout << i << " " << xcom[i] <<" " << ycom[i] << endl;
    if(xcom[i] >= bx)
    {
        xcom[i] -= bx;
        mon[i].x -= bx;
        errorCount++;
    }
    if(ycom[i] >= by)
    {
        ycom[i] -=bx;
        mon[i].y -= bx;
        errorCount++;
    }
    if(xcom[i] < bx && ycom[i] < by &&  xcom[i] >= 0 && ycom[i] >= 0)
    {
  	T.insert(Point(xcom[i],ycom[i]));
    }else
    {
        cout << i << " " << bx << " " << by << " " << xcom[i] << " " << ycom[i] << endl;
        return -1;
    }
    }
//cout <<"end" << endl;

int check = 0;
for(int i =0; i < numpoints; i++)
{
    //cout << mon[i].type << endl;
    check++;
}
//cout << check << endl;

  Periodic_triangle pt;
  Triangle t_bd;
  /*
      std::cout << T.domain()[0].x() << ' ' << T.domain()[0].y() << std::endl;
      std::cout << T.domain()[1].x() << ' ' << T.domain()[1].y() << std::endl;
      std::cout << T.domain()[2].x() << ' ' << T.domain()[2].y() << std::endl;
      std::cout << T.domain()[3].x() << ' ' << T.domain()[3].y() << std::endl;
  */

  int numtp=0;
  int itr=0;
  int id;



    ofstream outT;
    if(lo == -1) // adjust outT in the loop together
    {
    stringstream Tname;
    string tname;
    Tname << "T" << lo << ".xy";
    Tname >> tname;
    outT.open(tname, ios::app);
    }

  for (Periodic_triangle_iterator ptit = T.periodic_triangles_begin(P2DT2::UNIQUE_COVER_DOMAIN);
       ptit != T.periodic_triangles_end(P2DT2::UNIQUE_COVER_DOMAIN); ++ptit)
    {
      pt = *ptit;//array index

        //cout << "x1" << endl;
      int temp[3];
      for(int i=0; i<3; i++){
        id = itr + i;
        tp[id].x = pt[i].first.x()+pt[i].second.x()*bx;
        tp[id].y = pt[i].first.y()+pt[i].second.y()*by;//affect boundary
        //cout << "x1.5" << endl;
        float tempX = tp[id].x;
        float tempY = tp[id].y;
        //cout << tempX << ' ' << tempY << endl;

        if(tempX < 0){tempX += bx;}
        else if(tempX >= bx){tempX -= bx;} //sometimes I did not care the equal situation, this will make the segment default
        if(tempY < 0){tempY +=by;}
        else if(tempY >= by){tempY -= by;}

        for(int ii = 0; ii < numpoints; ii++)
        {
            //if(mon[ii].x == tp[id].x&&mon[ii].y == tp[id].y)
            if(fabs(mon[ii].x - tempX)<0.001&&fabs(mon[ii].y-tempY)<0.001)
            {
                temp[i] = mon[ii].Index;
                //cout << temp[i] << endl;
            }
        }

    //cout << i << endl;



      }
         //cout << temp[0] << " "<<temp[1] <<" " <<temp[2] << endl;
        mon[temp[0]].neighbor.push_back(temp[1]);
        //cout << "x3" << endl;
        mon[temp[0]].neighbor.push_back(temp[2]);
        //cout << "x4" << endl;
        mon[temp[1]].neighbor.push_back(temp[0]);
        //cout << "x5" << endl;
        mon[temp[1]].neighbor.push_back(temp[2]);
        //cout << "x6" << endl;
        mon[temp[2]].neighbor.push_back(temp[0]);
        //cout << "x7" << endl;
        mon[temp[2]].neighbor.push_back(temp[1]);
        //cout << "x8" << endl;


/*
      for(int i=0; i<3; i++){
        id = itr + i;
        cout <<tp[id].x <<" " <<tp[id].y <<endl;
      }
      cout <<tp[itr].x <<" " <<tp[itr].y <<endl <<endl;
*/


      //std::cout << pt[0].first.x()+pt[0].second.x()*bx << ' ' << pt[0].first.y()+pt[0].second.y()*by << std::endl;//", ";
      //std::cout << pt[1].first.x()+pt[1].second.x()*bx << ' ' << pt[1].first.y()+pt[1].second.y()*by << std::endl;// ", ";
      //std::cout << pt[2].first.x()+pt[2].second.x()*bx << ' ' << pt[2].first.y()+pt[2].second.y()*by << std::endl;
      //std::cout << pt[0].first.x()+pt[0].second.x()*bx << ' ' << pt[0].first.y()+pt[0].second.y()*by << std::endl << std::endl;//", ";

//cout << "t4" << endl;
if(lo == -1)
{
    outT << pt[0].first.x()+pt[0].second.x()*bx << ' ' << pt[0].first.y()+pt[0].second.y()*by << std::endl;//", ";
    outT << pt[1].first.x()+pt[1].second.x()*bx << ' ' << pt[1].first.y()+pt[1].second.y()*by << std::endl;// ", ";
    outT << pt[2].first.x()+pt[2].second.x()*bx << ' ' << pt[2].first.y()+pt[2].second.y()*by << std::endl;
    outT << pt[0].first.x()+pt[0].second.x()*bx << ' ' << pt[0].first.y()+pt[0].second.y()*by << std::endl << std::endl;//", ";
}
      itr+=3;

    }

    outT.close();








///////////////////////////////////////////////////////////////////////
//remove duplicate

    for(int i = 0; i < numpoints; i++)
    {
        sort(mon[i].neighbor.begin(),mon[i].neighbor.end());
        mon[i].neighbor.erase(unique(mon[i].neighbor.begin(), mon[i].neighbor.end()),mon[i].neighbor.end());
    }

    for(int i = 0; i < numpoints; i++)
    {
        //cout <<' ' << mon[i].Index<< ' ' << mon[i].x <<' ' << mon[i].y << endl;

        for(int j = 0; j <mon[i].neighbor.size(); j++ )
        {
            int temp = mon[i].neighbor[j];
            //cout << mon[i].neighbor[j] << ' ' <<mon[temp].x << ' ' <<mon[temp].y <<endl;
        }
        //cout << endl;
    }


    ///////////////////////////////////////////////////////////////////////////////////////////
    //output neighbors for each cell
    outNeighbors << numpoints << endl;
    for(int i = 0; i < numpoints; i++)
    {
        outNeighbors << mon[i].Index << " " << mon[i].x << " " << mon[i].y << " " << mon[i].type << " " << mon[i].area << " ";

        for(int j = 0; j <mon[i].neighbor.size(); j++ )
        {
            outNeighbors << mon[i].neighbor[j] << " ";
        }
        outNeighbors << endl;
    }

 //////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////
    //Find Domains
    vector<int> Coundex;
    for(int i = 0; i < numpoints; i++)
    {
        Coundex.push_back(i);
    }

    while(Coundex.size()>0)
    {
        //cout << Coundex.size()<< endl;
        Domain tempDom;
        tempDom.type = mon[Coundex[0]].type;
        tempDom.cell.push_back(Coundex[0]);
        vector<int>::iterator it1 = Coundex.begin();
        it1 = Coundex.erase(it1);
        tempDom = FindDom(tempDom, tempDom.cell[0], mon);
        if(tempDom.type == 1)
        {
            Doms1.push_back(tempDom);
        }
        else if(tempDom.type == 2)
        {
            Doms2.push_back(tempDom);

        }
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
    }
   // cout << "Find domain" << endl;
    ////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//find area of domain
    for(int i = 0; i < Doms1.size(); i++)
    {
        Doms1[i].area = 0;
        for(int j = 0; j < Doms1[i].cell.size(); j++)
        {
            Doms1[i].area += mon[Doms1[i].cell[j]].area;
            //cout << Doms1[i].cell[j] << " ";
        }
        //cout << endl;
        /*
        for(int j = 0; j < mon[Doms1[i].cell[0]].neighbor.size();j++)
        {
            cout <<mon[Doms1[i].cell[0]].neighbor[j] << " ";
        }
        */
        //cout << i << " " << Doms1[i].area <<" " << Doms1[i].cell.size() << endl;
    }

    for(int i = 0; i < Doms2.size(); i++)
    {
        Doms2[i].area = 0;
        for(int j = 0; j < Doms2[i].cell.size(); j++)
        {
            Doms2[i].area += mon[Doms2[i].cell[j]].area;
            //cout << Doms2[i].cell[j] << " ";
        }
        /*
        for(int j = 0; j < mon[Doms2[i].cell[0]].neighbor.size();j++)
        {
            cout <<mon[Doms2[i].cell[0]].neighbor[j] << " ";
        }
        */
        //cout << i << " " << Doms2[i].area << " " << Doms2[i].cell.size() << endl;
    }
////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////
    //circumference
    //cout << "test1" << endl;

    int LengthCounter = 0;
    for(int i = 0; i < numpoints; i++)
    {
        for(int j = 0; j < mon[i].neighbor.size();j++)
        {
            //cout<< "test3" << endl;
            if(mon[i].type != mon[mon[i].neighbor[j]].type)
            {
                LengthCounter++;
                //cout << "test2" << endl;
            }
        }
    }
    LengthCounter /=2;
    TLength.push_back(LengthCounter);
    //cout << "Circumference: " << LengthCounter << endl;
//////////////////////////////////////////////////////////////////////

    //find average neighbor
    if(lo == -1)
    {
    ofstream rig0,rig1,rig2,sof0,sof1,sof2,rig4,rig8,rig9,sof4,sof8,sof9;
    stringstream rsName;
    string rsname;
    rsName << "R0_" << lo;
    rsName >> rsname;
    rig0.open(rsname,ios::app);
    rsName.clear();
    rsName << "R1_" << lo;
    rsName >> rsname;
    rig1.open(rsname,ios::app);
    rsName.clear();
    rsName << "R2_" << lo;
    rsName >> rsname;
    rig2.open(rsname,ios::app);
    rsName.clear();
    rsName << "S0_" << lo;
    rsName >> rsname;
    sof0.open(rsname,ios::app);
    rsName.clear();
    rsName << "S1_" << lo;
    rsName >> rsname;
    sof1.open(rsname,ios::app);
    rsName.clear();
    rsName << "S2_" << lo;
    rsName >> rsname;
    sof2.open(rsname,ios::app);
    rsName.clear();

    rsName << "R4_" << lo;
    rsName >> rsname;
    rig4.open(rsname,ios::app);
    rsName.clear();
    rsName << "R8_" << lo;
    rsName >> rsname;
    rig8.open(rsname,ios::app);
    rsName.clear();
    rsName << "R9_" << lo;
    rsName >> rsname;
    rig9.open(rsname,ios::app);
    rsName.clear();
    rsName << "S4_" << lo;
    rsName >> rsname;
    sof4.open(rsname,ios::app);
    rsName.clear();
    rsName << "S8_" << lo;
    rsName >> rsname;
    sof8.open(rsname,ios::app);
    rsName.clear();
    rsName << "S9_" << lo;
    rsName >> rsname;
    sof9.open(rsname,ios::app);
    rsName.clear();

    }

    double neicount = 0;
    int neinum0 = 0; // neighbor number is 5
    int neinum1 = 0; // neighbor number is 6
    int neinum2 = 0; // neighbor number is 7
    int neinum4 = 0; // neighbor number is 4
    int neinum8 = 0; // neighbor number is 8
    int neinum9 = 0; // neighbor number is 9
    int rigneinum0 = 0, rigneinum1 = 0, rigneinum2 = 0, sofneinum0 = 0, sofneinum1 = 0, sofneinum2 = 0,rigneinum4 = 0, rigneinum8 = 0, rigneinum9 = 0, sofneinum4 = 0, sofneinum8 = 0, sofneinum9 = 0;
    for(int i = 0; i < numpoints; i++)
    {

        neicount += mon[i].neighbor.size();
        //cout << mon[i].neighbor.size();
        if(mon[i].neighbor.size() == 5)
        {
            if(mon[i].type == 1)
            {
                rigneinum0++;
                //rig0 << mon[i].x << " " << mon[i].y << endl;
            }else if(mon[i].type == 2)
            {
                sofneinum0++;
                //sof0 << mon[i].x << " " << mon[i].y << endl;
            }
            neinum0++;
        }else if(mon[i].neighbor.size() == 6)
        {
            if(mon[i].type == 1)
            {
                rigneinum1++;
                //rig1 << mon[i].x << " " << mon[i].y << endl;
            }else if(mon[i].type == 2)
            {
                sofneinum1++;
                //sof1 << mon[i].x << " " << mon[i].y << endl;
            }
            neinum1++;
        }else if(mon[i].neighbor.size() == 7)
        {
            if(mon[i].type == 1)
            {
                rigneinum2++;
                //rig2 << mon[i].x << " " << mon[i].y << endl;
            }else if(mon[i].type == 2)
            {
                sofneinum2++;
                //sof2 << mon[i].x << " " << mon[i].y << endl;
            }
            neinum2++;
        }else if(mon[i].neighbor.size() == 4)
        {
            if(mon[i].type == 1)
            {
                rigneinum4++;
                //rig4 << mon[i].x << " " << mon[i].y << endl;
            }else if(mon[i].type == 2)
            {
                sofneinum4++;
                //sof4 << mon[i].x << " " << mon[i].y << endl;
            }
            neinum4++;
        }
        else if(mon[i].neighbor.size() == 8)
        {
            if(mon[i].type == 1)
            {
                rigneinum8++;
                //rig8 << mon[i].x << " " << mon[i].y << endl;
            }else if(mon[i].type == 2)
            {
                sofneinum8++;
                //sof8 << mon[i].x << " " << mon[i].y << endl;
            }
            neinum8++;
        }
        else if(mon[i].neighbor.size() == 9)
        {
            if(mon[i].type == 1)
            {
                rigneinum9++;
                //rig9 << mon[i].x << " " << mon[i].y << endl;
            }else if(mon[i].type == 2)
            {
                sofneinum9++;
                //sof9 << mon[i].x << " " << mon[i].y << endl;
            }
            neinum9++;
        }
        //cout << neicount << " ";
    }
    neicount /= numpoints;

    neighborsize.push_back(neicount); // Find the average numbers of each monomer for each frame

    neinum[0].push_back(neinum0);
    neinum[1].push_back(neinum1);
    neinum[2].push_back(neinum2);
    rigidneinum[0].push_back(rigneinum0);
    rigidneinum[1].push_back(rigneinum1);
    rigidneinum[2].push_back(rigneinum2);
    softneinum[0].push_back(sofneinum0);
    softneinum[1].push_back(sofneinum1);
    softneinum[2].push_back(sofneinum2);
    /*
    rig0.close();
    rig1.close();
    rig2.close();
    sof0.close();
    sof1.close();
    sof2.close();
    */
    neinum[3].push_back(neinum4);
    neinum[4].push_back(neinum8);
    neinum[5].push_back(neinum9);
    rigidneinum[3].push_back(rigneinum4);
    rigidneinum[4].push_back(rigneinum8);
    rigidneinum[5].push_back(rigneinum9);
    softneinum[3].push_back(sofneinum4);
    softneinum[4].push_back(sofneinum8);
    softneinum[5].push_back(sofneinum9);
    /*
    rig4.close();
    rig8.close();
    rig9.close();
    sof4.close();
    sof8.close();
    sof9.close();
*/
    //cout << "Findnei" << endl;

    //average area for 2 type domain
    double temparea1 = 0;
    double tempcella1 =0;
    for(int i = 0; i < Doms1.size(); i++)
    {
        temparea1 +=Doms1[i].area;
        tempcella1 += Doms1[i].area;
    }
    double temparea2 = 0;
    double tempcella2 = 0;
    for(int i = 0; i < Doms2.size(); i++)
    {
        temparea2 +=Doms2[i].area;
        tempcella2 += Doms2[i].area;
        //cout << temparea2 <<endl;
    }
    temparea1 /= Doms1.size();
    temparea2 /= Doms2.size();
    aveArea1.push_back(temparea1);
    aveArea2.push_back(temparea2);
    tempcella1 /= (numpoints/2);
    tempcella2 /= (numpoints - numpoints/2);
    aveCellArea1.push_back(tempcella1);
    aveCellArea2.push_back(tempcella2);




   /*
    numtp=itr-3;
    int numtriangles = numtp/3;
    //cout <<"number of points " <<numtp <<endl <<"number of triangles " <<numtriangles <<endl <<endl;

    int toggle, s;
    vector< vector <int> > nbr (numtp);
    int i_id, j_id;

    //add nbrs for all 3 points of each triangle
    for(int i=0; i<numtp; i+=3){
      nbr[i+0].push_back(i+1);
      nbr[i+0].push_back(i+2);
      nbr[i+1].push_back(i+0);
      nbr[i+1].push_back(i+2);
      nbr[i+2].push_back(i+0);
      nbr[i+2].push_back(i+1);
    }

    //combine nbrs
    for(int i=0; i<numtp-3; i+=3){
       for(int j=i+3; j<numtp; j+=3){
         for(int k=0; k<3; k++){
           if(tp[i+k].x == tp[j+0].x){
             nbr[i+k].push_back(j+1);
             nbr[i+k].push_back(j+2);
           }
           if(tp[i+k].x == tp[j+1].x){
             nbr[i+k].push_back(j+0);
             nbr[i+k].push_back(j+2);
           }
           if(tp[i+k].x == tp[j+2].x){
             nbr[i+k].push_back(j+0);
             nbr[i+k].push_back(j+1);
           }
         }
      }
    }

    for(int i=0; i<nbr.size(); i++){
      cout <<i <<" " << tp[i].x <<" " <<tp[i].y <<endl;
      for( int j=0; j< nbr[i].size(); j++){
        id = nbr[i][j];
        cout <<" " <<id <<" " <<tp[id].x <<" " <<tp[id].y <<endl;
      }
      cout <<endl;
    }
    */
    int tempnum = Doms1.size() + Doms2.size();
    numDomain.push_back(tempnum);
    tempnum = Doms1.size();
    FnumDomain.push_back(tempnum);
    tempnum = Doms2.size();
    SnumDomain.push_back(tempnum);
    mon.clear();
    Doms1.clear();
    Doms2.clear();
    T.clear();


}
}


GetCellPosition.close();
outNeighbors.close();
//////////////////////////////////////////////////////
//output domain
cout << "time   TotalDomain   FirstDomain   SecondDomain" << endl;
for(int i = 0; i < numDomain.size(); i++) // it did not take average for each density
{
    cout <<  i << " " << numDomain[i] << " " << FnumDomain[i] << " " << SnumDomain[i] << endl;
}
cout << endl;

int domcount = 0;
vector<double> domtemp1;
vector<double> domtemp2;
vector<double> domtemp;
for(int i = 0; i < density.size(); i++)
{
    domtemp.push_back(0);
    domtemp1.push_back(0);
    domtemp2.push_back(0);
}
for(int i = 0; i < numDomain.size(); i++)
{
    if(i%(numDomain.size()/density.size()) == 0 && i != 0)
    {
        domcount++;
    }
    domtemp[domcount] += numDomain[i];
    domtemp1[domcount] += FnumDomain[i];
    domtemp2[domcount] += SnumDomain[i];
}

cout << "density   TotalDomain   FirstDomain   SecondDomain" << endl;
for(int i = 0; i < density.size(); i++)
{
    domtemp1[i]/= numDomain.size()/density.size();
    domtemp2[i]/= numDomain.size()/density.size();
    domtemp[i]/= numDomain.size()/density.size();
    cout << density[i] << " " << domtemp[i] << " " << domtemp1[i] << " " << domtemp2[i] << endl;
}
cout << endl;

//////////////////////////////////////////////////////


/////////////////////////////////////////////////////
//interface length
cout << "Interface with time: " << endl;
for(int i = 0; i < TLength.size(); i++)
{
    cout << TLength[i] << " " << endl;

}
cout << endl;

int Incount = 0;
vector<double> aveinterface;
for(int i = 0; i < density.size(); i++)
{
    aveinterface.push_back(0);
}
for(int i = 0; i < TLength.size(); i++)
{
    if(i%(TLength.size()/density.size()) == 0 && i != 0)
    {
        Incount++;
    }
    aveinterface[Incount] += TLength[i];
}


    cout << "density    Interface" << endl;
for(int i = 0; i < aveinterface.size(); i++)
{
    aveinterface[i] /= (TLength.size()/density.size());
    cout << density[i] << " " << aveinterface[i] << endl;
}


////////////////////////////////////////////////////




/////////////////////////////////////////////////////
/*
cout << "Average of number of each monnmer " << endl;
for(int i = 0; i < neighborsize.size();i++)
{
    cout << neighborsize[i] << " " << endl;
}
cout << endl;
*/
///////////////////////////////////////////////////
//number of different neighbors

cout << "time total4 total5 total6 total7 total8 total9 Rigid4 Rgid5 Rigid6 Rigid7 Rigid8 Rigid9 Soft4 Soft5 Soft6 Soft7 Soft8 Soft9" << endl;
for(int i =  0; i < neighborsize.size();i++) //it did not take average for each density
{
    cout << neinum[3][i] << " " << neinum[0][i] << " " << neinum[1][i] <<" " << neinum[2][i] << " " << neinum[4][i] << " " << neinum[5][i] << " ";
    cout << rigidneinum[3][i] << " " << rigidneinum[0][i] << " " << rigidneinum[1][i] <<" " << rigidneinum[2][i] << " " << rigidneinum[4][i] << " " << rigidneinum[5][i] << " ";
    cout << softneinum[3][i] << " " << softneinum[0][i] << " " << softneinum[1][i] <<" " << softneinum[2][i] << " " << softneinum[4][i] << " " << softneinum[5][i] << endl;
}
cout << endl;


//take average of neighbors for different density
int neicount = 0;
vector<vector<double>> neitemp;
vector<vector<double>> Rneitemp;
vector<vector<double>> SNeitemp;
neitemp.resize(6);
Rneitemp.resize(6);
SNeitemp.resize(6);
//cout << density.size() << " " << neitemp.size() << endl;
for(int i = 0; i < density.size(); i++)
{
    for(int j = 0; j < neitemp.size(); j++)
    {
        neitemp[j].push_back(0);
        Rneitemp[j].push_back(0);
        SNeitemp[j].push_back(0);
    }
}
//cout << "test1" << endl;
for(int i = 0; i < neinum[0].size(); i++) // size should be the number of frames
{
    if(i%(neinum[0].size()/density.size()) == 0 && i != 0)
    {
        neicount++;
    }
    //cout << "i" << i << endl;
    for(int j = 0; j < neitemp.size();j++)
    {
        //cout << neicount<< " "<< j << " " << i << endl;
        neitemp[j][neicount] += double(neinum[j][i]);
        Rneitemp[j][neicount] += double(rigidneinum[j][i]);
        SNeitemp[j][neicount] += double(softneinum[j][i]);
        //cout << j << endl;
    }

}
//cout << "test2" << endl;
cout << "(neinum.size()/density.size())" << " " << (neinum[0].size()/density.size()) << endl;
for(int i = 0; i < neitemp.size();i++)
{
    for(int j = 0; j < density.size();j++)
    {
        neitemp[i][j] /= double(neinum[0].size())/double(density.size());
        Rneitemp[i][j] /= double(neinum[0].size())/double(density.size());
        SNeitemp[i][j] /= double(neinum[0].size())/double(density.size());

    }
}
cout << "Aveneighbors:" <<endl;
for(int i = 0; i < density.size(); i++)
{
    cout << density[i] << " " <<neitemp[3][i] << " " << neitemp[0][i] << " " << neitemp[1][i] <<" " << neitemp[2][i] << " " << neitemp[4][i] << " " << neitemp[5][i] << " ";
    cout << Rneitemp[3][i] << " " << Rneitemp[0][i] << " " << Rneitemp[1][i] <<" " << Rneitemp[2][i] << " " << Rneitemp[4][i] << " " << Rneitemp[5][i] << " ";
    cout << SNeitemp[3][i] << " " << SNeitemp[0][i] << " " << SNeitemp[1][i] <<" " << SNeitemp[2][i] << " " << SNeitemp[4][i] << " " << SNeitemp[5][i] << endl;
}



////////////////////////////////////////////////////
cout << "average Domain area" << endl;
for(int i = 0; i < aveArea1.size();i++)
{
    cout << aveArea1[i] << " " << aveArea2[i] << endl;
}
cout << endl;

//////////////////////////////////////

cout << "average Cell area" << endl;
for(int i = 0; i < aveCellArea1.size();i++)
{
    cout << aveCellArea1[i] << " " << aveCellArea2[i] << endl;
    //cout << density[i]<< " " << (aveCellArea1[i] + aveCellArea2[i])/2 << " " <<  << endl;
}
cout << endl;


////////////////////////////
int areaCcount = 0;
vector<double> Cella1;
vector<double> Cella2;
for(int i = 0; i < density.size(); i++)
{
    Cella1.push_back(0);
    Cella2.push_back(0);
}

for(int i = 0; i < aveCellArea1.size();i++)
{
    if(i%(aveCellArea1.size()/density.size()) == 0 && i != 0)
    {
        areaCcount++;
    }
    Cella1[areaCcount] += aveCellArea1[i];
    Cella2[areaCcount] += aveCellArea2[i];
}
cout << "average Cell area with density" << endl;

for(int i = 0; i < Cella1.size();i++)
{
    Cella1[i] /= (aveCellArea1.size()/density.size());
    Cella2[i] /= (aveCellArea1.size()/density.size());
    cout << density[i] << " " << (Cella1[i] +  Cella2[i])/2 << " " << endl;
}
return 0;
}


Domain FindDom(Domain Dom, int Index, vector<monomer> mon)
{
  //int Flag = Dom.type;
  for(int i = 0; i < mon[Index].neighbor.size(); i++)
  {
    if(mon[mon[Index].neighbor[i]].type == Dom.type&&count(Dom.cell.begin(),Dom.cell.end(),mon[Index].neighbor[i])==0)
    {

        Dom.cell.push_back(mon[Index].neighbor[i]);
        Dom = FindDom(Dom, mon[Index].neighbor[i], mon);

    }
  }
  return Dom;
}

















