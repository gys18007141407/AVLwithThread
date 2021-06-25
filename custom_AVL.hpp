/*
 * *@*	Description: 
 * *@*	Version: 
 * *@*	Author: yusheng Gao
 * *@*	Date: 2021-06-22 18:56:49
 * *@*	LastEditors: yusheng Gao
 * *@*	LastEditTime: 2021-06-25 14:02:41
 */



#ifndef CUSTOM_AVL_HPP
#define CUSTOM_AVL_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <cassert>

#include <iostream>
#include <cstdio>

#define AVL_ERROR (4)

namespace customTool{

template<typename _Tp>
class CLAVLNode{
public:

    typedef _Tp                 value_type;
    typedef _Tp*                pointer;
    typedef _Tp&                reference;
    typedef const _Tp&          const_reference;
    typedef const _Tp*          const_pointer;

    value_type  val;
    int8_t      bf;
    CLAVLNode*  left;
    CLAVLNode*  right;
    bool        lIsNode;
    bool        rIsNode;

    CLAVLNode():bf(0), left(nullptr), right(nullptr), lIsNode(false), rIsNode(false) { }
    CLAVLNode(value_type __val): val(__val), bf(0), left(nullptr), right(nullptr), lIsNode(false), rIsNode(false) { }
    ~CLAVLNode(){  }

    CLAVLNode(const CLAVLNode& other) = delete;
    CLAVLNode(CLAVLNode&& other) = delete;
    CLAVLNode& operator=(const CLAVLNode& other) = delete;
    CLAVLNode& operator=(CLAVLNode&& other) = delete;


    bool operator<(const CLAVLNode& rhs){ return val<rhs.val; }
    bool operator>(const CLAVLNode& rhs){ return rhs.val<val; }
    bool operator>=(const CLAVLNode& rhs){ return !(val<rhs.val); }
    bool operator<=(const CLAVLNode& rhs){ return !(rhs.val<val); }
    bool operator==(const CLAVLNode& rhs){ return !(val<rhs.val) && !(rhs.val<val); }
    bool operator!=(const CLAVLNode& rhs){ return val<rhs.val || rhs.val<val; }

};

template<typename _Tp, typename _Alloc>
class CLAVLTree{
public:

    typedef _Tp                 value_type;
    typedef _Tp*                pointer;
    typedef _Tp&                reference;
    typedef const _Tp&          const_reference;
    typedef const _Tp*          const_pointer;
    typedef CLAVLNode<_Tp>      TreeNode;

    typedef typename _Alloc::template rebind<TreeNode>::other Alloc;

    explicit CLAVLTree(bool __allowRepeat = false):root(nullptr), allowRepeat(__allowRepeat){ }

    CLAVLTree(const CLAVLTree& other) = delete;
    CLAVLTree& operator=(const CLAVLTree& other) = delete;
    CLAVLTree(CLAVLTree&& other);
    CLAVLTree& operator=(CLAVLTree&& other);

    bool insert(value_type __val);

    TreeNode* const lower_bound(value_type __val) const;
    TreeNode* const upper_bound(value_type __val) const;

    bool exist(value_type __val, TreeNode** __p = nullptr) const ;

    bool erase(value_type __val);

    bool eraseAllValue(value_type __val);

    std::size_t count(value_type __val) const;

public:

    void showInOrderTraverse() const;

    void showInOrderBF() const;

    value_type getSmallestVal() const { assert(root); return getSmallest()->val; }

private:

    TreeNode* root;
    Alloc alloctor;
    const bool allowRepeat;

private:
    
    int8_t insert(value_type __val, TreeNode** __root);

    int8_t erase(TreeNode* __p, TreeNode** __root);

    std::size_t count(const_reference __val, TreeNode* __p) const;

    TreeNode* getPrior(TreeNode* __root)const;
    TreeNode* getPosterior(TreeNode* __root)const;
    TreeNode* getMostLeft(TreeNode* __root)const;
    TreeNode* getMostRight(TreeNode* __root)const;
    TreeNode* getSmallest()const;
    TreeNode* getGreatest()const;

