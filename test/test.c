/* 
* Course: High Performance Computing 2021/2022
* 
* Lecturer: Francesco Moscato   fmoscato@unisa.it
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
    @file test.c
*/

// PURPOSE OF THE FILE: Test the functions for the radix sort algorithm. 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "../include/radixsort.h"

#ifdef _OPENMP
	#include <omp.h>
#else
	#define get_thread_num() 0
#endif

typedef void (* decorableInit)(ELEMENT_TYPE **, int, int);
typedef int (* decorableMax)(ELEMENT_TYPE *, int);
typedef void (* decorableSort)(ELEMENT_TYPE *, int);

/**
 * @brief This function tests the initialization of the vector needed in the program.
 * @param array      pointer to the vector.
 * @param size       size of array.
 * @param threads    number of threads.
 */
void init_structure_test(ELEMENT_TYPE *array, int size, int threads){

    int expected_array[8] = {226586,828486,77349,41049,731641,625895,441364,364456};

    for (int i=0; i<size; i++){
        assert(array[i] == expected_array[i]);
    }

}

/**
 * @brief This function tests the max of the vector.
 * @param array     pointer to the vector.
 * @param size      size of array.
 * @param getMax    decorated function getMax.
 */
void getMax_test(ELEMENT_TYPE *array, int size, decorableMax getMax){

    int max = getMax(array, size);

    int expected_max = 828486;

    assert(max == expected_max);
}

/**
 * @brief This function tests the sorting algorithm.
 * @param array        pointer to the vector to be sorted.
 * @param size         size of array.
 * @param radixsort    decorated function radixsort.
 */
void radixsort_test(ELEMENT_TYPE *array, int size, decorableSort radixsort){

    radixsort(array, size);

    int expected_array[8] = {41049,77349,226586,364456,441364,625895,731641,828486};

    for (int i = 0; i < size; i++)
        assert(array[i] == expected_array[i]);
}


int main(char const **argv, int argc){

	ELEMENT_TYPE *array;
    int threads = 3;
    int size = 8;

    (decorableInit) init_structures;
    init_structures (&array, size, threads);

    init_structure_test(array, size, threads);

    getMax_test(array, size, getMax);

    radixsort_test(array, size, radixsort);

    exit(EXIT_SUCCESS);
}
