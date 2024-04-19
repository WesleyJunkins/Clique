#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iomanip>
using namespace std;

class node
{
public:
    vector<int> connectedNodes;
    int nodeName; //nodeName will be the index of the row of the node in the input file
};

class graph
{
public:
    vector<node *> nodes;

    void printContents()
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            cout << "Node " << this->nodes.at(i)->nodeName << " is connected to: ";
            for (int j = 0; j < nodes.at(i)->connectedNodes.size(); j++)
            {
                cout << nodes.at(i)->connectedNodes.at(j) << " ";
            };
            cout << endl;
        };
    };
};

graph *readFile(string fileName)
{
    // Read the file
    ifstream file(fileName);
    if (!file.is_open())
    {
        cout << fileName << " could not be opened" << endl;
        return 0;
    };
    // Create a new graph
    graph *myGraph = new graph();
    string textLine;
    // Start at row 0
    int rowNumber = 0;
    // On each row, do:
    while (getline(file, textLine))
    {
        // Create the node for this specific row
        node *rowNode = new node();
        rowNode->nodeName = rowNumber;
        // Add this new node to the nodes contained in myGraph
        myGraph->nodes.push_back(rowNode);
        istringstream iss(textLine);
        // Start at column 0 for this row
        int columnNumber = 0;
        int connectStatus;
        // On each column, do:
        while (iss >> connectStatus)
        {
            // If we find a node that this row node connects to, add it to the connectedNodes of the row node
            if ((connectStatus == 1) && (rowNumber != columnNumber))
            {
                myGraph->nodes.at(rowNumber)->connectedNodes.push_back(columnNumber);
                myGraph->nodes.at(columnNumber)->connectedNodes.push_back(rowNumber);
            };
            columnNumber++;
        }
        rowNumber++;
    };

    return myGraph;
};

node *findPopularNode(graph *myGraph)
{
    node *currentNode;
    node *popularNode = myGraph->nodes.at(0);
    for (int i = 0; i < myGraph->nodes.size(); i++)
    {
        currentNode = myGraph->nodes.at(i);
        if (currentNode->connectedNodes.size() > popularNode->connectedNodes.size())
        {
            popularNode = currentNode;
        };
    };
    return popularNode;
};

bool connectionExists(graph* myGraph, int index1, int index2)
{
    //Check whether there is a connection between index1 and index2
    for(int i = 0; i < myGraph->nodes.at(index1)->connectedNodes.size(); i++)
    {
        if(myGraph->nodes.at(index1)->connectedNodes.at(i) == index2)
        {
            //There is a connection between the node at index1 and index2
            return true;
        };
    };
    return false;
};

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
    cout << "|" << endl;
    // TEXT GOES HERE
    cout << "|   Reading " << fileName << "" << endl;
    // TEXT GOES HERE
    cout << "|" << endl;
    cout << "+--------------------+" << endl
         << endl
         << endl;

    // Read file and make graph with all nodes and connections
    graph *myGraph = readFile(fileName);

    // Find the node with the largest number of connections
    node *popularNode = findPopularNode(myGraph);


};