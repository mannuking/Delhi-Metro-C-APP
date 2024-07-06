#include "Graph_M.h" 
#include <iostream>
#include <string>
#include <limits>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
    Graph_M g;
    Graph_M::Create_Metro_Map(g);

    cout << "\n\t\t\t****WELCOME TO THE METRO APP*****\n\n";

    while (true) {
        cout << "\t\t\t\t~~LIST OF ACTIONS~~\n\n";
        cout << "1. LIST ALL THE STATIONS IN THE MAP\n";
        cout << "2. SHOW THE METRO MAP\n";
        cout << "3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "6. GET SHORTEST PATH (TIME WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "7. EXIT THE MENU\n";
        cout << "\nENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 7): ";

        int choice;
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 7) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input. Please enter a number between 1 and 7.\n\n";
            continue;
        }

        cout << "\n***********************************************************\n";
        if (choice == 7) {
            break;
        }

        switch (choice) {
            case 1:
                g.display_Stations();
                break;

            case 2:
                g.display_Map();
                break;

            case 3: {
                vector<string> keys;
                for (const auto& pair : g.vtces) {
                    keys.push_back(pair.first);
                }
                vector<string> codes = g.generateCodeList(g);

                cout << "\n1. TO ENTER SERIAL NO. OF STATIONS\n";
                cout << "2. TO ENTER CODE OF STATIONS\n";
                cout << "3. TO ENTER NAME OF STATIONS\n";
                cout << "ENTER YOUR CHOICE: ";

                int ch;
                cin >> ch;

                if (cin.fail() || ch < 1 || ch > 3) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\nInvalid choice.\n\n";
                    break;
                }

                string st1, st2;
                cout << "\nENTER THE SOURCE AND DESTINATION STATIONS: ";

                if (ch == 1) {
                    int idx1, idx2;
                    cin >> idx1 >> idx2;
                    if (cin.fail() || idx1 < 1 || idx1 > keys.size() || idx2 < 1 || idx2 > keys.size()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "\nInvalid station indices.\n\n";
                        break;
                    }
                    st1 = keys[idx1 - 1];
                    st2 = keys[idx2 - 1];
                } else if (ch == 2) {
                    string a, b;
                    cin >> a >> b;
                    transform(a.begin(), a.end(), a.begin(), ::toupper);
                    transform(b.begin(), b.end(), b.begin(), ::toupper);
                    int j;
                    for (j = 0; j < codes.size(); ++j) {
                        if (a == codes[j]) break;
                    }
                    if (j == codes.size()) {
                        cout << "\nInvalid source station code.\n\n";
                        break;
                    }
                    st1 = keys[j];

                    for (j = 0; j < codes.size(); ++j) {
                        if (b == codes[j]) break;
                    }
                    if (j == codes.size()) {
                        cout << "\nInvalid destination station code.\n\n";
                        break;
                    }
                    st2 = keys[j];
                } else if (ch == 3) {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, st1);
                    getline(cin, st2);
                }

                unordered_map<string, bool> processed;
                if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed)) {
                    cout << "\nTHE INPUTS ARE INVALID\n\n";
                } else {
                    cout << "\nSHORTEST DISTANCE FROM " << st1 << " TO " << st2 << " IS "
                         << g.dijkstra(st1, st2, false) << "KM\n\n";
                }
                break;
            }
            case 4: {
                string sat1, sat2;
                cout << "ENTER THE SOURCE STATION: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, sat1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, sat2);

                unordered_map<string, bool> processed1;
                cout << "\nSHORTEST TIME FROM (" << sat1 << ") TO (" << sat2 << ") IS "
                     << g.dijkstra(sat1, sat2, true) / 60 << " MINUTES\n\n";
                break;
            }
            case 5: {
                string s1, s2;
                cout << "ENTER THE SOURCE AND DESTINATION STATIONS:\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, s1);
                getline(cin, s2);

                unordered_map<string, bool> processed2;
                if (!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2)) {
                    cout << "\nTHE INPUTS ARE INVALID\n\n";
                } else {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                    size_t len = str.size();
                    cout << "SOURCE STATION : " << s1 << endl;
                    cout << "DESTINATION STATION : " << s2 << endl;
                    cout << "DISTANCE : " << str[len - 1] << endl;
                    cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << endl;
                    cout << "~~~~~~~~~~~~~\n";
                    cout << "START  ==>  " << str[0] << endl;
                    for (size_t i = 1; i < len - 2; ++i) {
                        cout << str[i] << endl;
                    }
                    cout << str[len - 2] << "   ==>    END\n";
                    cout << "~~~~~~~~~~~~~\n\n";
                }
                break;
            }
            case 6: {
                string ss1, ss2;
                cout << "ENTER THE SOURCE STATION: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3)) {
                    cout << "\nTHE INPUTS ARE INVALID\n\n";
                } else {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(ss1, ss2));
                    size_t len = str.size();
                    cout << "SOURCE STATION : " << ss1 << endl;
                    cout << "DESTINATION STATION : " << ss2 << endl;
                    cout << "TIME : " << str[len - 1] << " MINUTES\n";
                    cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << endl;
                    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                    cout << "START  ==>  " << str[0];
                    for (size_t i = 1; i < len - 2; ++i) {
                        cout << " ==>  " << str[i];
                    }
                    cout << "   ==>    END\n";
                    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
                }
                break;
            }

            default:
                cout << "\nPlease enter a valid option!\n";
                cout << "The options you can choose are from 1 to 6.\n\n";
        }
    }

    cout << "\nExiting Metro App. Goodbye!\n";
    return 0;
}