// Program Information /////////////////////////////////////////////////////////
/**
 * @file SimulatorFunctions.c
 *
 * @brief Memory Address Locator Implementations for CS 446/646 at the 
 *		  Univserity of Nevada, Reno
 * 
 * @details This file includes the necessary function implementations for 
 *			assignment 2 of the CS 446/646 course simulator.
 *
 * @author Vineeth Rajamohan
 *
 * @version 1.01 (27 Jan 17)
 */

// PRECOMPILER DIRECTIVES //////////////////////////////////////////////////////

#ifndef MEM_FUNC_C
#define MEM_FUNC_C

// HEADER FILES ////////////////////////////////////////////////////////////////

#include "MemoryFunction.h"

// GLOBAL CONSTANTS ////////////////////////////////////////////////////////////

 // None

/**
 * @brief Memory Adress Return Function
 *
 * @details Reads in an integer representing kilobytes of total memory. Using
 *			the total memory, the function returns the address as a hexadecimal
 *			string.
 *          
 * @param in: total memory in kb (int)
 *
 * @pre None
 *
 * @post Hexadecimal string address returned
 *
 * @exception Requires value > 0
 *
 * @exception Address pointer should be null
 */
unsigned int allocateMemory( int totMem )
{
	unsigned int address;
	
	srand(time(NULL));
	
	if( totMem > 0 )
	{
		address = rand() % totMem;
	}
	return address;
}

#endif // MEM_FUNC_C
