/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright © 2020 Xiwei Wang 
 * Created by Xiwei Wang on 07/26/2020
 * All rights reserved.                                               
 *                                               
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef TIME_HPP
#define TIME_HPP

#include "CustomizedDataType.hpp"

class MyException : public std::exception {
	const char *what() const throw() {
		return "Year is Nan.";
	}
};

struct Time {
	char Year[5], Month[3], Day[3], Hour[3], Minute[3], Second[3];
};

class DateTime {
public:
	typedef Time time_type;
protected:
	std::unique_ptr<time_type> ptr;
public:
	DateTime() {};
	DateTime(const char *Datetime, const char *Type);
	DateTime(const DateTime &TimeInstanceClass);
	~DateTime() {};
	bool operator<(const DateTime& TimeInstanceClass) const;
	bool operator>(const DateTime& TimeInstanceClass) const;
	bool operator==(const DateTime& TimeInstanceClass) const;
	bool Assignment(T year, T month, T day, T hour, T minute, T second);
	void Print() const;
	void Time_delta_second();
	void Time_delta_minute();
	void Time_delta_hour();
	void Time_delta_day();
	void Time_delta_month();
	void Time_delta_year();
	bool Time_YMDHMS(const char * str);
	bool Time_MDYHMS(const char * str);
	bool Time_DMYHMS(const char * str);
	void TimeToChar(char *p) const;
};

DateTime::DateTime(const char *Datetime, const char *Type = "%Y-%M-%D %H:%M:%S") {
	if (strcmp(Type, "%Y-%M-%D %H:%M:%S") == 0) {
		Time_YMDHMS(Datetime);
	}
	else if (strcmp(Type, "%M-%D-%Y %H:%M:%S") == 0) {
		Time_MDYHMS(Datetime);
	}
	else if (strcmp(Type, "%D-%M-%Y %H:%M:%S") == 0) {
		Time_DMYHMS(Datetime);
	}
	else {

	}
}

/*
Time format: "2006/01/01 04:19:67"
*/
inline bool DateTime::Time_YMDHMS(const char * str) {
	char year[5] = "0000", month[3] = "00", day[3] = "00", hour[3] = "99", minute[3] = "99", second[3] = "99";
	const char *p = str;
	bool First = true;
	while (true) {
		if (*p == '/' || *p == '-' || *p == ':' || *p == ' ' || *p == '\0') {
			if (strcmp(year, "0000") == 0) {
				if (First) { First = false; MyStrcpy(year, str, p); }
				else { MyStrcpy(year, str + 1, p); }
				str = p;
			}
			else if (strcmp(month, "00") == 0) {
				if (First) { First = false; MyStrcpy(month, str, p); }
				else { MyStrcpy(month, str + 1, p); }
				str = p;
			}
			else if (strcmp(day, "00") == 0) {
				if (First) { First = false; MyStrcpy(day, str, p); }
				else { MyStrcpy(day, str + 1, p); }
				str = p;
			}
			else if (strcmp(hour, "99") == 0) {
				MyStrcpy(hour, str + 1, p);
				str = p;
			}
			else if (strcmp(minute, "99") == 0) {
				MyStrcpy(minute, str + 1, p);
				str = p;
			}
			else if (strcmp(second, "99") == 0) {
				MyStrcpy(second, str + 1, p);
				str = p;
			}
		}
		if (*p == '\0') { break; }
		p++;
	}
	Assignment(year, month, day, hour, minute, second);

	return true;
}

/*
Time format: "01/01/2006 04:19:67"
*/
inline bool DateTime::Time_MDYHMS(const char * str) {
	char year[5] = "0000", month[3] = "00", day[3] = "00", hour[3] = "99", minute[3] = "99", second[3] = "99";
	const char *p = str;
	bool First = true;
	while (true) {
		if (*p == '/' || *p == '-' || *p == ':' || *p == ' ' || *p == '\0') {
			if (strcmp(month, "00") == 0) {
				if (First) { First = false; MyStrcpy(month, str, p); }
				else { MyStrcpy(month, str + 1, p); }
				str = p;
			}
			else if (strcmp(day, "00") == 0) {
				if (First) { First = false; MyStrcpy(day, str, p); }
				else { MyStrcpy(day, str + 1, p); }
				str = p;
			}
			else if (strcmp(year, "0000") == 0) {
				if (First) { First = false; MyStrcpy(year, str, p); }
				else { MyStrcpy(year, str + 1, p); }
				str = p;
			}
			else if (strcmp(hour, "99") == 0) {
				MyStrcpy(hour, str + 1, p);
				str = p;
			}
			else if (strcmp(minute, "99") == 0) {
				MyStrcpy(minute, str + 1, p);
				str = p;
			}
			else if (strcmp(second, "99") == 0) {
				MyStrcpy(second, str + 1, p);
				str = p;
			}
		}
		if (*p == '\0') { break; }
		p++;
	}
	Assignment(year, month, day, hour, minute, second);

	return true;
}

