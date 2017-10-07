#include <iostream>
#include <fstream>
#include <conio.h>
#include <iomanip>
#include <windows.h>

using namespace std;

struct activity
{
public:
	char title[50];
	int userId;
	int duration;
	float priority;
};

struct timeOfactivity
{
public:
	int month, day, hour, activityNo;
};

struct timePeriod
{
public:
	int start_date, start_month, end_date, end_month, numberOfDays;
};

struct stats_for_month {
public:
	int totalNumberOfActivities, busiestUser, noOfActivitiesOfBusiestUser, busiestDay, noOfActivitiesBusiestDay;
	int noOfActivitiesInADay;
	float averageNumberofActivities;
};


void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void tellDaysinMonth(int &month, int &days)
{
	if (month < 7)							//To check for august
	{
		if (month == 1)
			days = 29;
		else if (month % 2 == 0)
			days = 31;
		else
			days = 30;
	}
	else
	{
		if (month % 2 == 0)
			days = 30;
		else
			days = 31;
	}
}

void check_wrong_date(timePeriod &date)
{
	while (date.start_month < 1 || date.start_month>12)
	{
		cout << endl << "Please enter valid start month" << endl;
		cin >> date.start_month;
	}
	while (date.end_month < 1 || date.end_month>12)
	{
		cout << endl << "Please enter valid end month" << endl;
		cin >> date.end_month;
	}
	int days_in_month;
	tellDaysinMonth(date.start_month, days_in_month);
	while (date.start_date<1 || date.start_date>days_in_month)
	{
		cout << endl << "Please enter a valid start date" << endl;
		cin >> date.start_date;
	}
	tellDaysinMonth(date.end_month, days_in_month);
	while (date.end_date<1 || date.end_month>days_in_month)
	{
		cout << endl << "Please enter a valid start date" << endl;
		cin >> date.start_date;
	}
}

activity** createBiggerArray(activity **calendar, int &capacity)
{
	capacity = 0;
	while (calendar[capacity] != nullptr)
		capacity++;
	
	activity **temp = new activity*[capacity + 2];

	for (int i = 0; i < capacity; i++)
	{
		temp[i] = new activity;
		*(temp[i]) = *(calendar[i]);
	}

	temp[capacity] = nullptr;
	temp[capacity+1] = nullptr;

	for (int i = 0; i < capacity; i++)
		delete calendar[i];
	
	delete[]calendar;
	calendar = temp;
	capacity++;
	return calendar;
}

int* changeByk(int *arr, int &capacity, int changeBy)
{
	int *temp = new int[capacity + changeBy];
	for (int i = 0; i < capacity; i++)
	{
		temp[i] = arr[i];
	}
	capacity += changeBy;
	delete[] arr;
	arr = temp;
	return arr;
}

timePeriod freeTime(int &userId, activity *****calendar)
{
	int days_in_month,j=0;
	int daysCount=0;
	bool startCounting = false;
	timePeriod longest1,longest2;
	longest1.start_month = 0, longest1.start_date = 0, longest2.start_month = 0, longest2.start_date = 0;
	longest1.numberOfDays = 0;
	for (int i = 0; i<12; i++)
	{
		tellDaysinMonth(i, days_in_month);
		for (j=0; j < days_in_month ; j++)
		{
			if (calendar[i][j] != nullptr)
			{
				for (int k = 0; k < 24 ; k++)
				{
					if (calendar[i][j][k] != nullptr)
					{
						for (int l = 0; calendar[i][j][k][l] != nullptr ; l++)
						{
							if (calendar[i][j][k][l]->userId == userId)
							{
								if (longest1.numberOfDays == 0)		//the first time userId has been matched
								{
									longest1.end_month = i, longest1.end_date = j, longest1.numberOfDays = daysCount;
									longest2.start_month = i, longest2.start_date = j;
								}
								else
								{
									longest2.end_date = j, longest2.end_month = i, longest2.numberOfDays = daysCount;
									if (daysCount > longest1.numberOfDays)
										longest1 = longest2;
									longest2.start_month = i, longest2.start_date = j;
								}
								daysCount = 0;
							}
						}
					}
				}
			}
			daysCount++;
			if (i==11&&j==30)
			{
				longest2.end_date = j, longest2.end_month = i, longest2.numberOfDays = daysCount;
				if (daysCount > longest1.numberOfDays)
					longest1 = longest2;
			}
		}
	}
	return longest1;
}

