# myCunstomTools


CLAVLTree is a balanced tree which the height of left substree and right substree is different no more than 1.
	
Besides, in my CLAVLTree, there are flags to find prior node and posterior node much more faster.
when flag lIsNode set to be true, then its left pointer is its left child. Otherwise, its left pointer is its prior node.
when flag rIsNode set to be true, then its right pointer is its right child. Otherwise, its right pointer is its posterior node.
so all the nullptr pointer in previous tree are used to act as a thread to get its prior node or posterior node

In my test, CAVLTree is faster than std::set and std::mutiset which use rb_tree in several operations
