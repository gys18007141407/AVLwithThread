/*
 * *@*	Description: 
 * *@*	Version: 
 * *@*	Author: yusheng Gao
 * *@*	Date: 2021-06-22 14:30:20
 * *@*	LastEditors: yusheng Gao
 * *@*	LastEditTime: 2021-06-25 14:16:09
 */

#include "custom_allocator.hpp"
#include "custom_AVL.hpp"
#include <vector>
#include <list>
#include <deque>
#include <iostream>
using namespace customTool;
using namespace std;

struct data{
    int a = 6;
    double b = 1.234;
    char c = 'k';
};

int main(){


    CLAllocator<data, 128> alloc;

    for(int i = 0; i < 20; ++i){
        auto p = alloc.newObj();

        cout << p->a << " " << p->b << " " << p->c << endl;

        alloc.deleteObj(p);

    }
    
    list<int, CLAllocator<int, 64>> v;

    //vector<int, std::allocator<int>> v;  //  vector will realloc when size reach its capacity, so it may cause error

    for(int i = 0; i < 50; ++i) v.push_back(i);

    cout << endl;
    for(auto & a : v) cout << a << "\t";
    cout << endl;

    for(int i = 0; i < 50; i += 3) v.pop_front();

    cout << endl;
    for(auto& a : v) cout << a << "\t";
    cout << endl;

#ifdef _WIN32
    system("pause");
#endif

    return 0;
}