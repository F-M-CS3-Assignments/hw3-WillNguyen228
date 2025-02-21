#include <iostream>
#include <assert.h>
using namespace std;

#include "TimeCode.h"
#include "TimeCode.cpp"

void TestComponentsToSeconds(){ //First Test
	cout << "Testing ComponentsToSeconds" << endl;
	
	// Random but "safe" inputs
	long long unsigned int t = TimeCode::ComponentsToSeconds(3, 17, 42);
	assert(t == 11862);
	
	// More tests go here! Very large constructor and negative 

	long long unsigned int t2 = TimeCode::ComponentsToSeconds(0, 0, 0);
    assert(t2 == 0);

    assert(TimeCode::ComponentsToSeconds(0, 0, 1) == 1);
    assert(TimeCode::ComponentsToSeconds(0, 1, 0) == 60);
    assert(TimeCode::ComponentsToSeconds(1, 0, 0) == 3600);
	
	cout << "PASSED!" << endl << endl;
}


void TestDefaultConstructor(){ //Second Test
	cout << "Testing Default Constructor" << endl;
	TimeCode tc;
	
	//cout << "Testing ToString()" << endl;
	//cout << "tc: " << tc.ToString() << endl;
	assert(tc.ToString() == "0:0:0");
	
	cout << "PASSED!" << endl << endl;
}


void TestComponentConstructor(){ //Third Test
	cout << "Testing Component Constructor" << endl;
	TimeCode tc = TimeCode(0, 0, 0);
	//cout << "Testing ToString()" << endl;
	//cout << "tc: " << tc.ToString() << endl;
	assert(tc.ToString() == "0:0:0");
	
	// more tests go here!

	// Roll-over inputs
	TimeCode tc2 = TimeCode(2, 71, 234719572143); // test very large second and minute resulting in roll overs  
	assert(tc2.ToString() == "65199884:20:3");
	
	TimeCode tc3 = TimeCode(3, 71, 3801);
	assert(tc3.ToString() == "5:14:21");
	
	// More tests go here!
	TimeCode tc4 = TimeCode(0, 120, 3661); // 120 min = 2h, 3661 sec = 1h 1m 1s
    assert(tc4.ToString() == "3:1:1");
	
	cout << "PASSED!" << endl << endl;
}


void TestGetComponents(){ //Fourth Test
	cout << "Testing GetComponents" << endl;
	
	unsigned int h;
	unsigned int m;
	unsigned int s;
	
	// Regular values
	TimeCode tc = TimeCode(5, 2, 18);
	tc.GetComponents(h, m, s);
	assert(h == 5 && m == 2 && s == 18);
	
	// More tests go here!
	TimeCode tc2 = TimeCode(0, 0, 0);
    tc2.GetComponents(h, m, s);
    assert(h == 0 && m == 0 && s == 0);

	TimeCode tc3 = TimeCode(3, 120, 3801); // 120 min = 2h, 3801 sec = 1h 3m 21s → total: 6h 3m 21s
    tc3.GetComponents(h, m, s);
    assert(h == 6 && m == 3 && s == 21);

	TimeCode tc4 = TimeCode(187, 53, 27);
    tc4.GetComponents(h, m, s);
    assert(tc4.ToString() == to_string(h) + ":" + to_string(m) + ":" + to_string(s));
	
	cout << "PASSED!" << endl << endl;
}


void TestSubtract(){ //Fifth Test
	cout << "Testing Subtract" << endl;
	TimeCode tc1 = TimeCode(1, 0, 0);
	TimeCode tc2 = TimeCode(0, 50, 0);
	TimeCode tc3 = tc1 - tc2;
	assert(tc3.ToString() == "0:10:0");
	
	
	TimeCode tc4 = TimeCode(4, 15, 45);
	try{
		TimeCode tc5 = tc1 - tc4; 
		cout << "tc5: " << tc5.ToString() << endl;
		assert(false);
	}
	catch(const invalid_argument& e){
		cout << "Substracting by larger time caught" << endl;
	}

	// more tests
	
	cout << "PASSED!" << endl << endl;
}

void TestAddOperator(){ //Sixth Test
	cout << "Testing + Operator" << endl;

	//This also has roll over
	TimeCode tc1 = TimeCode(2, 30, 45);
	TimeCode tc2 = TimeCode(1, 15, 30);
	TimeCode tc3 = tc1 + tc2;
	assert(tc3.ToString() == "3:46:15");

	cout << "PASSED!" << endl << endl;
}

void TestMultiplyOperator(){ //Seventh Test
	cout << "Testing * Operator" << endl;

	TimeCode tc1 = TimeCode(1, 30, 0);
	TimeCode tc2 = tc1 * 2;
	assert(tc2.ToString() == "3:0:0");

	// Invalid multiplication test
	try {
		TimeCode tc3 = tc1 * -2; // should throw
		assert(false);
	} catch (const invalid_argument& e) {
		cout << "Multiplying by negative number caught" << endl;
	}

	// Multiplication by a double
    TimeCode tc4 = tc1 * 1.5;
    assert(tc4.ToString() == "2:15:0");

    TimeCode tc5 = tc1 * 0.5;
    assert(tc5.ToString() == "0:45:0");

	TimeCode tc6 = tc1 * 0;
    assert(tc6.ToString() == "0:0:0");

	cout << "PASSED!" << endl << endl;
}

