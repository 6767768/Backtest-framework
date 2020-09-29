/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright © 2020 Xiwei Wang 
 * Created by Xiwei Wang on 07/26/2020
 * All rights reserved.                                               
 *                                               
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include "Event.hpp"
#include "DataHandler.hpp"

class Strategy {
	virtual void calculate_signals(Event *event) = 0;
};

class BuyAndHoldStrategy :public Strategy {
protected:
	std::mutex m_mutex;
	std::shared_ptr<std::map<char *, int> > symbol_list;
	std::shared_ptr<ThreadQueue<Event *> > ptr_event;
	std::unique_ptr<bool[]> bought;
	DataHandler *ptr_bar;
	SignalEvent signal_event[TICKER_NUMBER];
	int ticker_count;
public:
	BuyAndHoldStrategy() {};
	BuyAndHoldStrategy(DataHandler *ptr_bar, std::shared_ptr<ThreadQueue<Event *> > ptr_queue, std::shared_ptr<std::map<char *, int> > symbol_list);
	~BuyAndHoldStrategy() {};
	void calculate_signals(Event *event);
};
/*Unfinished*/
BuyAndHoldStrategy::BuyAndHoldStrategy(DataHandler *ptr_bar, std::shared_ptr<ThreadQueue<Event *> > ptr_queue, std::shared_ptr<std::map<char *, int> > symbol_list) {

	ticker_count = 0;
	/*Pass pointer*/

	this->ptr_bar = ptr_bar;

	/*Initialize bool list*/
	std::unique_ptr<bool[]> ptr_bought(new bool[symbol_list->size() + 1]);
	for (std::map<char *, int>::iterator pos = symbol_list->begin(); pos != symbol_list->end(); pos++) {
		ptr_bought[pos->second] = false;
	}

	bought = std::move(ptr_bought);
	ptr_event = ptr_queue;
	this->symbol_list = symbol_list;
}

void BuyAndHoldStrategy::calculate_signals(Event *event) {
	if (strcmp(event->GetType(), "MARKET") == 0) {
		for (std::map<char *, int>::iterator pos = symbol_list->begin(); pos != symbol_list->end(); pos++) {
			if ((ptr_bar->get_latest_bars(pos->first))->size() > 0) {
				if (!(bought[pos->second])) {
					/*(Symbol, Datetime, Type = LONG, SHORT or EXIT)*/
					char datetime[TIME_LENGTH];
					
					(((ptr_bar->get_latest_bars(pos->first))->begin())->first).TimeToChar(datetime);

					signal_event[ticker_count].Init(pos->first, datetime, "LONG");

					ptr_event->push(&(signal_event[ticker_count]));
					ticker_count++;
					bought[pos->second] = true;
				}
			}
		}
	}
}

#endif // !STRATEGY_HPP
