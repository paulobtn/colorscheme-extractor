/* a small library to create arrays of strings */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// given the address of a string array, its current size and some string str
// add str to the last position of the array. If it doesn't exist, the
// allocation is automatic
// the return value is the new array size
size_t arr_str_push(char *** str_arr, unsigned int arr_size, char* str);


// given the address of a string array, its current size and an index
// deletes the position dealocating properly.
// if the array only has one element, the array will be freed
// it returns the new size
size_t arr_str_delete(char *** str_arr, size_t arr_size, unsigned int pos);

//dealocate the array of strings deleting all elements in a memory safe way
void arr_str_free(char **str_arr, size_t arr_size);

