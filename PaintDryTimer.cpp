#include <ctime> // for time(0)
#include <iostream> // for cin and cout
#include <cmath> // for M_PI and others
#include <vector> // for vectors (duh)
#include <cstdlib> // for random
#include <cassert> // for assert in the tests() function
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
};

long long int get_time_remaining(DryingSnapShot dss){
    time_t currentTime = time(0);
    time_t endTime = dss.startTime + dss.timeToDry->GetTimeCodeAsSeconds();
    return endTime - currentTime;
}

string drying_snap_shot_to_string(DryingSnapShot dss){
    long long int remaining = get_time_remaining(dss);
    return dss.name + " takes " + to_string(remaining) + " seconds";
}


double get_sphere_sa(double rad){
    return 4 * M_PI * pow(rad, 2); //This follows the formular for the surface area of a cube 4 * pi * r^2
}


TimeCode *compute_time_code(double surfaceArea){
	return new TimeCode(0, 0, static_cast<int>(surfaceArea));;
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


	// get_sphere_sa
	double sa = get_sphere_sa(2.0);
	assert (50.2654 < sa && sa < 50.2655);
	// add more tests here


	// compute_time_code
	TimeCode *tc2 = compute_time_code(1.0);
	//cout << "tc: " << tc.GetTimeCodeAsSeconds() << endl;
	assert(tc2->GetTimeCodeAsSeconds() == 1);
	delete tc2;


	// add more tests here


	cout << "ALL TESTS PASSED!" << endl;

}


int main(){
	// replace with your code
	tests();

    cout << "Enter the radius of the objects: ";
    double radius;
    cin >> radius;

    double surfaceArea = get_sphere_sa(radius);
    TimeCode *dryingTime = compute_time_code(surfaceArea);

    DryingSnapShot dss {"Batch 1", time(0), dryingTime};

    cout << drying_snap_shot_to_string(dss) << endl;

    while (get_time_remaining(dss) > 0) {
        cout << "Time remaining: " << get_time_remaining(dss) << " seconds\r";
        cout.flush();
        // this_thread::sleep_for(chrono::seconds()); //Delay for 1 seconds
    }

    cout << "\nPaint has dried!" << endl;
    delete dryingTime;
    return 0;
}
