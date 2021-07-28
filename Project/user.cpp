#include "user.h"
#include<map>
#include<iostream>
#include<ctime>
#include<time.h>
#include<fstream>
#include<cstdio>

user::user(){
	read_file();
	Reminder();
	done();
}

/*___________________________________________________________________________________________________________________________________________________________________________
1 - (ADD)
___________________________________________________________________________________________________________________________________________________________________________*/
void user::Add(){
	vector<Event> events2;//vector used in upcoming_events map
	Event events;
	time_t now = time(0);
	struct tm currtime;
	localtime_s(&currtime, &now);
	date temp;
	map<date, vector<Event>>::iterator it;
	vector<Event>::iterator it2;


	do{

		cout << "Enter The Start Date of The Event (Day[1->31]/Month[1->12]/Year[>=2018]) : ";
		cin >> events.start_date.day >> events.start_date.month >> events.start_date.year;
		cout << "Enter The Start Time of The Event (Hour[0->23]:Min[0->59]:Sec[0->59]) : ";
		cin >> events.start_date.hour >> events.start_date.min >> events.start_date.sec;
		events.start_date = limit_date(events.start_date);

		temp.day = currtime.tm_mday;
		temp.month = currtime.tm_mon + 1;
		temp.year = currtime.tm_year + 1900;
		temp.hour = currtime.tm_hour;
		temp.min = currtime.tm_min;
		temp.sec = currtime.tm_sec;

		events.start_date = prevent_precede(events.start_date, temp);

		cout << "Enter Event's Place : ";
		cin.ignore();
		getline(cin, events.place);

	} while (collapse(events.start_date, events.place));
	//#Set the reminder date for the event
	if (events.start_date.day == 1 && events.start_date.month == 1){
		events.reminder.day = 31;
		events.reminder.month = 12;
		events.reminder.year = events.start_date.year - 1;
	}
	else if (events.start_date.day == 1){
		events.reminder.day = 31;
		events.reminder.month = events.start_date.month - 1;
		events.reminder.year = events.start_date.year;
	}
	else{
		events.reminder.day = events.start_date.day - 1;
		events.reminder.month = events.start_date.month;
		events.reminder.year = events.start_date.year;
	}
	events.reminder.hour = events.start_date.hour;
	events.reminder.min = events.start_date.min;
	events.reminder.sec = events.start_date.sec;

	cout << "Enter Event's Name : ";
	getline(cin, events.name);

	do{
		cout << "Enter The End Date of The Event (Day[1->31]/Month[1->12]/Year[>=2018]) : ";
		cin >> events.end_date.day >> events.end_date.month >> events.end_date.year;
		cout << "Enter The End Time of The Event (Hour[0->23]:Min[0->59]:Sec[0->59]) : ";
		cin >> events.end_date.hour >> events.end_date.min >> events.end_date.sec;
		events.end_date = limit_date(events.end_date);
		events.end_date = prevent_precede(events.end_date, events.start_date);
	} while (collapse(events.end_date, events.place));

	it = upcoming_events.find(events.start_date);
	if (it != upcoming_events.end()){
		for (it = upcoming_events.begin(); it != upcoming_events.end(); it++){
			if (events.start_date.day == it->first.day&&events.start_date.month == it->first.month&&events.start_date.year == it->first.year&&events.start_date.hour == it->first.hour&&events.start_date.min == it->first.min&&events.start_date.sec == it->first.sec){
				it->second.push_back(events);
				break;
			}
		}
	}
	else{
		events2.push_back(events);
		upcoming_events[events.start_date] = events2;
	}
}

/*____________________________________________________________________________________________________________________________________________________________________
2-(DISPLAY UPCOMING EVENTS)
______________________________________________________________________________________________________________________________________________________________________*/

void user::Display_upcoming()
{
	map <date, vector<Event> >::iterator it;
	vector<Event>::iterator it2;
	for (it = upcoming_events.begin(); it != upcoming_events.end(); it++)
	{
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			cout << "---------------------------------------------------------" << endl;
			cout << " You are now viewing your event details :) " << endl;
			cout << "__________________________________________________" << endl;
			cout << " | Event Start Date | " << it->first.day << "/" << it->first.month << "/" << it->first.year << endl;
			cout << " | Event Start Time | " << it->first.hour << " : " << it->first.min << " : " << it->first.sec << endl;
			cout << " | Event End Date   | " << it2->end_date.day << "/" << it2->end_date.month << "/" << it2->end_date.year << endl;
			cout << " | Event End Time   | " << it2->end_date.hour << " : " << it2->end_date.min << " : " << it2->end_date.sec << endl;
			cout << " | Event Name       | " << it2->name << endl;
			cout << " | Event place      | " << it2->place << endl;
			cout << " |___________________________________________________" << endl;
		}
	}

}