void all_Free_At_a_Time(activity *****calendar, int userArray[], int size,timePeriod &allFree)
{
	int days_in_month, j = allFree.start_date-1;
	int arr[24],count=0;
	for (int i = allFree.start_month - 1; i < allFree.end_month; i++)
	{
		tellDaysinMonth(i, days_in_month);
		for (; j < days_in_month; j++)
		{
			if (calendar[i][j] != nullptr)
			{
				for (int initialize = 0; initialize < 24; initialize++)
					arr[initialize] = 0;
				for (int m = 0; m < size; m++)
				{
					count = 0;
					for (int k = 0; k < 24; k++)
					{
						bool skip = false;
						if (calendar[i][j][k] != nullptr)
						{
							for (int l = 0; skip == 0 && calendar[i][j][k][l] != nullptr; l++)
							{
								if (userArray[m] == calendar[i][j][k][l]->userId)
								{
									for (int n = k; n <= calendar[i][j][k][l]->duration; n++)
									{
										if (arr[count] != 1)
										{
											arr[count] = 1;
											count++;
										}
									}
									skip = true; k = k - 1 + calendar[i][j][k][l]->duration;  //-1 is done because k++ will be done in the next iteration of loop
								}
							}
						}
					}
				}
				for (int m = 0; m < 24; m++)
				{
					if (arr[m]==0)
					cout << j + 1 << "/" << i + 1 << ",   " << m << " o' clock." << endl;
				}
			}
			else
				cout << j + 1 << "/" << i + 1 << ",   All Day i.e. 24 hours"<< endl;
			if (i == allFree.end_month - 1 && j == allFree.end_date-1)
				break;
		}
		j = 0;
	}
}

void clashingActivities(int &userid, int &user2id, activity *****calendar, timePeriod &clashingActivity)
{
	int days_in_month, j = clashingActivity.start_date - 1;

	for (int i = clashingActivity.start_month - 1; i < clashingActivity.end_month; i++)
	{
		tellDaysinMonth(i, days_in_month);
		for (; j < days_in_month; j++)
		{
			if (calendar[i][j] != nullptr)
			{
				for (int k = 0; k < 24; k++)
				{
					if (calendar[i][j][k] != nullptr)
					{
						for (int l = 0; calendar[i][j][k][l] != nullptr ; l++)
						{
							if (calendar[i][j][k][l]->userId == userid || calendar[i][j][k][l]->userId == user2id)
							{
								int tempUserId,tempDuration= calendar[i][j][k][l]->duration, time=k+tempDuration;
								if (calendar[i][j][k][l]->userId == userid)
									tempUserId = user2id; 
								else
									tempUserId = userid;
								int n = l;
								for (int m = k; m <= time ; m++)
								{
									if (calendar[i][j][m] != nullptr)
									{
										for (; calendar[i][j][m][n] != nullptr; n++)
										{
											if (calendar[i][j][m][n]->userId == tempUserId)
											{
												cout << "Clash between user " << calendar[i][j][k][l]->userId << " with an activity : " << calendar[i][j][k][l]->title << " on "
													 << l << "o clock to " << time << "o clock, And user " << tempUserId << " with an activity " << calendar[i][j][m][n]->title << " on " << m << "o clock to " << m + calendar[i][j][m][n]->duration << " o clock. On "<<i+1<<"/"<<j+1<<endl;
											}
										}
										n = 0;
									}
								}
							}
						}
					}
				}
			}
			if (i == clashingActivity.end_month - 1 && j == clashingActivity.end_date-1)
				break;
		}
		j = 0;
	}
}