void TestDivideOperator(){ //Eigth Test
	cout << "Testing / Operator" << endl;

	TimeCode tc1 = TimeCode(2, 30, 0);
	TimeCode tc2 = tc1 / 2;
	assert(tc2.ToString() == "1:15:0");

	// Invalid division tests
	try {
		TimeCode tc3 = tc1 / 0; // should throw
		assert(false);
	} catch (const invalid_argument& e) {
		cout << "Dividing by zero caught" << endl;
	}

	try {
		TimeCode tc4 = tc1 / -1; // should throw
		assert(false);
	} catch (const invalid_argument& e) {
		cout << "Dividing by minus 1 caught" << endl;
	}

	// Division by a double
    TimeCode tc5 = tc1 / 1.5;
    assert(tc5.ToString() == "1:40:0");

    TimeCode tc6 = tc1 / 0.5;
    assert(tc6.ToString() == "5:0:0");

	cout << "PASSED!" << endl << endl;
}

void TestSetHours(){ //Ninth Test
	cout << "Testing SetHours" << endl;

	TimeCode tc = TimeCode(8, 5, 9);
	tc.SetHours(10); // test valid change
	assert(tc.ToString() == "10:5:9");

	tc.SetHours(0); // resetting hours
	assert(tc.ToString() == "0:5:9");

	tc.SetHours(33); // test large hour
	assert(tc.ToString() == "33:5:9");

	TimeCode tc2 = TimeCode(8, 5, 9);

	try {
		tc2.SetHours(-1);
		cout << "tc2: " << tc2.ToString() << endl;
		assert(false);
	} catch (const invalid_argument& e) {
		cout << "Setting negative hour caught" << endl;
	}

	cout << "PASSED!" << endl << endl;
}

void TestSetMinutes() //Tenth Test
{
	cout << "Testing SetMinutes" << endl;

	TimeCode tc = TimeCode(8, 5, 9);
	tc.SetMinutes(15); // test valid change
	assert(tc.ToString() == "8:15:9");

	try
	{
		tc.SetMinutes(80);  // test invalid change
		assert(false);
	}
	catch (const invalid_argument &e)
	{
		cout << "Setting very large minute caught" << endl;
	}

	TimeCode tc2 = TimeCode(8, 5, 9);
	try
    {
        tc2.SetMinutes(-1);  // test invalid change (negative minutes)
        assert(false);  // should not reach here
    }
    catch (const invalid_argument &e)
    {
        cout << "Setting negative minute caught" << endl;
    }

	assert(tc2.ToString() == "8:5:9");

	cout << "PASSED!" << endl << endl;
}

// Many More Tests...

void TestSetSeconds(){ //Eleventh Test
	cout << "Testing SetSeconds" << endl;

	TimeCode tc = TimeCode(8, 5, 9);
	tc.SetSeconds(45); // test valid change
	assert(tc.ToString() == "8:5:45");

	// more tests
	TimeCode tc2 = TimeCode(2, 30, 30);

	try
    {
        tc2.SetSeconds(60); // should throw an error
        assert(false);  // should not reach here
    }
    catch (const invalid_argument &e)
    {
        cout << "Setting large second caught" << endl;
    }

	try
    {
        tc.SetSeconds(-10);  // test invalid change (negative seconds)
        assert(false);  // should not reach here
    }
    catch (const invalid_argument &e)
    {
        cout << "Setting very negative seconds caught" << endl;
    }

	assert(tc.ToString() == "8:5:45");

	cout << "PASSED!" << endl << endl;
}

void TestComparisonOperators(){ //Twelfth Test
	cout << "Testing Comparison Operators" << endl;

	TimeCode tc1 = TimeCode(1, 30, 0);
	TimeCode tc2 = TimeCode(1, 30, 0);
	TimeCode tc3 = TimeCode(2, 0, 0);

	assert(tc1 == tc2); 
	assert(tc1 != tc3); 
	assert(tc1 < tc3);  
	assert(tc1 <= tc2); 
	assert(tc3 > tc1);  
	assert(tc3 >= tc1); 

	cout << "PASSED!" << endl << endl;
}

void TestReset(){ //Thirtenth Test
	cout << "Testing Reset" << endl;

	TimeCode tc = TimeCode(5, 10, 20);
	tc.reset(); // should set time to 0
	assert(tc.ToString() == "0:0:0");

	cout << "PASSED!" << endl << endl;
}

void TestGetTimeCodeAsSeconds() { // Fourtenth Test
    cout << "Testing GetTimeCodeAsSeconds" << endl;

    TimeCode tc1 = TimeCode(1, 30, 0); 
    unsigned long long expected1 = 1 * 3600 + 30 * 60 + 0; 
    assert(tc1.GetTimeCodeAsSeconds() == expected1);

	cout << "PASSED!" << endl << endl;
}

int main(){

	// I got help from my friends who just gave me warnings about negative values
	// and the maximum values for seconds and minutes
	
	TestComponentsToSeconds();
	TestDefaultConstructor();
	TestComponentConstructor();
	TestGetComponents();
	TestSubtract();
	TestSetMinutes();
	
	// Many othere test functions...

	TestAddOperator();
	TestMultiplyOperator();
	TestDivideOperator();
	TestSetHours();
	TestSetSeconds();
	TestComparisonOperators();
	TestReset();
	TestGetTimeCodeAsSeconds();
	
	cout << "PASSED ALL TESTS!!!" << endl;
	return 0;
}
