#include <iostream>
#include <string>
#include <cstring>
#include "dl.h"
#include "mem_manager.h"
using std::cout, std::cin, std::endl, std::string;

int main()
{
    MemoryManager manager(100);
    manager.malloc(5);
    manager.malloc(3);
    manager.malloc(7);
    manager.malloc(12);
    manager.malloc(4);
    manager.segList.print_list();
    cout << endl;
    manager.free(8);
    manager.free(5);
    manager.free(0);
    manager.free(4);
    manager.segList.print_list();
    cout << endl;
    manager.free(15);
    manager.free(27);
    manager.malloc(5);
    manager.malloc(7);
    manager.malloc(12);
    manager.free(5);
    manager.malloc(10);
    manager.malloc(3);
    manager.malloc(15);
    manager.segList.print_list();
    cout << endl;
    manager.free(0);
    manager.free(12);
    manager.free(24);
    manager.free(34);
    manager.free(37);
    manager.segList.print_list();
    cout << endl;

    memory_segment* segment;
    int i = 1;
    while (segment != nullptr) {
        segment = manager.malloc(i);
        i++;
    }
    manager.malloc(9);
    manager.segList.print_list();
    cout << endl;

    int sum = 78;
    i = 12;
    segment = manager.segList.back();
    while (segment != nullptr) {
        segment = manager.free(sum);
        sum -= i;
        i--;
    }
    manager.free(91);
    manager.segList.print_list();
    manager.free_list();
    manager.free_heap();
    return 0;
}