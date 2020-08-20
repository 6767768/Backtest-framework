#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include "Event.hpp"
#include "DataHandler.hpp"

class Portfolio {
public:
	virtual void update_signal(Event *event) = 0;
	virtual void update_fill(Event *event) = 0;
	virtual void Evaluation() = 0;
	virtual double MaxDrawDown() = 0;
	virtual double WinRate() = 0;
	virtual double SharpRatio() = 0;
	virtual double Mars() = 0;
	virtual double AnnualisedReturn() = 0;
	virtual Holdings* NetCurve() = 0;
};

class NaivePortfolio:public Portfolio {
protected:
	std::shared_ptr<ThreadQueue<Event *> > ptr_event;
	/*Store symbol_list*/
	std::shared_ptr<std::map<char *, int> > symbol_list;
	//char start_date[TIME_LENGTH];
	double initial_capital;
	
	DataHandler *ptr_bar;

	Holdings all_positions[ITERATION];
	Holdings current_positions;

	Holdings all_holdings[ITERATION];
	Holdings current_holdings;

	Holdings curve;
	int iteration;

	OrderEvent order[TICKER_NUMBER];
	int ticker_count;
public:
	NaivePortfolio() {};
	NaivePortfolio(DataHandler *ptr_bar, std::shared_ptr<ThreadQueue<Event *> > ptr_queue,
		std::shared_ptr<std::map<char *, int> > symbol_list, double initial_capital);
	~NaivePortfolio() {};
	void construct_all_positions();
	void construct_current_positions();
	void construct_all_holdings();
	void construct_current_holdings();
	void update_timeindex(Event *event);
	void update_positions_from_fill(Event *fill);
	void update_holdings_from_fill(Event *fill);
	void update_fill(Event *event);
	OrderEvent generate_naive_order(Event *signal);
	void update_signal(Event *event);
	void create_equity_curve();
	void Evaluation();
	double MaxDrawDown();
	double WinRate();
	double SharpRatio();
	double Mars();
	double AnnualisedReturn();
	Holdings *NetCurve();
	Holdings *PositionAddress();
};

Holdings *NaivePortfolio::PositionAddress() {
	return all_positions;
}

Holdings* NaivePortfolio::NetCurve() {
	all_positions[ITERATION].total = iteration;
	return all_positions;
}

double NaivePortfolio::SharpRatio() {
	return 0;
}

double NaivePortfolio::Mars() {
	return AnnualisedReturn() / MaxDrawDown();
}

double NaivePortfolio::AnnualisedReturn() {
	return all_positions[iteration - 1].total / all_positions[0].total - 1;
}

double NaivePortfolio::WinRate() {
	double Win = 0;
	for (int i = 1; i < iteration; i++) {
		if (all_positions[i].total > all_positions[i - 1].total) {
			Win++;
		}
	}
	return (Win / iteration);
}

double NaivePortfolio::MaxDrawDown() {
	double max = 0;
	double temp;

	for (int i = 0; i < iteration; i++) {
		if (i == iteration - 1) {
			if (all_positions[i].total < all_positions[i - 1].total) {
				max = ((temp - all_positions[i].total) / temp > max) ? (temp - all_positions[i].total) / temp : max;
				break;
			}
		}
		if ((all_positions[i].total < all_positions[i - 1].total) && (all_positions[i].total < all_positions[i + 1].total)) {
			max = ((temp - all_positions[i].total) / temp > max) ? (temp - all_positions[i].total) / temp : max;
		}
		else if ((all_positions[i].total > all_positions[i - 1].total) && (all_positions[i].total > all_positions[i + 1].total)) {
			temp = all_positions[i].total;
		}
	}
	return max;
}

void NaivePortfolio::Evaluation() {
	cout << "============================ Account Information ============================\n";
	cout << "Current cash: " << current_holdings.cash << "\nPositions: ";
	for (std::map<char *, int>::iterator pos = symbol_list->begin(); pos != symbol_list->end(); pos++) {
		cout << pos->first << ": " << current_positions.Postions[pos->second] << "\t";
	}
	cout << "\n============================ Summary ============================\n";
	cout << "MaxDrawDown: " << MaxDrawDown() << endl;
	cout << "AnnulizedReturn: " << AnnualisedReturn() << endl;
	cout << "Mars: " << Mars() << endl;
	cout << "WinRate: " << WinRate() << endl;

}

