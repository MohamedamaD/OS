#include "kheap.h"

#include <inc/memlayout.h>
#include <inc/dynamic_allocator.h>
#include "memory_manager.h"

//==================================================================//
//==================================================================//
//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)//
//==================================================================//
//==================================================================//

void initialize_dyn_block_system()
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] initialize_dyn_block_system
	// your code is here, remove the panic and write your code
	LIST_INIT(&FreeMemBlocksList);
	LIST_INIT(&AllocMemBlocksList);
#if STATIC_MEMBLOCK_ALLOC

#else
	MAX_MEM_BLOCK_CNT = NUM_OF_KHEAP_PAGES;

	uint32 MaxMemBlockNodes = (sizeof (struct MemBlock));
	uint32 va = MaxMemBlockNodes * MAX_MEM_BLOCK_CNT;

	allocate_chunk(ptr_page_directory,KERNEL_HEAP_START,ROUNDUP(va,PAGE_SIZE),PERM_WRITEABLE);
	MemBlockNodes = (struct MemBlock*)KERNEL_HEAP_START;

#endif
	initialize_MemBlocksList(NUM_OF_KHEAP_PAGES);
	struct MemBlock* temp = LIST_FIRST(&AvailableMemBlocksList);
	LIST_REMOVE(&AvailableMemBlocksList, temp);
	temp->size = ROUNDDOWN(((KERNEL_HEAP_MAX - KERNEL_HEAP_START) - va), PAGE_SIZE);

	temp->sva = KERNEL_HEAP_START + ROUNDUP(va,PAGE_SIZE);
	LIST_INSERT_HEAD(&(FreeMemBlocksList), temp);
	//kpanic_into_prompt("initialize_dyn_block_system() is not implemented yet...!!");
}
void* kmalloc(unsigned int size)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kmalloc
	// your code is here, remove the panic and write your code
//	kpanic_into_prompt("kmalloc() is not implemented yet...!!");

	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details
	// use "isKHeapPlacementStrategyFIRSTFIT() ..." functions to check the current strategy

	//change this "return" according to your answer
	size = ROUNDUP(size, PAGE_SIZE);
	struct MemBlock* blk;
	if(isKHeapPlacementStrategyFIRSTFIT())
		blk = alloc_block_FF(size);
	else if(isKHeapPlacementStrategyBESTFIT())
		blk = alloc_block_BF(size);
	int ret;
	if(blk != NULL)
	{
		ret = allocate_chunk(ptr_page_directory, blk->sva, size, PERM_WRITEABLE);
		if(ret == 0){
			insert_sorted_allocList(blk);
			return (void *)(blk->sva);
		}
		else
			return NULL;
	}
	else
		return NULL;
}

void kfree(void* virtual_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kfree
	// Write your code here, remove the panic and write your code
	panic("kfree() is not implemented yet...!!");

}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_virtual_address
	// Write your code here, remove the panic and write your code
	panic("kheap_virtual_address() is not implemented yet...!!");

	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details
	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_physical_address
	// Write your code here, remove the panic and write your code
	panic("kheap_physical_address() is not implemented yet...!!");

	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details
}


void kfreeall()
{
	panic("Not implemented!");

}

void kshrink(uint32 newSize)
{
	panic("Not implemented!");
}

void kexpand(uint32 newSize)
{
	panic("Not implemented!");
}




//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT MS2 - BONUS] [KERNEL HEAP] krealloc
	// Write your code here, remove the panic and write your code
	panic("krealloc() is not implemented yet...!!");
}
