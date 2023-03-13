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
    @file main.c
*/

#include "radixsort.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]){

    double time_init = 0.0, time_radixsort = 0.0;

    ELEMENT_TYPE *array;
    int threads, size;

    if(argc < 3){
        printf("ERROR! Usage: ./main threads size");
        exit(1);
    }

    size = atoi(argv[1]);
    threads = atoi(argv[2]);

    // Init Structure
    STARTTIME(1);
    init_structures(&array, size, threads);
    ENDTIME(1, time_init);

    // Radix Sort
    STARTTIME(2);
    radixsort(array, size);
    ENDTIME(2, time_radixsort);

    // Print results
    //printArray(array, size);
    printf("%d;%d;%f;%f\n", size, threads, time_init, time_radixsort);
    free(array);

    return 0;
}