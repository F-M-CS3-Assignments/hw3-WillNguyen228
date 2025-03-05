#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "TimeCode.h"
#include "TimeCode.cpp"

using namespace std;

//This helper function splits a string by a a given 
vector<string> split(const string& str, char delim) {
    vector<string> elements;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delim)) {
        elements.push_back(token);
    }
    return elements;
}

// Function to parse a CSV line and extract the time
TimeCode parse_line(const string& line) {
    vector<string> tokens = split(line, ',');  // Everything is comma-separated so we separate by that 
    if (tokens.size() < 4) return -1; //if it doesn't have a date it will be retuned as a default

    string dateTime = tokens[4]; //Date is in the fourth column there is a comma btw the year and the actual time :)))
    // cout << dateTime << endl;

    size_t pos = dateTime.find("UTC"); //Checks if there is the UTC part
    if (pos == string::npos) return -1; //string::npos is a special constant in C++ that indicates "not found."

    //I am using UTC as an anchor point to extract the exact time
    string timePart = dateTime.substr(pos - 6, 5);
    vector<string> timeComponents = split(timePart, ':');

    if (timeComponents.size() != 2) return -1;  // Ensure we are getting XX:XX

    int hours = stoi(timeComponents[0]);
    int minutes = stoi(timeComponents[1]);

    return TimeCode(hours, minutes, 0);  // Construct the timeCode object
}

//This function computes the average launch time
TimeCode calculate_average_time(const vector<TimeCode>& times) {
    if (times.empty()) return TimeCode(); 

    TimeCode total(0, 0, 0);
    for (const auto& t : times) { // I got all of this from researching Stackoverflow. const prevents t from being changed, auto here automatically assigned t with its type, & prevents copying
        total = total + t;  // 
    }
    // cout << total.ToString() << endl;

    return total / times.size(); 
}

int main() {
    ifstream file("Space_Corrected.csv");
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    vector<TimeCode> launchTimes;
    string line;
    getline(file, line);  // This skips the header

    while (getline(file, line)) {
        TimeCode launchT = parse_line(line);

        if (launchT != -1) {
            // cout << launchT.ToString() << endl;
            launchTimes.push_back(launchT);
        }
    }

    file.close();

    cout << launchTimes.size() << " data points." << endl;

    if (launchTimes.empty()) {
        cout << "No valid launch times found." << endl;
        return 1;
    }

    TimeCode avgTime = calculate_average_time(launchTimes);
    cout << "AVERAGE: " << avgTime.ToString() << endl;

    return 0;
}