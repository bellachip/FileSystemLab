#include "test_main.h"


/*
 * f_create thakes in file path, name, ext, diror file) and  creates a directory or file at path/name.ext 
 * NULL path indicates root directory
 **/
void f_create(char *filepath, char *filename, char *extension, int dirorfile)
{
    long dir_index; //parent index to new file 

    //NULL path implies root; otherwirse get directory location
    if (filepath == NULL)
    {
        dir_index = 0;
    }
    else
    {
        dir_index = get_directory_location(filepath);
    }


    //declare directory pointer, dir pontier, and allocate memory 
    struct directory *dirpointer = malloc(sizeof(*dirpointer));

    //cpy the dir at direcotyr location in to the pointer 
    memcpy(dirpointer, &blocks[dir_index], sizeof(*dirpointer)); 


    int index_indir; //index location in directory 
    if ((index_indir = is_entry_empty(dirpointer)) == -1)
    {
        printf("the directory was full\n");
        return;
    }

    long index; //index of new file 
    if (!(index = is_fat_empty())) //find free mat 
    {
        printf("the disk is full!\ndelete a file or directory to continue\n");
        return;
    }
    //init the dir entry with new file info
    strcpy(dirpointer->entry[index_indir].filename, filename);
    strcpy(dirpointer->entry[index_indir].extension, extension);
    dirpointer->entry[index_indir].fileID = index;
    dirpointer->entry[index_indir].dirorfile = dirorfile;
    dirpointer->entry[index_indir].time = time(0);

    //copt the dirpointer into the directory location
    memcpy(&blocks[dir_index], dirpointer, sizeof(*dirpointer)); //cpy the pointer into the dir location

    free(dirpointer); //dirpointer has no more use 

    //set fat for this file to 0 that means end of file 
    fatpointer->file_allocation_table[index] = 0; //set fat table for the current file to 0 meaning end of file 
    //if this is to be a directory 
    if (dirorfile == 1)
    {
        //write metata directory entries into a directory pointer 
        struct directory *create_directory = initialize_directory();
        //copty that directory pointer into blocks[index]
        memcpy(&blocks[index], create_directory, sizeof(*create_directory));
        free(create_directory);
    }
}

/* 
 *f_open(filepath, filename, extension) opens the file (or directory) in the directory 
 * specified by path with the name "name.ext"
 * path must only specify directory path */
metadata *f_open(char *filepath, char *filename, char *extension)
{

    printf("f_open(%s, %s, %s)\n", filepath, filename, extension);
    long index_indisk;    //holds the index of block[] 
    if (filepath == NULL) //if the file is in root set the location to 0th index in the drive
                          //else get the init the dir path
    {
        index_indisk = 0;
    }
    else
    {

        index_indisk = get_directory_location(filepath);
    }

    //allocate the directory pointer
    struct directory *dirpointer = malloc(sizeof(*dirpointer));
    //open top directory 
    memcpy(dirpointer, &blocks[index_indisk], sizeof(*dirpointer));
    //find index of name.extension in the directory
    int index_indir = file_location_entry(filename, extension, dirpointer);
    
    
    //if -1 its empty
    if (index_indir == -1)
    {
        return NULL;//fail 
    }

    printf("opening file:\n");
    print_file_meta(&dirpointer->entry[index_indir]);

    //return a pointer to the file entry 
    return &dirpointer->entry[index_indir]; //returning poitner to the entry
}

/*
 * f_open (filepath, filename, extsion) deletes the directory or file located at path/name.ext 
 * a NULL path implies the root directory
 * a path must be all directories
 * */
void f_delete(char *filepath, char *filename, char *extension)
{
  //first locate the file at the end of the path 
    long index_indisk;  //blocks[] index
    if (filepath == NULL)
    {
        index_indisk = 0;
    }
    else
        index_indisk = get_directory_location(filepath);
    //declare and allocate direcotyr pointer
    struct directory *dirpointer = malloc(sizeof(*dirpointer));
    //open top directory 
    memcpy(dirpointer, &blocks[index_indisk], sizeof(*dirpointer));
    //directory entry number
    int index_indir; //entry index
    //find the directory entry mathching name.ext
    index_indir = file_location_entry(filename, extension, dirpointer);
    if (index_indir == -1)
    {
        printf("could NOT find %s.%s in %s\n", filename, extension, filepath);
        return;
    }

    //second clear the fat 

    //the first fat entry is the file number 
    long fat = dirpointer->entry[index_indir].fileID;

    /* 
     * if there may be more than one fat_table filled
     * i.e. dirorfile  == 0
     * */

    if (!(dirpointer->entry[index_indir].dirorfile))
    {
        do
        {
            //sets the fat entry to -1 (empty)
            fatpointer->file_allocation_table[fat] = -1;

        }// while fat table entry does not equal 0 or -1  
        while ((fat = fatpointer->file_allocation_table[fat]) && fat != -1);
    }

    else
        // set fat entry to -1 (empty) 
        fatpointer->file_allocation_table[fat] = -1;

//third, set the direstory entry as empty
    strcpy(dirpointer->entry[index_indir].filename, "");
    strcpy(dirpointer->entry[index_indir].extension, "");
    dirpointer->entry[index_indir].fileID = -1;
    dirpointer->entry[index_indir].dirorfile = -1;
    dirpointer->entry[index_indir].time = (time_t)NULL;
    //copt the directory pointer back into the blocks 
    memcpy(&blocks[index_indisk], dirpointer, sizeof(*dirpointer));

    printf("%s%s.%s deleted\n", filepath, filename, extension);
}