int busiestUsers(timePeriod &busyTime, activity *****calendar,int &busiestUserId,bool printActivities)
{
	int days_in_month, j = busyTime.start_date - 1;
	int capacity=100;
	int *userActivities = new int[capacity]();
		
	for (int i = busyTime.start_month - 1; i < busyTime.end_month; i++)
	{
		tellDaysinMonth(i, days_in_month);
		for (; j < days_in_month; j++)
		{
			if (calendar[i][j] != nullptr)
			{
				for (int k = 0; k < 24; k++)
				{
					if (calendar[i][j][k] != nullptr)
					{
							for (int l = 0;  calendar[i][j][k][l] != nullptr ; l++)
							{
								if ((calendar[i][j][k][l]->userId - 1)>=capacity)
									changeByk(userActivities, capacity, calendar[i][j][k][l]->userId - capacity + 5);
								userActivities[calendar[i][j][k][l]->userId]++;
							}
					}
				}
			}
			if (i == busyTime.end_month - 1 && j == busyTime.end_date-1)
				break;
		}
		j = 0;
	}
	int *indexes = new int[capacity];
	for (int i = 0; i < capacity; i++)
		indexes[i] = i;

	int  min;
	int index;
	for (int i = capacity; i > 0; i--)					//selection sort (in descending order)
	{
		min = userActivities[0], index = 0;

		for (int j = 1; j < i; j++)
		{
			if (userActivities[j] < min)
			{
				index = j;
				min = userActivities[j];
			}
		}
		swap(userActivities[index], userActivities[i - 1]);
		swap(indexes[index], indexes[i - 1]);
	}
	if (printActivities)
	{
		for (int i = 0; i < 5 && i < capacity; i++)
			cout << "User " << indexes[i] + 1 << " with " << userActivities[i] << " activities." << endl;
	}
	busiestUserId = indexes[0];
	return userActivities[0];  
	
}

void allStats(timePeriod &stats, activity *****calendar,bool isForMonth)
{
	int days_in_month, j=stats.start_date-1;
	stats_for_month statistics;
	statistics.totalNumberOfActivities=0, statistics.noOfActivitiesBusiestDay=0, statistics.noOfActivitiesInADay=0;
	
	for (int i = stats.start_month - 1; i < stats.end_month; i++)
	{
		tellDaysinMonth(i, days_in_month);
		if (!isForMonth)
			statistics.noOfActivitiesInADay = 0;
		for (; j < days_in_month; j++)
		{
			if (calendar[i][j] != nullptr)
			{
				if(isForMonth)
					statistics.noOfActivitiesInADay = 0;
				for (int k = 0; k < 24; k++)
				{
					if (calendar[i][j][k] != nullptr)
					{
						for (int l = 0; calendar[i][j][k][l] != nullptr; l++)
						{
							statistics.totalNumberOfActivities++;
							statistics.noOfActivitiesInADay++;
						}
					}
				}
				if (isForMonth)
				{
					if (statistics.noOfActivitiesInADay > statistics.noOfActivitiesBusiestDay)
					{
						statistics.noOfActivitiesBusiestDay = statistics.noOfActivitiesInADay;
						statistics.busiestDay = j+1;
					}
				}
			}
			if (i == stats.end_month - 1 && j == stats.end_date-1)
				break;
		}
		if (!isForMonth)
		{
			if (statistics.noOfActivitiesInADay > statistics.noOfActivitiesBusiestDay)
			{
				statistics.noOfActivitiesBusiestDay = statistics.noOfActivitiesInADay;
				statistics.busiestDay = j+1;
			}
		}
		j = 0;
	}
	if (isForMonth)
	{
		tellDaysinMonth(stats.start_month, days_in_month);
		statistics.averageNumberofActivities = statistics.totalNumberOfActivities*1.0 / days_in_month;
		statistics.noOfActivitiesOfBusiestUser = busiestUsers(stats, calendar, statistics.busiestUser, 0);

		cout << "Total number of activities in the month are " << statistics.totalNumberOfActivities << "." << endl
			<< "Average number of activities per day is " << statistics.averageNumberofActivities << "." << endl
			<< "The busiest user of the month is user " << statistics.busiestUser << " with " << statistics.noOfActivitiesOfBusiestUser << " activities." << endl
			<< "The busiest day of the month is "<<statistics.busiestDay<< " with " << statistics.noOfActivitiesBusiestDay << " activities scheduled on this day." << endl;
	}
	else
	{
		statistics.averageNumberofActivities = statistics.totalNumberOfActivities / 12.0;
		cout << "Total number of activities in the year are " << statistics.totalNumberOfActivities<< "." << endl
			 << "Average number of activities per month is " << statistics.averageNumberofActivities << "." << endl;
		for (int i = 1; i <= 12; i++)
		{
			stats.start_month = i, stats.end_month = i;
			statistics.noOfActivitiesOfBusiestUser = busiestUsers(stats, calendar, statistics.busiestUser, 0);
			cout<< "The busiest user of the month "<<i<<" is user " << statistics.busiestUser << " with " << statistics.noOfActivitiesOfBusiestUser << " activities." << endl;
		}
		cout<<"The busiest month of the year is "<< statistics.busiestDay+1<<" with "<<statistics.noOfActivitiesBusiestDay << " activities scheduled on this month." << endl;
	}
	
		
}

