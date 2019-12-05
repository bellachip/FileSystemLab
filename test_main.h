#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>

#define METASIZE 64 //size of metata data 
#define BLOCK_SIZE 512
#define FILE_SIZE 2101248  //2^21 (blocks[] size) + 2^21 / 2^9 (fat table size) */
#define ENTRY 8 //number of metadta which fit in one block  /* BLOCK_SIZE/ENTRY_SIZE */
#define NUM_BLOCKS 4096  //(FILE_SIZE - NUNM_BLOCKS * 4)/BLOCK_SIZE */

typedef char block[BLOCK_SIZE];

typedef struct
{
    char filename[36]; //46
    char extension[3]; //3
    long fileID; //block index of the file
    int dirorfile; // 0 not a dir 
    time_t time; //last modified time
} metadata;

typedef struct
{
    long file_allocation_table[NUM_BLOCKS]; // 0 oef and -1 empty 
} FAT;
//directory is an array of file entries which is containedin a single block 
struct directory
{
    metadata entry[BLOCK_SIZE / METASIZE];
};
//global pointers
FAT *fatpointer;
block *blocks; //block pointer 

//test_util.c
struct directory *initialize_directory(void);
long is_fat_empty(void);
int is_entry_empty(struct directory *);
int is_entry_free_a(long dir_loc);
int file_location_entry(char *filename, char *extension, struct directory *dirp);
int file_location_entry_a(char *filename, char *extension, long dir_loc);
char **get_dir(char *path_t);
long get_directory_location(char *path);
void print_file_meta(metadata *filepointer);
void print_directory(char *path);
void print_directory_a(long dir_loc);

//test_func.c
metadata *f_open(char *path, char *filename, char *extension);
void f_create(char *path, char *filename, char *extension, int dirorfile);
void f_delete(char *path, char *filename, char *extension);
void f_write(metadata *filepointer, char *buf);
void f_writing(long fat, char *buf);
void f_read(metadata *filepointer);

//test_test.c
void f_test(void);
