/*
 * Program to calculate difference between date and times.
 * 
 * Command line usage syntax:
 * - Only dates:
 *     calctime <Start Date> <End Date>
 * - Date and Time with 24-hour clock:
 *     calctime <Start Date> <Start Time> <End Date> <End Time>
 * - Date and Time with 12-hour clock:
 *     calctime <Start Date> <Start Time> <AM/PM> <End Date> <End Time> <AM/PM>
 * 
 * Command line usage examples:
 * calctime 2017-07-28 2017-7-28
 * calctime 2017-07-28 8:44:53 2017-07-28 09:32:57
 * calctime 2017-7-28 08:44:53 AM 2017-07-28 9:32:57 AM
 * 
 * Syntax to type date:
 * YYYY-MM-DD
 * 
 * Syntax to type time:
 * hh-mm-ss
 * 
 * Note that you can also input single digits when the number is less than 10.
 * For example:
 * 2014-05-01 is equal to 2014-5-1
 * 
 * and the delimeter to separate the numbers can be the following 4 characters:
 * - / . :
*/

#include <iostream>
#include <ctime>
#include <math.h>
#include <cmath>
#include <string>

using namespace std;

class Time {
	public:
		int hour;
		int minute;
		int second;
		int meridiem; // 1: AM, 2: PM, 0: 24 hour clock
	
		Time() {
			this->hour = 0;
			this->minute = 0;
			this->second = 0;
			this->meridiem = 0;
		}

		Time(int hour, int minute, int second) {
			this->hour = hour;
			this->minute = minute;
			this->second = second;
			this->meridiem = 0;
		}

		Time(int hour, int minute, int second, int meridiem) {
			this->hour = hour;
			this->minute = minute;
			this->second = second;
			this->meridiem = meridiem;

			this->cto24();
		}

		void cto24() {
			if (this->meridiem == 1 && this->hour == 12) this->hour == 0;
			else if (this->meridiem == 2) {
				if (this->hour != 12)
					this->hour += 12;
			}
		}
};

bool timeDifference(Time* t2, Time* t1, Time* result) {
	bool deductDay = false;
	
	if (t1->second < t2->second) {
		t1->minute--;
		t1->second += 60;
	}
	result->second = t1->second - t2->second;
	
	if (t1->minute < t2->minute) {
		t1->hour--;
		t1->minute += 60;
	}
	result->minute = t1->minute - t2->minute;
	
	if (t1->hour < t2->hour) {
		deductDay = true;
		t1->hour += 24;
	}
	result->hour = t1->hour - t2->hour;

	return deductDay;
}

int differenceDatesInDays(int y, int m, int d, int _y, int _m, int _d) {
    tm start = {0};
    start.tm_year = y - 1900;
    start.tm_mon = m - 1;
    start.tm_mday = d;
    time_t start_secs = mktime(&start);

    tm end = {0};
    end.tm_year = _y - 1900;
    end.tm_mon = _m - 1;
    end.tm_mday = _d;
    time_t end_secs = mktime(&end);
    return (end_secs - start_secs) / (24*60*60);
}

double getTimeInHours(Time* t) {
	double result = ((double)t->hour) + (((double)t->minute)/60.0) + (((double)t->second)/(3600.0));
	return result;
}

double my_fmod(double n1, double n2) {
	return fmod(n1, n2);
}

string daysExpand(int days) {
	double y, m, d;
	string result;
	
	d = (double) days;
	
	y = d / 365.4;
	d = my_fmod(d, 365.4);
	m = d / 30.0;
	d = my_fmod(d, 30.0);
	
	result = to_string((int)y) + "Y " + to_string((int)m) + "M " + to_string((int)d) + "D";
	return result;
}

string timeExpand(Time* time) {
	string result = to_string(time->hour) + "h " + to_string(time->minute) + "m " + to_string(time->second) + "s";
	return result;
}

