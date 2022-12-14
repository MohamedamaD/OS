/*
 * chunk_operations.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#include <kern/trap/fault_handler.h>
#include <kern/disk/pagefile_manager.h>
#include "kheap.h"
#include "memory_manager.h"

/******************************/
/*[1] RAM CHUNKS MANIPULATION */
/******************************/

//===============================
// 1) CUT-PASTE PAGES IN RAM:
//===============================
//This function should cut-paste the given number of pages from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int cut_paste_pages(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 num_of_pages)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] cut_paste_pages
	// Write your code here, remove the panic and write your code
	uint32 start_src = source_va ;//ROUNDDOWN(source_va, PAGE_SIZE);
	uint32 start_dest =dest_va;// ROUNDDOWN(dest_va, PAGE_SIZE);
	uint32 end_src =source_va + (num_of_pages*PAGE_SIZE);// ROUNDUP(source_va + (num_of_pages*PAGE_SIZE) , PAGE_SIZE);
	uint32 end_dest =dest_va + (num_of_pages*PAGE_SIZE);// ROUNDUP(dest_va + (num_of_pages*PAGE_SIZE) ,PAGE_SIZE);
	for (uint32 i = start_dest; i < end_dest; i+=PAGE_SIZE)
	{
		uint32* dest_ptr_page_table = NULL;
		struct FrameInfo *dest_ptr_Frame_Info = get_frame_info(page_directory, i, &dest_ptr_page_table);
		if (dest_ptr_Frame_Info != NULL)
			return -1;
	}

	for (uint32 i = start_src, j = start_dest; i < end_src; i+=PAGE_SIZE,j+=PAGE_SIZE)
	{
		uint32* src_ptr_page_table = NULL;
		struct FrameInfo *src_ptr_Frame_Info = get_frame_info(page_directory,i,&src_ptr_page_table);
		uint32 perms = pt_get_page_permissions(page_directory, i);
		int ret = map_frame(page_directory, src_ptr_Frame_Info, j, perms);
		unmap_frame(page_directory, i);
	}
	return 0;
}

//===============================
// 2) COPY-PASTE RANGE IN RAM:
//===============================
//This function should copy-paste the given size from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int copy_paste_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 size)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] copy_paste_chunk
	// Write your code here, remove the panic and write your code
	uint32 start_src =source_va; //ROUNDDOWN(source_va, PAGE_SIZE);//srcstrt
	uint32 end_src =source_va+size; //ROUNDUP(source_va+size, PAGE_SIZE);//srcend
	uint32 start_dest =dest_va; //ROUNDDOWN(dest_va, PAGE_SIZE);//desstrt
	uint32 end_dest =dest_va+size;// ROUNDUP(dest_va+size, PAGE_SIZE);//desend


	for (uint32 i = start_dest; i < end_dest; i += PAGE_SIZE)
	{
		uint32 *dest_table;
		struct FrameInfo *desframe = get_frame_info(page_directory, i, &dest_table);
		if (desframe != NULL)
		{
			uint32 dest_entry = dest_table[PTX(i)];
			uint32 permition_write = dest_entry & PERM_WRITEABLE;
			if (permition_write == 0)
				return -1;
		}
		if (dest_table == NULL)
			create_page_table(page_directory, i);

		uint32 s_srs = start_src;//uint32 j = start_dest;
		uint32 *src_table;
		struct FrameInfo *src_frame = get_frame_info(page_directory, s_srs, &src_table);
		struct FrameInfo *dest_frame = get_frame_info(page_directory, i, &dest_table);
		uint32 allperms = pt_get_page_permissions(page_directory, s_srs);			// you get the user permission with the rest of them
		if (dest_frame == NULL && src_frame != NULL)
		{
			struct FrameInfo *newframe;
			int ret = allocate_frame(&newframe);
			if (ret != 0)		// To handle not enough memory
				return -1;
			else
			{
				map_frame(page_directory, newframe, i, allperms);
				pt_set_page_permissions(page_directory, i, PERM_WRITEABLE, 0);
			}
		}
		else{
			pt_set_page_permissions(page_directory, i, allperms, 0);
		}

		s_srs += PAGE_SIZE;
	}
	unsigned char * src;
	unsigned char * des;
	for(uint32 i = source_va, j = dest_va; i < source_va+size && j < dest_va+size; i++, j++)
	{
		src = (unsigned char *)(i);
		des = (unsigned char *)(j);
		*des = *src;
	}

	return 0;
}

