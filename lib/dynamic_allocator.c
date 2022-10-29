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
	struct MemBlock* blk = blockList->lh_first;
	while(blk != NULL)
	{
		if(blk->sva == va)
			return blk;
		blk = LIST_NEXT(blk);
	}
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
	struct MemBlock *elements;
	uint32 arr_size[LIST_SIZE(&FreeMemBlocksList)];
	uint32 i = 0;
	LIST_FOREACH(elements, &FreeMemBlocksList)
	{
		arr_size[i] = elements->size;
		i++;
		if(elements->size == size)
		{
			struct MemBlock tmp = *elements;
			LIST_REMOVE(&FreeMemBlocksList, elements);
			elements = &tmp;
			return elements;
		}
	}
	uint32 min;
	int flag = 1;
	for (i = 0; i < sizeof(arr_size) ;i++)
	{
		if (arr_size[i] > size)
		{
			if (flag == 1)
			{
				min = arr_size[i];
				flag = 0;
			}
			if (arr_size[i] < min)
				min = arr_size[i];
		}
	}
	LIST_FOREACH(elements, &FreeMemBlocksList)
	{
		if (elements->size == min)
		{
			struct MemBlock* newBlock = LIST_FIRST(&AvailableMemBlocksList);
			newBlock->sva=elements->sva;
			newBlock->size = size;

			LIST_REMOVE(&AvailableMemBlocksList, newBlock);

			elements->sva = elements->sva + size;
			elements->size = elements->size - size;
			return newBlock;
		}
	}
	return NULL;
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

	uint32 siize = LIST_SIZE(&FreeMemBlocksList);
	if(siize == 0){
		LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);
	}
	else{
		if(blockToInsert->sva + blockToInsert->size <= LIST_FIRST(&FreeMemBlocksList)->sva){

			    if(blockToInsert->sva + blockToInsert->size == LIST_FIRST(&FreeMemBlocksList)->sva){
					 LIST_FIRST(&FreeMemBlocksList)->size +=blockToInsert->size ;
					 LIST_FIRST(&FreeMemBlocksList)->sva =blockToInsert->sva ;
					 blockToInsert->size = 0 ;
					 blockToInsert->sva= 0 ;
					 LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
				}else{

					LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);
				}
		}
		else if(blockToInsert->sva >= LIST_LAST(&FreeMemBlocksList)->sva + LIST_LAST(&FreeMemBlocksList)->size){

			    if (blockToInsert->sva == LIST_LAST(&FreeMemBlocksList)->sva + LIST_LAST(&FreeMemBlocksList)->size){

					LIST_LAST(&FreeMemBlocksList)->size = LIST_LAST(&FreeMemBlocksList)->size + blockToInsert->size;
					blockToInsert->size=0;
					blockToInsert->sva=0;
					LIST_INSERT_TAIL(&(AvailableMemBlocksList),blockToInsert);
			  }
			    else{
			    	LIST_INSERT_TAIL(&FreeMemBlocksList,blockToInsert);
			    }
		  }

		else{
			struct MemBlock *tmp;

						LIST_FOREACH(tmp, &FreeMemBlocksList){

							struct MemBlock* tmp_next = LIST_NEXT(tmp);

							  if(blockToInsert->sva > (tmp->sva + tmp->size) && blockToInsert->sva + blockToInsert->size < (tmp_next->sva)){

									LIST_INSERT_AFTER(&FreeMemBlocksList, tmp, blockToInsert);
									break;
							   }

							  else if(blockToInsert->sva == tmp->sva + tmp->size && blockToInsert->sva + blockToInsert->size == tmp_next->sva){
								tmp->size = tmp->size + blockToInsert->size + tmp_next->size;
								LIST_REMOVE(&FreeMemBlocksList, tmp_next);
								blockToInsert->sva = 0 ;
								blockToInsert->size = 0 ;
								tmp_next->size = 0 ;
								tmp_next->sva = 0 ;
								LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
								LIST_INSERT_TAIL(&AvailableMemBlocksList, tmp_next);
						        break;
							  }

							  else if(blockToInsert->sva == tmp->sva + tmp->size){
								tmp->size = tmp->size + blockToInsert->size;
								blockToInsert->sva = 0;
								blockToInsert->size = 0;
								LIST_INSERT_TAIL(&AvailableMemBlocksList,blockToInsert);
								break;
							}

							  else if(blockToInsert->sva +blockToInsert->size == tmp->sva){
								tmp->sva = blockToInsert->sva;
								tmp->size = tmp->size + blockToInsert->size;
								blockToInsert->sva= 0 ;
								blockToInsert->size= 0 ;
								LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
								break;
							}
						}
		}
	}

	cprintf("\nAFTER INSERT with MERGE:\n=====================\n");
	print_mem_block_lists();
}

