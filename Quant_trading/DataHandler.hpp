/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright © 2020 Xiwei Wang 
 * Created by Xiwei Wang on 07/26/2020
 * All rights reserved.                                               
 *                                               
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP

#include "Event.hpp"

typedef DateTime time_type;
class DataHandler {
public:
	/*Return last N bars from the lastest_symbol list*/
	virtual table_type *get_latest_bars(char *symbol, int N = 1) = 0;
	/*Push the lastest bar to the lastest symbol structure for all symbols in the sumbol list.*/
	virtual void update_bars() = 0;
	virtual char* FindClose() = 0;
	virtual char* FindVolume() = 0;
};

class HistoricCSVDataHandler :public DataHandler {
protected:
	std::shared_ptr<std::map<char *, int> > symbol_list;
	std::shared_ptr<ThreadQueue<Event *> > ptr_event;
	std::unique_ptr<symbol_type> symbol_data;
	std::unique_ptr<symbol_type> latest_symbol_data;
	table_type::iterator Iterator[TICKER_NUMBER];
	table_type::iterator rIterator[TICKER_NUMBER];
	std::unique_ptr<table_type> ptr_temp;
	char field[COUNTER][CHARACTER];
	bool continue_backtest;
	MarketEvent market;
public:
	HistoricCSVDataHandler() {};
	HistoricCSVDataHandler(std::shared_ptr<ThreadQueue<Event *> > ptr_queue, std::shared_ptr<std::map<char *, int> > symbol_list, const char *start_date, const char *end_date, const char *path);
	~HistoricCSVDataHandler() {};
	table_type *get_latest_bars(char *symbol, int N);
	void update_bars();
	bool backtest_status();
	char* FindClose();
	char* FindVolume();
};

char *HistoricCSVDataHandler::FindClose() {
	for (int i = 0; i < COUNTER; i++) {
		if (strcmp(field[i], "Close") == 0) {
			return field[i];
		}
	}
	return NULL;
}

char *HistoricCSVDataHandler::FindVolume() {
	for (int i = 0; i < COUNTER; i++) {
		if (strcmp(field[i], "Volume") == 0) {
			return field[i];
		}
	}
	return NULL;
}

bool HistoricCSVDataHandler::backtest_status() {
	return continue_backtest;
}

inline void HistoricCSVDataHandler::update_bars() {
	for (std::map<char *, int>::iterator pos = symbol_list->begin(); pos != symbol_list->end(); pos++)
	{
		int count = (*symbol_list)[pos->first];
		if (Iterator[count] != next(rIterator[count])) {
			(*latest_symbol_data)[pos->first][Iterator[count]->first] = Iterator[count]->second;
			Iterator[count] = next(Iterator[count]);
			if (Iterator[count] == (*symbol_data)[pos->first].end()) {
				cerr << "Start date should be ahead of end date!" << endl;
				exit(1);
			}
		}
		else {
			continue_backtest = false;
		}
	}
	/*Push event to queue.*/
	//cout << "Push MarketEvent" << endl;
	if (continue_backtest == true) {
		ptr_event->push(&market);
	}
}

inline std::map<time_type, std::map<key_type, value_type>, TimeCompare> *HistoricCSVDataHandler::get_latest_bars(char *symbol, int N = 1) {

	ptr_temp->clear();
	int count = 0;

	for (table_type::reverse_iterator rpos = (*latest_symbol_data).at(symbol).rbegin(); rpos != (*latest_symbol_data).at(symbol).rend(); rpos++) {

		if (count == N) { break; }
		(*ptr_temp)[rpos->first] = rpos->second;
		count++;
		
	}

	return ptr_temp.get();
}

HistoricCSVDataHandler::HistoricCSVDataHandler(std::shared_ptr<ThreadQueue<Event *> > ptr_queue, std::shared_ptr<std::map<char *, int> > symbol_list, const char *start_date, const char *end_date, const char *path = "./") {

	auto ptr_symbol_data = std::make_unique<symbol_type>();

	/*Intialize symbol_list*/
	this->symbol_list = symbol_list;

	/*Initialize symbol_data*/
	char temp[PATH_LENGTH];
	for (std::map<char *, int>::iterator pos = this->symbol_list->begin(); pos != this->symbol_list->end(); pos++) {
		sprintf_s(temp, PATH_LENGTH, "%s%s.csv", path, pos->first);
		/*Call ReadCsv function and construct dataframe*/
		//table_type temp1;
		ReadData((*ptr_symbol_data)[pos->first], field, temp);
	}

	/*Initialize Iterator[] and latest_symbol_data*/
	DateTime start(start_date);
	DateTime end(end_date);
	for (std::map<char *, int>::iterator pos = this->symbol_list->begin(); pos != this->symbol_list->end(); pos++) {
		Iterator[pos->second] = (*ptr_symbol_data)[pos->first].find(start);
		rIterator[pos->second] = (*ptr_symbol_data)[pos->first].find(end);

		if ((Iterator[pos->second] == (*ptr_symbol_data)[pos->first].end()) || (rIterator[pos->second] == (*ptr_symbol_data)[pos->first].end())) {
			cerr << "Invalid start date or end date!" << endl;
			exit(1);
		}
	}

	symbol_data = std::move(ptr_symbol_data);
	ptr_event = ptr_queue;

	auto pptr_temp = std::make_unique<table_type>();
	ptr_temp = std::move(pptr_temp);

	auto ptr_latest_symbol_data = std::make_unique<symbol_type>();
	latest_symbol_data = std::move(ptr_latest_symbol_data);

	continue_backtest = true;
}

#endif // !DATAHANDLER_HPP