//===============================
// 3) SHARE RANGE IN RAM:
//===============================
//This function should share the given size from dest_va with the source_va
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int share_chunk(uint32* page_directory, uint32 source_va,uint32 dest_va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] share_chunk
	// Write your code here, remove the panic and write your code
	uint32 s = ROUNDDOWN(source_va, PAGE_SIZE );
	uint32 d = ROUNDDOWN(dest_va, PAGE_SIZE );
	uint32 end = ROUNDUP(dest_va+size, PAGE_SIZE) ;
	for(uint32 i = d ; i < end  ; i += PAGE_SIZE){
			uint32* ptr_page_table2 = NULL;

			struct FrameInfo *ptr_Frame_Info2 = get_frame_info(page_directory,d,&ptr_page_table2);
			if(ptr_Frame_Info2 != NULL){
				return -1 ;
			}
			if(ptr_page_table2 == NULL)
				create_page_table(page_directory, i);
	}
	for(uint32 i = d ; i < end  ; i += PAGE_SIZE ){

			uint32* ptr_page_table1 = NULL;
			uint32* ptr_page_table2 = NULL;
			// int ret = get_page_table(page_directory, d ,&ptr_page_table2 );
			struct FrameInfo *ptr_Frame_Info1 = get_frame_info(page_directory,s,&ptr_page_table1);
			struct FrameInfo *ptr_Frame_Info2 = get_frame_info(page_directory,d,&ptr_page_table2);
			if(ptr_Frame_Info2 != NULL){
				return -1 ;
			}else{

			//   uint32 prm = pt_get_page_permissions( page_directory, s );

			   map_frame(page_directory , ptr_Frame_Info1 , d , perms );
			}
			s = s + PAGE_SIZE ;
			d = d + PAGE_SIZE ;
	}
	return 0;
}

//===============================
// 4) ALLOCATE CHUNK IN RAM:
//===============================
//This function should allocate in RAM the given range [va, va+size)
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int allocate_chunk(uint32* page_directory, uint32 va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] allocate_chunk
	// Write your code here, remove the panic and write your code
	uint32 start_address = ROUNDDOWN(va, PAGE_SIZE);
	uint32 end_address = ROUNDUP(va,PAGE_SIZE)+size;

	for(uint32 i = start_address; i < end_address; i+= PAGE_SIZE)
	{
		uint32* ptr_page_table = NULL;
		struct FrameInfo *ptr_Frame_Info = get_frame_info(page_directory, i, &ptr_page_table);
		if(ptr_Frame_Info != NULL)
			return -1;
		if(ptr_page_table == NULL)
			create_page_table(page_directory, i);
	}
	for(uint32 i = start_address; i < end_address; i+= PAGE_SIZE)
	{
		uint32* ptr_page_table = NULL;
		struct FrameInfo *ptr_Frame_Info = get_frame_info(page_directory, i, &ptr_page_table);
		int ret2 = allocate_frame(&ptr_Frame_Info);
		int ret3 = 	map_frame(page_directory, ptr_Frame_Info, i, perms);
		if( ret3 != 0){
			free_frame(ptr_Frame_Info);
			return -1;
		}
	}
	return 0;
}

/*BONUS*/
//=====================================
// 5) CALCULATE ALLOCATED SPACE IN RAM:
//=====================================
void calculate_allocated_space(uint32* page_directory, uint32 sva, uint32 eva, uint32 *num_tables, uint32 *num_pages)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_allocated_space
	// Write your code here, remove the panic and write your code
	panic("calculate_allocated_space() is not implemented yet...!!");
}

/*BONUS*/
//=====================================
// 6) CALCULATE REQUIRED FRAMES IN RAM:
//=====================================
// calculate_required_frames:
// calculates the new allocation size required for given address+size,
// we are not interested in knowing if pages or tables actually exist in memory or the page file,
// we are interested in knowing whether they are allocated or not.
uint32 calculate_required_frames(uint32* page_directory, uint32 sva, uint32 size)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_required_frames
	// Write your code here, remove the panic and write your code
	panic("calculate_required_frames() is not implemented yet...!!");
}

//=================================================================================//
//===========================END RAM CHUNKS MANIPULATION ==========================//
//=================================================================================//

/*******************************/
/*[2] USER CHUNKS MANIPULATION */
/*******************************/

//======================================================
/// functions used for USER HEAP (malloc, free, ...)
//======================================================

//=====================================
// 1) ALLOCATE USER MEMORY:
//=====================================
void allocate_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	// Write your code here, remove the panic and write your code
	panic("allocate_user_mem() is not implemented yet...!!");
}

//=====================================
// 2) FREE USER MEMORY:
//=====================================
void free_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3] [USER HEAP - KERNEL SIDE] free_user_mem
	// Write your code here, remove the panic and write your code
	panic("free_user_mem() is not implemented yet...!!");

	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}

//=====================================
// 2) FREE USER MEMORY (BUFFERING):
//=====================================
void __free_user_mem_with_buffering(struct Env* e, uint32 virtual_address, uint32 size)
{
	// your code is here, remove the panic and write your code
	panic("__free_user_mem_with_buffering() is not implemented yet...!!");

	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Free any BUFFERED pages in the given range
	//4. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}

//=====================================
// 3) MOVE USER MEMORY:
//=====================================
void move_user_mem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3 - BONUS] [USER HEAP - KERNEL SIDE] move_user_mem
	//your code is here, remove the panic and write your code
	panic("move_user_mem() is not implemented yet...!!");

	// This function should move all pages from "src_virtual_address" to "dst_virtual_address"
	// with the given size
	// After finished, the src_virtual_address must no longer be accessed/exist in either page file
	// or main memory

	/**/
}

//=================================================================================//
//========================== END USER CHUNKS MANIPULATION =========================//
//=================================================================================//

