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
    int partitionId = -1;
    int partitionWaste;
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
    int partitionCount = NULL;
    
public:
    Memory(int processCount, int partitionCount){
        this->processCount = processCount;
        this->partitionCount = partitionCount;
        partitions = new Partition[partitionCount];
        processes = new Process[processCount];
        processSizes = new int[processCount];
        partitionSizes = new int[partitionCount];
        initPartitions();
        cout << endl << "Now, lets initialize the process sizes...";
        initProcesses();
    }
    void resetMemory(){
        delete[] partitions;
        delete[] processes;
        partitions = new Partition[partitionCount];
        processes = new Process[processCount];
        for(int i = 0; i < partitionCount; i++){
            partitions[i].size =  partitionSizes[i];
            partitions[i].partitionid = i;
        }
        for(int i = 0; i < processCount; i++){
            processes[i].constructor(i, processSizes[i]);
        }
    }
    void initPartitions(){
        int sizeOfPartitions = NULL;
        string errorMsg = "";
        for(int i = 0; i < partitionCount; i++){
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
        Memory* value = new Memory(mem.processCount, mem.partitionCount);
        value->processCount = this->processCount;
        value->partitionCount = this->partitionCount;
        for(int i = 0; i < processCount; i++){
            value->processes[i].constructor(i, processes[i].processSize);
        }
        for(int i = 0; i < partitionCount; i++){
            value->partitions[i].constructor(i, partitions[i].size);
        }
        return *value;
    }
    friend ostream& operator<<(ostream& cout, const Memory& mem){
        cout << "\n                      Partition Data                    \n" << endl;
        cout << "PART ID        PID             PART SIZE         PART STATUS" << endl;
        for(int i = 0; i < mem.partitionCount; i++){
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
        cout << "PID            PART PID        PROC SIZE        PART WASTE        PROC STATUS" << endl;
        for(int i = 0; i < mem.processCount; i++){
            cout << mem.processes[i].pid;
            cout << "                 ";
            cout << mem.processes[i].partitionId;
            cout << "                 ";
            cout << mem.processes[i].processSize;
            cout << "             ";
            cout << mem.processes[i].partitionWaste;
            cout << "             ";
            cout << mem.processes[i].status;
            
            
            cout << endl ;
        }
        return cout;
    }
    void sortPartitions(){
        Partition* tempPartitions = new Partition[processCount];
        for(int i = 0; i < processCount; i++){
            int smallestId;
            for(int partId = 0; partId < processCount; partId++){
                if(!smallestId){
                    smallestId = partId;
                } else if(partitions[smallestId].size > partitions[partId].size){
                    smallestId = partId;
                }
            }
            tempPartitions[i].size = partitions[smallestId].size;
            tempPartitions[i].partitionid = partitions[smallestId].partitionid;
            tempPartitions[i].pRunning = partitions[smallestId].pRunning;
            tempPartitions[i].assignedPid = partitions[smallestId].assignedPid;
        }
        delete[] partitions;
        partitions = tempPartitions;
    }
    void sortProcesses(){
        
    }
    void firstFit(){
        for(int procId = 0; procId < processCount; procId++){
            for(int partId = 0; partId < partitionCount; partId++){
                if(partitions[partId].size >= processes[procId].processSize){
                    if(partitions[partId].pRunning == "Waiting" && processes[procId].status == "Waiting"){
                        processes[procId].partitionId = partId;
                        processes[procId].status = "Running";
                        partitions[partId].assignedPid = procId;
                        partitions[partId].pRunning = "Running";
                    }
                }
            }
        }
        cout << *this;
        resetMemory();
    };
    
    void bestFit(){
        int bestWaste = 99999999;
        for(int procId = 0; procId < processCount; procId++){
            int currWaste = 9999999;
            int bestIdx = NULL;
            
            for(int partId = 0; partId < partitionCount; partId++){
                currWaste = partitions[partId].size - processes[procId].processSize;
                if(currWaste >= 0 && partitions[partId].pRunning == "Waiting"){
                    bestWaste = currWaste <= bestWaste ? currWaste : bestWaste;
                    bestIdx = currWaste <= bestWaste ? partId : bestIdx;
//                    cout << "IF STATEMENT: " << endl;
//                    cout << "PartId: " << partId << endl;
//                    cout << "ProcIdL " << procId << endl;
                }
//                cout << "OUTSIDE IF STATEMENT: " << endl;
//                cout << "PartId: " << partId << endl;
//                cout << "Current Waste: " << currWaste << endl;
//                cout << "ProcId: " << procId << endl;
//                cout << "Best Waste: " << bestWaste << endl;
//                cout << "Best Idx: " << bestIdx << endl;
//                cout << endl << endl;
//                cout << procId << ": Best Idx: " << bestIdx << endl;
            }
            if(partitions[bestIdx].pRunning == "Waiting" && processes[procId].status == "Waiting"){
                processes[procId].partitionWaste = bestWaste;
                processes[procId].partitionId = bestIdx;
                processes[procId].status = "Running";
                partitions[bestIdx].assignedPid = procId;
                partitions[bestIdx].pRunning = "Running";
            }

            bestIdx = NULL;
            currWaste = 9999999;
        }
        cout << *this;
        cout << "Best Waste: " << bestWaste << endl;
        resetMemory();
    };
    void nextFit(){};
    void worstFit(){};
};

#endif /* process_h */




