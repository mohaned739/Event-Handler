#pragma once
#include<string>
#include<map>
#include<vector>
#include"Event.h"
class user
{
	map<date, vector<Event>> upcoming_events;
	map<date, vector<Event>> done_events;

	void done();
	void Reminder();
	date limit_date(date x);
	date prevent_precede(date x, date y);
	bool collapse(date x, string y);
	void read_file();
	void write_file();
public:
	void Add();
	void Delete();
	void Update();
	void Display_upcoming();
	void Display_done();
	user();
	~user();
};