#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iomanip>
using namespace std;

int main(int argc, char *argv[])
{
    string fileName = argv[1];

    if (argc != 2)
    {
        cout << "ERROR: incorrect arguments. Use ./maxClique INPUTFILE" << endl;
        return 0;
    };

    cout << endl
         << endl
         << endl;
    cout << "+-----| Clique |-----+" << endl;
    cout<<"|"<<endl;
    //TEXT GOES HERE
    cout << "|   Reading " << fileName << "" << endl;
    //TEXT GOES HERE
    cout<<"|"<<endl;
    cout << "+--------------------+" << endl
         << endl
         << endl;
};