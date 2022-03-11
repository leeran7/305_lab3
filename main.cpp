//
//  main.cpp
//  lab3
//
//  Created by Leeran Farin on 3/2/22.
//

#include "process.h"

int main(int argc, const char * argv[]) {
    int processCount = NULL;
    while(!processCount || processCount < 1){
        cout << "How many processes would you like to create: ";
        cin >> processCount;
    }
    int partitionCount = NULL;
    while(!partitionCount || partitionCount < 1){
        cout << "How many partitions would you like to create: ";
        cin >> partitionCount;
    }
    Memory* memory = new Memory(processCount, partitionCount);
//    memory->bestFit();
//    memory->firstFit();
//    memory->nextFit();
    memory->worstFit();
    return 0;
}
