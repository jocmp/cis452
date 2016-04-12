/*
 * Author: Josiah Campbell & Kevin Tarquinio
 * Version: Winter 2016
 * FIle lab9.cpp
 */
#include "latlng.h"
#include <iostream>
#include <iomanip>
#include <unistd.h>

using namespace std;

// Global
int global_x = 128;
int global_y;

int function1() {
    int result = 48;
    int a = 8;
    int b = 8;
    int c = 8;
    int d = 16;
    
    cout << "\nStack: Local initialized var a = " << &a << endl;
    cout << "Stack: Local initialized var b = " << &b << endl;
    cout << "Stack: Local initialized var c = " << &c << endl;
    cout << "Stack: Local initialized var d = " << &d << "\n" << endl;
    return result;
}

int main(int argc, char **argv) {
    // Function
    cout << "\nCode: Funtion \"main\" = " 
        << hex << (void*) main << endl;
    cout << "Code: Funtion \"function1\" = " 
        << hex << (void*) function1 << endl;
    latlng* allendale = new latlng;
    int local_array[10] = {0};
    // Initialized
    cout << "\nData: Global initialized address = " << &global_x << endl;
    // Uninitialized
    cout << "Data: Global uninitialized address = " << &global_y << endl;
    // Pointers
    cout << "\nHeap: Local pointer address of latlng longitude = "
         << hex << &allendale->longitude << endl;
    // Structures
    cout << "Heap: struct latlng  = " << hex << (void*) allendale << endl;
    // Local
    int x = 10;
    int y;
    
    cout << "Stack: This is the 1st argument: " << hex << &argv[2] << endl;
    cout << "Stack: This is the 2nd argument: " << hex << &argv[3] << endl;
    
    cout << "\nStack: Local initialized var = " << &x << endl; 
    cout << "Stack: local uninitialized var = " << &y << endl;
    cout << "Stack: Local array = " << hex << local_array << endl;
    
    delete allendale;

    function1();

    return 0;
}
