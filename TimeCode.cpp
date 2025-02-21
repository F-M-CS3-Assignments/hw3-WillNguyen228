#include "TimeCode.h"
#include <iostream>

using namespace std;

// Function to check for negative values which can be seconds, minutes and hours
void checkNegative(int value, const string& valueName) {
    if (value < 0) {
        throw invalid_argument("Negative arguments not allowed for " + valueName);
    }
}

long long unsigned int TimeCode::ComponentsToSeconds(unsigned int hr, unsigned int min, unsigned long long int sec) {
    return hr * 3600 + min * 60 + sec;
}

// Setters
void TimeCode::SetHours(unsigned int hours) {
    checkNegative(hours, "hours");
    cout << hours << endl;
    t = ComponentsToSeconds(hours, GetMinutes(), GetSeconds());
}

void TimeCode::SetMinutes(unsigned int minutes) {

    checkNegative(minutes, "minutes");
    
    if (minutes >= 60) {
        throw std::invalid_argument("Minutes must be less than 60");
    }

    t = ComponentsToSeconds(GetHours(), minutes, GetSeconds());
}

void TimeCode::SetSeconds(unsigned int seconds) {
    
    checkNegative(seconds, "seconds");
    if (seconds >= 60) {
        throw std::invalid_argument("Minutes must be less than 60.");
    }

    t = ComponentsToSeconds(GetHours(), GetMinutes(), seconds);
}

// Constructor
TimeCode::TimeCode(unsigned int hr, unsigned int min, long long unsigned int sec) {
    t = ComponentsToSeconds(hr, min, sec);
}

// Copy Constructor
TimeCode::TimeCode(const TimeCode& tc) {
    t = tc.t;
}

void TimeCode::reset() {
    t = 0;
}

// Getters
unsigned int TimeCode::GetHours() const {
    return t / 3600;
}

unsigned int TimeCode::GetMinutes() const {
    return (t % 3600) / 60;
}

unsigned int TimeCode::GetSeconds() const {
    return t % 60;
}

// Get components
void TimeCode::GetComponents(unsigned int& hr, unsigned int& min, unsigned int& sec) const {
    hr = GetHours();
    min = GetMinutes();
    sec = GetSeconds();
}

string TimeCode::ToString() const {
    string result;
    result = to_string(GetHours()) + ":";

    if (GetMinutes() < 10 && GetMinutes() != 0) {
        result += to_string(GetMinutes()) + ":";
    }
    else {result += to_string(GetMinutes()) + ":";}

    if (GetSeconds() < 10 && GetSeconds() != 0) {
        result += to_string(GetSeconds());
    }
    else {result += to_string(GetSeconds());}

    return result;
}

// Arithmetic Operators
TimeCode TimeCode::operator+(const TimeCode& other) const {
    return TimeCode(0, 0, t + other.t);
}

TimeCode TimeCode::operator-(const TimeCode& other) const {
    if (t < other.t) {
        throw std::invalid_argument("Negative time code not allowed.");
    }
    return TimeCode(0, 0, t - other.t);
}

TimeCode TimeCode::operator*(double a) const {
    if (a < 0) {
        throw std::invalid_argument("Negative multiplication factor not allowed.");
    }
    return TimeCode(0, 0, static_cast<long long unsigned int>(t * a));
}

TimeCode TimeCode::operator/(double a) const {
    if (a <= 0) {
        throw std::invalid_argument("Division by zero or negative value not allowed.");
    }
    return TimeCode(0, 0, static_cast<long long unsigned int>(t / a));
}

// Comparison Operators
bool TimeCode::operator==(const TimeCode& other) const {
    return t == other.t;
}

bool TimeCode::operator!=(const TimeCode& other) const {
    return t != other.t;
}

bool TimeCode::operator<(const TimeCode& other) const {
    return t < other.t;
}

bool TimeCode::operator<=(const TimeCode& other) const {
    return t <= other.t;
}

bool TimeCode::operator>(const TimeCode& other) const {
    return t > other.t;
}

bool TimeCode::operator>=(const TimeCode& other) const {
    return t >= other.t;
}