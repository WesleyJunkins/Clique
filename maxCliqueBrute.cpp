// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <sstream>

// using namespace std;

// // Function to check if a given set of nodes forms a clique
// bool isClique(const vector<vector<int>> &graph, int subset)
// {
//     int n = graph.size();
//     for (int i = 0; i < n; i++)
//     {
//         if (!(subset & (1 << i)))
//         {
//             continue;
//         }
//         for (int j = i + 1; j < n; j++)
//         {
//             if ((subset & (1 << j)) && graph[i][j] == 0)
//             {
//                 return false;
//             }
//         }
//     }
//     return true;
// }

// // Function to count set bits in an integer
// int countBits(int subset)
// {
//     int count = 0;
//     while (subset)
//     {
//         count += subset & 1;
//         subset >>= 1;
//     }
//     return count;
// }

// // Main function to find the maximum clique
// int maximumClique(const vector<vector<int>> &graph)
// {
//     int n = graph.size();
//     int maxCliqueSize = 0;
//     int bestClique = 0;

//     for (int i = 1; i < (1 << n); i++)
//     {
//         if (isClique(graph, i))
//         {
//             int subsetSize = countBits(i);
//             if (subsetSize > maxCliqueSize)
//             {
//                 maxCliqueSize = subsetSize;
//                 bestClique = i;
//             }
//         }
//     }

//     cout << "Maximum Clique Size: " << maxCliqueSize << endl;
//     cout << "Nodes in the Maximum Clique: ";
//     for (int i = 0; i < n; i++)
//     {
//         if (bestClique & (1 << i))
//             cout << i << " ";
//     }
//     cout << endl;

//     return maxCliqueSize;
// }

// int main(int argc, char *argv[])
// {
//     ifstream fin(argv[1]);
//     if (!fin)
//     {
//         cerr << "Error opening file." << endl;
//         return 1;
//     }

//     vector<vector<int>> graph;
//     string line;
//     int max_node_index = -1;

//     // Read the graph
//     while (getline(fin, line))
//     {
//         vector<int> row;
//         istringstream iss(line);
//         int node, connection;
//         iss >> node; // Read the node number
//         while (iss >> connection)
//         {
//             row.push_back(connection);
//         }
//         graph.push_back(row);
//         max_node_index = max(max_node_index, node);
//     }
//     fin.close();

//     // Ensure all rows have correct number of elements
//     for (auto &row : graph)
//     {
//         if (row.size() < graph.size())
//         {
//             row.resize(graph.size(), 0); // Fill missing connections with 0
//         }
//     }

//     // Debug: Print the graph
//     cout << "Graph structure:" << endl;
//     for (const auto &row : graph)
//     {
//         for (int elem : row)
//         {
//             cout << elem << " ";
//         }
//         cout << endl;
//     }

//     maximumClique(graph);

//     return 0;
// }


#include <iostream>
#include <vector>

using namespace std;

// Function to check if a given set of nodes forms a clique
bool isClique(const vector<vector<int>>& graph, const vector<int>& subset) {
    for (int i = 0; i < subset.size(); i++) {
        for (int j = i + 1; j < subset.size(); j++) {
            if (graph[subset[i]][subset[j]] == 0) {  // Check if there is no edge between any two nodes in the subset
                return false;
            }
        }
    }
    return true;
}

// Recursive function to generate all subsets
void generateSubsets(vector<vector<int>>& subsets, vector<int>& current, int start, int n) {
    subsets.push_back(current);
    for (int i = start; i < n; i++) {
        current.push_back(i);
        generateSubsets(subsets, current, i + 1, n);
        current.pop_back();
    }
}

// Function to find the maximum clique
int maximumClique(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<vector<int>> subsets;
    vector<int> current;
    generateSubsets(subsets, current, 0, n);

    int maxCliqueSize = 0;
    vector<int> bestClique;

    for (const auto& subset : subsets) {
        if (isClique(graph, subset)) {
            int subsetSize = subset.size();
            if (subsetSize > maxCliqueSize) {
                maxCliqueSize = subsetSize;
                bestClique = subset;
            }
        }
    }

    cout << "Maximum Clique Size: " << maxCliqueSize << endl;
    cout << "Nodes in the Maximum Clique: ";
    for (int node : bestClique) {
        cout << node << " ";
    }
    cout << endl;

    return maxCliqueSize;
}

int main() {
    vector<vector<int>> graph = {
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0},
        {1, 0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0, 0}
    };

    maximumClique(graph);

    return 0;
}
