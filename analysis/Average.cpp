#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>

using namespace std;
int main(int argc, char* argv[])
{

    if(argc!=3)
    {
        std::cout << "Usage: " << argv[0] << " fileName StartIndex\n";
        return 0;
    }

    std::vector<double> a;
    int StartIndex = atoi(argv[2]);

    std::ifstream ReadFile;
    ReadFile.open(argv[1], std::ios::in);
    char buf[1024];

    while(!ReadFile.eof())
    {
        ReadFile.getline(buf, 1024);
        stringstream input(buf);
        int tempi;
        double tempj;
        input >> tempi >> tempj;
        a.push_back(tempj);

    }

    double average = 0;
    for(int i = StartIndex; i < a.size(); i++)
    {
        average += a[i];
    }

    average /=(a.size()-StartIndex);
    cout << average << endl;
    return 0;
}
