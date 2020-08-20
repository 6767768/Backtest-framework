
#include<Python.h>
#include "Time.hpp"
#include "ThreadQueue.hpp"
#include "Execution.hpp"
#include "Portfolio.hpp"
#include "Strategy.hpp"
#include "DataHandler.hpp"

void PyExecution(Holdings *Position);
void Run();
void Evaluation(Holdings *Position, int *flag);
int Size(char TICKER[TICKER_NUMBER][TICKER_LENGTH]);

int main(){
	Run();
	return 0;
}

void Run() {
	clock_t begin, end;
	begin = clock();

	auto  symbol_list = std::make_shared<std::map<char *, int> >();
	char TICKER[TICKER_NUMBER][TICKER_LENGTH] = { {"AAPL"},{"GS"},{"CAT"} };
	for (int i = 0; i < Size(TICKER); i++) {
		(*symbol_list)[TICKER[i]] = i;
	}
	if (static_cast<int>(symbol_list->size()) == 0) { cout << "Initialization failure!" << endl; exit(1); }
	//(*symbol_list)[TICKER[0]] = 0;
	//(*symbol_list)[TICKER[1]] = 1;
	//(*symbol_list)[TICKER[2]] = 2;
	auto ptr_queue = std::make_shared<ThreadQueue<Event *> >();
	int flag = 0;
	Event *event;

	HistoricCSVDataHandler bars(ptr_queue, symbol_list, "2002-12-31", "2003-12-30");
	BuyAndHoldStrategy strategy(&bars, ptr_queue, symbol_list);
	NaivePortfolio portfolio(&bars, ptr_queue, symbol_list, 1500000);
	SimulatedExecutionHandler execution(ptr_queue, &bars, "ALGO");

	std::thread evaluation(Evaluation, portfolio.PositionAddress(), &flag);
	while (true) {
		if (bars.backtest_status()) {
			bars.update_bars();
		}
		else {
			break;
		}

		while (true) {
			if (ptr_queue->empty()) {
				break;
			}
			else {
				ptr_queue->pop(event);
				//cout << event->GetType() << endl; /*Print Queue*/
				if (strcmp(event->GetType(), "NULL") == 0) { continue; }
				if (strcmp(event->GetType(), "MARKET") == 0) {
					//cout << "MARKET" << endl;
					strategy.calculate_signals(event);
					portfolio.update_timeindex(event);
				}

				else if (strcmp(event->GetType(), "SIGNAL") == 0) {
					//cout << "SIGNAL" << endl;
					portfolio.update_signal(event);
				}
				else if (strcmp(event->GetType(), "ORDER") == 0) {
					//cout << "ORDER" << endl;
					execution.execute_order(event);
				}
				else if (strcmp(event->GetType(), "FILL") == 0) {
					//cout << "FILL" << endl;
					portfolio.update_fill(event);
				}
				else {
				}
			}
		}
		/*Set Heartbeats*/
		//Sleep(1);
	}
	flag = 1;
	evaluation.join();

	end = clock();
	cout << "Total time: " << (double)(end - begin) / CLOCKS_PER_SEC << endl;
	
	portfolio.Evaluation();
	PyExecution(portfolio.NetCurve());
}

void Evaluation(Holdings *Position, int *flag) {
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	cout << "============================== Details ==============================\n";
	for (int i = 0; i < ITERATION; i++) {
		if (abs(Position[i].total) < 0.00001) {
			if (*flag == 1) {
				break;
			}
			i--;
		}
		else {
			cout << Position[i].total << " " << Position[i].datetime << endl;
		}
	}
}

void PyExecution(Holdings *Position) {

	Py_Initialize();
	if (!Py_IsInitialized()) { cerr << "Error in Initialization" << endl; exit(1); }

	//PyEval_InitThreads();

	PyObject *pModule = NULL;
	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;
	PyObject *pArgs = PyTuple_New(2);
	PyObject *pListX = PyList_New(static_cast<Py_ssize_t>(Position[ITERATION].total));
	PyObject *pListY = PyList_New(static_cast<Py_ssize_t>(Position[ITERATION].total));

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('..')");

	for (int i = 0; i < ITERATION; i++) {
		if (abs(Position[i].total) < 0.00001) { break; }
		PyList_SetItem(pListX, i, Py_BuildValue("s", Position[i].datetime));
		PyList_SetItem(pListY, i, Py_BuildValue("f", Position[i].total));
	}
	/*As a pointer, determine whether pArgs changes will change corresponding variables in Python interpreter*/
	PyTuple_SetItem(pArgs, 0, pListX);
	PyTuple_SetItem(pArgs, 1, pListY);

	//PyEval_ReleaseThread(PyThreadState_Get());
	//std::thread thread(Test2, pArgs);
	/*Note, when Interpreter begin to import module, nothing about C operation can be executed.*/
	pModule = PyImport_ImportModule("PyPlot");
	if (pModule == NULL) { cerr << "Error in importing PyModule." << endl; exit(1); }

	pFunc = PyObject_GetAttrString(pModule, "PolyPlot");
	if (pFunc == NULL) { cerr << "Error in obtaining functions." << endl; exit(1); }
	pValue = PyObject_CallFunctionObjArgs(pFunc, pArgs);
	if (pValue == NULL) { cerr << "Error in calling functions." << endl; exit(1); }

	//thread.join();

	Py_DECREF(pListX);
	Py_DECREF(pListY);
	Py_DECREF(pArgs);
	Py_DECREF(pModule);
	Py_DECREF(pFunc);
	Py_DECREF(pValue);
	//PyGILState_Ensure();
	Py_Finalize();
}

int Size(char TICKER[TICKER_NUMBER][TICKER_LENGTH]) {
	for (int i = 0; i < TICKER_NUMBER; i++) {
		if (strcmp(TICKER[i], "") == 0) {
			return i;
			break;
		}
	}
	return 0;
}