#ifndef EVENT_HPP
#define EVENT_HPP

#include "Time.hpp"
#include "ThreadQueue.hpp"

typedef DateTime time_type;
typedef std::map<time_type, std::map<key_type, value_type>, TimeCompare> table_type;
typedef std::map<char *, std::map<time_type, std::map<key_type, value_type>, TimeCompare> > symbol_type;

void ReadData(std::map<time_type, std::map<key_type, value_type>, TimeCompare> &Table, char field[COUNTER][CHARACTER], const char *symbol, const char *Type);

void Print(std::map<time_type, std::map<char *, double> > &Table);

struct Holdings {
	double Postions[TICKER_NUMBER];
	char datetime[TIME_LENGTH];
	double cash;
	double commission;
	double total;
	double slippage;
};

class Event {
public:
	virtual void NullType() = 0;
	virtual double GetCommision() = 0;
	virtual char *GetDirection() = 0;
	virtual char *GetSymbol()const = 0;
	virtual double GetQuantity() = 0;
	virtual double GetStrength() = 0;
	virtual char* GetSignalType() = 0;
	virtual char *GetType() = 0;
	virtual double GetSlippage() = 0;
};

class MarketEvent :public Event {
protected:
	char Type[TYPE_LENGTH];
public:
	MarketEvent() { Init(); }
	MarketEvent(const MarketEvent &market) { Init(); }
	void Init() { strcpy_s(Type, 7, "MARKET"); }
	char *GetType() { return Type; }
	void NullType() { strcpy_s(Type, 5, "NULL"); }
	double GetCommision() { return -1; }
	char *GetDirection() { return NULL; }
	char *GetSymbol() const { return NULL; }
	double GetQuantity() { return -1; }
	double GetStrength() { return -1; }
	char* GetSignalType() { return NULL; }
	double GetSlippage() { return -1; }
};

class SignalEvent :public Event {
protected:
	char Type[TYPE_LENGTH];
	char *symbol;
	char datetime[TIME_LENGTH];
	char signal_type[TYPE_LENGTH];
	double strength;
public:
	SignalEvent(){ strcpy_s(Type, 7, "SIGNAL"); }
	SignalEvent(const SignalEvent &signal);
	SignalEvent(char *symbol, const char * datetime, const char *signal_type, double strength);
	void Init(char *symbol, const char * datetime, const char *signal_type, double strength);
	char *GetType() { return Type; }
	void NullType() { strcpy_s(Type, 5, "NULL"); }
	double GetStrength() { return strength; }
	char* GetSignalType() { return signal_type; }
	char *GetSymbol() const { return symbol; }
	double GetCommision() { return -1; }
	char *GetDirection() { return NULL; }
	double GetQuantity() { return -1; }
	double GetSlippage() { return -1; }
};

class OrderEvent :public Event {
protected:
	char Type[TYPE_LENGTH];
	char *symbol;
	char order_type[TYPE_LENGTH];
	double quantity;
	char direction[DIRECTION_LENGTH];
public:
	OrderEvent() { strcpy_s(Type, 6, "ORDER"); }
	OrderEvent(char *symbol, const char *order_type, double quantity, const char *direction);
	OrderEvent(OrderEvent &order);
	void print_order();
	void Init(char *symbol, const char *order_type, double quantity, const char *direction);
	char *GetType() { return Type; }
	void NullType() { strcpy_s(Type, 5, "NULL"); }
	double GetQuantity() { return quantity; }
	char *GetSymbol() const { return symbol; }
	char *GetDirection() { return direction; }
	double GetStrength() { return -1; }
	char* GetSignalType() { return NULL; }
	double GetCommision() { return -1; }
	double GetSlippage() { return -1; }
};

class FillEvent :public Event {
protected:
	char Type[TYPE_LENGTH];
	char timeindex[TIME_LENGTH];
	char *symbol;
	char exchange[EXCHANGE_LENGTH];
	double quantity;
	char direction[DIRECTION_LENGTH];
	double fill_cost;
	double commission;
	double slippage;
public:
	FillEvent() { strcpy_s(Type, 5, "FILL"); }
	FillEvent(const FillEvent& fill);
	FillEvent(const char *timeindex, char *symbol, const char *exchange, double quantity, const char *direction, double fill_cost, double commission, double slippage);
	double calculate_ib_commission();
	void Init(const char *timeindex, char *symbol, const char *exchange, double quantity, const char *direction, double fill_cost, double commission, double slippage);
	char *GetType() { return Type; }
	void NullType() { strcpy_s(Type, 5, "NULL"); }
	double GetCommision() { return commission; }
	char *GetSymbol()const { return symbol; }
	char *GetDirection() { return direction; }
	double GetQuantity() { return quantity; }
	double GetStrength() { return -1; }
	char* GetSignalType() { return NULL; }
	double GetSlippage() { return slippage; }
};

OrderEvent::OrderEvent(OrderEvent &order) {
	strcpy_s(Type, 6, "ORDER");
	this->symbol = order.symbol;
	strcpy_s(order_type, strlen(order.order_type) + 1, order.order_type);
	strcpy_s(direction, strlen(order.direction) + 1, order.direction);
	quantity = order.quantity;
}

OrderEvent::OrderEvent(char *symbol, const char *order_type, double quantity, const char *direction) {
	Init(symbol, order_type, quantity, direction);
}

