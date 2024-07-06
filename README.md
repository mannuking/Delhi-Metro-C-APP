# 🚇 Delhi Metro Rail App (C++ Implementation)

This is a C++ program that simulates a Delhi Metro navigation app. It allows users to:

* 📋 List all stations in the Delhi Metro network.
* 🗺️ View a representation of the metro map.
* 🛤️ Calculate the shortest distance between two stations.
* ⏱️ Calculate the shortest travel time between two stations.
* 🔍 Get the shortest path (distance-wise) between two stations, including interchanges.
* 🕒 Get the shortest path (time-wise) between two stations, including interchanges.

## 📚 Algorithms and Data Structures

This project utilizes several key algorithms and data structures to achieve its functionality:

**1. Graph Representation:**

* **Data Structure:** The metro network is represented using an adjacency list graph, implemented using the `unordered_map` container in C++.
* **Implementation:** The `Graph_M` class encapsulates the graph structure. Each vertex (station) is a string, and its neighbors are stored in an `unordered_map` where the key is the neighboring station name, and the value is the distance (edge weight) between them.

**2. Shortest Path Algorithms:**

* **Dijkstra's Algorithm:** This algorithm is used to find the shortest path (in terms of distance or time) between two stations.
    * **Time Complexity:**
        * Best Case: O(E + V log V), where E is the number of edges and V is the number of vertices, assuming using a priority queue implemented with a Fibonacci heap.
        * Average Case: O(E + V log V) (same as the best case with a Fibonacci heap).
        * Worst Case: O(E + V log V) (same as the best and average case with a Fibonacci heap).
    * **Space Complexity:** O(V), where V is the number of vertices. The space is mainly used for the priority queue, the distance map, and the visited set.

* **Breadth-First Search (BFS):** Used to determine if a path exists between two stations (used in the `hasPath` function).
    * **Time Complexity:** O(V + E), where V is the number of vertices and E is the number of edges.
    * **Space Complexity:** O(V) in the worst case, as it might need to store all vertices in the queue.

**3. Finding Interchanges:**

* **Algorithm:** A simple algorithm is implemented in the `get_Interchanges` function to identify interchanges along a route. It analyzes the line codes associated with each station in the path and determines if a change of line is required.
* **Time Complexity:** O(N), where N is the number of stations in the path.
* **Space Complexity:** O(N) to store the resulting vector of interchanges.

## 🛠️ Compilation and Running

1. Make sure you have a C++ compiler installed (like g++ from MinGW).
2. Open a terminal or command prompt and navigate to the project directory.
3. Compile the code: `g++ main.cpp -o metro_app`
4. Run the program: `metro_app.exe`

## 📝 Notes

* The code includes comments explaining various parts of the implementation.
* The time complexities mentioned above assume an efficient priority queue implementation for Dijkstra's algorithm.
* You can easily expand the metro map data by adding more stations and connections in the `Create_Metro_Map` function.