/*____________________________________________________________________________________________________________________________________________________________________
3-(DISPLAY DONE EVENTS)
______________________________________________________________________________________________________________________________________________________________________*/

void user::Display_done() {
	map<date, vector<Event>>::iterator it = done_events.begin();
	while (it != done_events.end())
	{
		vector<Event>::iterator it2 = it->second.begin();
		while (it2 != it->second.end()){
			cout << "---------------------------------------------------------" << endl;
			cout << " You are now viewing your event details :) " << endl;
			cout << "__________________________________________________" << endl;
			cout << " | Event Start Date | " << it->first.day << "/" << it->first.month << "/" << it->first.year << endl;
			cout << " | Event Start Time | " << it->first.hour << " : " << it->first.min << " : " << it->first.sec << endl;
			cout << " | Event End Date   | " << it2->end_date.day << "/" << it2->end_date.month << "/" << it2->end_date.year << endl;
			cout << " | Event End Time   | " << it2->end_date.hour << " : " << it2->end_date.min << " : " << it2->end_date.sec << endl;
			cout << " | Event Name       | " << it2->name << endl;
			cout << " | Event place      | " << it2->place << endl;
			cout << " |___________________________________________________" << endl;
			it2++;
		}
		it++;
	}
}

/*____________________________________________________________________________________________________________________________________________________________________
4-(REMINDER)
______________________________________________________________________________________________________________________________________________________________________*/

void user::Reminder(){
	time_t now = time(0);
	struct tm currtime;
	localtime_s(&currtime, &now);
	map<date, vector<Event>>::iterator it = upcoming_events.begin();
	while (it != upcoming_events.end()){
		vector<Event>::iterator it2 = it->second.begin();
		while (it2 != it->second.end()){
			if (currtime.tm_mday >= it2->reminder.day&&currtime.tm_mday <= it->first.day&&currtime.tm_mon + 1 >= it2->reminder.month&&currtime.tm_mon + 1 <= it->first.month&&currtime.tm_year + 1900 >= it2->reminder.year&&currtime.tm_year + 1900 <= it->first.year){
				cout << "(***********************************NOTE********************************************************)" << endl;
				cout << "You Have Event At : " << it->first.day << "/" << it->first.month << "/" << it->first.year << "\t" << it->first.hour << ":" << it->first.min << ":" << it->first.sec << "\nEvent's Name : " << it2->name << "\nEvent's Place : " << it2->place << "\n Ends At : " << it2->end_date.day << "/" << it2->end_date.month << "/" << it2->end_date.year << "\t" << it2->end_date.hour << ":" << it2->end_date.min << ":" << it2->end_date.sec << endl;
				cout << "*************************************************************************************************" << endl;
			}
			it2++;
		}
		it++;
	}
}

/*____________________________________________________________________________________________________________________________________________________________________
5-(DONE)
______________________________________________________________________________________________________________________________________________________________________*/

