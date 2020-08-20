#ifndef EXECUTION_HPP
#define EXECUTION_HPP

#include "Event.hpp"
#include "DataHandler.hpp"

class ExecutionHandler {
	virtual void execute_order(Event *event) = 0;
};

class SimulatedExecutionHandler :public ExecutionHandler {
protected:
	std::shared_ptr<ThreadQueue<Event *> > ptr_event;
	DataHandler *ptr_bar;
	FillEvent fill_event[TICKER_NUMBER];
	int ticker_count;
	char ALGO[TYPE_LENGTH];
public:
	SimulatedExecutionHandler() {};
	//SimulatedExecutionHandler(std::shared_ptr<ThreadQueue<Event *> > ptr_queue);
	SimulatedExecutionHandler(std::shared_ptr<ThreadQueue<Event *> > ptr_queue, DataHandler *ptr_bar, const char *ALGO);
	~SimulatedExecutionHandler() {};
	void execute_order(Event *event);
	void calculate_slippage(Event *event, double &quantity_t, double &slippage_t);
};

/* https://www.quantopian.com/help#ide-slippage */
void SimulatedExecutionHandler::calculate_slippage(Event *event, double &quantity_t, double &slippage_t) {
	int fill_dir = 0;
	if (strcmp(event->GetDirection(), "BUY") == 0) {
		fill_dir = 1;
	}
	else if (strcmp(event->GetDirection(), "SELL") == 0) {
		fill_dir = -1;
	}

	double quantity = event->GetQuantity();
	double slippage = 0;
	double basisPointDeduc = 1;
	double volumeLimit = 0.025;

	std::map<key_type, value_type> temp = ptr_bar->get_latest_bars(event->GetSymbol())->rbegin()->second;
	double volume = temp.at(ptr_bar->FindVolume());
	double lastclose = temp.at(ptr_bar->FindClose());

	if (quantity > volume*volumeLimit) {
		cerr << "Failed to fill" << quantity - (volume*volumeLimit)*fill_dir << "shares of " << event->GetSymbol() << endl;
		quantity_t = floor(volume*volumeLimit);
	}
	slippage_t = volume * lastclose * (basisPointDeduc / 10000);

}

void SimulatedExecutionHandler::execute_order(Event *event) {
	if (strcmp(event->GetType(), "ORDER") == 0) {
		char systemtime[TIME_LENGTH];
		SystemTime(systemtime, TIME_LENGTH);

		double quantity = event->GetQuantity();
		double slippage = event->GetSlippage();

		if (strcmp(ALGO, "ALGO") == 0) {
			calculate_slippage(event, quantity, slippage);
		}

		fill_event[ticker_count].Init(systemtime, event->GetSymbol(), "NASDAQ", quantity, event->GetDirection(), 1000000, 0, slippage);
		//cout << "Push FillEvent" << endl;
		ptr_event->push(&(fill_event[ticker_count]));
		ticker_count++;
	}
	else {
		cout << "Execution.hpp SimulatedExecutionHandler::execute_order" << endl;
		exit(1);
	}
}

//SimulatedExecutionHandler::SimulatedExecutionHandler(std::shared_ptr<ThreadQueue<Event *> > ptr_queue) {
//	ptr_event = ptr_queue;
//	ticker_count = 0;
//}

SimulatedExecutionHandler::SimulatedExecutionHandler(std::shared_ptr<ThreadQueue<Event *> > ptr_queue, DataHandler *ptr_bar, const char *ALGO = "ALGO") {
	ptr_event = ptr_queue;
	ticker_count = 0;
	this->ptr_bar = ptr_bar;
	strcpy_s(this->ALGO, strlen(ALGO) + 1, ALGO);
}

class IBExecutionHandler :public ExecutionHandler {
	/*More interferces should be added based on different platform's api.*/
protected:
	std::shared_ptr<ThreadQueue<Event *> > ptr_event;
	char *order_routing;
	char *currency;
public:
	IBExecutionHandler() {};
	IBExecutionHandler(std::shared_ptr<ThreadQueue<Event *> > ptr_queue, const char *order_rounting, const char *currency);
	~IBExecutionHandler() {};
};

IBExecutionHandler::IBExecutionHandler(std::shared_ptr<ThreadQueue<Event *> > ptr_queue, const char *order_routing = "SMART", const char *currency = "USD") {
	ptr_event = ptr_queue;
	strcpy_s(this->order_routing, strlen(order_routing) + 1, order_routing);
	strcpy_s(this->currency, strlen(currency) + 1, currency);
}

#endif // !EXECUTION_HPP