void NaivePortfolio::create_equity_curve() {
	int count = 0;
	for (std::map<char *, int>::iterator pos = symbol_list->begin(); pos != symbol_list->end(); pos++) {
		double cumprod = 1;
		for (int i = 0; i < iteration; i++) {
			cumprod *= all_holdings[i].Postions[(*symbol_list)[pos->first]];
		}
		curve.Postions[count++] = cumprod;
	}
}

void NaivePortfolio::update_signal(Event *event) {
	if (strcmp(event->GetType(), "SIGNAL") == 0) {

		order[ticker_count] = generate_naive_order(event);
		//cout << "Push OrderEvent" << endl;
		ptr_event->push(&(order[ticker_count]));
		ticker_count++;
	}
	else {
		cout << "Portfolio.hpp NaivePortfolio::update_signal" << endl;
		exit(1);
	}
}

OrderEvent NaivePortfolio::generate_naive_order(Event *signal) {

	double mkt_quantity;
	double cur_quantity;
	const char *order_type = "MKT";

	mkt_quantity = int(100 * signal->GetStrength());
	cur_quantity = current_positions.Postions[(*symbol_list)[signal->GetSymbol()]];

	if ((strcmp(signal->GetSignalType(), "LONG") == 0) && (abs(cur_quantity) < 0.00001)) {
		OrderEvent order(signal->GetSymbol(), order_type, mkt_quantity, "BUY");
		return order;
	}
	else if ((strcmp(signal->GetSignalType(), "SHORT") == 0) && (abs(cur_quantity) < 0.00001)) {
		OrderEvent order(signal->GetSymbol(), order_type, mkt_quantity, "SELL");
		return order;
	}
	else if ((strcmp(signal->GetSignalType(), "EXIT") == 0) && (abs(cur_quantity) > 0)) {
		OrderEvent order(signal->GetSymbol(), order_type, abs(cur_quantity), "SELL");
		return order;
	}
	else if ((strcmp(signal->GetSignalType(), "EXIT") == 0) && (abs(cur_quantity) < 0)) {
		OrderEvent order(signal->GetSymbol(), order_type, abs(cur_quantity), "BUY");
		return order;
	}
	else {
		OrderEvent order(signal->GetSymbol(), "NULL", 0, "NULL");
		order.NullType();
		return order;
	}
}

void NaivePortfolio::update_fill(Event *event) {
	if (strcmp(event->GetType(), "FILL") == 0) {

		update_holdings_from_fill(event);
		update_positions_from_fill(event);
	}
	else {
		cout << "Portfolio.hpp NaivePortfolio::update_fill" << endl;
		exit(1);
	}
}

void NaivePortfolio::update_holdings_from_fill(Event *fill) {
	/*update current holdings*/
	//cout << "update_holdings_from_fill" << endl;
	int fill_dir = 0;
	double fill_cost, cost;
	if (strcmp(fill->GetDirection(), "BUY") == 0) {
		fill_dir = 1;
	}
	else if (strcmp(fill->GetDirection(), "SELL") == 0) {
		fill_dir = -1;
	}

	fill_cost = (ptr_bar->get_latest_bars(fill->GetSymbol())->begin()->second).at(ptr_bar->FindClose());

	cost = fill_dir * fill_cost * fill->GetQuantity();

	current_holdings.Postions[(*symbol_list)[fill->GetSymbol()]] += fill->GetQuantity();
	current_holdings.commission += fill->GetCommision();
	current_holdings.slippage += fill->GetSlippage();

	/*Note: Commission is nonzero*/
	//cout << cost << " " << fill->GetCommision() << " " << fill->GetSlippage() << endl;
	current_holdings.cash -= (cost + fill->GetCommision() + fill->GetSlippage());

}

void NaivePortfolio::update_positions_from_fill(Event *fill) {
	/*update current positions*/
	int fill_dir = 0;
	if (strcmp(fill->GetDirection(), "BUY") == 0) {
		fill_dir = 1;
	}
	else if (strcmp(fill->GetDirection(), "SELL") == 0) {
		fill_dir = -1;
	}
	/*update positions in current_position*/
	current_positions.Postions[(*symbol_list)[fill->GetSymbol()]] += fill_dir * fill->GetQuantity();

}

