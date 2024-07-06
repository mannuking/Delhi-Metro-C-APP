#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <cmath>

using namespace std;

// Custom Pair structure for Dijkstra's Algorithm
struct DijkstraPair {
    string vname;
    string psf;
    int cost;
    bool dirty; // Flag to indicate if the cost is outdated

    DijkstraPair() : vname(""), psf(""), cost(0), dirty(false) {}

    DijkstraPair(string v, string p, int c) : vname(v), psf(p), cost(c), dirty(false) {}

    // Custom comparison for priority queue (min-heap based on cost)
    bool operator>(const DijkstraPair& other) const {
        return cost > other.cost;
    }
};

// Custom comparison for priority queue
struct CompareDijkstraPair {
    bool operator()(const DijkstraPair& a, const DijkstraPair& b) {
        return a.cost > b.cost; // Min-heap based on cost
    }
};

class Graph_M {
public:
    class Vertex {
    public:
        unordered_map<string, int> nbrs;
    };

    unordered_map<string, Vertex> vtces;

public:
    Graph_M() {}

    int numVetex() {
        return vtces.size();
    }

    bool containsVertex(const string& vname) {
        return vtces.find(vname) != vtces.end();
    }

    void addVertex(const string& vname) {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    void removeVertex(const string& vname) {
        if (!containsVertex(vname)) {
            return;
        }
        for (auto& entry : vtces) {
            entry.second.nbrs.erase(vname);
        }
        vtces.erase(vname);
    }

    int numEdges() {
        int count = 0;
        for (const auto& pair : vtces) {
            count += pair.second.nbrs.size();
        }
        return count / 2;
    }

    bool containsEdge(const string& vname1, const string& vname2) {
        if (!containsVertex(vname1) || !containsVertex(vname2)) {
            return false;
        }
        return vtces[vname1].nbrs.find(vname2) != vtces[vname1].nbrs.end();
    }

    void addEdge(const string& vname1, const string& vname2, int value) {
        if (!containsVertex(vname1) || !containsVertex(vname2) || containsEdge(vname1, vname2)) {
            return;
        }
        vtces[vname1].nbrs[vname2] = value;
        vtces[vname2].nbrs[vname1] = value;
    }

    void removeEdge(const string& vname1, const string& vname2) {
        if (!containsEdge(vname1, vname2)) {
            return;
        }
        vtces[vname1].nbrs.erase(vname2);
        vtces[vname2].nbrs.erase(vname1);
    }

    void display_Map() {
        cout << "\t Delhi Metro Map\n";
        cout << "\t------------------\n";
        cout << "----------------------------------------------------\n\n";

        for (const auto& pair : vtces) {
            const string& key = pair.first;
            const Vertex& vtx = pair.second;

            cout << key << " =>\n";
            for (const auto& nbr : vtx.nbrs) {
                cout << "\t" << nbr.first << "\t";
                if (nbr.first.length() < 16) cout << "\t";
                if (nbr.first.length() < 8) cout << "\t";
                cout << nbr.second << "\n";
            }
            cout << endl;
        }
        cout << "\t------------------\n";
        cout << "---------------------------------------------------\n\n";
    }

    void display_Stations() {
        cout << "\n***********************************************************************\n";
        int i = 1;
        for (const auto& pair : vtces) {
            cout << i << ". " << pair.first << endl;
            i++;
        }
        cout << "\n***********************************************************************\n";
    }

    bool hasPath(const string& vname1, const string& vname2, unordered_map<string, bool>& processed) {
        if (containsEdge(vname1, vname2)) {
            return true;
        }

        processed[vname1] = true;

        for (const auto& nbr : vtces[vname1].nbrs) {
            if (processed.find(nbr.first) == processed.end() &&
                hasPath(nbr.first, vname2, processed)) {
                return true;
            }
        }

        return false;
    }

    int dijkstra(const string& src, const string& des, bool nan) {
        priority_queue<DijkstraPair, vector<DijkstraPair>, CompareDijkstraPair> heap;
        unordered_map<string, DijkstraPair> map;

        for (const auto& pair : vtces) {
            DijkstraPair dp(pair.first, "", numeric_limits<int>::max());
            if (pair.first == src) {
                dp.cost = 0;
                dp.psf = src;
            }
            map[pair.first] = dp; 
            heap.push(dp);
        }

        while (!heap.empty()) {
            DijkstraPair rp = heap.top();
            heap.pop();

            if (rp.dirty) {
                continue;
            }

            if (rp.vname == des) {
                return rp.cost;
            }

            map[rp.vname].dirty = true; 

            for (const auto& nbr : vtces[rp.vname].nbrs) {
                if (map.find(nbr.first) != map.end() && !map[nbr.first].dirty) {
                    int oc = map[nbr.first].cost;
                    int nc = (nan) ? rp.cost + 120 + 40 * nbr.second : rp.cost + nbr.second;

                    if (nc < oc) {
                        map[nbr.first].psf = rp.psf + nbr.first;
                        map[nbr.first].cost = nc;

                        heap.push(map[nbr.first]);
                    }
                }
            }
        }

        return -1; // Destination unreachable 
    }


    // Helper function for Get_Minimum_Distance 
    string getMinDistancePath(const string& src, const string& dst) {
        int min = numeric_limits<int>::max();
        string ans = "";
        unordered_map<string, bool> processed;
        queue<DijkstraPair> q; 

        q.push(DijkstraPair(src, src + "  ", 0)); 

        while (!q.empty()) {
            DijkstraPair rp = q.front();
            q.pop();

            if (processed.find(rp.vname) != processed.end()) {
                continue; 
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) { 
                if (rp.cost < min) {
                    ans = rp.psf;
                    min = rp.cost;
                }
                continue; 
            }

            for (const auto& nbr : vtces[rp.vname].nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    q.push(DijkstraPair(nbr.first, rp.psf + nbr.first + "  ", rp.cost + nbr.second));
                }
            }
        }

        ans += to_string(min); 
        return ans;
    }
    
