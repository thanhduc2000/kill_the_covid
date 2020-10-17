#include <bits/stdc++.h>
using namespace std;

const int number_of_borders = 6;

// These 2 arrays are to find the neighbors of a hexagon through x and y coordinates' difference
int x_diff[number_of_borders] = {0, 1, 1, 0, -1, -1};
int y_diff[number_of_borders] = {2, 1, -1, -2, -1, 1};

// Hexagon class including its name and coordination
class Hexagon {
  public:
    string hexagon_name;
    int x;
    int y;
};

// Function to check neighbor of a given hexagon
vector<pair<int, string> > check_neighbors(vector<Hexagon> hexagons, string hexagon_name) {
    vector<pair<int, string> > neighbors;

    // Find that hexagon and check if the neighbor coordinates of that hexagons have some that are also hexagons
    // and append the borders and their names
    for (int i = 0; i < hexagons.size(); i++) {
        if(hexagons[i].hexagon_name == hexagon_name) {
            for (int j = 0; j < number_of_borders; j++) {
                int neighbor_x = hexagons[i].x + x_diff[j];
                int neighbor_y = hexagons[i].y + y_diff[j];
                for (int k = 0; k < hexagons.size(); k++) {
                    if (hexagons[k].x == neighbor_x && hexagons[k].y == neighbor_y) {
                        neighbors.push_back(make_pair(j, hexagons[k].hexagon_name));
                    }
                }
            }
            break;
        }
    }
    return neighbors;
}

// Check whether 2 hexagons are connected using BFS traversing
bool is_linked(vector<Hexagon> hexagons, map<string, vector<pair<int, string> > > neighbors_list, string source_hexagon, string destination_hexagon) {
    if (source_hexagon == destination_hexagon) {
        return true;
    }
    map<string, bool> visited;
    for (int i = 0; i < hexagons.size(); i++) {
        visited[hexagons[i].hexagon_name] = false;
    }
    queue<string> queues;
    visited[source_hexagon] = true;
    queues.push(source_hexagon);
    while(!queues.empty()) {
        string hexagon = queues.front();
        queues.pop();
        for (int i = 0; i < neighbors_list[hexagon].size(); i++) {
            string neighbor_hexagon = neighbors_list[hexagon][i].second;
            if(neighbor_hexagon == destination_hexagon) {
                return true;
            }
            if(!visited[neighbor_hexagon]) {
                visited[neighbor_hexagon] = true;
                queues.push(neighbor_hexagon);
            }
        }
    }
    return false;
}

int main() {
    int number_of_hexagons;
    vector<Hexagon> hexagons;
    map<string, vector<pair<int, string> > > neighbors_list;
    cin >> number_of_hexagons;

    // Updating the hexagons list
    for (int i = 0; i < number_of_hexagons; i++) {
        Hexagon hexagon;
        cin >> hexagon.hexagon_name >> hexagon.x >> hexagon.y;
        hexagons.push_back(hexagon);
    }

    // Updating the neighbor list for each hexagon
    for (int i = 0; i < number_of_hexagons; i++) {
        neighbors_list[hexagons[i].hexagon_name] = check_neighbors(hexagons, hexagons[i].hexagon_name);
    }

    while(true) {
        string operation;
        string hexagon_name;
        cin >> operation;
        if(operation == "exit") {
            cout << "See you next time :)" << endl;
            break;
        } else {
            cin >> hexagon_name;
            if(operation == "query") {
                // Print the query
                vector<pair<int, string> > neighbors = neighbors_list[hexagon_name];
                cout << "Neighbors of this hexagon is:" << endl;
                for (int i = 0; i < neighbors.size(); i++) {
                    cout << "(" << neighbors[i].first <<", " << neighbors[i].second << ")" << endl;
                }
            }
            else if (operation == "add") {
                Hexagon hexagon;
                string neighbor_name;
                int border;
                cin >> neighbor_name >> border;
                hexagon.hexagon_name = hexagon_name;
                for (int i = 0; i < hexagons.size(); i++) {
                    if (hexagons[i].hexagon_name == neighbor_name) {
                        hexagon.x = hexagons[i].x + x_diff[border];
                        hexagon.y = hexagons[i].y + y_diff[border];
                    }
                }

                // Updating hexagon in the new list
                hexagons.push_back(hexagon);
                neighbors_list[hexagon_name] = check_neighbors(hexagons, hexagon_name);
                for (int i = 0; i < neighbors_list[hexagon_name].size(); i++) {
                    neighbors_list[neighbors_list[hexagon_name][i].second] = check_neighbors(hexagons, neighbors_list[hexagon_name][i].second);
                }
                cout << "Hexagon " << hexagon_name << " has been added!" << endl;
            }
            else if (operation == "remove") {
                vector<pair<int, string> > neighbors = neighbors_list[hexagon_name];
                Hexagon hexagon;
                for (int i = 0; i < hexagons.size(); i++) {
                    if (hexagons[i].hexagon_name == hexagon_name) {
                        hexagon = hexagons[i];
                        // remove the hexagon from the original list
                        auto it = hexagons.begin();
                        it += i;
                        hexagons.erase(it);

                        // Remove the hexagon from its neighbors' neighbors' list
                        for (int j = 0; j < neighbors.size(); j++) {
                            string neighbor_hexagon = neighbors[j].second;
                            for (int k = 0; i < neighbors_list[neighbor_hexagon].size(); k++) {
                                if (neighbors_list[neighbor_hexagon][k].second == hexagon_name) {
                                    auto it2 = neighbors_list[neighbor_hexagon].begin();
                                    it2 += k;
                                    neighbors_list[neighbor_hexagon].erase(it2);
                                    break;
                                }
                            }
                        }

                        // Remove the hexagon from the neighbors' list
                        neighbors_list.erase(hexagon_name);
                    }
                }
                // Check whether the hexagon is removable or not
                bool removable = true;
                for (int i = 0; i < neighbors.size(); i++) {
                    for (int j = i + 1; j < neighbors.size(); j++) {
                        if (!is_linked(hexagons, neighbors_list, neighbors[i].second, neighbors[j].second)) {
                            removable = false;
                            break;
                        }
                    }
                }
                if (removable) {
                    cout << "This hexagon is removable!" << endl;;
                } else {
                    cout << "This hexagon cannot be removed!" << endl;
                    // Re-add the hexagon
                    hexagons.push_back(hexagon);
                    neighbors_list[hexagon_name] = check_neighbors(hexagons, hexagon_name);
                    for (int i = 0; i < neighbors_list[hexagon_name].size(); i++) {
                        neighbors_list[neighbors_list[hexagon_name][i].second] = check_neighbors(hexagons, neighbors_list[hexagon_name][i].second);
                    }
                }
            }
        }
    }
    return 0;
}