    void LL(TreeNode** __root);
    void RR(TreeNode** __root);
    void LR(TreeNode** __root);
    void RL(TreeNode** __root);
};

template<typename _Tp, typename _Alloc>
inline bool CLAVLTree<_Tp, _Alloc>::insert(value_type __val){
    if(!allowRepeat && exist(__val)) return true;
    return AVL_ERROR != insert(__val, &root);
}

template<typename _Tp, typename _Alloc>
inline int8_t CLAVLTree<_Tp, _Alloc>::insert(value_type __val, TreeNode** __root){ // return increased height
    
    if(!(*__root)){
        *__root = alloctor.newObj(__val);

        if(*__root) return 1;
        return AVL_ERROR;                           // 4 for error
    }

    int8_t increasedHeight = 0;

    if(__val < (*__root)->val) {                   // insert into left subtree

        if(!(*__root)->lIsNode){

            TreeNode* newNode = alloctor.newObj(__val);
            if(!newNode) return AVL_ERROR;

            newNode->left = (*__root)->left;
            newNode->right = *__root;
            (*__root)->lIsNode = true;
            (*__root)->left = newNode;

            increasedHeight = 1;

        }else{

            increasedHeight = insert(__val, &((*__root)->left));

        }

        (*__root)->bf += increasedHeight;

        if((*__root)->bf > 1){
            
            if((*__root)->left->bf == 1) LL(__root);
            else LR(__root);

            increasedHeight = 0;

        }else if((*__root)->bf == 0) increasedHeight = 0;

    }else{                                         // insert into right subtree

        if(!(*__root)->rIsNode){

            TreeNode* newNode = alloctor.newObj(__val);
            if(!newNode) return AVL_ERROR;

            newNode->left = *__root;
            newNode->right = (*__root)->right;
            (*__root)->rIsNode = true;
            (*__root)->right = newNode;

            increasedHeight = 1;

        }else{

            increasedHeight = insert(__val, &((*__root)->right));

        }

        (*__root)->bf -= increasedHeight;

        if((*__root)->bf < -1){

            if((*__root)->right->bf == -1) RR(__root);
            else RL(__root);

            increasedHeight = 0;

        }else if((*__root)->bf == 0) increasedHeight = 0;
    }

    return increasedHeight;
}

template<typename _Tp, typename _Alloc>
inline bool CLAVLTree<_Tp, _Alloc>::erase(value_type __val){
    TreeNode* __p = nullptr;
    if(exist(__val, &__p)){
        erase(__p, &root);
        return true;
    }
    return false;
}

template<typename _Tp, typename _Alloc>
inline bool CLAVLTree<_Tp, _Alloc>::eraseAllValue(value_type __val){
    bool flag = false;
    
    while(erase(__val)) flag = true;

    return flag;
}

template<typename _Tp, typename _Alloc>
inline bool CLAVLTree<_Tp, _Alloc>::exist(value_type __val, TreeNode** __p) const {
    
    TreeNode* cur = root;
    
    while(cur){
        if(cur->val < __val){

            cur = cur->rIsNode ? cur->right : nullptr;

        }else if(__val < cur->val){
            
            cur = cur->lIsNode ? cur->left : nullptr;
        
        }else{
            
            break;
        }
    }

    if(__p) *__p = cur;

    return cur;
}

template<typename _Tp, typename _Alloc>
std::size_t CLAVLTree<_Tp, _Alloc>::count(value_type __val) const {

    TreeNode* p = root;
    return count(__val, p);

}

template<typename _Tp, typename _Alloc>
std::size_t CLAVLTree<_Tp, _Alloc>::count(const_reference __val, TreeNode* __p) const {

    if(!__p) return 0;

    if(__p->val < __val) return __p->rIsNode ? count(__val, __p->right) : 0;

    if(__p->val > __val) return __p->lIsNode ? count(__val, __p->left) : 0;

    return 1 + (__p->rIsNode ? count(__val, __p->right) : 0) + (__p->lIsNode ? count(__val, __p->left) : 0);

}

template<typename _Tp, typename _Alloc>
inline typename CLAVLTree<_Tp, _Alloc>::TreeNode* const CLAVLTree<_Tp, _Alloc>::lower_bound(value_type __val) const {

    TreeNode* cur = root;

    while(cur){
        if(cur->val < __val) {
            
            cur = cur->rIsNode ? cur->right : nullptr;          // <
        
        }else{                                                  // >=

            while(cur->lIsNode && !(cur->left->val<__val)) cur = cur->left;

            TreeNode* priorOfCur = cur;
            do{

                cur = priorOfCur;
                priorOfCur = getPrior(cur);
                
            }while(priorOfCur && !(priorOfCur->val<__val));

            break;
        }
    }

    return cur;
}

template<typename _Tp, typename _Alloc>
inline typename CLAVLTree<_Tp, _Alloc>::TreeNode* const CLAVLTree<_Tp, _Alloc>::upper_bound(value_type __val) const {

    TreeNode* cur = root;

    while(cur){
        if(!(__val < cur->val)) {

            cur = cur->rIsNode ? cur->right : nullptr;          // <=
        
        }else{                                                  // >

            while(cur->lIsNode && __val<cur->left->val) cur = cur->left;
            
            TreeNode* priorOfCur = cur;
            do{

                cur = priorOfCur;
                priorOfCur = getPrior(cur);
                
            }while(priorOfCur && __val<priorOfCur->val);

            break;
        }
    }

    return cur;
}

template<typename _Tp, typename _Alloc>
inline void CLAVLTree<_Tp, _Alloc>::LL(TreeNode** __root){

    TreeNode* newRoot = (*__root)->left;

    // usually (*__root)->bf == 2 but sometimes this will be 1 (when operate (RL) )
    // usually (*__root)->left->bf == 1 but sometimes this will be 2 (when operate (LR) )

    if((*__root)->bf == 2) {

        if((*__root)->left->bf == 1) (*__root)->bf = (*__root)->left->bf = 0;
        else if((*__root)->left->bf == -1) (*__root)->bf = -1, (*__root)->left->bf = 0;
        else (*__root)->bf = 0, (*__root)->left->bf = 1;

    }
    else{
        if((*__root)->left->bf == 1) (*__root)->bf = (*__root)->left->bf = -1;
        else (*__root)->bf = 0, (*__root)->left->bf = -2;
    }

    if(newRoot->rIsNode){

        (*__root)->left = newRoot->right;
        newRoot->right = *__root;

    }else{

        newRoot->rIsNode = true;
        (*__root)->lIsNode = false;

    }

    *__root = newRoot;
    
    //newRoot->right->bf = newRoot->bf = 0;

}

template<typename _Tp, typename _Alloc>
inline void CLAVLTree<_Tp, _Alloc>::RR(TreeNode** __root){

    TreeNode* newRoot = (*__root)->right;

    // usually (*__root)->bf == -2 but sometimes this will be -1 (when operate (LR) )
    // usually (*__root)->right->bf == -1 but sometimes this will be -2 (when operate (RL) )

    if((*__root)->bf == -2){

        if((*__root)->right->bf != -2) (*__root)->bf = (*__root)->right->bf = 0;
        else (*__root)->bf = 1, (*__root)->right->bf = 0;

    }else{

        if((*__root)->right->bf == -1) (*__root)->bf = (*__root)->right->bf = 1;
        else if((*__root)->right->bf == 1) (*__root)->bf = 0, (*__root)->right->bf = 2;
        else (*__root)->bf = 0, (*__root)->right->bf = 1;

    }

    if(newRoot->lIsNode){

        (*__root)->right = newRoot->left;
        newRoot->left = *__root;

    }else{

        newRoot->lIsNode = true;
        (*__root)->rIsNode = false;

    }

    *__root = newRoot;

    //newRoot->left->bf = newRoot->bf = 0;

}

template<typename _Tp, typename _Alloc>
inline void CLAVLTree<_Tp, _Alloc>::LR(TreeNode** __root){

    RR(&((*__root)->left));
    LL(__root);

}

template<typename _Tp, typename _Alloc>
inline void CLAVLTree<_Tp, _Alloc>::RL(TreeNode** __root){

    LL(&((*__root)->right));
    RR(__root);

}

template<typename _Tp, typename _Alloc>
inline typename CLAVLTree<_Tp, _Alloc>::TreeNode* CLAVLTree<_Tp, _Alloc>::getPrior(TreeNode* __root)const{
    if(!__root) return nullptr;
    if(!__root->lIsNode) return __root->left;

    return getMostRight(__root->left);
}

template<typename _Tp, typename _Alloc>
inline typename CLAVLTree<_Tp, _Alloc>::TreeNode* CLAVLTree<_Tp, _Alloc>::getPosterior(TreeNode* __root)const{
    if(!__root) return nullptr;
    if(!__root->rIsNode) return __root->right;

    return getMostLeft(__root->right);

}

template<typename _Tp, typename _Alloc>
inline typename CLAVLTree<_Tp, _Alloc>::TreeNode* CLAVLTree<_Tp, _Alloc>::getMostLeft(TreeNode* __root)const{
    while(__root && __root->lIsNode) __root = __root->left;
    return __root;
}

template<typename _Tp, typename _Alloc>
inline typename CLAVLTree<_Tp, _Alloc>::TreeNode* CLAVLTree<_Tp, _Alloc>::getMostRight(TreeNode* __root)const{
    while(__root && __root->rIsNode) __root = __root->right;
    return __root;
}


template<typename _Tp, typename _Alloc>
inline int8_t CLAVLTree<_Tp, _Alloc>::erase(TreeNode* __p, TreeNode** __root){
    if(!__p || !__root || *__root == nullptr) return 0;

    int8_t decreasedHeight = 0;  // -1 means left subtree decrease 1 height, and 1 means right subtree decrease 1 height

    if(__p == (*__root)){
        
        if(!(*__root)->lIsNode && !(*__root)->rIsNode){  // is leaf node

            if(!(*__root)->left && !(*__root)->right){   // is root node

                alloctor.deleteObj(__p);
                (*__root) = nullptr;

            }else{

                TreeNode* temp = (*__root);

                if(!(*__root)->left || (*__root)->left->right != (*__root)){ // is left child

                    (*__root)->right->lIsNode = false;  // left flag of its posterior
                    (*__root) = (*__root)->left;

                }else{     // is right child

                    (*__root)->left->rIsNode = false;   // right flag of its prior
                    (*__root) = (*__root)->right;

                }

                alloctor.deleteObj(temp);
                temp = nullptr;

            }
            return 1;
        }

        // is branch node (after delete may cause rotate)
        if(__p->bf == 1){   // replaced by its prior node

            TreeNode* priorOfP = getPrior(__p);
            assert(priorOfP);
            value_type tmp = priorOfP->val;
            decreasedHeight -= erase(priorOfP, __root);
            __p->val = tmp;

            return -decreasedHeight;

        }else{    // replaced by its posterior node

            TreeNode* posteriorOfP = getPosterior(__p);
            assert(posteriorOfP);
            value_type tmp = posteriorOfP->val;
            decreasedHeight += erase(posteriorOfP, __root);
            __p->val = tmp;

            return decreasedHeight;

        }
    }else if(__p->val < (*__root)->val) decreasedHeight -= erase(__p, &(*__root)->left);
    else if((*__root)->val < __p->val) decreasedHeight += erase(__p, &(*__root)->right);
    else{

        TreeNode* pos = *__root;
        while(pos && pos != __p && pos->val == __p->val) pos = getPosterior(pos);

        if(pos == __p) decreasedHeight += erase(__p, &(*__root)->right);
        else decreasedHeight -= erase(__p, &(*__root)->left);

        return 0;
        
    }

    (*__root)->bf += decreasedHeight;

    if((*__root)->bf < -1){   // has erased from left subtree and cause inbalance

        if((*__root)->right->bf == 0){
                    /*

                               O
                              / \
                             K   O
                                / \
                               O   O
                              / \ / \
                              K K K K

                    */
            TreeNode* newRoot = (*__root)->right;
            (*__root)->right = newRoot->left;
            newRoot->left = *__root;
            newRoot->bf = 1;
            (*__root)->bf = -1;
            *__root = newRoot;

            decreasedHeight = 0;

        }else if((*__root)->right->bf == -1) RR(__root);
        else RL(__root);

    }else if((*__root)->bf > 1){   // has erased from right subtree and cause inbalance

        if((*__root)->left->bf == 0){
                    /*

                                O
                               / \ 
                              O   K
                             / \      
                            O   O 
                           / \ / \
                           k k k k   

                    */
            TreeNode* newRoot = (*__root)->left;
            (*__root)->left = newRoot->right;
            newRoot->bf = -1;
            (*__root)->bf = 1;
            *__root = newRoot;

            decreasedHeight = 0;

        }else if((*__root)->left->bf == 1) LL(__root);
        else LR(__root);
  
    }else{      // has erased node but this subtree is still in a balance

        if(decreasedHeight != 0 && ((*__root)->bf) != 0) decreasedHeight = 0;

    }

    return decreasedHeight < 0 ? -decreasedHeight : decreasedHeight;
}

template<typename _Tp, typename _Alloc>
inline typename CLAVLTree<_Tp, _Alloc>::TreeNode* CLAVLTree<_Tp, _Alloc>::getSmallest() const {
    if(!root) return nullptr;

    TreeNode* p = root;
    while(p->lIsNode) p = p->left;

    return p;
}

template<typename _Tp, typename _Alloc>
inline typename CLAVLTree<_Tp, _Alloc>::TreeNode* CLAVLTree<_Tp, _Alloc>::getGreatest() const {
    if(!root) return nullptr;

    TreeNode* p = root;
    while(p->rIsNode) p = p->right;

    return p;
}

template<typename _Tp, typename _Alloc>
inline void CLAVLTree<_Tp, _Alloc>::showInOrderTraverse() const {     // show all value in this tree
    TreeNode* p = getSmallest();
    if(!p) return;
    printf("IN:\t");
    do{
        std::cout << p->val << "\t";
        //printf("%d\t",p->val);
    }while(p = getPosterior(p));
    putchar('\n');
    
}

template<typename _Tp, typename _Alloc>
inline void CLAVLTree<_Tp, _Alloc>::showInOrderBF() const {         // show all bf in this tree
    TreeNode* p = getSmallest();
    if(!p) return;
    printf("BF:\t");
    do{
        printf("%d\t", 1*p->bf);
    }while(p = getPosterior(p));
    putchar('\n');
}

}



#endif