void user::done()
{
	Event e;
	time_t now = time(0);
	struct tm currtime;
	localtime_s(&currtime, &now);
	map <date, vector<Event> >::iterator it;
	vector<Event>::iterator it2;
	vector<Event> temp;
	for (it = upcoming_events.begin(); it != upcoming_events.end(); it++)
	{
	vector<Event> events3;//vector used in done_events map
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++){
			if (it2->end_date.year <= currtime.tm_year + 1900 && it2->end_date.month <= currtime.tm_mon + 1 && it2->end_date.day <= currtime.tm_mday&&it2->end_date.hour <= currtime.tm_hour&&it2->end_date.min <= currtime.tm_min&&it2->end_date.sec <= currtime.tm_sec)
			{
				cout << "__________________________________________________" << endl;
				cout << " | Event Start Date | " << it->first.day << "/" << it->first.month << "/" << it->first.year << endl;
				cout << " | Event Start Time | " << it->first.hour << " : " << it->first.min << " : " << it->first.sec << endl;
				cout << " | Event End Date   | " << it2->end_date.day << "/" << it2->end_date.month << "/" << it2->end_date.year << endl;
				cout << " | Event End Time   | " << it2->end_date.hour << " : " << it2->end_date.min << " : " << it2->end_date.sec << endl;
				cout << " | Event Name       | " << it2->name << endl;
				cout << " | Event place      | " << it2->place << endl;
				cout << " |___________________________________________________" << endl;
				cout << "Did you do this event ?  [y/n]. : ";
				char ch;
				cin >> ch;
				e.start_date.day = it->first.day;
				e.start_date.month = it->first.month;
				e.start_date.year = it->first.year;
				e.start_date.hour = it->first.hour;
				e.start_date.min = it->first.min;
				e.start_date.sec = it->first.sec;

				e.reminder.day = it2->reminder.day;
				e.reminder.month = it2->reminder.month;
				e.reminder.year = it2->reminder.year;
				e.reminder.hour = it2->reminder.hour;
				e.reminder.min = it2->reminder.min;
				e.reminder.sec = it2->reminder.sec;

				e.place = it2->place;
				e.name = it2->name;

				e.end_date.day = it2->end_date.day;
				e.end_date.month = it2->end_date.month;
				e.end_date.year = it2->end_date.year;
				e.end_date.hour = it2->end_date.hour;
				e.end_date.min = it2->end_date.min;
				e.end_date.sec = it2->end_date.sec;
				
				if (ch == 'y')
				{
					map <date, vector<Event> >::iterator i=done_events.find(e.start_date);
					if (i == done_events.end()){
						events3.push_back(e);
						done_events[e.start_date] = events3;
					}
					else{
						i->second.push_back(e);
					}
				}
			}
		}
		temp.push_back(e);
	}
	for (it2 = temp.begin(); it2 != temp.end(); it2++){
		upcoming_events.erase(e.start_date);
	}
}


/*____________________________________________________________________________________________________________________________________________________________________
6-(DELETE EVENT)
______________________________________________________________________________________________________________________________________________________________________*/

void user::Delete(){
	Display_upcoming();
	map<date, vector<Event>>::iterator it;
	vector<Event>::iterator it2;
	date k;
	string p;
	cout << "Enter the start date of the event you want to delete(day/month/year) : ";
	cin >> k.day >> k.month >> k.year;
	cout << "Enter the start time of the event you want to delete(hour:min:sec) : ";
	cin >> k.hour >> k.min >> k.sec;
	for (it = upcoming_events.begin(); it != upcoming_events.end(); it++)
	{
		if (k.day == it->first.day&&k.month == it->first.month&&k.year == it->first.year&&k.hour == it->first.hour&&k.min == it->first.min&&k.sec == it->first.sec){
			if (it->second.size() == 1){
				upcoming_events.erase(it->first);
				break;
			}
			else if (it->second.size() > 1){
				cout << "Enter The Place Of The Event : ";
				cin >> p;
				bool deleted = false;
				for (it2 = it->second.begin(); it2 != it->second.end(); it++)
				{
					if (p == it2->place){
						it->second.pop_back();
						deleted = true;
						break;
					}
				}
				if (deleted){
					break;
				}
			}
		}
	}
}

/*_____________________________________________________________________________________________________________________________________________________________________________
7-(UPDATE)
_______________________________________________________________________________________________________________________________________________________________________________*/

