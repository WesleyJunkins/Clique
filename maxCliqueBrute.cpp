#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class node
{
public:
    vector<int> connectedNodes;
    int nodeName;
};

vector<node *> readFile(string fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        cout << fileName << " could not be opened" << endl;
        vector<node *> emptyVector;
        return emptyVector;
    };
    vector<node *> graph;
    string textLine;
    int rowNumber = 0;
    while (getline(file, textLine))
    {
        node *rowNode = new node();
        rowNode->nodeName = rowNumber;
        graph.push_back(rowNode);
        istringstream iss(textLine);
        int doNotUseRowNumberThisIsStupid;
        iss >> doNotUseRowNumberThisIsStupid;
        int columnNumber = 0;
        int connectStatus;
        while (iss >> connectStatus)
        {
            if ((connectStatus == 1) && (rowNumber != columnNumber))
            {
                graph.at(rowNumber)->connectedNodes.push_back(columnNumber);
                graph.at(columnNumber)->connectedNodes.push_back(rowNumber);
            };
            columnNumber++;
        }
        rowNumber++;
    };
    return graph;
};

bool isConnectionBetween(node *node1, node *node2, vector<node *> graph)
{
    bool isConnection = false;
    for (int i = 0; i < node1->connectedNodes.size(); i++)
    {
        if (node2->nodeName == node1->connectedNodes.at(i))
        {
            isConnection = true;
        }
    }
    return isConnection;
};

bool isClique(const vector<node *> &clique)
{
    for (int i = 0; i < clique.size(); i++)
    {
        for (int j = i + 1; j < clique.size(); j++)
        {
            if (find(clique[i]->connectedNodes.begin(), clique[i]->connectedNodes.end(), clique[j]->nodeName) == clique[i]->connectedNodes.end())
            {
                return false;
            }
        }
    }
    return true;
}

void findCliques(vector<node *> &graph, vector<node *> &currentClique, int startIndex, vector<vector<node *>> &allCliques)
{
    if (isClique(currentClique))
    {
        allCliques.push_back(currentClique);
    }
    for (int i = startIndex; i < graph.size(); i++)
    {
        currentClique.push_back(graph[i]);
        findCliques(graph, currentClique, i + 1, allCliques);
        currentClique.pop_back();
    }
}

vector<vector<node *>> allCliques(vector<node *> graph)
{
    vector<vector<node *>> allCliques;
    vector<node *> currentClique;
    findCliques(graph, currentClique, 0, allCliques);
    return allCliques;
}

int main(int argc, char *argv[])
{
    vector<node *> graph = readFile(argv[1]);

    for (int i = 0; i < graph.size(); i++)
    {
        cout << "Node " << graph.at(i)->nodeName << " is connected to: ";
        for (int j = 0; j < graph.at(i)->connectedNodes.size(); j++)
        {
            cout << graph.at(i)->connectedNodes.at(j) << " ";
        }
        cout << endl;
    }

    vector<vector<node *>> cliques = allCliques(graph);
    cout << "Total cliques found: " << cliques.size() << endl;
    for (int i = 0; i < cliques.size(); i++)
    {
        cout << "Clique " << i + 1 << ": ";
        for (node *n : cliques[i])
        {
            cout << n->nodeName << " ";
        }
        cout << endl;
    }
}