timeOfactivity* all_activities_of_user(timePeriod &allActivities, int userId, int &count, activity *****calendar)
{
	int days_in_month, j = allActivities.start_date-1;
	int capacity = 3;
	count = 0;
	timeOfactivity* data = new timeOfactivity[capacity];

	for (int i = allActivities.start_month - 1; i < allActivities.end_month; i++)
	{
		tellDaysinMonth(i, days_in_month);
		for (; j < days_in_month; j++)
		{
			if (calendar[i][j] != nullptr)
			{
				for (int k = 0; k < 24; k++)
				{
					if (calendar[i][j][k] != nullptr)
					{
						for (int l = 0; calendar[i][j][k][l] != nullptr; l++)
						{
							if (calendar[i][j][k][l]->userId == userId)
							{	
								if (count < capacity)
									data[count].month=i,data[count].day=j, data[count].hour=k, data[count].activityNo=l;
								else
								{
									timeOfactivity* temp = new timeOfactivity[capacity * 2];
									for (int m = 0; m < capacity; m++)
										temp[m] = data[m];
									delete[]data;
									data = temp;
									capacity *= 2;
									data[count].month = i, data[count].day = j, data[count].hour = k, data[count].activityNo = l;
								}
								count++;
							}
						}
					}
				}
			}
			if (i == allActivities.end_month - 1 && j == allActivities.end_date)
				break;
		}
		j = 0;
	}
	return data;
}

