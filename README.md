## myCunstomTools
---

CLAVLTree is a balanced tree which the height of left substree and right substree is different no more than 1.
Besides, in my CLAVLTree, there are flags in each nullpter to find prior node and posterior node much more faster.
when flag lIsNode set to be true, then its left pointer is its left child. Otherwise, its left pointer is its prior node.
when flag rIsNode set to be true, then its right pointer is its right child. Otherwise, its right pointer is its posterior node.
so all the nullptr pointer in previous tree are used to act as a thread to get its prior node or posterior node.

---
In my test, CAVLTree is faster than std::set and std::mutiset which use rb_tree in several operations by default

if compile with -O3, then CLAVLTree is faster than std::set and std::multise in insertion, search and count. But spend more time in delete operation.
Because CLAVLTree is strictly balanced, so in delete operation there will be more rotate than rb-tree.

---
set block size too large will affect the performance of our CLAVLTree
I think it should be 100~200 times of key size.(you can test different block size to find a better threshold)

