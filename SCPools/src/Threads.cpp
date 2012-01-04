#include "Threads.h"
#include <pthread.h>
#include <sched.h>
#include "ProducerThread.h"
#include "ConsumerThread.h"
#include "ArchEnvironment.h"

void assignToCPU(int cpu){
	pthread_t threadId = pthread_self();
	cpu_set_t cpuSet;
	CPU_ZERO(&cpuSet);
	CPU_SET(cpu,&cpuSet);
	int s = pthread_setaffinity_np(threadId,sizeof(cpu_set_t),&cpuSet);
	if(s != 0)
	{
		cout << "Thread Assignment Failed! exiting.." << endl;
		exit(-1);
	}
}

void* prodRun(void* _arg){
	producerArg* arg = (producerArg*)_arg;
	int id = arg->id;
	string forceAssignment;
	Configuration::getInstance()->getVal(forceAssignment, "forceAssignment");
	if(forceAssignment.compare("yes") == 0)
	{
		int cpu = ArchEnvironment::getInstance()->getProducerCore(id);
		assignToCPU(cpu);
	}
	ProducerThread* producerThread = new ProducerThread(id);
	producerThread->run();
	
	// build the returned stats
	producerStats* prodStats = new producerStats();
	prodStats->id = id;
	list<long>* measurements = producerThread->getTimeMeasurements();
	int peakLength = producerThread->getPeakLength();
	prodStats->numOfProducedTasks = peakLength*measurements->size();
	list<long>::iterator it;
	double sum = 0;
	for(it = measurements->begin(); it !=  measurements->end(); it++)
	{
		sum += ((double)*it)/1000000;
	}
	double averageInsertionTime = sum/measurements->size(); // average burst length in [ms]
	prodStats->producerThroughput = peakLength *(1/averageInsertionTime);  //insertion throughput in tasks/ms

	delete producerThread;
	return (void*)prodStats;
}

void* consRun(void* _arg){
	consumerArg* arg = (consumerArg*)_arg;
	int id = arg->id;
	string forceAssignment;
	Configuration::getInstance()->getVal(forceAssignment, "forceAssignment");
	if(forceAssignment.compare("yes") == 0)
	{
		int cpu = ArchEnvironment::getInstance()->getConsumerCore(id);
		assignToCPU(cpu);
	}
	ConsumerThread* consumerThread = new ConsumerThread(id,arg->poolPtr);
	consumerThread->run();
	
	// build the returned stats
	consumerStats* consStats = new consumerStats();
	consStats->id = id;
	consStats->numOfRetrievedTasks = consumerThread->getNumOfTasks();
	consStats->consumerThroughput = consumerThread->getThroughput();
	
	delete consumerThread;
	return (void*)consStats;
}