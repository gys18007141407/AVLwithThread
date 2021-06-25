/*
 * *@*	Description: 
 * *@*	Version: 
 * *@*	Author: yusheng Gao
 * *@*	Date: 2021-06-23 21:46:03
 * *@*	LastEditors: yusheng Gao
 * *@*	LastEditTime: 2021-06-25 14:32:48
 */


#include "custom_AVL.hpp"
#include "custom_allocator.hpp"

#include <cstring>
#include <set>
#include <ctime>
using namespace customTool;
using namespace std;

#define MAXLOOPS (1000000)

class KEY{
public:
    KEY(): w(0), val(0){  }

    ~KEY() = default;

    KEY(int w_, int val_, const char* id_) : w(w_), val(val_){
        memcpy(id, id_, min(int(strlen(id_))+1, 20));
    }   

    friend std::ostream& operator<<(std::ostream&, const KEY&);

    bool operator<(const KEY& other) const {
        if(w*val < other.w*other.val) return true;

        if(w*val > other.w*other.val) return false;

        if(w < other.w) return true;

        if(w > other.w) return false;

        if(val < other.val) return true;

        return false;
        
    }

private:

    int w;
    int val;
    char id[20];

};

inline std::ostream& operator<<(std::ostream& _out, const KEY& _t){

    std::cout << "w=" << _t.w << "\tval=" << _t.val << "\tid=" << _t.id << std::endl;

    return _out;
}

int main(){

    // CLAVLTree<int, CLAllocator<int, 128>> avl(true);

    // avl.insert(3), avl.showInOrderTraverse(), avl.showInOrderBF();
    // avl.insert(2), avl.showInOrderTraverse(), avl.showInOrderBF();
    // avl.insert(4), avl.showInOrderTraverse(), avl.showInOrderBF();
    // avl.insert(6), avl.showInOrderTraverse(), avl.showInOrderBF();
    // avl.insert(24), avl.showInOrderTraverse(), avl.showInOrderBF();
    // avl.insert(4), avl.showInOrderTraverse(), avl.showInOrderBF();
    // avl.insert(16), avl.showInOrderTraverse(), avl.showInOrderBF();
    // avl.insert(9), avl.showInOrderTraverse(), avl.showInOrderBF();

    // avl.insert(0), avl.showInOrderTraverse(), avl.showInOrderBF();
    // avl.insert(1), avl.showInOrderTraverse(), avl.showInOrderBF();
    // avl.insert(2), avl.showInOrderTraverse(), avl.showInOrderBF();


    // for(int i = 0; i < 10; ++i) avl.insert(i), avl.showInOrderTraverse(), avl.showInOrderBF();

    // for(int i = 0; i < 10; ++i) avl.insert(i), avl.showInOrderTraverse(), avl.showInOrderBF();
    // for(int i = 0; i < 30; ++i) std::cout << i << " count : " << avl.count(i) << std::endl;
    // for(int i = 0; i < 30; ++i) std::cout << i << " lower : " << (avl.lower_bound(i) ? avl.lower_bound(i)->val : 0) << std::endl;
    // for(int i = 0; i < 30; ++i) std::cout << i << " upper : " << (avl.upper_bound(i) ? avl.upper_bound(i)->val : 0) << std::endl;

    // avl.erase(0), avl.showInOrderTraverse(), avl.showInOrderBF();
    // avl.erase(3), avl.showInOrderTraverse(), avl.showInOrderBF();
    // avl.erase(5), avl.showInOrderTraverse(), avl.showInOrderBF();

    // avl.eraseAllValue(4), avl.showInOrderTraverse(), avl.showInOrderBF();

    // for(int i = 2; i < 20; i += 3) avl.erase(i), avl.showInOrderTraverse(), avl.showInOrderBF();

    // auto ge = avl.lower_bound(9);
    // auto gr = avl.upper_bound(9);

    // if(ge) std::cout << ge->val << std::endl;
    // else std::cout << "lower_bound not exist!" << std::endl;

    // if(gr) std::cout << gr->val << std::endl;
    // else std::cout << "upper_bound not exist!" << std::endl;

    // for(int i = 0; i < 20; ++i) if(avl.exist(i)) printf("%d is existed\n", i);


    // CLAVLTree<KEY, CLAllocator<KEY, 1024>> avl2(true);

    // avl2.insert({1,3,"1"});
    // avl2.insert({2,1,"2"});
    // avl2.insert({2,2,"3"});
    // avl2.insert({2,3,"4"});
    // avl2.insert({4,6,"5"});
    // avl2.insert({4,1,"6"});
    // avl2.insert({4,4,"7"});
    // avl2.insert({3,3,"8"});
    // avl2.insert({3,5,"9"});

    // avl2.insert({});

    // avl2.showInOrderTraverse();
    // avl2.showInOrderBF();

    std::multiset<int> s;
    // std::set<int> s;

    auto t = clock();
    for(int i = 0; i < MAXLOOPS; ++i) s.insert(i);
    for(int i = 0; i < MAXLOOPS; ++i) s.insert(i);
    t = clock()-t;

    std::cout << "c11 default set insert cost " << 1.0*t/CLOCKS_PER_SEC << "s" << std::endl;

    t = clock();
    for(int i = 0; i < MAXLOOPS; ++i) s.find(i);
    for(int i = 0; i < MAXLOOPS; ++i) s.find(i);
    t = clock()-t;

    std::cout << "c11 default set find cost " << 1.0*t/CLOCKS_PER_SEC << "s" << std::endl;

    t = clock();
    for(int i = 0; i < MAXLOOPS; ++i) s.count(i);
    t = clock()-t;

    std::cout << "c11 default set count cost " << 1.0*t/CLOCKS_PER_SEC << "s" << std::endl;

    t = clock();
    for(int i = 0; i < MAXLOOPS; ++i) s.erase(i);
    for(int i = 0; i < MAXLOOPS; ++i) s.erase(i);
    t = clock()-t;

    std::cout << "c11 default set erase cost " << 1.0*t/CLOCKS_PER_SEC << "s" << std::endl;

    CLAVLTree<int, CLAllocator<int, 409600>> setk;

    t = clock();
    for(int i = 0; i < MAXLOOPS; ++i) setk.insert(i);
    for(int i = 0; i < MAXLOOPS; ++i) setk.insert(i);
    t = clock()-t;

    std::cout << "my customed setk insert cost " << 1.0*t/CLOCKS_PER_SEC << "s" << std::endl;

    t = clock();
    for(int i = 0; i < MAXLOOPS; ++i) setk.exist(i);
    for(int i = 0; i < MAXLOOPS; ++i) setk.exist(i);
    t = clock()-t;

    std::cout << "my customed setk find cost " << 1.0*t/CLOCKS_PER_SEC << "s" << std::endl;

    t = clock();
    for(int i = 0; i < MAXLOOPS; ++i) setk.count(i);
    t = clock()-t;

    std::cout << "my customed setk count cost " << 1.0*t/CLOCKS_PER_SEC << "s" << std::endl;

    t = clock();
    for(int i = 0; i < MAXLOOPS; ++i) setk.erase(i);
    for(int i = 0; i < MAXLOOPS; ++i) setk.erase(i);
    t = clock()-t;
  
    std::cout << "my customed setk erase cost " << 1.0*t/CLOCKS_PER_SEC << "s" << std::endl;

#ifdef _WIN32
    system("pause");
#endif

    return 0;
}