    string Get_Minimum_Distance(const string& src, const string& dst) {
        return getMinDistancePath(src, dst);
    }


    // Helper function for Get_Minimum_Time
    string getMinTimePath(const string& src, const string& dst) {
        int min = numeric_limits<int>::max();
        string ans = "";
        unordered_map<string, bool> processed;
        queue<DijkstraPair> q;

        q.push(DijkstraPair(src, src + "  ", 0));

        while (!q.empty()) {
            DijkstraPair rp = q.front();
            q.pop();

            if (processed.find(rp.vname) != processed.end()) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                if (rp.cost < min) {
                    ans = rp.psf;
                    min = rp.cost;
                }
                continue;
            }

            for (const auto& nbr : vtces[rp.vname].nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    q.push(DijkstraPair(nbr.first, rp.psf + nbr.first + "  ", rp.cost + 120 + 40 * nbr.second));
                }
            }
        }
        double minutes = ceil((double)min / 60);
        ans += to_string(minutes);
        return ans;
    }
    
    string Get_Minimum_Time(const string& src, const string& dst) {
        return getMinTimePath(src, dst);
    }

    vector<string> get_Interchanges(const string& str) {
        vector<string> arr;
        stringstream ss(str);
        string token;
        while (getline(ss, token, ' ')) { 
            if (!token.empty()) {
                arr.push_back(token);
            }
        }

        int count = 0; 
        for (size_t i = 1; i < arr.size() - 1; ++i) {
            size_t index = arr[i].find('~'); 
            string s = arr[i].substr(index + 1); 

            if (s.length() == 2) {
                string prev = arr[i - 1].substr(arr[i - 1].find('~') + 1); 
                string next = arr[i + 1].substr(arr[i + 1].find('~') + 1); 

                if (prev == next) {
                    // No interchange required
                } else {
                    arr[i] += " ==> " + arr[i + 1]; 
                    ++i; 
                    ++count; 
                }
            } 
        }
        arr.push_back(to_string(count)); 
        return arr;
    }

    static void Create_Metro_Map(Graph_M& g) {
        g.addVertex("Noida Sector 62~B");
        g.addVertex("Botanical Garden~B");
        g.addVertex("Yamuna Bank~B");
        g.addVertex("Rajiv Chowk~BY");
        g.addVertex("Vaishali~B");
        g.addVertex("Moti Nagar~B");
        g.addVertex("Janak Puri West~BO");
        g.addVertex("Dwarka Sector 21~B");
        g.addVertex("Huda City Center~Y");
        g.addVertex("Saket~Y");
        g.addVertex("Vishwavidyalaya~Y");
        g.addVertex("Chandni Chowk~Y");
        g.addVertex("New Delhi~YO");
        g.addVertex("AIIMS~Y");
        g.addVertex("Shivaji Stadium~O");
        g.addVertex("DDS Campus~O");
        g.addVertex("IGI Airport~O");
        g.addVertex("Rajouri Garden~BP");
        g.addVertex("Netaji Subhash Place~PR");
        g.addVertex("Punjabi Bagh West~P");

        // Adding edges (connections) with distances
        g.addEdge("Noida Sector 62~B", "Botanical Garden~B", 8);
        g.addEdge("Botanical Garden~B", "Yamuna Bank~B", 10);
        g.addEdge("Yamuna Bank~B", "Vaishali~B", 8);
        g.addEdge("Yamuna Bank~B", "Rajiv Chowk~BY", 6);
        g.addEdge("Rajiv Chowk~BY", "Moti Nagar~B", 9);
        g.addEdge("Moti Nagar~B", "Janak Puri West~BO", 7);
        g.addEdge("Janak Puri West~BO", "Dwarka Sector 21~B", 6);
        g.addEdge("Huda City Center~Y", "Saket~Y", 15);
        g.addEdge("Saket~Y", "AIIMS~Y", 6);
        g.addEdge("AIIMS~Y", "Rajiv Chowk~BY", 7);
        g.addEdge("Rajiv Chowk~BY", "New Delhi~YO", 1);
        g.addEdge("New Delhi~YO", "Chandni Chowk~Y", 2);
        g.addEdge("Chandni Chowk~Y", "Vishwavidyalaya~Y", 5);
        g.addEdge("New Delhi~YO", "Shivaji Stadium~O", 2);
        g.addEdge("Shivaji Stadium~O", "DDS Campus~O", 7);
        g.addEdge("DDS Campus~O", "IGI Airport~O", 8);
        g.addEdge("Moti Nagar~B", "Rajouri Garden~BP", 2);
        g.addEdge("Punjabi Bagh West~P", "Rajouri Garden~BP", 2);
        g.addEdge("Punjabi Bagh West~P", "Netaji Subhash Place~PR", 3); 
    }

    static vector<string> generateCodeList(const Graph_M& g) {
        cout << "List of station along with their codes:\n\n";
        vector<string> keys;
        for (const auto& pair : g.vtces) {
            keys.push_back(pair.first);
        }

        vector<string> codes(keys.size());
        int i = 1, m = 1;
        for (const string& key : keys) {
            stringstream ss(key);
            string temp;
            codes[i - 1] = "";
            while (getline(ss, temp, '~')) {
                for (char c : temp) {
                    if (isalnum(c)) {
                        codes[i - 1] += toupper(c);
                    }
                }
            }

            if (codes[i - 1].length() < 2) {
                codes[i - 1] += toupper(temp[1]);
            }

            cout << i << ". " << key << "\t";
            if (key.length() < (22 - m)) cout << "\t";
            if (key.length() < (14 - m)) cout << "\t";
            if (key.length() < (6 - m))  cout << "\t";
            cout << codes[i - 1] << endl;

            ++i;
            if (i == static_cast<int>(pow(10, m))) {
                ++m;
            }
        }
        cout << endl;
        return codes;
    }
};