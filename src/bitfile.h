/*
READ BITS by bathtaters
*/

#ifndef READ_BITS_H
#define READ_BITS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* -- LIBRARY OPTIONS -- */

/* Add spacing between bytes for printbin() */
#define PRINT_BYTE_SPACES true
/* Access mode for temp file */
#define TMP_FILE_ACCESS "wb+"

/* -- CONSTANTS & MACROS -- */

/* Max size of access_mode (including NUL) */
#define ACCESS_MODE_LEN 4
/* Length of 1 byte in bits */
#define BYTE_LEN 8
/* x / y rounded up to nearest whole number (Expects integer x & y) */
#define CEIL_DIV(x,y) (1 + (((x) - 1) / (y)))

/* -- DATA TYPES -- */

/* Type used to store raw byte */
typedef uint8_t byte_t;
/* Format string for raw byte */
#define BYTE_STR "d"

/* Size in bits of a bit file */
typedef uint64_t bsize_t;
/* Format string for size in bits of a bit file */
#define BSIZE_STR "llu"

/* Bit position within a bit file */
typedef int64_t bpos_t;
/* Format string for bit position within a bit file */
#define BPOS_STR "lls"

/* Full position within a file */
typedef struct bfpos_t {
    fpos_t byte;
    uint8_t bit;
} bfpos_t;

/* Data object for bitfile functions
   (DO NOT modify this directly!) */
typedef struct BITFILE {
    /* File object to read from */
    FILE* _fileobj;
    /* Current byte being read */
    int _currbyte;
    /* Current bit offset within this.byte */
    int8_t _bitoffset;
    /* Read starting from left (true) or right (false) */
    bool _msb;
} BITFILE;


/* -- OPEN/CLOSE FUNCTIONS -- */

/* Opens the file pointed to by filename using the given mode & bit order
    - Access Modes: r, w, a, r+, w+, a+
    - MSB_First = true: Most-Sig Bit First (Left-to-right)
    - MSB_First = false: Least-Sig Bit First (Right-to-left) */
BITFILE* bfopen(const char* filename, const char* access_mode, bool msb_first);
/* Flushes all buffers and closes the file */
int bfclose(BITFILE* bitfile);
/* Associates a new filename with the given bitfile while closing the old file in stream. */
/* Creates a temporary file in update mode (wb+).
    - nametemplate must end with "XXXXXX" */
BITFILE* tmpbitfile(char* nametemplate, bool msb_first);


/* --- READ/WRITE FUNCTIONS --- */

/* Reads data from the given file into the array pointed to by ptr
    - save_to_ptr size must be at least bitCount/8 */
uint64_t bfread(void* save_to_ptr, bsize_t number_of_bits, BITFILE* bitfile);


/* --- POSITION FUNCTIONS --- */

/* Sets the file position of the stream to the offsets from the whence position
    - Accepts negative offsets and bit_offset > 8
    - Whence: SEEK_CUR, SEEK_SET, SEEK_END */
int bfseek(BITFILE* bitfile, bpos_t offset, int whence);
/* Returns the current bit position of the given bit file. */
bpos_t bftell(BITFILE *bitfile);
/* Sets the position to the beginning of the bit file. */
void bfrewind(BITFILE *bitfile);
/* Gets the current position of the bit file and writes it to pos. */
int bfgetpos(BITFILE *bitfile, bfpos_t *pos);
/* Sets the file position of the given bit file to the given position. */
int bfsetpos(BITFILE *bitfile, const bfpos_t *pos)


/* -- UTILITY FUNCTIONS -- */

/* Swap endianess of bin_data of length number_of_bits */
void swapendian(void* bin_data, bsize_t number_of_bits);
/* Print binary value of bin_data of length number_of_bits */
void printbin(const void* bin_data, bsize_t number_of_bits);


#endif