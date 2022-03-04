#ifndef process_h
#define process_h

#include <string>
#include <iostream>
using namespace std;

class Partition {
public:
    int partitionid;
    int size;
    int assignedPid= -1;
    string pRunning = "Waiting";
    
    void constructor(int id, int partitionSize){
        this->partitionid = id;
        this->assignedPid = -1;
        this->size = partitionSize;
    }
};

class Process {
public:
    int pid;
    int processSize;
    string status;
    int partitionId = NULL;
    void constructor(int id, int processSize){
        this->pid = id;
        this->status = "Waiting";
        this->processSize = processSize;
    }
};;

class Memory {
    int* processSizes;
    int* partitionSizes;
    Partition* partitions;
    Process* processes;
    int processCount;
    
public:
    Memory(int processCount){
        this->processCount = processCount;
        partitions = new Partition[processCount];
        processes = new Process[processCount];
        processSizes = new int[processCount];
        partitionSizes = new int[processCount];
        initPartitions();
        cout << endl << "Now, lets initialize the process sizes...";
        initProcesses();
    }
    void resetMemory(){
        delete[] partitions;
        delete[] processes;
        partitions = new Partition[processCount];
        processes = new Process[processCount];
        for(int i = 0; i < processCount; i++){
            partitions[i].size =  partitionSizes[i];
            partitions[i].partitionid = i;
            processes[i].constructor(i, processSizes[i]);
        }
    }
    void initPartitions(){
        int sizeOfPartitions = NULL;
        string errorMsg = "";
        for(int i = 0; i < processCount; i++){
            while(!sizeOfPartitions || sizeOfPartitions % 50 != 0){
                cout << errorMsg << endl;
                cout << "Partition size for index " << i <<  " (increments of 50): ";
                cin >> sizeOfPartitions;
                errorMsg = "Process size must be in increments of 50...";
            }
            partitionSizes[i] = sizeOfPartitions;
            partitions[i].size =  sizeOfPartitions;
            partitions[i].partitionid = i;
            sizeOfPartitions = NULL;
            errorMsg = "";
        }
    }
    void initProcesses(){
        int sizeOfProcess = NULL;
        string errorMsg = "";
        for(int i = 0; i < processCount; i++){
            while(!sizeOfProcess || sizeOfProcess % 50 != 0){
                cout << errorMsg << endl;
                cout << "Process size for index " << i <<  " (increments of 50): ";
                cin >> sizeOfProcess;
                errorMsg = "Process size must be in increments of 50...";
            }
            processSizes[i] = sizeOfProcess;
            processes[i].constructor(i, sizeOfProcess);
            errorMsg = "";
            sizeOfProcess = NULL;
        }
        
    }
    Memory operator=(const Memory& mem){
        Memory* value = new Memory(mem.processCount);
        value->processCount = this->processCount;
        for(int i = 0; i < processCount; i++){
            value->partitions[i].constructor(i, partitions[i].size);
            value->processes[i].constructor(i, processes[i].processSize);
        }
        return *value;
    }
    friend ostream& operator<<(ostream& cout, const Memory& mem){
        cout << "\n                      Partition Data                       \n" << endl;
        cout << "PART ID        PID             PART SIZE        PART STATUS" << endl;
        for(int i = 0; i < mem.processCount; i++){
            cout << mem.partitions[i].partitionid;
            cout << "                 ";
            cout << mem.partitions[i].assignedPid;
            cout << "                 ";
            cout << mem.partitions[i].size;
            cout << "             ";
            cout << mem.partitions[i].pRunning;
            
            
            cout << endl;
        }
        cout << "                      Process Data                       \n" << endl;
        cout << "PID            PART PID        PROC SIZE        PROC STATUS" << endl;
        for(int i = 0; i < mem.processCount; i++){
            cout << mem.processes[i].pid;
            cout << "                 ";
            cout << mem.processes[i].partitionId;
            cout << "                 ";
            cout << mem.processes[i].processSize;
            cout << "             ";
            cout << mem.processes[i].status;
            
            
            cout << endl ;
        }
        return cout;
    }
    
    void firstFit(){
        for(int procId = 0; procId < processCount; procId++){
            for(int partId = 0; partId < processCount; partId++){
                if(this->partitions[partId].size >= this->processes[procId].processSize){
                    if(this->partitions[partId].pRunning == "Waiting" && this->processes[procId].status == "Waiting"){
                        this->processes[procId].partitionId = partId;
                        this->processes[procId].status = "Running";
                        this->partitions[partId].assignedPid = procId;
                        this->partitions[partId].pRunning = "Running";
                    }
                }
            }
        }
        cout << *this;
        this->resetMemory();
    };
    void bestFit(){
        
    };
    void nextFit(){};
    void worstFit(){};
};

#endif /* process_h */