void user::Update()
{
	int choice;
	Event temp_event, temp_event2;
	date temp_start_date, temp_start_date2, temp_end_date, temp_end_date2, temp_reminder, temp_reminder2;
	string temp_name, temp_name2, temp_place, temp_place2;
	Display_upcoming();
	string name;
	bool found, startDateChanged = false;
	cout << "Enter the name of the event : ";
	cin >> name;
	while (true)
	{
		found = Event::containsName(upcoming_events, name);
		if (found)
		{
			break;
		}
		else
		{
			cout << "There is no Event with such name" << endl;
			cout << "Enter the name of an existing event : ";
			cin >> name;
		}
	}
	temp_event = Event::searchEventsByName(upcoming_events, name);
	temp_name = temp_event.name;
	temp_place = temp_event.place;
	temp_start_date = temp_event.start_date;
	temp_end_date = temp_event.end_date;
	temp_reminder = temp_event.reminder;
	temp_event2 = temp_event;
	temp_name2 = temp_name;
	temp_place2 = temp_place;
	temp_start_date2 = temp_start_date;
	temp_end_date2 = temp_end_date;
	temp_reminder2 = temp_reminder;
	cout << "Press :" << endl;
	cout << "	1 to Update event name" << endl;
	cout << "	2 to Update event place" << endl;
	cout << "	3 to Update event start date" << endl;
	cout << "	4 to Update event end date" << endl;
	cout << "	5 to Update event reminder date" << endl;
	cin >> choice;
	switch (choice)
	{
	case 1:
	{
			  string newName;
			  cout << "Enter the new Event name : ";
			  cin >> newName;
			  temp_event.name = newName;
			  break;
	}
	case 2:
	{
			  string newPlace;
			  cout << "Enter the new Event Place : ";
			  cin >> newPlace;
			  temp_event.place = newPlace;
			  break;
	}
	case 3:
	{
			  date newStartDate;
			  char changeEndDate;
			  cout << "Enter the new Start Day : ";
			  cin >> newStartDate.day;
			  cout << "Enter the new Start Month : ";
			  cin >> newStartDate.month;
			  cout << "Enter the new Start Year : ";
			  cin >> newStartDate.year;
			  cout << "Enter the new Start Hour : ";
			  cin >> newStartDate.hour;
			  cout << "Enter the new Start Minute : ";
			  cin >> newStartDate.min;
			  cout << "Enter the new Start Second : ";
			  cin >> newStartDate.sec;
			  temp_event.start_date = limit_date(newStartDate);
			  temp_start_date = temp_event.start_date;
			  startDateChanged = true;
			  cout << "The start date have changed :" << endl;
			  cout << "Here is the details of the event you updated :" << endl;
			  Event::displayEvent(temp_event);
			  cout << "Do you want to change the End Date as well?(Y/N)" << endl;
			  cin >> changeEndDate;
			  switch (changeEndDate)
			  {
			  case 'Y':
			  case 'y':
			  {
						  date newEndDate;
						  cout << "Enter the new End Day : ";
						  cin >> newEndDate.day;
						  cout << "Enter the new End Month : ";
						  cin >> newEndDate.month;
						  cout << "Enter the new End Year : ";
						  cin >> newEndDate.year;
						  cout << "Enter the new End Hour : ";
						  cin >> newEndDate.hour;
						  cout << "Enter the new End Minute : ";
						  cin >> newEndDate.min;
						  cout << "Enter the new End Second : ";
						  cin >> newEndDate.sec;
						  temp_event.end_date = limit_date(newEndDate);
						  break;
			  }
			  case 'N':
			  case 'n':
			  {
						  break;
			  }
			  }
			  break;
	}
	case 4:
	{
			  date newEndDate;
			  cout << "Enter the new End Day : ";
			  cin >> newEndDate.day;
			  cout << "Enter the new End Month : ";
			  cin >> newEndDate.month;
			  cout << "Enter the new End Year : ";
			  cin >> newEndDate.year;
			  cout << "Enter the new End Hour : ";
			  cin >> newEndDate.hour;
			  cout << "Enter the new End Minute : ";
			  cin >> newEndDate.min;
			  cout << "Enter the new End Second : ";
			  cin >> newEndDate.sec;
			  temp_event.end_date = limit_date(newEndDate);
			  break;
	}
		//m4 by5tar el reminder b mzago
	case 5:
		date newReminder;
		cout << "Enter the new Reminder Day : ";
		cin >> newReminder.day;
		cout << "Enter the new Reminder Month : ";
		cin >> newReminder.month;
		cout << "Enter the new Reminder Year : ";
		cin >> newReminder.year;
		cout << "Enter the new Reminder Hour : ";
		cin >> newReminder.hour;
		cout << "Enter the new Reminder Minute : ";
		cin >> newReminder.min;
		cout << "Enter the new Reminder Second : ";
		cin >> newReminder.sec;
		temp_event.reminder = limit_date(newReminder);
		break;

	}
	vector<Event> V1, V2;
	vector<Event>::iterator it;
	bool equalPlace, equalName, equalEndDate, equalReminder;
	if (startDateChanged)
	{
		it = upcoming_events[temp_start_date2].begin();
		for (it; it != upcoming_events[temp_start_date2].end(); it++)
		{
			equalName = (it->name == temp_name2);
			equalPlace = (it->place == temp_place2);
			equalEndDate = Event::compareDates(it->end_date, temp_end_date2);
			equalReminder = Event::compareDates(it->reminder, temp_reminder2);
			if (equalName && equalPlace && equalEndDate && equalReminder)
			{
				V2.push_back(temp_event);
			}
			else
			{
				V1.push_back((*it));
			}

		}
		upcoming_events[temp_start_date2] = V1;
		upcoming_events[temp_start_date] = V2;
	}
	else
	{
		it = upcoming_events[temp_start_date2].begin();
		for (it; it != upcoming_events[temp_start_date2].end(); it++)
		{
			equalName = (it->name == temp_name2);
			equalPlace = (it->place == temp_place2);
			equalEndDate = Event::compareDates(it->end_date, temp_end_date2);
			equalReminder = Event::compareDates(it->reminder, temp_reminder2);
			if (equalName && equalPlace && equalEndDate && equalReminder)
			{

				*it = temp_event;
				break;
			}
		}

	}
}

