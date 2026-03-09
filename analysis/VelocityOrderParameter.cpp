//skips to every nSkip frame
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <math.h>
using namespace std;

int main(int argc, char **argv)
{
	if(argc!=3)
	{
		cout << "usage: " << argv[0] << " filename nSkip" << endl;
		return 0;
	}
	fstream inputFile;
	inputFile.open(argv[1], ios::in);
	int skip=atoi(argv[2]);
	char line[1024];
	int n, lineNumber=0;
	if(!inputFile.is_open())
	{
		cout << argv[1] << " not found!" << endl;
		return 0;
	}


	vector<double> OrderParameter;
	while(!inputFile.eof())
	{
		//cout << lineNumber << endl;
		inputFile.getline(line,1024);
		if(!atoi(line))
		{
			inputFile.close();
			return 0;
		}
		n=atoi(line);
		//if(lineNumber%skip==0)
        //cout << line << '\n';

		/*inputFile.getline(line,1024);
		if(lineNumber%skip==0)
			cout << line << '\n';
			*/
        vector<vector<double> > Data;
        Data.resize(n);
		for(int i=0;i<n;i++)
		{
			if(inputFile.eof())
			{
				cout << "File format not recognized!" << endl;
				inputFile.close();
				return 0;
			}
			inputFile.getline(line,1024);
			stringstream getAngle(line);
			double temp;
			if(lineNumber%skip==0)
            {
                while(getAngle >> temp)
                {
                    Data[i].push_back(temp);
                    //cout << temp << " ";
                }
                //cout << endl;

				//cout << line << '\n';
            }
		}

		//do calculation
		if(lineNumber%skip == 0)
        {
            double Avesin = 0;
            double Avecos = 0;
            for(int i = 0; i < Data.size(); i++)
            {
                double tempAngle = Data[i][2]/180*3.1415926;
                double tempcos = cos(tempAngle);
                double temsin = sin(tempAngle);
                Avecos += tempcos;
                Avesin += temsin;
            }
            Avecos /= Data.size();
            Avesin /= Data.size();
            double Aveangle = atan2(Avesin, Avecos)/3.1415926*180; // range is between -180 and 180, exclude boundary
            if(Aveangle < 0)
            {
                Aveangle += 360;
            }
            //cout << Aveangle << endl;

            //cout << Aveangle << endl;

            //cout << AveAngle << endl;
            double temppara = 0;
            for(int i = 0; i < Data.size(); i++)
            {
                double theta = (Data[i][2] - Aveangle)/180.0*3.1415926;
                double cosT = cos(2.0*theta);
                temppara += cosT;
            }
            temppara/= Data.size();
            OrderParameter.push_back(temppara);

            //cout <<temppara << endl;
            cout << lineNumber/skip << " " << OrderParameter[lineNumber/skip] << endl;

        }

        //cout << lineNumber << endl;
		lineNumber++;
	}

	inputFile.close();
	std::cout << "test" << endl;
    cout << OrderParameter.size() << endl;
	for(int i = 0; i < OrderParameter.size(); i++)
    {
        cout << i << OrderParameter[i] << endl;
    }


	return 0;
}
