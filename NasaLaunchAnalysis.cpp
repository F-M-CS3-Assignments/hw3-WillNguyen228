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
    if (tokens.size() < 4) return TimeCode(); //if it doesn't have a date it will be retuned as a default

    string dateTime = tokens[4]; //Date is in the firth column there is a comma btw the year and the actual time :)))
    cout << dateTime << endl;

    size_t pos = dateTime.find("UTC"); //Checks if there is the UTC part
    if (pos == string::npos) return TimeCode(); //string::npos is a special constant in C++ that indicates "not found."

    //I am using UTC as an anchor point to extract the exact time
    string timePart = dateTime.substr(pos - 6, 5);
    vector<string> timeComponents = split(timePart, ':');

    if (timeComponents.size() != 2) return TimeCode();  // Ensure we are getting XX:XX

    int minutes = stoi(timeComponents[0]);
    int seconds = stoi(timeComponents[1]);

    return TimeCode(0, minutes, seconds);  // Construct the timeCode object
}

//This function computes the average launch time
TimeCode calculate_average_time(const vector<TimeCode>& times) {
    if (times.empty()) return TimeCode(); 

    TimeCode total(0, 0, 0);
    for (const auto& t : times) { // I got all of this from researching Stackoverflow. const prevents t from being changed, auto here automatically assigned t with its type, & prevents copying
        total = total + t;  // 
    }

    return total / times.size(); 
}

int main() {
    ifstream file("Space_Corrected_Short.csv");
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    vector<TimeCode> launchTimes;
    string line;
    getline(file, line);  // This skips the header

    while (getline(file, line)) {
        TimeCode launchT = parse_line(line);

        if (launchT.GetMinutes() != 0 || launchT.GetSeconds() != 0) {
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
    cout << "Average launch time: " << avgTime.ToString() << endl;

    return 0;
}