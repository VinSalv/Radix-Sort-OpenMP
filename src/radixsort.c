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
    @file radixsort.c
*/

// PURPOSE OF THE FILE: Implementation of the functions for the radix sort algorithm. 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "radixsort.h"

#ifdef _OPENMP
#include <omp.h>
#else
#define get_thread_num() 0
#endif

#define MAX 10 // Number of plates

/**
* @brief This function prints an array.
* @param array    pointer to the vector to be sorted.
* @param size     size of array.
*/
void printArray(ELEMENT_TYPE *array, int size) {

    for (int i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

/**
* @brief This function reads the array from file.
* @param array    pointer to the vector to be sorted.
* @param size     size of array.
*/
void read_array(ELEMENT_TYPE *array, int size){

    // Init variables
    FILE *fd;
    char buf[10];
    char *res;
    int i=0;

    // Check the file
    fd=fopen("random_numbers.txt", "r");
    if( fd==NULL ) {
        perror("Errore in apertura del file");
        exit(1);
    }

    // Read from file to fill the array
    while(i<size) {
        res=fgets(buf, 10, fd);
        array[i++] = atoi(buf);
    }

    fclose(fd);
}

/**
* @brief This function initializes all the data structures needed in the program.
* @param array      pointer to the vector to be sorted.
* @param size       size of array.
* @param threads    number of threads.
*/
void init_structures(ELEMENT_TYPE **array, int size, int threads){

    ELEMENT_TYPE *temp_array;

    // Init array
    temp_array = (ELEMENT_TYPE *)malloc((size) * sizeof(ELEMENT_TYPE));
    if (array == NULL)
        perror("Memory Allocation - array");

    // Read the array from file
    read_array(temp_array, size);

    *array = temp_array;
}

/**
* @brief This function returns the maximum array's element.
* @param array    pointer to the vector to be sorted.
* @param size     size of array.
*/
/*int getMax(ELEMENT_TYPE *array, int size) {
    
    // Init variables
    int max = -1;
    int ret_max = max;

    #pragma omp parallel shared(ret_max) firstprivate(max)
    {

        // Init variables
        int i;
        int nthreads = omp_get_num_threads(); // Number of threads
        int num_elem_chunk_size = size / nthreads;
        if (num_elem_chunk_size == 0) 
            num_elem_chunk_size=1;

        // Find max array's element
        #pragma omp for schedule(static, num_elem_chunk_size)
        for (i = 0; i < size; i++)
            if (array[i] > max)
                max = array[i];

        #pragma omp critical
        {
            if (max > ret_max) 
                ret_max=max;
        }

        #pragma omp barrier
    }

    return ret_max;
}*/

/**
* @brief This function returns the maximum array's element.
* @param array    pointer to the vector to be sorted.
* @param size     size of array.
*/
int getMax(ELEMENT_TYPE *array, int size) {
    int max_val = array[0];

    #pragma omp parallel for reduction(max:max_val) 
    for (int idx = 0; idx < size; idx++)
        if (array[idx] > max_val)
            max_val = array[idx];

    return max_val;
}

/**
* @brief This function sorts the vector on the basis of significant place.
* @param array    pointer to the vector to be sorted.
* @param size     size of array.
* @param place    current digit which it is considered (units, tens, hundreds,...).
*/
void countingSort(ELEMENT_TYPE *array, int size, int place) {

    // Init variables
    ELEMENT_TYPE *output; // Vector in which is stored the sorted array by place
    output = (ELEMENT_TYPE *) malloc((size) * sizeof(ELEMENT_TYPE));
    if (output == NULL)
        perror("Memory Allocation - array");
    int count[MAX] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Vector that takes in account the occurrence of elements in each individual place
    int shared_count[MAX] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Vector for cumulative sum

    #pragma omp parallel shared(output, size, place, shared_count, array) firstprivate(count)
    {

        // Init variables
        int i;
        int nthreads = omp_get_num_threads(); // Number of threads
        int num_elem_chunk_size = size / nthreads;
        if (num_elem_chunk_size == 0) 
            num_elem_chunk_size = 1;

        // Init output
        #pragma omp for nowait
        for(i = 0; i<size; i++)
            output[i] = 0;

        // Set count with the number of repeated digits on the basis of place
        #pragma omp for schedule (static, num_elem_chunk_size) nowait
        for (i = 0; i < size; i++)
            count[(array[i] / place) % 10]++;

        // Cumulative sum of count
        #pragma omp critical
        for (i = 0; i < 10; i++){
            if (i != 0)
                count[i] += count[i - 1];
            shared_count[i] += count[i];
        }

        #pragma omp barrier

        // Sort output by place
        #pragma omp single
        {
            for (i = size - 1; i >= 0; i--) {
                output[shared_count[(array[i] / place) % 10] - 1] = array[i];
                shared_count[(array[i] / place) % 10]--;
            }
        }

        // Store output's result into array
        #pragma omp for schedule (static, num_elem_chunk_size)
        for (i = 0; i < size; i++)
            array[i] = output[i];
    }

    free(output);
}

/**
* @brief This function implements Radix Sort algorithm.
* @param array    pointer to the vector to be sorted.
* @param size     size of array.
*/
void radixsort(ELEMENT_TYPE *array, int size) {

    // Get maximum array's element
    int max = getMax(array, size);

    // Sort elements on the basis of place.
    for (int place = 1; max / place > 0; place *= 10)
        countingSort(array, size, place);
}