void OrderEvent::Init(char *symbol, const char *order_type, double quantity, const char *direction) {
	strcpy_s(Type, 6, "ORDER");
	this->symbol = symbol;
	strcpy_s(this->order_type, strlen(order_type) + 1, order_type);
	this->quantity = quantity;
	strcpy_s(this->direction, strlen(direction) + 1, direction);
	//print_order();
}

void OrderEvent::print_order() {
	cout << "Symbol: " << symbol <<
		"	Order_type: " << order_type <<
		"	Quantity: " << quantity <<
		"	Direction: " << direction << endl;
}

void SignalEvent::Init(char *symbol, const char * datetime, const char *signal_type, double strength = 100) {
	strcpy_s(Type, 7, "SIGNAL");
	this->symbol = symbol;
	strcpy_s(this->datetime, strlen(datetime) + 1, datetime);
	strcpy_s(this->signal_type, strlen(signal_type) + 1, signal_type);
	this->strength = strength;
}

SignalEvent::SignalEvent(char *symbol, const char * datetime, const char *signal_type, double strength = 100) {
	Init(symbol, datetime, signal_type, strength);
}

SignalEvent::SignalEvent(const SignalEvent &signal) {
	Init(signal.GetSymbol(), signal.datetime, signal.signal_type, signal.strength);
}

FillEvent::FillEvent(const char *timeindex, char *symbol, const char *exchange, double quantity, const char *direction, double fill_cost, double commission = 0, double slippage = 0) {
	Init(timeindex, symbol, exchange, quantity, direction, fill_cost, commission, slippage);
}

void FillEvent::Init(const char *timeindex, char *symbol, const char *exchange, double quantity, const char *direction, double fill_cost, double commission = 0, double slippage = 0) {
	strcpy_s(Type, 5, "FILL");
	strcpy_s(this->timeindex, strlen(timeindex) + 1, timeindex);
	this->symbol = symbol;
	strcpy_s(this->exchange, strlen(exchange) + 1, exchange);
	this->quantity = quantity;
	strcpy_s(this->direction, strlen(direction) + 1, direction);
	this->fill_cost = fill_cost;
	if (abs(commission) < 0.0001) {
		this->commission = calculate_ib_commission();
	}
	else {
		this->commission = commission;
	}
	this->slippage = slippage;
}

FillEvent::FillEvent(const FillEvent& fill) {
	Init(fill.timeindex, fill.symbol, fill.exchange, fill.quantity, fill.direction, fill.fill_cost, fill.commission);
}

double FillEvent::calculate_ib_commission() {
	double full_cost = 1.3;
	if (this->quantity <= 500) {
		full_cost = (1.3 > 0.013*quantity) ? 1.3 : (0.013*quantity);
	}
	else {
		full_cost = (1.3 > 0.008*quantity) ? 1.3 : (0.008*quantity);
	}
	full_cost = (full_cost > 0.005*quantity*fill_cost) ? (0.005*quantity*fill_cost) : full_cost;
	return full_cost;
}

void ReadData(std::map<time_type, std::map<key_type, value_type>, TimeCompare> &Table, char field[COUNTER][CHARACTER], const char *symbol, const char *Type = "%Y-%M-%D %H:%M:%S") {
	/*Extract table*/
	std::ifstream in(symbol);
	char cell[COUNTER*CHARACTER];
	int count = 0;
	DateTime time;
	char temp[CHARACTER];

	if (in.is_open()) {
		bool First = true;
		in.getline(cell, COUNTER*CHARACTER);
		char *p = cell, *q = cell;

		while (true) {
			if (*p == '\0') { MyStrcpy(field[count++], q + 1, p); break; }
			if (*p == ',') {
				if (First) { //MyStrcpy(field[count++], q, p); 
					First = false;
				}
				else { MyStrcpy(field[count++], q + 1, p); }
				q = p;
			}
			p++;
		}
		while (!in.eof()) {
			First = true;
			in.getline(cell, COUNTER*CHARACTER);
			if (strcmp(cell, "") == 0) { break; }/*filter null line*/
			p = cell, q = cell;
			count = 0;

			while (true) {
				if (*p == '\0') {
					MyStrcpy(temp, q + 1, p);
					Table[time][field[count++]] = atof(temp);
					break;
				}
				if (*p == ',') {
					if (First) {
						MyStrcpy(temp, q, p);
						if (strcmp(Type, "%Y-%M-%D %H:%M:%S") == 0) {
							time.Time_YMDHMS(temp);
						}
						else if (strcmp(Type, "%M-%D-%Y %H:%M:%S") == 0) {
							time.Time_MDYHMS(temp);
						}
						else if (strcmp(Type, "%D-%M-%Y %H:%M:%S") == 0) {
							time.Time_DMYHMS(temp);
						}
						else {
							time.Time_YMDHMS(temp);
						}
						First = false;
					}
					else {
						MyStrcpy(temp, q + 1, p);
						Table[time][field[count++]] = atof(temp);
					}
					q = p;
				}
				p++;
			}
		}
	}
}

void Print(std::map<time_type, std::map<char *, double> > &Table) {
	for (std::map<key_type, value_type>::iterator ppos = (Table.begin()->second).begin(); ppos != (Table.begin()->second).end(); ppos++) {
		cout << ppos->first << " ";
	}
	cout << endl;
	for (std::map<time_type, std::map<key_type, value_type> >::iterator pos = Table.begin(); pos != Table.end(); pos++) {
		pos->first.Print();
		for (std::map<key_type, value_type>::iterator ppos = pos->second.begin(); ppos != pos->second.end(); ppos++) {
			cout << ppos->second << " ";
		}
		cout << endl;
	}
}

#endif // !EVENT_HPP