void removeActivitiesOfUser(timePeriod &removeActivities, int &userId,bool removeImportantActivities, activity *****calendar)
{
	int count = 0,capacity=0;
	timeOfactivity* allActivities = all_activities_of_user(removeActivities, userId, count, calendar);
	
	if (!removeImportantActivities)
	{
		float *priorityList = new float[count];
		int *indexes = new int[count];
		for (int i = 0; i < count; i++)
		{
			priorityList[i] = calendar[allActivities[i].month][allActivities[i].day][allActivities[i].hour][allActivities[i].activityNo]->priority;
			indexes[i] = i;
		}
		float  min;
		int index;
		for (int i = count; i > 0; i--)					//selection sort (in descending order)
		{
			min = priorityList[0], index = 0;

			for (int j = 1; j < i; j++)
			{
				if (priorityList[j] < min)
				{
					index = j;
					min = priorityList[j];
				}
			}
			swap(priorityList[index], priorityList[i - 1]);
			swap(indexes[index], indexes[i - 1]);
		}
		delete[] priorityList;
		for (int i = 5; i<count; i++)
		{
			if (allActivities[indexes[i]].month >= removeActivities.start_month - 1 && allActivities[indexes[i]].day >= removeActivities.start_date - 1)
			{
				capacity = 0;
				while (calendar[allActivities[indexes[i]].month][allActivities[indexes[i]].day][allActivities[indexes[i]].hour][capacity] != nullptr)
					capacity++;

				for (int j = allActivities[indexes[i]].activityNo; j < capacity; j++)
				{
					calendar[allActivities[indexes[i]].month][allActivities[indexes[i]].day][allActivities[indexes[i]].hour][j] = calendar[allActivities[indexes[i]].month][allActivities[indexes[i]].day][allActivities[indexes[i]].hour][j + 1];
				}
				delete calendar[allActivities[indexes[i]].month][allActivities[indexes[i]].day][allActivities[indexes[i]].hour][capacity];
				calendar[allActivities[indexes[i]].month][allActivities[indexes[i]].day][allActivities[indexes[i]].hour][capacity] = nullptr;
			}
			if (allActivities[indexes[i]].month == removeActivities.end_month - 1 && allActivities[indexes[i]].day == removeActivities.end_date - 1)
				break;
		}
		delete[] indexes;
		
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			if (allActivities[i].month >= removeActivities.start_month - 1 && allActivities[i].day >= removeActivities.start_date - 1)
			{
				capacity = 0;
				while (calendar[allActivities[i].month][allActivities[i].day][allActivities[i].hour][capacity] != nullptr)
					capacity++;

				for (int j = allActivities[i].activityNo; j < capacity; j++)
				{
					calendar[allActivities[i].month][allActivities[i].day][allActivities[i].hour][j] = calendar[allActivities[i].month][allActivities[i].day][allActivities[i].hour][j + 1];
				}
				delete calendar[allActivities[i].month][allActivities[i].day][allActivities[i].hour][capacity];
				calendar[allActivities[i].month][allActivities[i].day][allActivities[i].hour][capacity] = nullptr;
			}
			if (allActivities[i].month == removeActivities.end_month - 1 && allActivities[i].day == removeActivities.end_date - 1)
				break;
		}
	}
	cout << endl << "Calendar updated successfully." << endl;
}

int printMenu()
{
	cout << " ---------------THE ULTIMATE CALENDAR 2016------------------" << endl << endl;
	cout << "----------------Menu---------------" << endl << endl;
	cout << "You can Perform the following actions." << endl << endl
		<< "Querying Functionalities" << endl << endl
		<< "1. List all activities of a user in a given time period." << endl
		<< "2. List 5 most important activities of a  user in a given time period." << endl
		<< "3. For a given user, print the longest free period: that is, the longest consecutive number of days in which the user has no activity." << endl
		<< "4. List all the clashing activities of a pair of users, during a time period." << endl
		<< "5. For a list of users, and a time period, list all hourly slots that are free for all these users in this time period." << endl
		<< "6. List the five most busy users in a given time period." << endl
		<< "7. Print activity stats for a given month." << endl
		<< "8. Print calendar stats for the whole year." << endl << endl

		<< "Update Functionalities" << endl << endl
		<< "9. Remove all activities of a given user during a given time period." << endl
		<< "10. Remove a user from the calendar" << endl
		<< "11. Remove all except the top five most important activities of a user during a given time period." << endl
		<< "12. Save the calendar " << endl << endl
		<< "13. Print the calendar" << endl << endl
		<< "14. About me." << endl;
		
	cout << endl << "Please enter a choice. Enter -1 to quit." << endl;
	int choice;
	cin >> choice;
	while ((choice < 1 || choice > 14) && choice!=-1)
	{
		cout << "Please enter a valid choice" << endl;
		cin >> choice;
	}

	return choice;
}