void NaivePortfolio::update_timeindex(Event *event) {

	//cout << "update_timeindex" << endl;
	/*update positions*/
	double *temp = all_positions[iteration].Postions;
	double *current = current_positions.Postions;
	double market_value;
	for (std::map<char *, int>::iterator pos = symbol_list->begin(); pos != symbol_list->end(); pos++) {
		*temp = *current;
		temp++;
		current++;
	}

	((ptr_bar->get_latest_bars(symbol_list->begin()->first))->begin()->first).TimeToChar(all_positions[iteration].datetime);
	//cout << all_positions[iteration].datetime << endl;
	/*update holdings*/
	strcpy_s(all_holdings[iteration].datetime, strlen(current_holdings.datetime) + 1, current_holdings.datetime);
	all_holdings[iteration].cash = current_holdings.cash;
	all_holdings[iteration].commission = current_holdings.commission;
	all_holdings[iteration].slippage = current_holdings.slippage;

	temp = all_positions[iteration].Postions;
	current = current_positions.Postions;

	for (std::map<char *, int>::iterator pos = symbol_list->begin(); pos != symbol_list->end(); pos++) {
		market_value = (*current) *(ptr_bar->get_latest_bars(pos->first)->begin()->second.at(ptr_bar->FindClose()));
		/*Net Assets*/
		*temp = market_value;
		all_positions[iteration].total += market_value;
		temp++;
		current++;
	}
	all_positions[iteration].total += current_holdings.cash;
	iteration++;
	
}

void NaivePortfolio::construct_all_positions() {
	/*
Only
	Positions, double Position[]
	datetime
	total
are utilized
*/
	double *temp = all_positions[0].Postions;
	for (std::map<char *, int>::iterator pos = symbol_list->begin(); pos != symbol_list->end(); pos++) {
		*temp = 0;
		temp++;
	}
	//strcpy_s(all_positions[0].datetime, strlen(start_date) + 1, start_date);
}
void NaivePortfolio::construct_current_positions() {
	/*
Only
	Positions, double Position[]
are utilized
*/
	double *temp = current_positions.Postions;
	for (std::map<char *, int>::iterator pos = symbol_list->begin(); pos != symbol_list->end(); pos++) {
		*temp = 0;
		temp++;
	}
}
void NaivePortfolio::construct_all_holdings() {
	/*
	Only
		Positions, double Position[]
		cash double
		commission double
		total double
		datetime
	are utilized
	*/
	double *temp = all_holdings[0].Postions;
	for (std::map<char *, int>::iterator pos = symbol_list->begin(); pos != symbol_list->end(); pos++) {
		*temp = 0;
		temp++;
	}
	//strcpy_s(all_holdings[0].datetime, strlen(start_date) + 1, start_date);
	all_holdings[0].cash = initial_capital;
	all_holdings[0].commission = 0;
	all_holdings[0].slippage = 0;
	all_holdings[0].total = initial_capital;
}
void NaivePortfolio::construct_current_holdings() {
	/*
	Only
		Positions, double Position[]
		cash double
		commission double
		total double
	are utilized
	*/
	double *temp = current_holdings.Postions;
	for (std::map<char *, int>::iterator pos = symbol_list->begin(); pos != symbol_list->end(); pos++) {
		*temp = 0;
		temp++;
	}
	current_holdings.cash = initial_capital;
	current_holdings.commission = 0;
	current_holdings.slippage = 0;
}

NaivePortfolio::NaivePortfolio(DataHandler *ptr_bar, std::shared_ptr<ThreadQueue<Event *> > ptr_queue, 
	std::shared_ptr<std::map<char *, int> > symbol_list, double initial_capital) {
	ticker_count = 0;
	this->symbol_list = symbol_list;
	ptr_event = ptr_queue;
	//strcpy_s(this->start_date, strlen(start_date) + 1, start_date);
	this->ptr_bar = ptr_bar;
	this->initial_capital = initial_capital;
	construct_all_holdings();
	construct_all_positions();
	construct_current_holdings();
	construct_current_positions();
	iteration = 0;
}

#endif // !PORTFOLIO_HPP
