/*
 * *@*	Description: DO NOT APPLY WITH DEFAULT VECTOR !!! Besides, this only can apply one object space one time
 * *@*	Version: 
 * *@*	Author: yusheng Gao
 * *@*	Date: 2021-06-21 21:03:16
 * *@*	LastEditors: yusheng Gao
 * *@*	LastEditTime: 2021-06-24 20:42:14
 */


#ifndef CUSTOM_ALLOCATOR_HPP
#define CUSTOM_ALLOCATOR_HPP

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <cassert>

namespace customTool{


template<typename _Tp, std::size_t _BlockSize = 4096>
class CLAllocator{
public:
    static_assert(_BlockSize>2*sizeof(_Tp), "_BlockSize too small");
    typedef _Tp                 value_type;
    typedef _Tp*                pointer;
    typedef _Tp&                reference;
    typedef const _Tp*          const_pointer;
    typedef const _Tp&          const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;
    typedef std::false_type     propagate_on_container_copy_assignment;
    typedef std::true_type      propagate_on_container_move_assignment;
    typedef std::true_type      propagate_on_container_swap;

    template<typename _Tp1> struct rebind { typedef CLAllocator<_Tp1, _BlockSize> other; };

public:
    
    CLAllocator() noexcept;
    CLAllocator(const CLAllocator&) = delete;
    CLAllocator(CLAllocator&&) noexcept;
    template<typename _Up> CLAllocator(const CLAllocator<_Up>&) = delete;
    ~CLAllocator() noexcept;

    CLAllocator& operator=(const CLAllocator&) = delete;
    CLAllocator& operator=(CLAllocator&&) noexcept;

public:

    pointer address(reference __x) const noexcept { return &__x; }
    const_pointer address(const_reference __x) const noexcept { return &__x; }

    pointer allocate(size_type = 1, const_pointer = nullptr);

    void deallocate(pointer, size_type);

    size_type max_size() const noexcept;

    template<typename _Up, typename... _Args>
    void construct(_Up* __p, _Args&&... __args) noexcept(
            noexcept(::new((void*)__p)_Up(std::forward<_Args>(__args)...))
            );

    template<typename _Up>
    void destroy(_Up* __p) noexcept( noexcept(__p->~_Up()) );

    template<typename _Up>
    friend bool operator==(const CLAllocator&, const CLAllocator<_Up>&) noexcept { return true; }

    template<typename _Up>
    friend bool operator!=(const CLAllocator&, const CLAllocator<_Up>&) noexcept { return false; }


    template<typename... _Args>
    pointer newObj(_Args&&... __args);

    void deleteObj(pointer __p);

private:

    union USlot{
        value_type elem;
        USlot* next;
    };

    typedef USlot               slot_type;
    typedef USlot*              slot_pointer;
    typedef char*               data_pointer;

    slot_pointer currentBlock;
    slot_pointer currentSlot;
    slot_pointer lastSlot;
    slot_pointer freeSlot;

    size_type padPointer(data_pointer __ptr, size_type __align) const noexcept{
        return (__align - reinterpret_cast<std::uintptr_t>(__ptr)) % __align;
    }

