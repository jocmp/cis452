#include <Windows.h>
#include <iostream>
#include <string>
#define MB 1024*1024

using namespace std;


string get_readable_state(MEMORY_BASIC_INFORMATION &info) {
        if (info.State == 0x1000)
                return "MEM_COMMIT";
        if (info.State == 0x10000)
                return "MEM_FREE";
        if (info.State == 0x2000)
                return "MEM_RESERVE";
        return "invalid";
}

int main(){

        MEMORY_BASIC_INFORMATION memInfo;
        SYSTEM_INFO systemInfo;
        int *intPtr;

        GetSystemInfo(&systemInfo);
        /* Print page size and allocates 1 MB */
        cout << "The page size is: " << systemInfo.dwPageSize << endl;

        intPtr = (int*) malloc(1 * MB * sizeof(int));
        /* Query after malloc */
        VirtualQuery(intPtr, &memInfo, sizeof(memInfo));

        cout << "Memory state, malloc: " << get_readable_state(memInfo) << endl;

        free(intPtr);
        /* Query after free */
        VirtualQuery(intPtr, &memInfo, sizeof(memInfo));
        /* Print memory after free */
        cout << "Memory state, freed: " << get_readable_state(memInfo) << endl;
        /* Allow time to view memory states */
        Sleep(10000);

        return 0;
}