/*
Time format: "26/12/2006 04:19:67"
*/
inline bool DateTime::Time_DMYHMS(const char * str) {
	char year[5] = "0000", month[3] = "00", day[3] = "00", hour[3] = "99", minute[3] = "99", second[3] = "99";
	const char *p = str;
	bool First = true;
	while (true) {
		if (*p == '/' || *p == '-' || *p == ':' || *p == ' ' || *p == '\0') {
			if (strcmp(day, "00") == 0) {
				if (First) { First = false; MyStrcpy(day, str, p); }
				else { MyStrcpy(day, str + 1, p); }
				str = p;
			}
			else if (strcmp(month, "00") == 0) {
				if (First) { First = false; MyStrcpy(month, str, p); }
				else { MyStrcpy(month, str + 1, p); }
				str = p;
			}
			else if (strcmp(year, "0000") == 0) {
				if (First) { First = false; MyStrcpy(year, str, p); }
				else { MyStrcpy(year, str + 1, p); }
				str = p;
			}
			else if (strcmp(hour, "99") == 0) {
				MyStrcpy(hour, str + 1, p);
				str = p;
			}
			else if (strcmp(minute, "99") == 0) {
				MyStrcpy(minute, str + 1, p);
				str = p;
			}
			else if (strcmp(second, "99") == 0) {
				MyStrcpy(second, str + 1, p);
				str = p;
			}
		}
		if (*p == '\0') { break; }
		p++;
	}
	Assignment(year, month, day, hour, minute, second);

	return true;
}

DateTime::DateTime(const DateTime &TimeInstanceClass) {

	bool Sign = Assignment(TimeInstanceClass.ptr->Year, TimeInstanceClass.ptr->Month, TimeInstanceClass.ptr->Day,
		TimeInstanceClass.ptr->Hour, TimeInstanceClass.ptr->Minute, TimeInstanceClass.ptr->Second);

	if (!Sign) {
		cout << "Memory distribution failure." << endl;
		exit(1);
	}
}

/*year += 1*/

void DateTime::Time_delta_year() {
	Accretion(this->ptr->Year, this->ptr->Year, 5);
}
//
///*month += 1*/
//
void DateTime::Time_delta_month() {

	if (strcmp(ptr->Month, "12") == 0) {
		Accretion(this->ptr->Month, "0", 3);
		Time_delta_year();
	}
	else {
		Accretion(this->ptr->Month, this->ptr->Month, 3);
	}
}
//
///*day += 1*/
//
void DateTime::Time_delta_day() {
	if (strcmp(ptr->Month, "01") == 0
		|| strcmp(ptr->Month, "03") == 0
		|| strcmp(ptr->Month, "05") == 0
		|| strcmp(ptr->Month, "07") == 0
		|| strcmp(ptr->Month, "08") == 0
		|| strcmp(ptr->Month, "10") == 0
		|| strcmp(ptr->Month, "12") == 0)
	{
		if (strcmp(ptr->Day, "31") == 0) {
			Accretion(ptr->Day, "0", 3);
			Time_delta_month();
		}
		else {
			Accretion(ptr->Day, ptr->Day, 3);
		}
	}
	else if (strcmp(ptr->Month, "02") == 0 && atoi(ptr->Year) % 4 == 0) {
		if (strcmp(ptr->Day, "29") == 0) {
			Accretion(ptr->Day, "00", 3);
			Time_delta_month();
		}
		else {
			Accretion(ptr->Day, ptr->Day, 3);
		}
	}
	else if (strcmp(ptr->Month, "02") == 0 && atoi(ptr->Year) % 4 != 0) {
		if (strcmp(ptr->Day, "28") == 0) {
			Accretion(ptr->Day, "00", 3);
			Time_delta_month();
		}
		else {
			Accretion(ptr->Day, ptr->Day, 3);
		}
	}
	else {
		if (strcmp(ptr->Day, "30")) {
			Accretion(ptr->Day, "00", 3);
			Time_delta_month();
		}
		else {
			Accretion(ptr->Day, ptr->Day, 3);
		}
	}
}
//
///*hour += 1*/
//
void DateTime::Time_delta_hour() {
	if (strcmp(ptr->Hour, "23") == 0) {
		Accretion(ptr->Hour, "-1", 3);
		Time_delta_day();
	}
	else {
		Accretion(ptr->Hour, ptr->Hour, 3);
	}
}
//
///*minute += 1*/

