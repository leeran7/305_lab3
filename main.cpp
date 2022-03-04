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
    Memory* memory = new Memory(processCount);
    memory->firstFit();
    return 0;
}