/*____________________________________________________________________________________________________________________________________________________________________
8-(LIMIT RANGE OF DATE AND TIME)
______________________________________________________________________________________________________________________________________________________________________*/

date user::limit_date(date x){
	while (x.day<1 || x.day>31 || x.month<1 || x.month>12 || x.year < 2018){
		cout << "You can't Enter a Date of Day out of range [1->31] , Month out of range[1->12] And Year Less than 2018 :/ " << endl;
		cout << "Please Re-enter The Date (Day[1->31],Month[1->12] And Year More Than 2018 : ";
		cin >> x.day >> x.month >> x.year;
	}
	while (x.hour<0 || x.hour>23 || x.min<0 || x.min>59 || x.sec<0 || x.sec>59){
		cout << "You can't Enter a Time of Hours out of range [0->23] , Minutes out of range [0->59] And Seconds Out Of Range [0->59] :/ " << endl;
		cout << "Please Re-enter The Time (Hours[0->23] , Minutes[0->59] And Seconds[0->59] : ";
		cin >> x.hour >> x.min >> x.sec;
	}
	return x;
}

/*____________________________________________________________________________________________________________________________________________________________________
9-(PREVENT PRECEDE)
______________________________________________________________________________________________________________________________________________________________________*/

date user::prevent_precede(date x, date y){
	time_t now = time(0);
	struct tm currtime;
	localtime_s(&currtime, &now);
	if (x.year < y.year){
		cout << "This Date Is Unavailable ):" << endl;
		cout << "Please Re-enter The Date (Day[1->31],Month[1->12] And Year More Than 2018 : ";
		cin >> x.day >> x.month >> x.year;
		x = limit_date(x);
		prevent_precede(x, y);
		return x;
	}
	else if (x.year == y.year){
		if (x.month < y.month){
			cout << "This Date Is Unavailable ):" << endl;
			cout << "Please Re-enter The Date (Day[1->31],Month[1->12] And Year More Than 2018 : ";
			cin >> x.day >> x.month >> x.year;
			x = limit_date(x);
			prevent_precede(x, y);
			return x;
		}
		else if (x.month == y.month){
			if (x.day < y.day){
				cout << "This Date Is Unavailable ):" << endl;
				cout << "Please Re-enter The Date (Day[1->31],Month[1->12] And Year More Than 2018 : ";
				cin >> x.day >> x.month >> x.year;
				x = limit_date(x);
				prevent_precede(x, y);
				return x;
			}
			else if (x.day == y.day){
				if (x.hour < y.hour){
					cout << "This Time Is Unavailable ):" << endl;
					cout << "Please Re-enter The Time (Hours[0->23] , Minutes[0->59] And Seconds[0->59] : ";
					cin >> x.hour >> x.min >> x.sec;
					x = limit_date(x);
					prevent_precede(x, y);
					return x;
				}
				else if (x.hour == y.hour){
					if (x.min < y.min){
						cout << "This Time Is Unavailable ):" << endl;
						cout << "Please Re-enter The Time (Hours[0->23] , Minutes[0->59] And Seconds[0->59] : ";
						cin >> x.hour >> x.min >> x.sec;
						x = limit_date(x);
						prevent_precede(x, y);
						return x;
					}
					else if (x.min == y.min){
						if (x.sec < y.sec){
							cout << "This Time Is Unavailable ):" << endl;
							cout << "Please Re-enter The Time (Hours[0->23] , Minutes[0->59] And Seconds[0->59] : ";
							cin >> x.hour >> x.min >> x.sec;
							x = limit_date(x);
							prevent_precede(x, y);
							return x;
						}
					}
				}
			}
		}
	}
	return x;
}

