/*
 * dyn_block_management.c
 *
 *  Created on: Sep 21, 2022
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"


//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//===========================
// PRINT MEM BLOCK LISTS:
//===========================

void print_mem_block_lists()
{
	cprintf("\n=========================================\n");
	struct MemBlock* blk ;
	struct MemBlock* lastBlk = NULL ;
	cprintf("\nFreeMemBlocksList:\n");
	uint8 sorted = 1 ;
	LIST_FOREACH(blk, &FreeMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nFreeMemBlocksList is NOT SORTED!!\n") ;

	lastBlk = NULL ;
	cprintf("\nAllocMemBlocksList:\n");
	sorted = 1 ;
	LIST_FOREACH(blk, &AllocMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nAllocMemBlocksList is NOT SORTED!!\n") ;
	cprintf("\n=========================================\n");

}

//********************************************************************************//
//********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//===============================
// [1] INITIALIZE AVAILABLE LIST:
//===============================
void initialize_MemBlocksList(uint32 numOfBlocks)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList
	// Write your code here, remove the panic and write your code
	LIST_INIT(&AvailableMemBlocksList);
	for (int i = 0; i < numOfBlocks; i++)
		LIST_INSERT_HEAD(&AvailableMemBlocksList, &(MemBlockNodes[i]));
}

//===============================
// [2] FIND BLOCK:
//===============================
struct MemBlock *find_block(struct MemBlock_List *blockList, uint32 va)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] find_block
	// Write your code here, remove the panic and write your code
<<<<<<< HEAD
	struct MemBlock* blk;
	LIST_FOREACH(blk, blockList)
=======
	struct MemBlock* blk = blockList->lh_first;
	while(blk != NULL)
>>>>>>> 91f86b1c7f6db110a0fdfc288ce8691491d1c50c
	{
		if(blk->sva == va)
			return blk;
	}
//	while(blk != blockList->lh_last)
//	{
//		if(blk->sva == va)
//			return blk;
//		blk = LIST_NEXT(blk);
//	}
	return NULL;
}

//=========================================
// [3] INSERT BLOCK IN ALLOC LIST [SORTED]:
//=========================================
void insert_sorted_allocList(struct MemBlock *blockToInsert)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_allocList
		uint32 length = LIST_SIZE(&AllocMemBlocksList);
		if (length == 0 || blockToInsert->sva < LIST_FIRST(&AllocMemBlocksList)->sva)
		{
			LIST_INSERT_HEAD(&AllocMemBlocksList, blockToInsert);
		}
		else if(blockToInsert->sva > LIST_LAST(&AllocMemBlocksList)->sva)
		{
			LIST_INSERT_TAIL(&AllocMemBlocksList, blockToInsert);
		}
		else
		{
			struct MemBlock * elements ;
			LIST_FOREACH(elements, &AllocMemBlocksList)
			{
				if (elements->sva < blockToInsert->sva)
				{
					if (LIST_NEXT(elements)->sva> blockToInsert->sva)
					{
						LIST_INSERT_AFTER(&(AllocMemBlocksList), elements, blockToInsert);
						break;
					}
				}
			}
		}
}

//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
struct MemBlock *alloc_block_FF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
	// Write your code here, remove the panic and write your code
	struct MemBlock* elements;
	LIST_FOREACH(elements, &FreeMemBlocksList)
	{
		if (elements->size == size)
		{
			struct MemBlock tmp = *elements;
			LIST_REMOVE(&FreeMemBlocksList, elements);
			elements = &tmp;
			return elements;
		}
		if (elements->size > size)//4 KB 40 KB
		{
			//edit block
			elements->sva += size;
			elements->size -= size;
			//free block to init new block
			LIST_REMOVE(&AvailableMemBlocksList, LIST_FIRST(&AvailableMemBlocksList));
			struct MemBlock newBLock;
			newBLock.size = size;
			newBLock.sva = elements->sva - size;
			elements = &newBLock;
			return elements;
		}
	}
	return NULL;
}

//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
struct MemBlock *alloc_block_BF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
	// Write your code here, remove the panic and write your code
	panic("alloc_block_BF() is not implemented yet...!!");
}


//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
struct MemBlock *alloc_block_NF(uint32 size)
{
	//TODO: [PROJECT MS1 - BONUS] [DYNAMIC ALLOCATOR] alloc_block_NF
	// Write your code here, remove the panic and write your code
	panic("alloc_block_NF() is not implemented yet...!!");

}

//===================================================
// [8] INSERT BLOCK (SORTED WITH MERGE) IN FREE LIST:
//===================================================
void insert_sorted_with_merge_freeList(struct MemBlock *blockToInsert)
{
	//cprintf("BEFORE INSERT with MERGE: insert [%x, %x)\n=====================\n", blockToInsert->sva, blockToInsert->sva + blockToInsert->size);
	//print_mem_block_lists() ;

	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_with_merge_freeList
	// Write your code here, remove the panic and write your code
	panic("insert_sorted_with_merge_freeList() is not implemented yet...!!");



	//cprintf("\nAFTER INSERT with MERGE:\n=====================\n");
	//print_mem_block_lists();

}