void parseInput(string input, int *inputArray) {
	string delim1 = ":";
	string delim2 = "/";
	string delim3 = ".";
	string delim4 = "-";
	
	size_t pos = 0;
	string token;
	int i = 0;
	string::size_type sz;

	while ((pos = input.find_first_of(delim1 + delim2 + delim3 + delim4)) != string::npos) {
		token = input.substr(0, pos);
		
		inputArray[i++] = stoi(*(new string(token)), &sz);

		input.erase(0, pos + delim1.length());
	}
}

int main(int argc, char **argv) {
	
	Time *Tstart, *Tend, res;
	
	string::size_type sz;
	double daysDiff, timeDiff;
	string inputString;
	int inputArray[14];

	try {
		if (argc == 3) {
			inputString = *(new string(argv[1])) + "-" + *(new string(argv[2])) + "-";
			
			parseInput(inputString, inputArray);
			
			daysDiff = (double) differenceDatesInDays(inputArray[0], inputArray[1], inputArray[2], inputArray[3], inputArray[4], inputArray[5]);
			
			timeDiff = daysDiff * 24.0;

			cout << daysExpand(daysDiff) << endl;
			cout << "Total days: " << floor(daysDiff * 100.)/100 << endl << "Total Hours: " << floor(timeDiff * 100.)/100 << endl;
		} else if (argc == 5) {
			inputString = *(new string(argv[1])) + "-" + (*new string(argv[2])) + "-" + *(new string(argv[3])) + "-" + *(new string(argv[4])) + "-";

			parseInput(inputString, inputArray);

			Tstart = new Time(inputArray[3], inputArray[4], inputArray[5]);
			Tend = new Time(inputArray[9], inputArray[10], inputArray[11]);

			daysDiff = (double) differenceDatesInDays(inputArray[0], inputArray[1], inputArray[2], inputArray[6], inputArray[7], inputArray[8]);
			if (timeDifference(Tstart, Tend, &res)) daysDiff--;

			timeDiff = getTimeInHours(&res) + (daysDiff*24.0);

			cout << daysExpand(daysDiff) << " - " << timeExpand(&res) << endl;
			cout << "Total days: " << floor(daysDiff * 100.)/100 << endl << "Total Hours: " << floor(timeDiff * 100.)/100 << endl;
		} else if (argc == 7) {
			inputString = *(new string(argv[1])) + "-" + *(new string(argv[2])) + "-" + ((*(new string(argv[3])) == "AM")? "1": "2") + "-" + *(new string(argv[4])) + "-" + *(new string(argv[5])) + "-" + ((*(new string(argv[6])) == "AM")? "1": "2") + "-";

			parseInput(inputString, inputArray);

			Tstart = new Time(inputArray[3], inputArray[4], inputArray[5], inputArray[6]);
			Tend = new Time(inputArray[10], inputArray[11], inputArray[12], inputArray[13]);

			daysDiff = differenceDatesInDays(inputArray[0], inputArray[1], inputArray[2], inputArray[7], inputArray[8], inputArray[9]);
			if (timeDifference(Tstart, Tend, &res)) daysDiff--;
			
			timeDiff = getTimeInHours(&res) + (daysDiff*24.0);
			
			cout << daysExpand(daysDiff) << " - " << timeExpand(&res) << endl;
			cout << "Total days: " << floor(daysDiff * 100.)/100 << endl << "Total Hours: " << floor(timeDiff * 100.)/100 << endl;
		} else {
			cout << "Correct usage: " << endl;
			cout << "- Only dates:" << endl;
			cout << "\tcalctime <Start Date> <End Date>" << endl;
			cout << "- Date and Time with 12-hour clock:" << endl;
			cout << "\tcalctime <Start Date> <Start Time> <AM/PM> <End Date> <End Time> <AM/PM>" << endl;
			cout << "- Date and Time with 24-hour clock:" << endl;
			cout << "\tcalctime <Start Date> <Start Time> <End Date> <End Time>" << endl;
		}
	} catch(exception e) {
		cout << "Error: " << e.what() << endl;
		cout << "Maybe the syntax is incorrect.";
	}
	return 0;
}
