//catch a certain frame
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
	if(argc!=3)
	{
		cout << "usage: " << argv[0] << " filename.xyz frameIndex" << endl;
		return 0;
	}
	fstream inputFile;
	inputFile.open(argv[1], ios::in);
	int frameIndex=atoi(argv[2]);
	char line[1024];
	int n, lineNumber=0;
	if(!inputFile.is_open())
	{
		cout << argv[1] << " not found!" << endl;
		return 0;
	}

	while(!inputFile.eof())
	{
		inputFile.getline(line,1024);
		if(!atoi(line))
		{
			inputFile.close();
			return 0;
		}
		n=484;
		if(lineNumber == frameIndex)
			cout << line << '\n';
		//inputFile.getline(line,1024);
		//if(lineNumber == frameIndex)
			//cout << line << '\n';
		for(int i=0;i<n;i++)
		{
			if(inputFile.eof())
			{
				cout << "File format not recognized!" << endl;
				inputFile.close();
				return 0;
			}
			inputFile.getline(line,1024);
			if(lineNumber == frameIndex)
				cout << line << '\n';
		}
		lineNumber++;
	}
	inputFile.close();

	return 0;
}
