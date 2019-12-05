#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>

#define BLOCK_SIZE 512 //one block size is 512
#define FILE_SIZE 2101248 //
#define NUM_BLOCKS 4096 // 
#define METASIZE 64 //size of metadata 
#define ENTRY_PER_DIR 8  //BLOCKSIZE/METASIZE


typedef struct{
    char filename[36];  //46 bytes
    char extension[3]; // 3 bytes
    int fileID; //block index of the file 
    int is_dir; // 0 = not a directory; 1 = is a direcotry 
    time_t time; //last modified time 
} metadata;

typedef struct{
    long fat[NUM_BLOCKS]; //  -2 is eof
} FAT;

typedef char block[BLOCK_SIZE];
// array of file entries in one block 
struct directory {
    metadata entry [BLOCK_SIZE/METASIZE];

};

FAT *fatpointer; //fat table pointer 
block *blockpointer; //block pointer 

//functionality functions 
struct directory * initialize_directory(void);
long is_fat_empty(void);
int is_entry_empty(struct directory *dirpointer);
int is_entry_empty_a(long dir_loc);
int file_location(char *filename, char *extension, struct directory *(dirpointer));
int file_location_a(char *filename, char *extension, long dir_loc);
char **get_folders(char *path_t);
long get_directory_location(char *path);
void print_file_entry(metadata *metaptr);
void print_dir(char *path);
void printdir(long dir_loc);

 //main
char *do_mapping(char *drivep);
void format(char *address);
void init_root(void);

void test(void);

//builtins

metadata *file_open(char *filepath, char *filename, char*fileextension);
void file_create(char *filepath, char*filename, char*extension, int is_dir);
void file_delete(char * filepath, char * filename, char * extension) ;
void file_writing(metadata * fp, char * buf) ;
void file_write_proc(long fat, char * buf) ;
void file_read(metadata * fp) ;





