/* 
* Course: High Performance Computing 2021/2022
* 
* Lecturer: Francesco Moscato	fmoscato@unisa.it
*
* Group:
* Lamberti      Martina     0622701476  m.lamberti61@studenti.unisa.it
* Salvati       Vincenzo    0622701550  v.salvati10@studenti.unisa.it
* Silvitelli    Daniele     0622701504  d.silvitelli@studenti.unisa.it
* Sorrentino    Alex        0622701480  a.sorrentino120@studenti.unisa.it
*
* Copyright (C) 2021 - All Rights Reserved
*
* This file is part of EsameHPC.
*
* Contest-OMP is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Contest-OMP is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Contest-OMP.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
    @file radixsort.h
*/

// PURPOSE OF THE FILE: Prototypes Definitions of the functions used in radixsort.c

#ifndef RADIXSORT_H_ 
#define RADIXSORT_H_

/** 
*   macros to get execution time: both macros have to be in the same scope
*   define a double variable to use in ENDTIME before STARTTIME:
*   double x;
*   the variable will hold the execution time in seconds.
*/

#include <time.h>

#define STARTTIME(id) \
    clock_t start_time_42_##id, end_time_42_##id; \
start_time_42_##id = clock()
#define ENDTIME(id, x) \
end_time_42_##id = clock(); \
x = ((double)(end_time_42_##id - start_time_42_##id)) / CLOCKS_PER_SEC

void init_structures(ELEMENT_TYPE **, int, int);

int getMax(ELEMENT_TYPE *, int);

void countingSort(ELEMENT_TYPE *, int, int);

void radixsort(ELEMENT_TYPE *, int);

void printArray(ELEMENT_TYPE *, int);

void read_array(ELEMENT_TYPE *, int);

#endif