    void allocateBlock(size_type = 1);


};  // declare


// ctor

template<typename _Tp, std::size_t _BlockSize>
CLAllocator<_Tp, _BlockSize>::CLAllocator() noexcept{
    currentBlock = nullptr;
    currentSlot = nullptr;
    lastSlot = nullptr;
    freeSlot = nullptr;
    allocateBlock(1);
//std::cout << "call constructor, cur blocksize = " << _BlockSize << std::endl;
}

template<typename _Tp, std::size_t _BlockSize>
CLAllocator<_Tp, _BlockSize>::CLAllocator(CLAllocator&& __other) noexcept {
    if(this != &__other){
        currentBlock = __other.currentBlock;
        currentSlot = __other.currentSlot;
        lastSlot = __other.lastSlot;
        freeSlot = __other.freeSlot;

        __other.currentBlock = nullptr;
        __other.currentSlot = nullptr;
        __other.lastSlot = nullptr;
        __other.freeSlot = nullptr;
    }

}

template<typename _Tp, std::size_t _BlockSize>
CLAllocator<_Tp, _BlockSize>& CLAllocator<_Tp, _BlockSize>::operator=(CLAllocator&& __other) noexcept {
    if(this != &__other){
        currentBlock = __other.currentBlock;
        currentSlot = __other.currentSlot;
        lastSlot = __other.lastSlot;
        freeSlot = __other.freeSlot;

        __other.currentBlock = nullptr;
        __other.currentSlot = nullptr;
        __other.lastSlot = nullptr;
        __other.freeSlot = nullptr;
    }
    return *this;
}

template<typename _Tp, std::size_t _BlockSize>
CLAllocator<_Tp, _BlockSize>::~CLAllocator() noexcept { 
    slot_pointer cur = currentBlock;
    while(cur){
        slot_pointer prior = cur->next;
        ::operator delete(reinterpret_cast<void*>(cur));
        cur = prior;
    }
}


// member function

template<typename _Tp, std::size_t _BlockSize>
void CLAllocator<_Tp, _BlockSize>::allocateBlock(size_type __numOfBlocks){

    size_type totalSize = __numOfBlocks*_BlockSize;

    data_pointer pNewBlock = reinterpret_cast<data_pointer>(::operator new(totalSize));

    reinterpret_cast<slot_pointer>(pNewBlock)->next = currentBlock;

    currentBlock = reinterpret_cast<slot_pointer>(pNewBlock);

    data_pointer pBlockBody = pNewBlock + sizeof(slot_pointer);

    currentSlot = reinterpret_cast<slot_pointer>(pBlockBody+padPointer(pBlockBody, alignof(slot_type)));

    lastSlot = reinterpret_cast<slot_pointer>(pNewBlock+_BlockSize-sizeof(slot_type)+1);
//std::cout << "allocate Block : " << currentBlock << " with size " << _BlockSize << std::endl;
}

template<typename _Tp, std::size_t _BlockSize>
inline typename CLAllocator<_Tp, _BlockSize>::pointer CLAllocator<_Tp, _BlockSize>::allocate(size_type __numOfObjs, const_pointer __hint){
    assert(__numOfObjs>0);

    if(freeSlot){
        pointer res = reinterpret_cast<pointer>(freeSlot);
        freeSlot = freeSlot->next;
//std::cout << "allocate : " << res << std::endl;
        return res;
    }

    if(currentSlot >= lastSlot) allocateBlock(1);  // == when all are nullptr
//std::cout << "allocate : " << currentSlot << std::endl;
    return reinterpret_cast<pointer>(currentSlot++);

}

template<typename _Tp, std::size_t _BlockSize>
inline void CLAllocator<_Tp, _BlockSize>::deallocate(pointer __p, size_type __n){
    if(__p){
        reinterpret_cast<slot_pointer>(__p)->next = freeSlot;
        freeSlot = reinterpret_cast<slot_pointer>(__p);
    }
//std::cout << "deallocate : " << __p << std::endl;
}

template<typename _Tp, std::size_t _BlockSize>
inline typename CLAllocator<_Tp, _BlockSize>::size_type CLAllocator<_Tp, _BlockSize>::max_size() const noexcept{
    size_type maxBlocks = -1 / _BlockSize;
    return (_BlockSize-sizeof(data_pointer)) / sizeof(slot_type) * maxBlocks;
}

template<typename _Tp, std::size_t _BlockSize>
template<typename _Up, typename... _Args>
void CLAllocator<_Tp, _BlockSize>::construct(_Up* __p, _Args&&... __args) noexcept(
            noexcept(::new((void*)__p)_Up(std::forward<_Args>(__args)...))
            ){
    ::new((void*)__p)_Up(std::forward<_Args>(__args)...);
}

template<typename _Tp, std::size_t _BlockSize>
template<typename _Up>
void CLAllocator<_Tp, _BlockSize>::destroy(_Up* __p) noexcept( noexcept(__p->~_Up()) ){
    __p->~_Up();
}


template<typename _Tp, std::size_t _BlockSize>
template<typename... _Args>
typename CLAllocator<_Tp, _BlockSize>::pointer CLAllocator<_Tp, _BlockSize>::newObj(_Args&&... __args){
    pointer res = allocate(1);
    construct<value_type>(res, std::forward<_Args>(__args)...);
    return res;
}

template<typename _Tp, std::size_t _BlockSize>
void CLAllocator<_Tp, _BlockSize>::deleteObj(pointer __p){
    destroy(__p);
    deallocate(__p, 1);
}


} // namespace


#endif