/*____________________________________________________________________________________________________________________________________________________________________
10-(PREVENT COLLAPSE)
_______________________________________________________________________________________________________________________________________________________________________*/

bool user::collapse(date x, string y){
	map<date, vector<Event>>::iterator it = upcoming_events.begin();
	vector<Event>::iterator it2;
	while (it != upcoming_events.end()){
		it2 = it->second.begin();
		while (it2 != it->second.end()){
			if ((x.year > it->first.year && x.year < it2->end_date.year) && y == it2->place) {
				cout << "There Is An Event During This Time And This Place ): " << endl;
				return true;
			}
			else if (x.year == it->first.year && x.year == it2->end_date.year&&y == it2->place){
				if (x.month > it->first.month && x.month < it2->end_date.month){
					cout << "There Is An Event During This Time And This Place ): " << endl;
					return true;
				}
				else if (x.month == it->first.month && x.month == it2->end_date.month){
					if (x.day > it->first.day && x.day < it2->end_date.day){
						cout << "There Is An Event During This Time And This Place ): " << endl;
						return true;
					}
					else if (x.day == it->first.day && x.day == it2->end_date.day){
						if (x.hour > it->first.hour && x.hour < it2->end_date.hour){
							cout << "There Is An Event During This Time And This Place ): " << endl;
							return true;
						}
						else if (x.hour == it->first.hour && x.hour == it2->end_date.hour){
							if (x.min > it->first.min && x.min < it2->end_date.min){
								cout << "There Is An Event During This Time And This Place ): " << endl;
								return true;
							}
							else if (x.min == it->first.min && x.min == it2->end_date.min){
								if (x.sec >= it->first.sec && x.sec <= it2->end_date.sec){
									cout << "There Is An Event During This Time And This Place ): " << endl;
									return true;
								}
							}
						}
					}
				}
			}
			it2++;
		}
		it++;
	}
	return false;
}

/*__________________________________________________________________________________________________________________________________________________________________________
11-(WRITE FILE)
_____________________________________________________________________________________________________________________________________________________________________________*/

void user::write_file(){
	ofstream events("Events.txt");
	map<date, vector<Event>>::iterator it = upcoming_events.begin();
	vector<Event>::iterator it2;
	while (it != upcoming_events.end()){
		it2 = it->second.begin();
		while (it2 != it->second.end()){
			events << it->first.day << " " << it->first.month << " " << it->first.year << " " << it->first.hour << " " << it->first.min << " " << it->first.sec << " " << it2->reminder.day << " " << it2->reminder.month << " " << it2->reminder.year << " " << it2->reminder.hour << " " << it2->reminder.min << " " << it2->reminder.sec << " " << it2->place << "," << it2->name << "," << it2->end_date.day << " " << it2->end_date.month << " " << it2->end_date.year << " " << it2->end_date.hour << " " << it2->end_date.min << " " << it2->end_date.sec << endl;
			it2++;
		}
		it++;
	}
	events.close();
	ofstream eventsdone("Done Events.txt");
	for (it = done_events.begin(); it != done_events.end(); it++){
		for (it2 = it->second.begin(); it2 != it->second.end(); it2++){
			eventsdone << it->first.day << " " << it->first.month << " " << it->first.year << " " << it->first.hour << " " << it->first.min << " " << it->first.sec << " " << it2->reminder.day << " " << it2->reminder.month << " " << it2->reminder.year << " " << it2->reminder.hour << " " << it2->reminder.min << " " << it2->reminder.sec << " " << it2->place << "," << it2->name << "," << it2->end_date.day << " " << it2->end_date.month << " " << it2->end_date.year << " " << it2->end_date.hour << " " << it2->end_date.min << " " << it2->end_date.sec << endl;
		}
	}
	eventsdone.close();
}

