#include<iostream>
#include<ctime>
#include"user.h"
#include<map>
#include<string>
using namespace std;
void main(){
	user m;
	int choice;
	do{
		cout << "1 - Add Event . \n2 - Delete Event . \n3 - Update Event .\n4 - Display Upcoming Events .\n5 - Display Done Events .\n6 - Exit .\nEnter your choice : ";
		cin >> choice;
		if (choice == 1){
			m.Add();
		}
		else if (choice == 2){
			m.Delete();
		}
		else if (choice == 3){
			m.Update();
		}
		else if (choice == 4){
			m.Display_upcoming();
		}
		else if (choice == 5){
			m.Display_done();
		}
	} while (choice != 6);
}