/* 
 * write(fp, buffer) appends the file pointed to by fp with buf 
 * fp is a file opened with open(path, name, ext) and buf is a string
 * */
void f_write(metadata *fp, char *buffer)
{

    int flag = 0; //flag = 1 indicates there is no more buf to write */

    long fat = fp->fileID; //the initial fat location

    long var; //var holds next fat location

    //if fat table [fat]is not empty
    if (fatpointer->file_allocation_table[fat] != -1)
    {
        //continue to eof block
        while ((var = fatpointer->file_allocation_table[fat]) != 0)
            fat = var;
    }

    block temporary; // temp block to hold a block's worth of buf 

     // copy EOF block (i.e. the end of the file) into temp 
    memcpy(temporary, &blocks[fat], BLOCK_SIZE);

    int i = 0;
    while something is written 
    while (temporary[i] != '\0')
        i++; //increment past it 

    
    int j = 0;
    //for the rest of the eof block 
    for (; i < BLOCK_SIZE; i++)
    {
        //if there is more to write 
        if (buffer[j] != '\0')
        {
            //copy it into temp
            temporary[i] = buffer[j];

            j++;
        }
        else
        {
            printf("flag = 1\n");
            flag = 1;
            break;
        }
    }

    memcpy(&blocks[fat], temporary, BLOCK_SIZE);

    if (!flag)
    {

        long next = is_fat_empty();

        printf("calling f_writing(%ld, %s)\n", next, buffer + j);

        fatpointer->file_allocation_table[fat] = next;
        fatpointer->file_allocation_table[next] = 0;

        f_writing(next, buffer + j);
    }
}


/* 
 * f_writing(fat, buf) is a recursive function which writes buf to blocks[] index fat 
 * if buf is larger than BLOCKSIZE then gfs_writing recursively calls
 *f_writiing (next, buf + BLOCKSIZE); where next is some free blocks[] block
 * */
void f_writing(long fat, char *buffer)
{
    // printf("writing %s to blocks[%ld]\n\n", buf, fat);
    int flag = 0; //fkag is set to 1 if we each the end of buf
    long next;

    block temporary;
    memcpy(temporary, &blocks[fat], BLOCK_SIZE);

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        //if buffer[i] is not \0 then set the buffer blocks ith char to buffers ith char
        if (buffer[i] != '\0')
        {
            temporary[i] = buffer[i];
        }
        else
        {
             // if buf[i] is NULL then we reached the end */
            printf("flag = 1\n");
            flag = 1; //set flag to 1 indicating no recursive call */
            break; //leave for loop */
        }
    }

    memcpy(&blocks[fat], temporary, BLOCK_SIZE);

//if there is more in buffer 
    if (!flag)
    {
        next = is_fat_empty(); //find a free block
        fatpointer->file_allocation_table[fat] = next; //set the current fat table entry to point to the next block for this file 
        //set the next block to eof
        fatpointer->file_allocation_table[next] = 0;
        //recursively call this function on next and buffer+BLOCKSIZE
        return f_writing(next, buffer + BLOCK_SIZE);
    }
}

/* f_open(fp) reads the file pointed to by fp and printfs it */
void f_read(metadata *fp)
{
    //declare and allocate a char pointer
    block temporary; //declares a buffer block temporary 
    char *buffer = malloc(8);
    //the first fat is the file number of fp
    long fat = fp->fileID; 
    //printf("fat = %ld\n", fat);

    do
    {
        //put data at fat into buffer block
        memcpy(temporary, &blocks[fat], BLOCK_SIZE);
        //append buffer with the data in temporary 
        strcat(buffer, temporary);
    } //while the correspoinding fat table entry !=0  
    while ((fat = fatpointer->file_allocation_table[fat]) != 0);
    //prints the buffer 
    printf("%s\n", buffer);
}
