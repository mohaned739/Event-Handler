#pragma once
#include<string>
#include<map>
#include<vector>
using namespace std;

struct date{
	int day, month, year;
	int hour, min, sec;
	bool operator < (const date &y)const{
		if (this->year != y.year){
			return this->year < y.year;
		}
		else if (this->month != y.month){
			return this->month < y.month;
		}
		else if (this->day != y.day){
			return this->day < y.day;
		}
		else if (this->hour != y.hour){
			return this->hour < y.hour;
		}
		else if (this->min != y.min){
			return this->min < y.min;
		}
		else if (this->sec != y.sec || this->sec == y.sec){
			return this->sec < y.sec;
		}
	}
};
class Event
{
public:
	date start_date;
	date reminder;
	string place;
	string name;
	date end_date;
	static bool compareDates(date, date);
	static Event searchEventsByName(map<date, vector<Event>>, string);
	static bool containsName(map<date, vector<Event>>, string);
	static void displayEvent(Event);
	Event();
};

