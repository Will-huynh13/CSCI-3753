/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */
// resources used: Google for basic LRU info. Colaborated with agus with R files
#include <stdio.h> 
#include <stdlib.h>
#include "simulator.h"
void pageit(Pentry q[MAXPROCESSES])
{ 
    int page;
    int CurProcess = 0;
    while(CurProcess < MAXPROCESSES)
    {
        if (q[CurProcess].active) // checking if process is active
        {
            int myPrediction[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // seeting the predicton array to all 0, meaning false
            page = q[CurProcess].pc / PAGESIZE; // grabbing the page (calculation the page)
            myPrediction[page] = 1; // in the prediction array, current page is paged in
            if (page == 2){myPrediction[4] = 1;myPrediction[3] = 1;myPrediction[0] = 1;} // when the page is 2, looking at R program, best fit pages will be set to true
            else if (page == 3){myPrediction[0]  = 1;myPrediction[4]  = 1;myPrediction[10] = 1;}
            else if (page == 7){myPrediction[0] = 1;myPrediction[8] = 1;myPrediction[9] = 1;}
            else if (page == 8){myPrediction[0]  = 1;myPrediction[9]  = 1;myPrediction[10] = 1;}
            else if (page == 10){myPrediction[0]  = 1;myPrediction[11] = 1;myPrediction[12] = 1;}
            else if (page == 11){myPrediction[0]  = 1;myPrediction[1]  = 1;myPrediction[12] = 1;}
            else if (page == 12){myPrediction[0]  = 1;myPrediction[9]  = 1;myPrediction[13] = 1;}
            else if (page == 13){myPrediction[0]  = 1;myPrediction[9]  = 1;myPrediction[10] = 1;myPrediction[14] = 1;}
            else if (page == 14){myPrediction[0] = 1;myPrediction[2] = 1;}
            else{myPrediction[0] = 1;myPrediction[page + 1] = 1;myPrediction[page + 2] = 1;} // this is accounting for the default case which are pages 0,1,4,5,6
            int numPages = 0;
            while(numPages < 15)
            {
                if (myPrediction[numPages] == 0){pageout(CurProcess,numPages);}
                if (myPrediction[numPages] == 1){pagein(CurProcess,numPages);}
                numPages++;
            }
        }
        CurProcess++;
    }
} 
