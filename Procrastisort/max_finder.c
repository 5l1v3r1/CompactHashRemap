/* Copyright 2015-19.  Triad National Security, LLC. This material was produced
 * under U.S. Government contract 89233218CNA000001 for Los Alamos National 
 * Laboratory (LANL), which is operated by Triad National Security, LLC
 * for the U.S. Department of Energy. The U.S. Government has rights to use,
 * reproduce, and distribute this software.  NEITHER THE GOVERNMENT NOR
 * TRIAD NATIONAL SECURITY, LLC MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR
 * ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.  If software is modified
 * to produce derivative works, such modified software should be clearly marked,
 * so as not to confuse it with the version available from LANL.   
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy
 * of the License at 
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * Under this license, it is required to include a reference to this work.
 *
 * This is LANL Copyright Disclosure C16017/LA-CC-15-102
 *
 * Authors: Bob Robey         XCP-2   brobey@lanl.gov
 *          Gerald Collom     XCP-2   gcollom@lanl.gov
 *          Colin Redman      XCP-2   credman@lanl.gov 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <omp.h>
#include <math.h>

struct timeval timer;

typedef unsigned int uint;


/* generate a randomly mixed up array with size size to be stored in pointer. the elements will have a minimum value min, and
    the difference between elements when sorted will be between mindx and maxdx. the maximum value is recorded in max. */
double generate_array( uint size, double *ptr, double mindx, double maxdx, double min, double *max ) {
    
    double swap;
    int index, front = 0;
    double running_min = maxdx;
        
    ptr[0] = min;        //start the array using the minimum value
    
    /* for each element, add a random value between mindx and maxdx to the previous element's value */
    for(int i = 1; i < size; i++) {
        ptr[i] = ptr[i-1] + mindx + ((double)rand() * (maxdx - mindx) / (double)RAND_MAX);
        if(ptr[i]-ptr[i-1] < running_min) running_min = ptr[i]-ptr[i-1];
    }

    *max = ptr[size-1];                    //set the max value to the last element's value
    //*max = min + (size-1) * maxdx;    //force the range for timings isolating a different variable
    
    /* Mix up the array by selecting elements from shrinking front portion of array and placing them on back end of array */
    for(int i = 0; (i < size) && (size - i != 0) ; i++) {
        index = (rand()) % (size - i - front) + front;
        swap = ptr[size-i-1];
        ptr[size-i-1] = ptr[index];
        ptr[index] = swap;
    }
    return running_min;
}
    
    
   

double find_max (double* arr, uint length){
    double max;
    double* sub_maxes = (double*)malloc(omp_get_max_threads()*sizeof(double));
    #pragma omp parallel shared(sub_maxes,length)
    {
        int id = omp_get_thread_num();
        int num = omp_get_num_threads();
        int chunk_size = length/num;
        int my_start = chunk_size*id;
        double* my_max = &sub_maxes[id];
        *my_max = arr[my_start];
        
        for(int i = my_start+1; i < my_start+chunk_size; i++){
            if(*my_max<arr[i]){
                *my_max=arr[i];
            }
        }
    }
    max = sub_maxes[0];
    for(int i = 1; i<omp_get_max_threads();i++){
        if(max<sub_maxes[i]){
            max = sub_maxes[i];
        }
    }
    
    
    return max;
}

int main (){
    double t1,t2;
    uint length = 0x1fffffff;
    double max = 0;
    double* arr = (double*)malloc(length*sizeof(double));
    generate_array(length,arr,1.,1.,0.,&max);
    for (int i = 0; i < length; i++){
    //    printf("arr: %f \n", arr[i]);
    }
    
    gettimeofday(&timer, NULL);
    t1 = timer.tv_sec+(timer.tv_usec/1000000.0);
    double parallel_max = find_max(arr,length);
    gettimeofday(&timer, NULL);
    t2 = timer.tv_sec+(timer.tv_usec/1000000.0);
	printf("\nparallel max time: %0.6lf\n", t2 - t1);
    
    printf("parallel max found: %f real max: %f\n", parallel_max, max);
    
    gettimeofday(&timer, NULL);
    t1 = timer.tv_sec+(timer.tv_usec/1000000.0);
    double serial_max = arr[0];
    for(int i = 1; i < length; i++){
        if(max<arr[i]){
            max = arr[i];
        }
    }
    gettimeofday(&timer, NULL);
    t2 = timer.tv_sec+(timer.tv_usec/1000000.0);
	printf("serial max time: %0.6lf\n", t2 - t1);
    
    printf("serial max found: %f real max: %f\n", serial_max, max);
}
