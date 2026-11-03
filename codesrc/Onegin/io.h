#ifndef IO_H
#define IO_H

#include "structs.h"

#include <stdlib.h>



//-------------------------------------------------------------------------------------
/// Opens the input and output files for reading and writing, respectively.
///
/// @param[in] inp The input file to open.
/// @param[in] out The output file to open.
/// @param[in] io_data The io_data struct containing the file names.
///
/// @return 0 on success, non-zero on failure.
//-------------------------------------------------------------------------------------
int opnfls(FILE** inp, FILE** out, struct io_data io_data);



//-------------------------------------------------------------------------------------
/// Closes the input and output files.
///
/// @param[in] inp The input file to close.
/// @param[in] out The output file to close.
///
/// @return 0 on success, non-zero on failure.
//-------------------------------------------------------------------------------------
int clsfls(FILE* inp, FILE* out);



//-------------------------------------------------------------------------------------
///Prints the contents of an array of strings to the console.
///
/// @param[in] arr The array of strings to print.
/// @param[in] nlines The number of lines in the array.
///
//-------------------------------------------------------------------------------------
void printArr(string arr[], int nlines, FILE* out);



//-------------------------------------------------------------------------------------
/// Reads the contents of a file into a filedata struct.
///
/// @param[in] fldt The filedata struct to read into.
///
/// @return 0 on success, non-zero on failure.
///
//-------------------------------------------------------------------------------------
int rdfrmfl(struct filedata* filedata);



#endif
