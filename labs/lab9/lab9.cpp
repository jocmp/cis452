/*
 * Author: Josiah Campbell & Kevin Tarquinio
 * Version: Winter 2016
 * FIle lab9.cpp
 */
#include "latlng.h"
#include <iostream>
#include <iomanip>
using namespace std;

// Global
int global_x = 128;
int global_y;

int function1() {
    int result = 48;
    return result;
}

int main() {
    latlng* allendale = new latlng;
    int local_array[10] = {0};
    // Initialized
    cout << "Data: Global initialized address = " << &global_x << endl;
    // Uninitialized
    cout << "Data: Global uninitialized address = " << &global_y << endl;
    // Local
    int x = 10;
    int y;
    cout << "Stack: Local initialized var = " << &x << endl;
    cout << "Stack: local uninitialized var = " << &y << endl;
    cout << "Stack: Local array = " << hex << local_array << endl;
    // Pointers
    cout << "Heap: Local pointer address of latlng longitude = "
         << hex << &allendale->longitude << endl;
    // Structures
    cout << "Heap: struct latlng  = " << hex << (void*) allendale << endl;
    // Function
    cout << "Code: Funtion \"function1\" = " 
         << hex << (void*) function1 << endl;
}