void DateTime::Time_delta_minute() {
	if (strcmp(ptr->Minute, "59") == 0) {
		Accretion(ptr->Minute, "-1", 3);
		Time_delta_hour();
	}
	else {
		Accretion(ptr->Minute, ptr->Minute, 3);
	}
}

///*second += 1*/
void DateTime::Time_delta_second() {
	if (strcmp(ptr->Second, "59") == 0) {
		Accretion(ptr->Second, "-1", 3);
		Time_delta_minute();
	}
	else {
		Accretion(ptr->Second, ptr->Second, 3);
	}
}

inline bool DateTime::operator>(const DateTime& TimeInstanceClass) const {
	if (strcmp(this->ptr->Year, TimeInstanceClass.ptr->Year) == 0) {
		if (strcmp(this->ptr->Month, TimeInstanceClass.ptr->Month) == 0) {

			if (strcmp(this->ptr->Day, TimeInstanceClass.ptr->Day) == 0) {

				if (strcmp(this->ptr->Hour, TimeInstanceClass.ptr->Hour) == 0) {

					if (strcmp(this->ptr->Minute, TimeInstanceClass.ptr->Minute) == 0) {

						if (strcmp(this->ptr->Second, TimeInstanceClass.ptr->Second) == 0) {
							return false;/*Must follow strict weak ordering. */
						}
						else if (strcmp(this->ptr->Second, TimeInstanceClass.ptr->Second) > 0) {
							return true;
						}
						else {
							return false;
						}
					}
					else if (strcmp(this->ptr->Minute, TimeInstanceClass.ptr->Minute) > 0) {
						return true;
					}
					else {
						return false;
					}
				}
				else if (strcmp(this->ptr->Hour, TimeInstanceClass.ptr->Hour) > 0) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (strcmp(this->ptr->Day, TimeInstanceClass.ptr->Day) > 0) {
				return true;
			}
			else {
				return false;
			}
		}
		else if (strcmp(this->ptr->Month, TimeInstanceClass.ptr->Month) > 0) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (strcmp(this->ptr->Year, TimeInstanceClass.ptr->Year) > 0) {
		return true;
	}
	else {
		return false;
	}

}

inline bool DateTime::operator<(const DateTime& TimeInstanceClass) const {
	return (TimeInstanceClass > *this);
}

inline bool DateTime::operator==(const DateTime& TimeInstanceClass) const {
	return (strcmp(this->ptr->Year, TimeInstanceClass.ptr->Year) == 0) && (strcmp(this->ptr->Month, TimeInstanceClass.ptr->Month) == 0) &&
		(strcmp(this->ptr->Day, TimeInstanceClass.ptr->Day) == 0) && (strcmp(this->ptr->Hour, TimeInstanceClass.ptr->Hour) == 0) &&
		(strcmp(this->ptr->Minute, TimeInstanceClass.ptr->Minute) == 0) && (strcmp(this->ptr->Second, TimeInstanceClass.ptr->Second) == 0);
}

void DateTime::Print() const{
	cout << ptr->Year << "-" << ptr->Month << "-" << ptr->Day << " " <<
		ptr->Hour << ":" << ptr->Minute << ":" << ptr->Second << " ";
}

bool DateTime::Assignment(T year, T month, T day, T hour, T minute, T second) {

	auto time = std::make_unique<time_type>();

	strcpy_s(time->Year, 5, year);
	strcpy_s(time->Month, 3, month);
	strcpy_s(time->Day, 3, day);
	strcpy_s(time->Hour, 3, hour);
	strcpy_s(time->Minute, 3, minute);
	strcpy_s(time->Second, 3, second);

	ptr = std::move(time);

	return true;
}

void DateTime::TimeToChar(char *p)const {

	if (strcmp(ptr->Month, "00") == 0) {
		sprintf_s(p, 21, "%s", ptr->Year);
	}
	else if (strcmp(ptr->Day, "00") == 0){
		sprintf_s(p, 21, "%s-%s", ptr->Year, ptr->Month);
	}
	else if (strcmp(ptr->Hour, "99") == 0) {
		sprintf_s(p, 21, "%s-%s-%s", ptr->Year, ptr->Month, ptr->Day);
	}
	else {
		sprintf_s(p, 21, "%s-%s-%s %s:%s:%s", ptr->Year, ptr->Month, ptr->Day, ptr->Hour, ptr->Minute, ptr->Second);
	}
}

/*Functors defined for table_type*/
struct TimeCompare {
	bool operator()(const DateTime &time1, const DateTime &time2) const {
		return (time1 < time2);
	}
};

#endif // !TIME_HPP
