/*
 * paging_helpers.c
 *
 *  Created on: Sep 30, 2022
 *      Author: HP
 */
#include "memory_manager.h"

/*[2.1] PAGE TABLE ENTRIES MANIPULATION */
inline void pt_set_page_permissions(uint32* page_directory, uint32 virtual_address, uint32 permissions_to_set, uint32 permissions_to_clear)
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] pt_set_page_permissions
	// Write your code here, remove the panic and write your code
	uint32 *ptr_page_table = NULL;
	int page_table = get_page_table(page_directory, virtual_address, &ptr_page_table);
	if (page_table != TABLE_IN_MEMORY)
		panic("Invalid va");
	else
	{
		ptr_page_table[PTX(virtual_address)] = ptr_page_table[PTX(virtual_address)] | (permissions_to_set);
		ptr_page_table[PTX(virtual_address)] = ptr_page_table[PTX(virtual_address)] & (~permissions_to_clear);
	}
	tlb_invalidate((void *)NULL, (void *)virtual_address);
}

inline int pt_get_page_permissions(uint32* page_directory, uint32 virtual_address )
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] pt_get_page_permissions
	// Write your code here, remove the panic and write your code
	uint32 *ptr_page_table = NULL;
	int page_table = get_page_table(page_directory, virtual_address, &ptr_page_table);
	if (page_table != TABLE_IN_MEMORY)
		return -1;
	else
	{
		uint32 table_entry = ptr_page_table[PTX(virtual_address)];
		return (table_entry & 0x00000FFF);
	}
}

inline void pt_clear_page_table_entry(uint32* page_directory, uint32 virtual_address)
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] pt_clear_page_table_entry
	// Write your code here, remove the panic and write your code
	uint32 *ptr_page_table = NULL;
	int page_table = get_page_table(page_directory, virtual_address, &ptr_page_table);
	if (page_table == TABLE_IN_MEMORY)
	{
		ptr_page_table[PTX(virtual_address)] = 0;
	}
	else
		panic("Invalid va");
	tlb_invalidate((void *)NULL, (void *)virtual_address);
}

/***********************************************************************************************/

/*[2.2] ADDRESS CONVERTION*/
inline int virtual_to_physical(uint32* page_directory, uint32 virtual_address)
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] virtual_to_physical
	// Write your code here, remove the panic and write your code
	uint32 *ptr_page_table = NULL;
	int page_table = get_page_table(page_directory, virtual_address, &ptr_page_table);
	if (page_table != TABLE_IN_MEMORY)
		return -1;
	else
	{
		uint32 table_entry = ptr_page_table[PTX(virtual_address)];
		uint32 offset = virtual_address & 0x00000FFF;
		uint32 frame = (table_entry & 0xFFFFF000);
		return frame & (~offset);
	}
}

/***********************************************************************************************/

/***********************************************************************************************/
/***********************************************************************************************/
/***********************************************************************************************/
/***********************************************************************************************/
/***********************************************************************************************/

///============================================================================================
/// Dealing with page directory entry flags

inline uint32 pd_is_table_used(uint32* page_directory, uint32 virtual_address)
{
	return ( (page_directory[PDX(virtual_address)] & PERM_USED) == PERM_USED ? 1 : 0);
}

inline void pd_set_table_unused(uint32* page_directory, uint32 virtual_address)
{
	page_directory[PDX(virtual_address)] &= (~PERM_USED);
	tlb_invalidate((void *)NULL, (void *)virtual_address);
}

inline void pd_clear_page_dir_entry(uint32* page_directory, uint32 virtual_address)
{
	page_directory[PDX(virtual_address)] = 0 ;
	tlbflush();
}
