//#ifndef CENTER_HPP
//#define CENTER_HPP
//
//#include<fstream>
//#include<sstream>
//#include<map>
//#include<time.h>
//#include<vector>
//#include<iterator>
//#include<algorithm>
//#include<cstring>
//#include "Time.hpp"
//#include "ThostFtdcMdApi.h"
//
//typedef DateTime time_type;
//struct Tableau {
//	typedef std::map<time_type, std::map<key_type, value_type>, TimeCompare> table_type;
//	ticker_type ticker[TICKER_LENGTH];
//	table_type table;
//	char key_table[COUNTER][CHARACTER];
//	int Count;
//};
//
//class Center {
//public:
//	typedef typename Tableau::table_type table_type;
//protected:
//	std::unique_ptr<Tableau> ptr;
//public:
//	Center() {};
//	~Center() {};
//	void Print();
//	int Size();
//	Tableau *Return_table();
//};
//
//inline Tableau *Center::Return_table() {
//	return ptr.get();
//}
//
//int Center::Size() {
//	return static_cast<int>(ptr->table.size());
//}
//
//void Center::Print() {
//	cout << "Ticker: " << ptr->ticker << endl;
//	for (int i = 0; i < ptr->Count; i++) {
//		cout << ptr->key_table[i] << " ";
//	}
//	cout << endl;
//	cout << "--------------------------------------------------------------------------------------------" << endl;
//	for (table_type::iterator TablePos = ptr->table.begin(); TablePos != ptr->table.end(); TablePos++) {
//		TablePos->first.Print();
//		int count = 0;
//		for (int i = 0; i < ptr->Count; i++) {
//			cout << " " << TablePos->second[ptr->key_table[i]];
//		}
//		cout << endl;
//	}
//};
//
///*Local*/
//
//class Local :public Center {
//public:
//	Local() {};
//	Local(char *File, const char *Type = "%Y - %M - %D %H:%M : %S") {
//
//		IO(File, Type);
//		if (ptr == nullptr) {
//			cout << "Failure occurred when reading file." << endl;
//			exit(1);
//		}
//	}
//	~Local() {};
//	bool IO(char *File, const char *Type);
//};
//
//
//inline bool Local::IO(char *File, const char *Type) {
//	clock_t begin, end;
//	begin = clock();
//
//	auto tableau = std::make_unique<Tableau>();
//	/*Extract ticker*/
//	char  *p = File, *q;
//	for (p = File; *p != '\0'; p++) {}
//	for (; *p != '.'; p--) {}
//	for (q = p; *q != '/'; q--) {}
//	MyStrcpy(tableau->ticker, q + 1, p);
//	
//	/*Extract table*/
//	std::ifstream in(File);
//	char cell[COUNTER*CHARACTER];
//	int count = 0;
//	time_type time;
//	char temp[CHARACTER];
//
//	if (in.is_open()) {
//		bool First = true;
//		in.getline(cell, COUNTER*CHARACTER);
//		p = cell, q = cell;
//		while (true) {
//			if (*p == '\0') { MyStrcpy(tableau->key_table[count++], q + 1, p); break; }
//			if (*p == ',') {
//				if (First) { MyStrcpy(tableau->key_table[count++], q, p); First = false; }
//				else { MyStrcpy(tableau->key_table[count++], q + 1, p); }
//				q = p;
//			}
//			p++;
//		}
//		tableau->Count = count - 1;
//		while (!in.eof()) {
//			First = true;
//			in.getline(cell, COUNTER*CHARACTER);
//			if (strcmp(cell, "") == 0) { break; }/*filter null line*/
//			p = cell, q = cell;
//			count = 0;
//
//			while (true) {
//				if (*p == '\0') {
//					MyStrcpy(temp, q + 1, p);
//					tableau->table[time][tableau->key_table[count++]] = atof(temp);
//					break;
//				}
//				if (*p == ',') {
//					if (First) {
//						MyStrcpy(temp, q, p);
//						if (strcmp(Type, "%Y-%M-%D %H:%M:%S") == 0) {
//							time.Time_YMDHMS(temp);
//						}
//						else if (strcmp(Type, "%M-%D-%Y %H:%M:%S") == 0) {
//							time.Time_MDYHMS(temp);
//						}
//						else if (strcmp(Type, "%D-%M-%Y %H:%M:%S") == 0) {
//							time.Time_DMYHMS(temp);
//						}
//						else {
//							time.Time_YMDHMS(temp);
//						}
//						First = false;
//					}
//					else {
//						MyStrcpy(temp, q + 1, p);
//						tableau->table[time][tableau->key_table[count++]] = atof(temp);
//					}
//					q = p;
//				}
//				p++;
//			}
//		}
//	}
//	ptr = std::move(tableau);
//
//	end = clock();
//	cout << "Total I/O time: " << (double)(end - begin) / CLOCKS_PER_SEC << endl;
//
//	return true;
//};
//
//
//
//#endif // !CENTER_HPP
