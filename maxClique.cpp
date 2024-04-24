#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;

// Class for nodes in a graph and their connections to other nodes
class node
{
public:
    vector<int> connectedNodes;
    int nodeName; // nodeName will be the index of the row of the node in the input file
};

// Class for a graph containing nodes and their connections
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

// Function to read a file containing a connection matrix and return a graph
graph *readFile(string fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        cout << fileName << " could not be opened" << endl;
        return nullptr;
    };

    graph *myGraph = new graph();
    string textLine;
    int rowNumber = 0;
    while (getline(file, textLine))
    {
        node *rowNode = new node();
        rowNode->nodeName = rowNumber;
        myGraph->nodes.push_back(rowNode);
        istringstream iss(textLine);
        int columnNumber = 0;
        int connectStatus;

        int nodeNamePass; // Use this if you want the node name to be specified by the first number in the row, and uncomment the below line. Leave it commented if you want node names to be specified by line number.
        iss >> nodeNamePass;
        // myGraph->nodes.at(rowNumber)->nodeName = nodeNamePass;

        while (iss >> connectStatus)
        {
            if ((connectStatus == 1) && (rowNumber != columnNumber))
            {
                myGraph->nodes.at(rowNumber)->connectedNodes.push_back(columnNumber);
                myGraph->nodes.at(columnNumber)->connectedNodes.push_back(rowNumber);
            };
            columnNumber++;
        }
        rowNumber++;
    };
    file.close();
    return myGraph;
};

// Function to find the node with the most connections in a given graph
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

// Function to determine if there is a connection between the node at index1 and the node at index2
bool connectionExists(graph *myGraph, int index1, int index2)
{
    // Check whether there is a connection between index1 and index2
    for (int i = 0; i < myGraph->nodes.at(index1)->connectedNodes.size(); i++)
    {
        if (myGraph->nodes.at(index1)->connectedNodes.at(i) == index2)
        {
            // There is a connection between the node at index1 and index2
            return true;
        };
    };
    return false;
};

// Function (recursive) to find a candidate for the maximum clique in a graph
vector<node *> findMaxClique(graph *subgraph, vector<node *> popularNodes)
{
    // If there are no nodes left in the subgraph, then there is no need to keep recursing
    if (subgraph->nodes.empty())
    {
        return popularNodes;
    };

    // Find the popular node from the subgraph and add it to the popular nodes vector
    node *popularNode = findPopularNode(subgraph);
    popularNodes.push_back(popularNode);

    // Test each node in the subgraph to see if it has a connection to all nodes in the popular nodes vector (This is redundant for the first round since all nodes have a connection to the first popular node)
    // If there is a connection, add it to the new subgraph
    graph *newSubgraph = new graph();
    for (int i = 0; i < subgraph->nodes.size(); i++)
    {
        bool isConnectedToAll = true; // Assume the node is connected to all until proven otherwise
        for (int j = 0; j < popularNodes.size(); j++)
        {
            if (!connectionExists(subgraph, i, popularNodes.at(j)->nodeName))
            {
                isConnectedToAll = false; // If one connection is missing, the node can't be in the clique
                break;                    // No need to check further, break out of the inner loop
            };
        };

        if (isConnectedToAll)
        {
            newSubgraph->nodes.push_back(subgraph->nodes.at(i)); // Add node to the new subgraph if it's connected to all popular nodes
        };
    };

    vector<node *> result = findMaxClique(newSubgraph, popularNodes);
    delete newSubgraph;
    return result;
};

// Function to print any vector that contains nodes
// Printing them and their connections
void printNodeVector(vector<node *> myVector)
{
    for (int i = 0; i < myVector.size(); i++)
    {
        cout << "|  [" << myVector.at(i)->nodeName << "]\t--->\t[";
        for (int j = 0; j < myVector.at(i)->connectedNodes.size(); j++)
        {
            cout << myVector.at(i)->connectedNodes.at(j);
            if ((j + 1) == myVector.at(i)->connectedNodes.size())
            {
                cout << "]" << endl;
            }
            else
            {
                cout << ", ";
            };
        };
    };
};

// Print the node vector, but without the connections
void printNodeVectorClean(vector<node *> myVector)
{
    cout << "|  [";
    for (int i = 0; i < myVector.size(); i++)
    {
        cout << myVector.at(i)->nodeName;
        if ((i + 1) < myVector.size())
        {
            cout << ", ";
        };
    };
    cout << "]" << endl;
};

// Function to perform the max clique function for a thread
void performThread(graph *myGraph, vector<node *> popularNodes, vector<vector<node *>> &results, int index)
{
    vector<node *> localResult = findMaxClique(myGraph, popularNodes);
    results[index] = localResult; // Store the result at the specific index
};

// Function to manage the threads in the program
vector<node *> threadManagement(graph *myGraph)
{
    int numNodes = myGraph->nodes.size();
    vector<thread> threads;
    vector<vector<node *>> results(numNodes); // To store results from each thread

    // Call all of the threads
    for (int i = 0; i < numNodes; i++)
    {
        vector<node *> popularNodes;
        graph *newGraph = new graph();
        popularNodes.push_back(myGraph->nodes.at(i));
        for (int j = 0; j < numNodes; j++)
        {
            if (i != j)
            {
                newGraph->nodes.push_back(myGraph->nodes.at(j));
            };
        };
        threads.emplace_back(performThread, newGraph, popularNodes, ref(results), i);
    };

    // Join the threads when they finish
    for (thread &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        };
    };

    // Find the largest clique from all results
    vector<node *> maxClique;
    for (const auto &res : results)
    {
        if (res.size() > maxClique.size())
        {
            maxClique = res;
        };
    };

    return maxClique;
}

// Main program
int main(int argc, char *argv[])
{
    string fileName = argv[1];

    if (argc != 2)
    {
        cout << endl
             << endl
             << endl;
        cout << "+-----| Clique |-----+" << endl;
        cout << "|" << endl;
        // TEXT GOES HERE
        cout << "   ERROR: incorrect arguments. Use ./maxClique INPUTFILE" << endl;
        // TEXT GOES HERE
        cout << "|" << endl;
        cout << "+--------------------+" << endl
             << endl
             << endl;
        return 0;
    };

    // Print that the file is reading
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
    if (!myGraph)
    {
        cout << "Failed to read the file or file is empty." << endl;
        return 1; // Return an error code.
    };

    // Call the thread management to begin the process of finding the max clique
    vector<node *> maxClique = threadManagement(myGraph);

    // Print the contents of the maxClique
    // cout << endl
    //      << endl
    //      << endl;
    // cout << "+-----| Clique |-----+" << endl;
    // cout << "|" << endl;
    // // TEXT GOES HERE
    // printNodeVector(maxClique);
    // // TEXT GOES HERE
    // cout << "|" << endl;
    // cout << "+--------------------+" << endl
    //      << endl
    //      << endl;

    // Print the maxClique without all the connections
    cout << endl
         << endl
         << endl;
    cout << "+-----| Clique |-----+" << endl;
    cout << "|" << endl;
    // TEXT GOES HERE
    printNodeVectorClean(maxClique);
    // TEXT GOES HERE
    cout << "|" << endl;
    cout << "+--------------------+" << endl
         << endl
         << endl;

    // Print that the program is closing
    cout << endl
         << endl
         << endl;
    cout << "+-----| Clique |-----+" << endl;
    cout << "|" << endl;
    // TEXT GOES HERE
    cout << "|   The program is closing..." << endl;
    // TEXT GOES HERE
    cout << "|" << endl;
    cout << "+--------------------+" << endl
         << endl
         << endl;

    return 0;
};