int main()
{
	activity *****calendar = new activity****[12];
	int days = 0;
	for (int i = 0; i < 12; i++)
	{
		tellDaysinMonth(i, days);
		calendar[i] = new activity***[days]();
	}
	ifstream fin("calendar1.dat");
	char slash;
	int day, month, start_time, end_time, userid, cap = 1;
	char title[50];
	float priority;
	int count = 0;

	while(true)
	{
		fin >> day;

		if (fin.eof())
			break; 

		fin>> slash >> month >> slash >> start_time >> slash >> end_time;
		fin.ignore(5, 'r');
		fin >> userid >> slash;
		fin.ignore(8, ',');
		fin.getline(title, 50, ',');
		fin >> priority;

		if (calendar[month - 1][day - 1] == nullptr)
			calendar[month - 1][day - 1] = new activity**[24]();

		if (calendar[month - 1][day - 1][start_time] == nullptr)
		{
			calendar[month - 1][day - 1][start_time] = new activity *[2];
			calendar[month - 1][day - 1][start_time][1] = nullptr;
		}
		else
			calendar[month - 1][day - 1][start_time] = createBiggerArray(calendar[month - 1][day - 1][start_time], cap);

		calendar[month - 1][day - 1][start_time][cap-1] = new activity;

		calendar[month - 1][day - 1][start_time][cap-1]->duration = (end_time - start_time);
		calendar[month - 1][day - 1][start_time][cap-1]->priority = priority;
		strcpy_s(calendar[month - 1][day - 1][start_time][cap-1]->title, title);
		calendar[month - 1][day - 1][start_time][cap-1]->userId = userid;
		
		cap = 1;
	
	}
	
	int choice=0;
	while (choice!=-1)
	{
		choice = printMenu();

		system("cls");
		if (choice == 1 || choice == 2)
		{
			cout << "Enter userId, start month,start date, end month and end date one by one." << endl;
			timePeriod allActivities;
			int  user__Id, count;
			cin >> user__Id >> allActivities.start_month >> allActivities.start_date >> allActivities.end_month >> allActivities.end_date;
			check_wrong_date(allActivities);
			timeOfactivity* Data = all_activities_of_user(allActivities, user__Id, count, calendar);
			if (count == 0)
			{
				cout << "No activities of the user in the given time period." << endl;
				choice = 0;
			}

			if (choice == 1)
			{
				cout << "Day/Month, Time, Duration, Title, Priority" << endl << endl << endl;
				activity temp;
				for (int i = 0; i < count; i++)
				{
					temp = *calendar[Data[i].month][Data[i].day][Data[i].hour][Data[i].activityNo];
					cout << Data[i].month + 1 << "/" << Data[i].day + 1 << "," << Data[i].hour << "-" << Data[i].hour + temp.duration << "," << temp.title << "," << temp.priority << endl;
				}
			}
			else
			{
				float *priorityList = new float[count];
				int *indexes = new int[count];
				for (int i = 0; i < count; i++)
				{
					priorityList[i] = calendar[Data[i].month][Data[i].day][Data[i].hour][Data[i].activityNo]->priority;
					indexes[i] = i;
				}
				float  min;
				int index;
				for (int i = count; i > 0; i--)					//selection sort (in descending order)
				{
					min = priorityList[0], index = 0;

					for (int j = 1; j < i; j++)
					{
						if (priorityList[j] < min)
						{
							index = j;
							min = priorityList[j];
						}
					}
					swap(priorityList[index], priorityList[i - 1]);
					swap(indexes[index], indexes[i - 1]);
				}
				activity temporary;
				for (int i = 0; i < count && i < 5; i++)
				{
					temporary = *calendar[Data[indexes[i]].month][Data[indexes[i]].day][Data[indexes[i]].hour][Data[indexes[i]].activityNo];
					cout << Data[i].month + 1 << "/" << Data[i].day + 1 << "," << Data[i].hour << "-" << Data[i].hour + temporary.duration << "," << temporary.title << "," << temporary.priority << endl;
				}
				delete[] priorityList;
				delete[]indexes;
			}
		}
		else if (choice == 3)
		{
			cout << "Enter the user ID" << endl;
			cin >> userid;
			timePeriod free = freeTime(userid, calendar);
			cout << "The Longest Time for which User " << userid << " is free is: " << free.numberOfDays << " days i.e. from " << free.start_date+1
				<< "/" << free.start_month+1 << " to " << free.end_date+1 << "/" << free.end_month+1 << "." << endl;
		}
		else if (choice == 4)
		{
			timePeriod clashingActivity;
			clashingActivity.numberOfDays = 0;

			cout << "Enter start date,start month,end date and end month." << endl;
			cin >> clashingActivity.start_date >> clashingActivity.start_month >> clashingActivity.end_date >> clashingActivity.end_month;
			check_wrong_date(clashingActivity);

			userid = 0;
			int user2id = 0;

			cout << "Enter two userIds one after the other." << endl;
			cin >> userid >> user2id;

			clashingActivities(userid, user2id, calendar, clashingActivity);
		}
		else if (choice == 5)
		{
			timePeriod allFree;
			allFree.numberOfDays = 0;

			cout << "Enter start date,start month,end date and end month." << endl;
			cin >> allFree.start_date >> allFree.start_month >> allFree.end_date >> allFree.end_month;
			check_wrong_date(allFree);

			int  capacity = 3, count;
			int *userIds = new int[capacity];
			cout << endl << "Enter the userIds one at a time. To end, enter -1." << endl;
			userid = 0;
			cin >> userid;
			for (count = 0; userid != -1; count++)
			{
				if (count >= capacity)
					userIds = changeByk(userIds, capacity, 4);

				userIds[count] = userid;
				count++;
				cin >> userid;
			}

			if (count + 1 != capacity)
				userIds = changeByk(userIds, capacity, count - capacity + 1);

			all_Free_At_a_Time(calendar, userIds, capacity, allFree);

			delete[]userIds;
		}
		else if (choice == 6)
		{
			cout << "Enter start date, start month, end date and end month." << endl;
			timePeriod busyTime;
			busyTime.numberOfDays = 0;

			cin >> busyTime.start_date >> busyTime.start_month >> busyTime.end_date >> busyTime.end_month;
			check_wrong_date(busyTime);
			int temp;
			busiestUsers(busyTime, calendar, temp, 1);

		}
		else if (choice == 7)
		{
			cout << "Enter a month." << endl;
			timePeriod stats;
			stats.numberOfDays = 0;

			cin >> stats.start_month;
			stats.start_date = 1, stats.end_month = stats.start_month;
			tellDaysinMonth(stats.start_month, stats.end_date);
			check_wrong_date(stats);
			allStats(stats, calendar, 1);
		}
		else if (choice == 8)
		{
			timePeriod stats;
			stats.numberOfDays = 0;
			stats.start_month = 1, stats.end_month = 12, stats.start_date = 1, stats.end_date = 31;
			allStats(stats, calendar, 0);
		}
		else if (choice == 9)
		{
			timePeriod removeActivities;
			cout << "Enter start month, start date, end month and end date and userId." << endl;
			cin >> removeActivities.start_month >> removeActivities.start_date >> removeActivities.end_month >> removeActivities.end_date >> userid;
			check_wrong_date(removeActivities);
			removeActivitiesOfUser(removeActivities, userid,1, calendar);
		}
		else if (choice==10)
		{
			timePeriod removeActivities;
			cout << "Enter userId" << endl;
			cin >> userid;
			removeActivities.start_month = 1, removeActivities.start_date = 1, removeActivities.end_month = 12, removeActivities.end_date = 31;
			removeActivitiesOfUser(removeActivities, userid, 1,calendar);
		}
		else if (choice==11)
		{
			timePeriod removeActivities;
			cout << "Enter start month, start date, end month and end date and userId." << endl;
			cin >> removeActivities.start_month >> removeActivities.start_date >> removeActivities.end_month >> removeActivities.end_date >> userid;
			check_wrong_date(removeActivities);
			removeActivitiesOfUser(removeActivities, userid,0, calendar);
		}
		else if (choice==12)
		{
			int counter = 0;
			char fileName[50];
			cout << "Enter filename" << endl;
			cin >> fileName;
			ofstream fout(fileName);
			for (int i = 0; i < 12; i++)
			{
				tellDaysinMonth(i, days);
				for (int j = 0; j < days; j++)
				{
					if (calendar[i][j] != nullptr)
					{
						for (int k = 0; k < 24; k++)
						{
							if (calendar[i][j][k] != nullptr)
							{
								for (int l = 0; calendar[i][j][k][l] != nullptr; l++)
								{
									fout << j + 1 << "/" << i + 1 << "," << k << "," << calendar[i][j][k][l]->duration + k << ",user" << calendar[i][j][k][l]->userId << ",act" << counter << "," << calendar[i][j][k][l]->title << "," << calendar[i][j][k][l]->priority << endl;
									counter++;
								}
							}
						}
					}
				}
			}
			cout << endl << "File successfully saved" << endl;
		}
		else if (choice==13)
		{
			
			cout << setw(15) <<"January" << setw(30) << "February" << setw(30) << "March" << setw(30) << "April"<<endl;
			gotoxy(0, 12);
			cout << setw(15) << "May" << setw(30) << "June" << setw(30) << "July" << setw(30) << "August" << endl;
			gotoxy(0, 25);
			cout << setw(15) << "September" << setw(30) << "October" << setw(30) << "November" << setw(30) << "December" << endl;
			gotoxy(0, 1);
			for (int i = 0; i < 3; i++)
			{
				cout << "   M   T   W   T   F   S   S"<<setw(30) << "    M   T   W   T   F   S   S" << setw(30) << "    M   T   W   T   F   S   S"<<setw(30) << "    M   T   W   T   F   S   S"<<endl;
				gotoxy(0, 13 * (i+1));
			}
			for (int i = 0; i < 12; i++)
			{
				int j,height;
				if (i == 0 || i == 3 || i == 6)
					j = 6;
				else if (i == 1 || i == 7)
					j = 2;
				else if (i == 2 || i == 10)
					j = 3;
				else if (i == 4)
					j = 1;
				else if (i == 5)
					j = 4;
				else if (i == 8 || i == 11)
					j = 5;
				else if (i == 9)
					j = 7;

				if (i < 4)
					height = 2;
				else if (i < 8)
					height = 15;
				else
					height = 27;
				gotoxy((i%4)*30, height);

				cout << setw(j*4);
				int days_in_month,day=1,line=0;
				tellDaysinMonth(i, days_in_month);
				for (; day <= days_in_month; day++,j++)
				{
					if (calendar[i][day-1]!=nullptr)
					{
						HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
						SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
						cout << day << setw(4);
						SetConsoleTextAttribute(hConsole,FOREGROUND_RED |FOREGROUND_GREEN |	FOREGROUND_BLUE |FOREGROUND_INTENSITY);
					}
					else
						cout << day<<setw(4);
					if (j %7 == 0)
					{
						cout << endl;
						j = 0;
						line++;
						gotoxy((i%4) * 30, line+height);
					}
				}
				line = 0;
			}
			_getch();
			system("cls");
		}
		else if (choice == 14)
		{
			cout << "Made by Abdul Hameed. Stay Updated for the next version. ;)";
		}
		cout << endl << "Enter any number to return to main menu. -1 to quit." << endl;
		cin >> choice;
		system("cls");
	}
	for (int i = 0; i < 12; i++)
	{
		tellDaysinMonth(i, days);
		for (int j = 0; j < days; j++)
		{
			if (calendar[i][j] != nullptr)
			{
				for (int k = 0; k < 24; k++)
				{
					if (calendar[i][j][k] != nullptr)
					{
						for (int l = 0; calendar[i][j][k][l] != nullptr; l++)
						{
							delete calendar[i][j][k][l];
						}
						delete calendar[i][j][k];
					}
				}
				delete calendar[i][j];
			}
		}
		delete calendar[i];
	}
	delete calendar;
	calendar = nullptr;

	return 0;
}