/*_____________________________________________________________________________________________________________________________________________________________________________________
12-(READ FILE)
_______________________________________________________________________________________________________________________________________________________________________________________*/
void user::read_file(){
	ifstream events1("Events.txt");
	Event event1;
	string line;
	events1.seekg(0, ios::end);
	int size = events1.tellg();
	events1.close();
	ifstream events("Events.txt");
	if (size != 0){
		while (!events.eof()){
			vector<Event>event2;
			events >> event1.start_date.day;
			events >> event1.start_date.month;
			events >> event1.start_date.year;
			events >> event1.start_date.hour;
			events >> event1.start_date.min;
			events >> event1.start_date.sec;
			events >> event1.reminder.day;
			events >> event1.reminder.month;
			events >> event1.reminder.year;
			events >> event1.reminder.hour;
			events >> event1.reminder.min;
			events >> event1.reminder.sec;
			getline(events, event1.place, ',');
			getline(events, event1.name, ',');
			events >> event1.end_date.day;
			events >> event1.end_date.month;
			events >> event1.end_date.year;
			events >> event1.end_date.hour;
			events >> event1.end_date.min;
			events >> event1.end_date.sec;
			map<date, vector<Event>>::iterator  it = upcoming_events.find(event1.start_date);
			vector<Event>::iterator it2;
			if (it != upcoming_events.end()){
				bool exist = false;
				for (it2 = it->second.begin(); it2!=it->second.end(); it2++){
					if (it2->name == event1.name&&it2->place == event1.place&&it2->end_date.day == event1.end_date.day&&it2->end_date.month == event1.end_date.month&&it2->end_date.year == event1.end_date.year&&it2->end_date.hour == event1.end_date.hour&&it2->end_date.min == event1.end_date.min&&it2->end_date.sec == event1.end_date.sec){
						exist = true;
						break;
					}
				}
				if (exist){
					continue;
				}
				else{
					it->second.push_back(event1);
				}
			}
			else{
				event2.push_back(event1);
				upcoming_events[event1.start_date] = event2;
			}
		}
	}
	events.close();
	ofstream events2("Events.txt", ios::trunc);
	events2.close();

	ifstream eventsdone1("Done Events.txt");
	eventsdone1.seekg(0, ios::end);
	int size2 = eventsdone1.tellg();
	eventsdone1.close();
	ifstream eventsdone("Done Events.txt");
	if (size2 != 0){
		while (!eventsdone.eof()){
			vector<Event>event2;
			eventsdone >> event1.start_date.day;
			eventsdone >> event1.start_date.month;
			eventsdone >> event1.start_date.year;
			eventsdone >> event1.start_date.hour;
			eventsdone >> event1.start_date.min;
			eventsdone >> event1.start_date.sec;
			eventsdone >> event1.reminder.day;
			eventsdone >> event1.reminder.month;
			eventsdone >> event1.reminder.year;
			eventsdone >> event1.reminder.hour;
			eventsdone >> event1.reminder.min;
			eventsdone >> event1.reminder.sec;
			getline(eventsdone, event1.place, ',');
			getline(eventsdone, event1.name, ',');
			eventsdone >> event1.end_date.day;
			eventsdone >> event1.end_date.month;
			eventsdone >> event1.end_date.year;
			eventsdone >> event1.end_date.hour;
			eventsdone >> event1.end_date.min;
			eventsdone >> event1.end_date.sec;
			map<date, vector<Event>>::iterator  it;
			vector<Event>::iterator it2;
			it = done_events.find(event1.start_date);
			if (it != done_events.end()){
				bool exist = false;
				for (it2 = it->second.begin();it2!=it->second.end(); it2++){
					if (it2->name == event1.name&&it2->place == event1.place&&it2->end_date.day == event1.end_date.day&&it2->end_date.month == event1.end_date.month&&it2->end_date.year == event1.end_date.year&&it2->end_date.hour == event1.end_date.hour&&it2->end_date.min == event1.end_date.min&&it2->end_date.sec == event1.end_date.sec){
						exist = true;
						break;
					}
				}
				if (exist){
					continue;
				}
				else{
					it->second.push_back(event1);
				}
			}
			else{
				event2.push_back(event1);
				done_events[event1.start_date] = event2;
			}
		}
	}
	eventsdone.close();
	ofstream eventsdone2("Done Events.txt", ios::trunc);
	eventsdone2.close();
}


user::~user(){
	write_file();
}