

#include <iostream>
#include <unistd.h>
#include "VirtualMachine.h"
#include "Machine.h"

#include <vector>
#include <queue>

using namespace std; 
volatile TVMTick globalTick;

extern "C"{

TVMMainEntry VMLoadModule(const char *module);

struct threadControlBlock{
	SMachineContext context;//context
	TVMThreadState state;//state
	TVMThreadID ID;//ID
	TVMMemorySize memsize; 
	TVMThreadPriority prio;
	void *stack = new uint8_t[0x100000];//stack
	TVMThreadEntry entry;//entry function
	void *param;//entry parameter
	int ticks; //for use with sleeping, need to keep track of how long it should sleep
	int result;
};

struct mutex{
	int locked = 0;//+1 -1 on this for locked unlocked. 1 = locked
	TVMMutexID ID;
	TVMThreadID owner;

	queue<threadControlBlock*> low; 
	queue<threadControlBlock*> medium; 
	queue<threadControlBlock*> high; 
};

vector<mutex*> mutexs; 

unsigned int curThreadID; 
threadControlBlock* idleThread = new threadControlBlock; 

vector<threadControlBlock*> threads;

//Priority Queues
queue<threadControlBlock*> highQueue;
queue<threadControlBlock*> normalQueue; 
queue<threadControlBlock*> lowQueue;
vector<threadControlBlock*> sleeping;


vector<threadControlBlock*>::iterator it;

TVMStatus VMMutexCreate (TVMMutexIDRef mutexref) 
{
	TVMStatus Status = VM_STATUS_SUCCESS;  

	TMachineSignalState Oldstate; 
	MachineSuspendSignals(&Oldstate); 
	

	mutex* newMutex = new mutex; 
	newMutex->ID = mutexs.size(); 
	newMutex->locked = 0; 	
	mutexs.push_back(newMutex); 
	*mutexref = newMutex->ID; 


	MachineResumeSignals(&Oldstate);
	
	return Status;  
}


 
void Schedule()
{
	threadControlBlock* newThread; 
	threadControlBlock currentThread; 
	
	TMachineSignalState Oldstate;
	MachineSuspendSignals(&Oldstate);


	//Check what thread has the highest priority 
        if (highQueue.empty())
        {
                if (normalQueue.empty())
                {
                        if(lowQueue.empty())
						{
							newThread = idleThread; 
						}
                        else
						{
                        	newThread = lowQueue.front();
							lowQueue.pop();
						} 
                }
                else
				{
					newThread = normalQueue.front();
					normalQueue.pop(); 
				}
        }
        else
		{
			newThread = highQueue.front();
			highQueue.pop();
		}


        for(it = threads.begin(); it < threads.end(); it++)
        {
                if((*it)->ID == curThreadID)
                {
			//found = 1; 
                        break;
                }

        }

	curThreadID = newThread->ID; 

	MachineContextSwitch(&(*it)->context, &newThread->context);
		
	newThread->state = VM_THREAD_STATE_RUNNING;
	MachineResumeSignals(&Oldstate);
}

void AlarmCallback(void *param){
	int done = 0;
	for (unsigned int i = 0;i < sleeping.size(); i++)
	{	
		sleeping[i]->ticks--;
		if (sleeping[i]->ticks <= 0)
		{
			/*1) Loop from start of sleep list
			2) Inside loop, decrement ticks of each thread in sleeping list
			3) If ticks are 0 of that thread, remove from sleep list, set state to ready and add to ready queue*/
			done = 1;
			sleeping[i]->state = VM_THREAD_STATE_READY;
			if (sleeping[i]->prio == VM_THREAD_PRIORITY_LOW)  
				lowQueue.push(sleeping[i]); 
			else if (sleeping[i]->prio == VM_THREAD_PRIORITY_NORMAL) 
				normalQueue.push(sleeping[i]); 
			else if (sleeping[i]->prio == VM_THREAD_PRIORITY_HIGH)
				highQueue.push(sleeping[i]);
			sleeping.erase(sleeping.begin() + i);//remove from the sleeping array because it is no longer sleeping
			i--;
		}
//need to reset i since it completely removed the object
		//globalTick--;//not used in multithreaded sleep
	}
	if (done ==1)
	{
		Schedule();
	}
}

void MyMachineFileCallback(void *calldata, int result){
	threadControlBlock *thr = (threadControlBlock*) calldata;
	thr->state = VM_THREAD_STATE_READY;
	thr->result = result;
	if (thr->prio == VM_THREAD_PRIORITY_LOW)
		lowQueue.push(thr);

	else if (thr->prio == VM_THREAD_PRIORITY_NORMAL)
		normalQueue.push(thr);

	else if (thr->prio == VM_THREAD_PRIORITY_HIGH)
		highQueue.push(thr);
	Schedule();
}

void skeleton(void * TCBref) {
	MachineEnableSignals();
	threadControlBlock* curTCB = (threadControlBlock *)TCBref;
	curTCB->entry(curTCB->param);//does it need the &?
	VMThreadTerminate(curTCB->ID);
}


void idle(void* TCBref)
{
        TMachineSignalState OldState;
		MachineEnableSignals();
        while(1)
        {
                MachineSuspendSignals(&OldState);
                MachineResumeSignals(&OldState);
        }
}



TVMStatus VMStart(int tickms, int machinetickms, int argc, char* argv[])
{
	//Variable definitions
	TVMStatus Status; 
	typedef void (*TVMMain)(int argc, char* argv[]); //Function pointer
	TVMMain VMMain;

	//Declare initial threads
	threadControlBlock *mainThread = new threadControlBlock;

	//Push threads onto vector of threads

  	mainThread->prio = VM_THREAD_PRIORITY_NORMAL; 
	idleThread->ID = 0; 
	mainThread->ID = 1;
	mainThread->state = VM_THREAD_STATE_RUNNING; 
	idleThread->state = VM_THREAD_STATE_DEAD;
	idleThread->entry = idle;
	MachineContextCreate(&idleThread->context, idle, NULL, idleThread->stack, 0x100000); 
	
	//threads.push_back(*idleThread);
	threads.push_back(idleThread);
	threads.push_back(mainThread);

	//Keep track of current thread 
	curThreadID = mainThread->ID; 
	
	//the things we have to call for signals to work
	MachineInitialize(machinetickms);
	MachineRequestAlarm(tickms*1000, AlarmCallback, NULL);
	MachineEnableSignals();
	
	//Set function pointer = return address of VMLoadModule()
	VMMain = VMLoadModule(argv[0]); 

	//Check if value returned is valid
	if(VMMain == NULL)
	{
		cout<<"error"<<endl;
		Status = VM_STATUS_FAILURE; 
	}
 
	//Call VMLoadModule()
	else
	{
		VMMain(argc, argv);
		Status = VM_STATUS_SUCCESS; 
	}

	return Status; 

}
TVMStatus VMFileOpen(const char *filename, int flags, int mode, int *filedescriptor){
	TVMStatus Status = VM_STATUS_SUCCESS;
	if (filename == NULL || filedescriptor == NULL) 
	{
		Status = VM_STATUS_ERROR_INVALID_PARAMETER; 
		return Status; 
	} 
	TMachineSignalState Oldstate; 
	MachineSuspendSignals(&Oldstate);
	threadControlBlock *myThread = threads[curThreadID];
	myThread->state = VM_THREAD_STATE_WAITING;
	MachineFileOpen(filename, flags, mode, MyMachineFileCallback, myThread);
	Schedule();
	*filedescriptor = myThread->result;

	if (!(myThread->result > 0))
		return VM_STATUS_FAILURE; 

	MachineResumeSignals(&Oldstate);
	return Status; 
}
TVMStatus VMFileClose(int filedescriptor){
	TVMStatus Status = VM_STATUS_SUCCESS;
	TMachineSignalState Oldstate; 
	MachineSuspendSignals(&Oldstate);
	threadControlBlock *myThread = threads[curThreadID];
	myThread->state = VM_THREAD_STATE_WAITING;
	MachineFileClose(filedescriptor, MyMachineFileCallback, myThread);
	Schedule();
	MachineResumeSignals(&Oldstate);
	return Status;
}
TVMStatus VMFileRead(int filedescriptor, void *data, int *length){
	TVMStatus Status = VM_STATUS_SUCCESS;

	if (data == NULL || length == NULL)
        {
                Status = VM_STATUS_ERROR_INVALID_PARAMETER;
                return Status;
        }




        TMachineSignalState Oldstate;
        MachineSuspendSignals(&Oldstate);

        threadControlBlock *myThread = threads[curThreadID];
        myThread->state = VM_THREAD_STATE_WAITING;
        MachineFileRead(filedescriptor, data, *length, MyMachineFileCallback, myThread);
        Schedule();
        *length = myThread->result;

	if (!(myThread > 0)) 
		return VM_STATUS_FAILURE; 



        return Status;
}
TVMStatus VMFileSeek(int filedescriptor, int offset, int whence, int *newoffset){
	TVMStatus Status = VM_STATUS_SUCCESS;
	TMachineSignalState Oldstate; 
	MachineSuspendSignals(&Oldstate);
	threadControlBlock *myThread = threads[curThreadID];
	myThread->state = VM_THREAD_STATE_WAITING;	
	MachineFileSeek(filedescriptor, offset, whence, MyMachineFileCallback, myThread);
	Schedule();
	*newoffset = myThread->result;

	if(!(myThread->result > 0))
		return VM_STATUS_FAILURE; 

	MachineResumeSignals(&Oldstate);
	return Status;
}
TVMStatus VMFileWrite(int fd, void *data, int *length)
{
	TVMStatus Status = VM_STATUS_SUCCESS; 
	if (data == NULL || length == NULL) 
	{
		Status = VM_STATUS_ERROR_INVALID_PARAMETER; 
		return Status; 
	}
	//write(fd, data, *length);
	//all stuff for when have multiple threads, write is fine for moving past hello
	TMachineSignalState Oldstate; 
	MachineSuspendSignals(&Oldstate);
	threadControlBlock *myThread = threads[curThreadID];
	myThread->state = VM_THREAD_STATE_WAITING;	
	MachineFileWrite(fd, data, *length, MyMachineFileCallback, myThread);//not sure about last two parameters
	Schedule();
	if(threads[curThreadID]->result > 0) 
		Status = VM_STATUS_SUCCESS; 
	else 
	{
		Status = VM_STATUS_FAILURE; 
		return Status;
	}
	
	MachineResumeSignals(&Oldstate);

	return Status; 
}

TVMStatus VMThreadCreate(TVMThreadEntry entry, void *param, TVMMemorySize memsize, TVMThreadPriority prio, TVMThreadIDRef tid){
	//memsize is 0x100000
	TVMStatus Status;

 	//Check if user entered valid TID and entry 
	if (tid == NULL || entry == NULL)
	{
		Status = VM_STATUS_ERROR_INVALID_PARAMETER;
		return Status; 
	}

	//Suspend signals
	TMachineSignalState Oldstate;
	MachineSuspendSignals(&Oldstate);

	threadControlBlock* TCB = new threadControlBlock;//initialize a new tcb

	//threads.push_back(*TCB); 
	*tid = threads.size(); 
	
	//Initialize new Thread variables	
	TCB->ID = threads.size();//counting will start at 1
	TCB->param = param;
	TCB->entry = entry;
	TCB->memsize = memsize; 
	TCB->prio = prio;  
	threads.push_back(TCB);
	//Push new Thread into vector
	//You will want to do thread create first, this will essentially just create a TCB, initialize it and call the MachineContextCreate
	//MachineContextCreate(&TCB.context, skeleton, param, TCB.stack, memsize);
	MachineResumeSignals(&Oldstate);
	Status = VM_STATUS_SUCCESS;	
	return Status;
}

TVMStatus VMThreadDelete(TVMThreadID thread);

TVMStatus VMThreadActivate(TVMThreadID thread)
{
	TVMStatus Status;
	
	int found = 0; 
 
        //Suspend signals
	TMachineSignalState Oldstate;
	MachineSuspendSignals(&Oldstate);

	//iterate through the vector till we find the thread with the ID
	for(it = threads.begin(); it < threads.end(); it++) 
	{
		if((*it)->ID == thread) 
		{
			found = 1; 
			break;
		}
	}


	//Thread is found
	if (found == 1)
	{
		//Make sure state of thread is dead
		if ((*it)->state != VM_THREAD_STATE_DEAD)
		{
			Status = VM_STATUS_ERROR_INVALID_STATE;
			return Status;
		}
	}

	//Thread not found
	else 
	{
		Status = VM_STATUS_ERROR_INVALID_ID;
		return Status; 
	}
    //(*it)->context = new SMachineContext();	
	MachineContextCreate(&(*it)->context, skeleton, (*it), (*it)->stack, (*it)->memsize);
	//Set thread state to ready 
	(*it)->state = VM_THREAD_STATE_READY;

	//Push thread onto appropriate queue based on priority 
	if ((*it)->prio == VM_THREAD_PRIORITY_LOW)  
		lowQueue.push((*it)); 
	
	else if ((*it)->prio == VM_THREAD_PRIORITY_NORMAL) 
		normalQueue.push((*it)); 
	
	else if ((*it)->prio == VM_THREAD_PRIORITY_HIGH)
		highQueue.push((*it)); 

	//readyQueue.push_back(threads[pos]);
	//need to do all the priority stuff after this line, scheduler function?
	if((*it)->prio > threads[curThreadID]->prio){//if activated thread a higher priority than running thread, switch immediately
		Schedule();
	}
	MachineResumeSignals(&Oldstate);
	
	Status = VM_STATUS_SUCCESS;
	return Status;
}


TVMStatus VMThreadTerminate(TVMThreadID thread)
{
	TVMStatus Status = VM_STATUS_SUCCESS; 
	int found = 0; 
	TMachineSignalState Oldstate;
	MachineSuspendSignals(&Oldstate);
	
	for(it = threads.begin(); it < threads.end(); it++)
	{
		if((*it)->ID == thread)
		{
			found = 1; 
			break;
		}
	}

	if (!found)
	{ 
		Status = VM_STATUS_ERROR_INVALID_ID; 
		return Status; 
	}


	if ((*it)->state == VM_THREAD_STATE_DEAD)
	{
		Status = VM_STATUS_ERROR_INVALID_STATE; 
		return Status; 
	}

	else 
		(*it)->state = VM_THREAD_STATE_DEAD; 

	Schedule();
	MachineResumeSignals(&Oldstate);
	return Status; 


}

;
TVMStatus VMThreadID(TVMThreadIDRef threadref);

TVMStatus VMThreadState(TVMThreadID thread, TVMThreadStateRef stateref){
	//If it doesn't exist in the list, then you would return the error. 
	//So if the specified ID ish  greater than the size of your list, or the location in your list is NULL.
	//returns VM_STATUS_ERROR_INVALID_ID if the thread identifier does not exist
	TVMStatus Status = VM_STATUS_SUCCESS; 

	if (stateref == NULL) 
		return VM_STATUS_ERROR_INVALID_PARAMETER; 

	int found = 0;
	int pos = 0;
	TMachineSignalState Oldstate;
	MachineSuspendSignals(&Oldstate);

	
	
	for (unsigned int i = 0; i < threads.size(); i++)
	{
		if (thread == threads[i]->ID)
		{
			found = 1;
			pos = (int)i;
		}
	}
	
	if (found == 0)//if found == 0; then thread doesnt exist and return error
	{
		cout<<"error"<<endl;
		Status = VM_STATUS_ERROR_INVALID_ID;
	}
	
	*stateref = threads[pos]->state;//change the state of selected thread
	
	//switch statement to handle the different states?
	MachineResumeSignals(&Oldstate);	
	return Status;
}

TVMStatus VMThreadSleep (TVMTick tick){
	/*1) Setting current thread->remainingTicks = tick
	2) Setting currentThread->state = WAITING
	3) Pushing currentThread to sleeping list
	4) Calling scheduler*/
	TMachineSignalState Oldstate;
	MachineSuspendSignals(&Oldstate);
	TVMStatus Status = VM_STATUS_SUCCESS;


	if (tick == VM_TIMEOUT_INFINITE) 
		return VM_STATUS_ERROR_INVALID_PARAMETER; 

	for(it = threads.begin(); it < threads.end(); it++)
	{
		if((*it)->ID == curThreadID)
		{                       
			break;
		}
	}

	
	(*it)->ticks = tick;
	(*it)->state = VM_THREAD_STATE_WAITING;
	sleeping.push_back(*it);
	//do 4 here to get a new thread scheduled
	Schedule();
	MachineResumeSignals(&Oldstate);
	return Status;
}

TVMStatus VMMutexDelete(TVMMutexID mutex);
TVMStatus VMMutexQuery(TVMMutexID mutex, TVMThreadIDRef ownerref);

	
TVMStatus VMMutexAcquire (TVMMutexID mutex, TVMTick timeout)
{
        TVMStatus Status = VM_STATUS_SUCCESS;
        TMachineSignalState Oldstate;
        MachineSuspendSignals(&Oldstate);

	//Let current thread acquire the mutex
        if (timeout == VM_TIMEOUT_IMMEDIATE && mutexs[mutex]->locked == 0)
        {
                mutexs[mutex]->owner = curThreadID;
                return VM_STATUS_SUCCESS;
        }

        else if (timeout == VM_TIMEOUT_IMMEDIATE && mutexs[mutex]->locked == 1)
        {
                return VM_STATUS_FAILURE;
        }

	//Wait until the mutex is released
        else if (timeout == VM_TIMEOUT_INFINITE && mutexs[mutex]->locked == 1)
        {
                threads[curThreadID]->state = VM_THREAD_STATE_WAITING;

                if (threads[curThreadID]->prio == VM_THREAD_PRIORITY_LOW)
                        mutexs[mutex]->low.push(threads[curThreadID]);

                else if (threads[curThreadID]->prio == VM_THREAD_PRIORITY_NORMAL)
                        mutexs[mutex]->medium.push(threads[curThreadID]);

                else if (threads[curThreadID]->prio == VM_THREAD_PRIORITY_HIGH)
                        mutexs[mutex]->high.push(threads[curThreadID]);

                Schedule();
        }

	else  
	{
		//cout<<"work"<<endl;
		threads[curThreadID]->ticks = timeout; 
		threads[curThreadID]->state = VM_THREAD_STATE_WAITING; 
		sleeping.push_back(threads[curThreadID]); 
		Schedule();
	}

	MachineResumeSignals(&Oldstate); 

        return Status;


}


TVMStatus VMMutexRelease(TVMMutexID mutex){
     TMachineSignalState Oldstate;
        MachineSuspendSignals(&Oldstate);
        TVMStatus Status = VM_STATUS_SUCCESS;
       
        threadControlBlock* newThread = new threadControlBlock;//not sure if necessary but included them
        threadControlBlock* currentThread = new threadControlBlock; 
	currentThread = threads[curThreadID];
       
        //mutexs[mutex]->state = VM_THREAD_STATE_READY;
       
        mutexs[mutex]->locked = 0;//unlock mutex
        mutexs[mutex]->owner = NULL;//no owner


	//search for thread with highest priority that's waiting for mutex
        if (mutexs[mutex]->high.empty())
        {
                        if (mutexs[mutex]->medium.empty())
                        {
                                        if(mutexs[mutex]->low.empty())
                                        {
                                                newThread = idleThread;
                                        }
                                        else
                                        {
                                                newThread = mutexs[mutex]->low.front();
                                               
 mutexs[mutex]->low.pop();
	

                                        }
                        }
                        else
                        {
                                newThread = mutexs[mutex]->medium.front();
                                mutexs[mutex]->medium.pop();
                        }
        }

	 else 
	{
                newThread = mutexs[mutex]->high.front();
                mutexs[mutex]->high.pop();
        }


	newThread->state = VM_THREAD_STATE_READY;

	mutexs[mutex]->owner = newThread->ID;

	if (newThread->prio == VM_THREAD_PRIORITY_LOW)
		lowQueue.push(newThread); 
	
	else if (newThread->prio == VM_THREAD_PRIORITY_NORMAL)
		normalQueue.push(newThread); 
		
	else if (newThread->prio == VM_THREAD_PRIORITY_HIGH)
		highQueue.push(newThread); 


	//schedule if priority higher than current thread
	if (newThread->prio > currentThread->prio)
	{

        	if (currentThread->prio == VM_THREAD_PRIORITY_LOW)
                	lowQueue.push(currentThread);

        	else if (currentThread->prio == VM_THREAD_PRIORITY_NORMAL)
                	normalQueue.push(currentThread);

        	else if (currentThread->prio == VM_THREAD_PRIORITY_HIGH)
                	highQueue.push(currentThread);



		MachineResumeSignals(&Oldstate); 




		Schedule();  

		Status = VM_STATUS_SUCCESS; 
	}

	else
	{
		MachineResumeSignals(&Oldstate); 
	}

		

	return Status; 

}
}	 
