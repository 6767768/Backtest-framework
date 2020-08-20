#ifndef CUSTOMIZEDDATATYPE_HPP
#define CUSTOMIZEDDATATYPE_HPP

#include<iostream>
#include<string>
#include<exception>
#include<memory>
#include<map>
#include<iterator>
#include<algorithm>
#include<cstring>
#include<fstream>
#include<sstream>
#include<typeinfo>
#include<time.h>

using std::cout;
using std::endl;
using std::cerr;

constexpr auto COUNTER = 10; /*Number of fields for each line*/
constexpr auto  CHARACTER = 11; /*Length of field for each character*/
constexpr auto  TICKER_LENGTH = 11; /*Length of ticker name*/
constexpr auto TICKER_NUMBER = 11; /*Number of ticker*/
constexpr auto ITERATION = 1000; 
constexpr auto PATH_LENGTH = 20; /*Path of the file*/
constexpr auto TYPE_LENGTH = 11;
constexpr auto TIME_LENGTH = 21; /*Length of time*/
constexpr auto DIRECTION_LENGTH = 11;
constexpr auto EXCHANGE_LENGTH = 11;/*Length of the exchange*/

typedef char ticker_type;
typedef char* key_type;
typedef double value_type;
typedef char *T;

typedef double cash_type;
typedef int position_type;
typedef int trade_type;
typedef double commission_type;

void MyStrcpy(char *Destination, const char *Source_begin, const char *Source_end) {

	if ((Source_end - Source_begin) == 1) {
		*Destination = '0';
		Destination++;
	}

	for (const char *p = Source_begin; p != Source_end; p++) {
		*Destination = *p;
		Destination++;
	}
	*Destination = '\0';
}

void Accretion(char *newObj, const char *Obj, int Buffer) {
	int temp = atoi(Obj) + 1;
	if (temp < 10) { sprintf_s(newObj, Buffer, "0%d", temp); }
	else { sprintf_s(newObj, Buffer, "%d", temp); }
}

void Accretion(char *newObj, char *Obj, int Buffer) {
	int temp = atoi(Obj) + 1;
	if (temp < 10) { sprintf_s(newObj, Buffer, "0%d", temp); }
	else { sprintf_s(newObj, Buffer, "%d", temp); }
}

void SystemTime(char *systemtime, int len) {
	struct tm t;
	time_t now;
	time(&now);
	localtime_s(&t, &now);
	strftime(systemtime, static_cast<size_t>(len), "%Y-%m-%d %H:%M:%S", &t);
}

#endif // !CUSTOMIZEDDATATYPE_HPP




