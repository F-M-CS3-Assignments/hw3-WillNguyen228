#include <ctime> // for time(0)
#include <iostream> // for cin and cout
#include <cmath> // for M_PI and others
#include <vector> // for vectors (duh)
#include <cstdlib> // for random
#include <cassert> // for assert in the tests() function
#include <chrono> // for testing
#include <thread> 
#include "TimeCode.h" // for timecode's (duh)
#include "TimeCode.cpp"

using namespace std;

struct DryingSnapShot {
	// This is a struct, it's like an object
	// that doesn't have any methods.
	// You can read more about them in the ZyBook
	// just search for "struct"
	string name;
	time_t startTime;
	TimeCode *timeToDry;
    bool done; //I added this to keep track whether it has already been marked as "DONE!". So that it can be removed after it has been shown. 
};

long long int get_time_remaining(DryingSnapShot dss){ 
    time_t currentTime = time(0);
    time_t endTime = dss.startTime + dss.timeToDry->GetTimeCodeAsSeconds();
    long long int remainingTime = endTime - currentTime;

    return remainingTime;
}

string drying_snap_shot_to_string(DryingSnapShot dss){
    long long int remainingTime = get_time_remaining(dss);

    TimeCode tc = TimeCode(0, 0, remainingTime);

    string remainingTimeStr = tc.ToString();
    if (remainingTime <= 0) {
        remainingTimeStr = "DONE!";
    }

    string result = dss.name + " (takes " + dss.timeToDry->ToString() + " to dry) time remaining " + remainingTimeStr;
    return result;
}

double get_sphere_sa(double rad){
    return 4 * M_PI * pow(rad, 2); //This follows the formular for the surface area of a cube 4 * pi * r^2
}


TimeCode *compute_time_code(double surfaceArea){
	return new TimeCode(0, 0, static_cast<int>(surfaceArea)); //The surface area no matter how high will be distributed to minute and time
}


void tests(){
	// get_time_remaining
	DryingSnapShot dss;
	dss.startTime = time(0);
	TimeCode tc = TimeCode(0, 0, 7);
	dss.timeToDry = &tc;
	long long int ans = get_time_remaining(dss);
	assert(ans > 6 && ans < 8);
	// add more tests here

    // testing for sth that just started drying
    dss.startTime = time(0);
    TimeCode tc3 = TimeCode(0, 0, 10);
    dss.timeToDry = &tc3;
    this_thread::sleep_for(chrono::seconds(1));
    ans = get_time_remaining(dss);
    assert(ans > 8 && ans < 10);

    // testing for sth that finished drying 
    dss.startTime = time(0) - 10; 
    TimeCode tc4 = TimeCode(0, 0, 10);
    dss.timeToDry = &tc4;
    ans = get_time_remaining(dss);
    assert(ans == 0); 

	// get_sphere_sa
	double sa = get_sphere_sa(2.0);
	assert (50.2654 < sa && sa < 50.2655);
	// add more tests here

    sa = get_sphere_sa(0.0);
    assert(sa == 0.0);

    sa = get_sphere_sa(1000.0);
    assert(12566370.0 < sa && sa < 12566371.0);

	// compute_time_code
	TimeCode *tc2 = compute_time_code(1.0);
	//cout << "tc: " << tc.GetTimeCodeAsSeconds() << endl;
	assert(tc2->GetTimeCodeAsSeconds() == 1);

	// add more tests here

    TimeCode *tc5 = compute_time_code(1000000.0);
    assert(tc5->GetTimeCodeAsSeconds() == 1000000);
    delete tc2;
    delete tc5;

	cout << "ALL TESTS PASSED!" << endl;
}


int main(){
	// replace with your code
	tests();

    vector<DryingSnapShot> dryingBatches;
    string ans;

    while (true) {
        cout << "Choose an option: (A)dd, (V)iew Current Items, (Q)uit: ";
        cin >> ans;

        for (char& c : ans) c = toupper(c); //This turns the answer into upper case

        if (ans == "A") {
            double radius;
            cout << "radius: ";
            cin >> radius;

            double surfaceArea = get_sphere_sa(radius);
            TimeCode *dryingTime = compute_time_code(surfaceArea);

            DryingSnapShot dss {"Batch " + to_string(rand()), time(0), dryingTime};
            dryingBatches.push_back(dss);
            cout << drying_snap_shot_to_string(dss) << endl;
        }
        else if (ans == "V") {
            if (dryingBatches.empty()) {
                cout << "No batches being tracked.\n";
            } else {
                for (size_t i = 0; i < dryingBatches.size(); ++i) {
                    // Get the current drying batch
                    DryingSnapShot& dss = dryingBatches[i];

                    string text = drying_snap_shot_to_string(dss);
                    //This checks if the word Done! is found in the string so that we can display it 
                    if (dss.done == false && text.find("DONE!") != string::npos )  {
                        cout << drying_snap_shot_to_string(dss) << endl;
                        dss.done = true;
                        continue;
                    } 

                    if (dss.done == true) {
                        delete dss.timeToDry; // Delete timecode object before erasing to prevent memory leak
                        dryingBatches.erase(dryingBatches.begin() + i);
                        --i; // this is to adjust the index since we removed an element 
                        continue;
                    }
                    
                    cout << drying_snap_shot_to_string(dss) << endl; //print it as normal if it is not done
                }
                cout << dryingBatches.size() << " batches being tracked.\n";
            }
        }
        else if (ans == "Q") {
            // Clean up dynamic memory before exiting
            for (auto& dss : dryingBatches) {
                delete dss.timeToDry; // Deleting dynamically allocated timeToDry
            }
            break;
        }
        else {
            cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
