/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */

#include <stdio.h> 
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES])
{ 
    /* This file contains the stub for an LRU pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];

    /* Local vars */
    int proctmp;
    int pagetmp;
    int page_LRU;
    int oldest_page;

    /* initialize static vars on first run */
    if(!initialized)
    {
	    for(proctmp = 0; proctmp < MAXPROCESSES; proctmp++)
        {
	        for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++)
            {
		        timestamps[proctmp][pagetmp] = 0; 
	        }
	    }
	    initialized = 1;
    }

    // beginning of LRU loop
    for (proctmp = 0; proctmp < MAXPROCESSES; proctmp++) // looping thorugh the number of processes
    {
        if (q[proctmp].active)  // checking if the process is active
        {
            pagetmp = q[proctmp].pc / PAGESIZE; // assigning the current page/ getting process current page

            if(!q[proctmp].pages[pagetmp] && !pagein(proctmp,pagetmp)) //checking if the page is swapped in
            {
                // need to set the oldest page to the tick
                oldest_page = tick; // like minimum tick in the page
                for (int i = 0; i < MAXPROCPAGES; i++)
                {
                    if (q[proctmp].pages[i] && timestamps[proctmp][i] < oldest_page) // if the page at i is already swapped and the page is not the oldest
                    {
                        oldest_page = timestamps[proctmp][i]; // reassigning the minimum tick

                        page_LRU = i; // the current page is set to the index to be removed
                    }
                }
                pageout(proctmp,page_LRU); // removes the current page
                break;
            }
            timestamps[proctmp][pagetmp] = tick; // sets the current process and current page to the current tick
        }
    }
    /* advance time for next pageit iteration */
    tick++;
} 
