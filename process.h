#ifndef process_h
#define process_h
#include <iomanip>
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
    int partitionCount;
    
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
        cout << endl;
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
    void setProcessAndPartition(int procId, int partId, int currWaste){
        processes[procId].partitionWaste = currWaste;
        processes[procId].partitionId = partId;
        processes[procId].status = "Running";
        partitions[partId].assignedPid = procId;
        partitions[partId].pRunning = "Running";
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
        cout << setw(45) << "Partition Data" << endl << endl;
        cout << setw(15)  << "PART PID" << setw(15) << "PID" << setw(15) << "PROC SIZE" << setw(15) << "PROC STATUS" << endl;
        for(int i = 0; i < mem.partitionCount; i++){
            cout << setw(15)  << mem.partitions[i].partitionid;
            cout << setw(15) << mem.partitions[i].assignedPid;
            cout << setw(15) << mem.partitions[i].size;
            cout << setw(15) << mem.partitions[i].pRunning;
            
            
            cout << endl;
        }
        cout << endl;
        int totalWaste = 0;
        cout << setw(43) << "Process Data" << endl << endl;
        cout << setw(15) << "PID" << setw(15) << "PART PID" << setw(15) << "PROC SIZE" << setw(15) << "PART WASTE" << setw(15) << "PROC STATUS" << endl;
        for(int i = 0; i < mem.processCount; i++){
            cout << setw(15) << mem.processes[i].pid;
            cout << setw(15) << mem.processes[i].partitionId;
            cout << setw(15) << mem.processes[i].processSize;
            cout << setw(15) << mem.processes[i].partitionWaste;
            cout << setw(15) << mem.processes[i].status;
            totalWaste += mem.processes[i].partitionWaste;
            cout << endl;
        }
        cout << setw(59) << "TOTAL WASTE: " << totalWaste << endl << endl;
        return cout;
    }

    void firstFit(){
        cout << setw(50)<< "<<<<<<<<<< FIRST FIT >>>>>>>>>>" << endl;
        int const initialWaste = 99999999;
        for(int procId = 0; procId < processCount; procId++){
            for(int partId = 0; partId < partitionCount; partId++){
                int currWaste = initialWaste;
                if(partitions[partId].size >= processes[procId].processSize){
                    currWaste = partitions[partId].size - processes[procId].processSize;
                    if(partitions[partId].pRunning == "Waiting" && processes[procId].status == "Waiting"){
                        processes[procId].partitionWaste = currWaste;
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
        cout << setw(50) << "<<<<<<<<<< BEST FIT >>>>>>>>>>" << endl;
        int const initialWaste = 99999999;
        int bestWaste = initialWaste;
        for(int procId = 0; procId < processCount; procId++){
            int currWaste = initialWaste;
            bestWaste = initialWaste;
            int bestIdx = NULL;
            currWaste = initialWaste;
            
            for(int partId = 0; partId < partitionCount; partId++){
                currWaste = partitions[partId].size - processes[procId].processSize;
                if(currWaste >= 0 && partitions[partId].pRunning == "Waiting" && currWaste <= bestWaste){
                    bestWaste = currWaste;
                    bestIdx = partId;
                }
            }
            if(partitions[bestIdx].pRunning == "Waiting" && processes[procId].status == "Waiting"){
                processes[procId].partitionWaste = bestWaste;
                processes[procId].partitionId = bestIdx;
                processes[procId].status = "Running";
                partitions[bestIdx].assignedPid = procId;
                partitions[bestIdx].pRunning = "Running";
            }
        }
        cout << *this;
        resetMemory();
    };
    void nextFit(){
        cout << setw(50)  << "<<<<<<<<<< NEXT FIT >>>>>>>>>>" << endl;
        int const initialWaste = 99999999;
        for(int procId = 0; procId < processCount; procId++){
            int currWaste = initialWaste;
            bool nextFit = false;
            for(int partId = 0; partId < partitionCount; partId++){
                if(partitions[partId].size >= processes[procId].processSize){
                    if(nextFit){
                        currWaste = partitions[partId].size - processes[procId].processSize;
                        if(partitions[partId].pRunning == "Waiting" && processes[procId].status == "Waiting"){
                            processes[procId].partitionWaste = currWaste;
                            processes[procId].partitionId = partId;
                            processes[procId].status = "Running";
                            partitions[partId].assignedPid = procId;
                            partitions[partId].pRunning = "Running";
                        }
                    } else {
                        nextFit = true;
                        partId = 0;
                        
                    }
                    
                }
            }
            
        }
        cout << *this;
        resetMemory();
    };
    void worstFit(){
        cout << setw(50) << "<<<<<<<<<< WORST FIT >>>>>>>>>>" << endl;
        int const initialWaste = NULL;
        int worseWaste = initialWaste;
        for(int procId = 0; procId < processCount; procId++){
            int currWaste = initialWaste;
            worseWaste = initialWaste;
            int worstIdx = NULL;
            currWaste = initialWaste;
            
            for(int partId = 0; partId < partitionCount; partId++){
                currWaste = partitions[partId].size - processes[procId].processSize;
                cout << "PartId: " << partId << endl;
                cout << "ProcId: " << procId << endl;
                cout << "CurrWaste: " << currWaste << endl;
                cout << "WorstWaste: " << worseWaste << endl;
                if(currWaste >= 0 && partitions[partId].pRunning == "Waiting" && currWaste >= worseWaste){
                    worseWaste = currWaste;
                    worstIdx = partId;
                }
            }
            if(partitions[worstIdx].pRunning == "Waiting" && processes[procId].status == "Waiting"){
                processes[procId].partitionWaste = worseWaste;
                processes[procId].partitionId = worstIdx;
                processes[procId].status = "Running";
                partitions[worstIdx].assignedPid = procId;
                partitions[worstIdx].pRunning = "Running";
            }
        }
        cout << *this;
        resetMemory();
    };
};

#endif /* process_h */




