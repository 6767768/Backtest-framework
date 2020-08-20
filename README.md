#### Structure

```mermaid
graph TB;
	Event.hpp-->Execution.hpp
	Event.hpp-->DataHandler.hpp
	ThreadQueue.hpp-->Event.hpp
	Event.hpp-->Strategy.hpp
	DataHandler.hpp--<br> DataHandler *ptr_bar </br>calculate_signals-->Strategy.hpp
	DataHandler.hpp--<br> DataHandler *ptr_bar </br>NaivePortfolio::NaivePortfolio::latest_symbol_data-->Portfolio.hpp
	DataHandler.hpp--<br> DataHandler *ptr_bar </br>calculate_slippage-->Execution.hpp
	Event.hpp-->Portfolio.hpp
	CustomizedDataType.hpp-->Time.hpp
	Time.hpp-->Event.hpp
	Execution.hpp-->main.cpp
	Strategy.hpp-->main.cpp
	DataHandler.hpp-->main.cpp
	Portfolio.hpp-->main.cpp
	PyPlot.py-.external PyFile.->main.cpp
	
```

#### Prototype

~~~c++
DataHandler();
DataHandler: public HistoricCSVDataHandler(std::shared_ptr<ThreadQueue<Event *> > ptr_queue, std::shared_ptr<std::map<char *, int> > symbol_list, const char *start_date, const char *end_date, const char *path = "./");

Strategy();
Strategy: public BuyAndHoldStrategy(DataHandler *ptr_bar, std::shared_ptr<ThreadQueue<Event *> > ptr_queue, std::shared_ptr<std::map<char *, int> > symbol_list);

Portfolio();
Portfolio: public NaivePortfolio(DataHandler *ptr_bar, std::shared_ptr<ThreadQueue<Event *> > ptr_queue, std::shared_ptr<std::map<char *, int> > symbol_list, double initial_capital);

ExecutionHandler();
ExecutionHandler: public SimulatedExecutionHandler(std::shared_ptr<ThreadQueue<Event *> > ptr_queue, DataHandler *ptr_bar, const char *ALGO = "ALGO")

ExecutionHandler: public IBExecutionHandler(std::shared_ptr<ThreadQueue<Event *> > ptr_queue, const char *order_routing = "SMART", const char *currency = "USD")
~~~

#### Latent variable

`symbol_list` `std::shared_ptr<std::map<char *, int> >`

`ptr_event` `std::shared_ptr<std::queue<Event *> >`

`symbol_data` `std::unique_ptr<symbol_type>`

`latest_symbol_data` `std::unique_ptr<symbol_type>`

`DataHandler object`

#### UML

```mermaid
sequenceDiagram
	participant User as User
	participant Queue as Queue
	participant DH as DataHandler
	participant STR as Strategy
	participant PO as Portfolio
	participant EXE as Execution
	participant GUI as GUI
	
	User->>DH:ReadData()
	Queue->>DH:first
	DH-->>Queue:MarketEvent 
	
	loop !Queue.empty()
		Queue->>STR:MarketEvent
		Queue->>PO:MarketEvent
		activate STR
			STR-->Queue:SignalEvent
			
		activate PO
			PO-->Queue:update_index
		PO-xGUI:all_positions
		Queue->>PO:SignalEvent
    	activate PO
    	alt Order triggered
    		PO-->Queue:OrderEvent
    	else
    		PO-->Queue:OrderEvent(NULL)
    	end
    	Queue->>EXE:OrdeEvent
    	activate EXE
    		EXE-->Queue:FillEvent
    	
    	Queue->>PO:FillEvent
    	activate PO
    		PO-->Queue:return true
    end
    GUI--xUser:join()
```





