#ifndef STRFUNCS_H
#define STRFUNCS_H

#include <stdio.h>
#include "structs.h"

//-------------------------------------------------------------------------------------
/// Counts the number of occurrences of a character in a buffer.
///
/// @param[in] buf The buffer to search.
/// @param[in] size The size of the buffer.
/// @param[in] smlp The character to search for.
///
/// @return The number of occurrences of the character.
///
//-------------------------------------------------------------------------------------
int chrncnt(char* line, char smpl, size_t size);



//-------------------------------------------------------------------------------------
/// Returns the size of the file in bytes
///
/// @param flnm Path to the fil
///
/// @return Size of the file in bytes on succes, -1 on failure.
///
//-------------------------------------------------------------------------------------
long long rdflsz(FILE* fl);



//-------------------------------------------------------------------------------------
/// Parses the data in the buffer into a buffer of pointers to lines.
///
/// @param fldt The file data structure.
/// @param sep  A separator
///
/// @note The previous data of the buffer to parse will be removed.
///
//-------------------------------------------------------------------------------------
void prsdata(struct filedata* fldt, const char sep = '\n');



//-------------------------------------------------------------------------------------
/// Frees the memory allocated for the file data structure and its data.
///
/// @param fldt Pointer to the file data structure.
///
//-------------------------------------------------------------------------------------
void filedatastrdestr(struct filedata* fldt);



//-------------------------------------------------------------------------------------
/// Parses the input and output file names from the command line arguments.
///
/// @param argc Number of command line arguments.
/// @param argv Array of command line arguments.
/// @param ionm Pointer to the io_data structure to store the parsed names.
///
/// @return 0 on success, non-zero on failure.
///
//-------------------------------------------------------------------------------------
int ioflnmsprs(int argc, char** argv, struct io_data* ionm);


#endif
