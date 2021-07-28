#include "Event.h"
#include<map>
#include<iostream>
#include<ctime>
#include<time.h>
#include<fstream>
using namespace std;

Event::Event(){
}

bool Event::containsName(map<date, vector<Event>> upcoming_map, string name)
{
	map < date, vector<Event>>::iterator it;
	vector<Event>::iterator it2;
	Event temp_event;
	string temp_name;
	bool found = false;
	for (it = upcoming_map.begin(); it != upcoming_map.end(); it++)
	{
		if (found)
		{
			break;
		}
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			if (it2->name == name)
			{
				found = true;
			}
		}
	}
	return found;
}

Event Event::searchEventsByName(map<date, vector<Event>> upcoming_map, string name)
{
	int duplicateCounter = 0;
	vector<Event> duplicateEvents;
	map < date, vector<Event>>::iterator it;
	vector<Event>::iterator it2;
	Event temp_event;
	for (it = upcoming_map.begin(); it != upcoming_map.end(); it++)
	{
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			if (it2->name == name)
			{

				temp_event.name = it2->name;
				temp_event.place = it2->place;
				temp_event.start_date = it2->start_date;
				temp_event.end_date = it2->end_date;
				temp_event.reminder = it2->reminder;
				duplicateEvents.push_back(temp_event);
				duplicateCounter++;
			}
		}
	}
	if (duplicateCounter == 1)
	{
		return duplicateEvents[0];
	}
	else
	{
		cout << "There are duplicate events with this name :" << endl;
		vector<Event>::iterator it;
		int counter = 0;
		for (it = duplicateEvents.begin(); it != duplicateEvents.end(); it++)
		{
			cout << counter + 1 << "-" << " ";
			Event::displayEvent(duplicateEvents[counter]);
			counter++;
		}
		int eventNum;
		cout << "Enter the number of the event you want to update : ";
		cin >> eventNum;
		return duplicateEvents[(eventNum - 1)];
	}
}

bool Event::compareDates(date D1, date D2)
{
	if ((D1.year == D2.year) && (D1.month == D2.month) && (D1.day == D2.day) && (D1.hour == D2.hour) && (D1.min == D2.min) && (D1.sec == D2.sec))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Event::displayEvent(Event E)
{
	Event temp_event;
	date temp_start_date, temp_end_date;
	string temp_name, temp_place;
	temp_event = E;
	temp_name = E.name;
	temp_place = E.place;
	temp_start_date = E.start_date;
	temp_end_date = E.end_date;
	cout << "Event Name : " << temp_name << " "
		<< "Event Place : " << temp_place << " "
		<< "Event Start Date : " << temp_start_date.day << "/" << temp_start_date.month << "/" << temp_start_date.year << " "
		<< temp_start_date.hour << ":";
	if (temp_start_date.min < 10)
	{
		cout << "0" << temp_start_date.min << ":";
	}
	else
	{
		cout << temp_start_date.min << ":";
	}
	if (temp_start_date.sec < 10)
	{
		cout << "0" << temp_start_date.sec << " ";
	}
	else
	{
		cout << temp_start_date.sec << " ";
	}
	cout << "Event End Date : " << temp_end_date.day << "/" << temp_end_date.month << "/" << temp_end_date.year << " "
		<< temp_end_date.hour << ":";
	if (temp_end_date.min < 10)
	{
		cout << "0" << temp_end_date.min << ":";
	}
	else
	{
		cout << temp_end_date.min << ":";
	}
	if (temp_end_date.sec < 10)
	{
		cout << "0" << temp_end_date.sec << endl;
	}
	else
	{
		cout << temp_end_date.sec << endl;
	}

}
