#include "filesystems.h"
/*------------------------------------------*/

metadata *file_open(char *filepath, char *filefilename, char*extension){
    printf("file_open(%s, %s, %s)\n", filepath, filefilename, extension);
    long location_i_in_disk ; // holds the i of block arr
    
    //if the file path is in the root directory 
    if (filepath == NULL) {
        location_i_in_disk = 0;

    } else {
        location_i_in_disk = get_directory_location(filepath);

    }

    //allocate directory pointers 
    struct directory *dirpointer = malloc (sizeof(*dirpointer));

    //open top direocty 
    memcpy(dirpointer, &blockpointer[location_i_in_disk], sizeof( *dirpointer));

    //find location in dir of nmae.extensionension 
    int location_in_directory = file_location(filefilename, extension, dirpointer);

    //if it is -2, then it is not there 
    if (location_in_directory == -1){
        return NULL; 
    }

    printf("opening: \n");
    print_file_entry(&dirpointer-> entry[location_in_directory]);

    //return pointer to the file entry 
    return &dirpointer->entry[location_in_directory];
}

//create function 

void file_create(char *filepath, char*filename, char*extension, int is_dir){
    long directory_location; 

    if (filepath == NULL) {
        directory_location =0;
    } else{
        directory_location = get_directory_location(filepath);

    }

    //declare directory pointer, dirpointer and allocate mem
    struct directory *dirpointer = malloc(sizeof(*dirpointer));

    //copty the directory at dirloc in to the pointer 
    memcpy(dirpointer, &blockpointer[directory_location], sizeof(* dirpointer));

    int location_in_directory;

    location_in_directory = is_entry_empty(dirpointer);

    if(location_in_directory == -1) {
        printf("could not find space. directory is full\n");
        return;
    }

    long i; //location of the new file 

    if (!(i = is_fat_empty())){
        printf("the disk is full \n delete a file or direcotry to continue\n");
        return;
    }

    strcpy(dirpointer-> entry[location_in_directory].filename, filename);
    strcpy(dirpointer->entry[location_in_directory].extension, extension);
    dirpointer->entry[location_in_directory]. fileID = i;
    dirpointer->entry[location_in_directory].is_dir = is_dir;
    dirpointer->entry[location_in_directory].time = time(0);

    //copty pointer into the directory location 
    memcpy(&blockpointer[directory_location], dirpointer, sizeof( *dirpointer));

    free(dirpointer);

    //set fat for this file to -1 for eof
    fatpointer -> fat[i] = 0;

    if (is_dir ==1){

        //write metadata directory entry into a directory pointer
        struct directory *new_directory = initialize_directory();

        //copy the newdirectory pointer to the block array 
        memcpy(&blockpointer[i], new_directory, sizeof(*new_directory));

        free(new_directory);
    }
}


void file_delete(char * filepath, char * filename, char * extension) {


    long disk_indexlocation;
    if (filepath == NULL) {
        disk_indexlocation = 0;
    }
    else
        disk_indexlocation = get_directory_location(filepath);
    

    struct directory * dirpointer = malloc(sizeof(* dirpointer));
    
   
    memcpy(dirpointer, &blockpointer[disk_indexlocation], sizeof(* dirpointer));
    
    int location_in_directory; 
    location_in_directory = find_file_entry(filename, extension, dirpointer);

    if ( location_in_directory == -1) {
        printf("could not to find %s.%s in %s\n", filename, extension, filepath);
        return; 
    }
    
    long fat_in = dirpointer->entry[location_in_directory].fileID;
    
   
    if (!(dirpointer->entry[location_in_directory].is_dir)) {
        do { 
            fatpointer->fat[fat_in] = -1;
            
        } 
        while ( (fat_in = fatpointer->fat[fat_in]) && fat_in != -1 );
    }
    
    else
       
        fatpointer->fat[fat_in] = -1;
    
    strcpy(dirpointer->entry[location_in_directory].filename, "");
    strcpy(dirpointer->entry[location_in_directory].extension, "");
    dirpointer->entry[location_in_directory].fileID = -1;
    dirpointer->entry[location_in_directory].is_dir = -1;
    dirpointer->entry[location_in_directory].time = (time_t)NULL;
    
    memcpy(&blockpointer[disk_indexlocation], dirpointer, sizeof(* dirpointer));
    
    printf("%s%s.%s deleted\n", filepath, filename, extension);
}


void file_write(metadata * filepointer, char * buffer) {
    
    int flag = 0; 
    
    long fat_index = filepointer->fileID; 
    
    long temp; 
    
    if (fatpointer->fat[fat_index] != -1) {

        
        while ( (temp = fatpointer->fat[fat_index] ) != 0 )
            fat_index = temp;
    }
    
    block tempor; 
    
    memcpy(tempor, &blockpointer[fat_index], BLOCK_SIZE);
    
    int i = 0;
    
    while (tempor[i] != '\0')
        i++;
    
    int j = 0;
    
    for ( ; i < BLOCK_SIZE; i++) {
       
        if (buffer[j] != '\0') {
           
            tempor[i] = buffer[j];
//             printf("i=%d, j=%d; (%c = temp[%d]) = ( buffer[%d] = %c )\n", i, j, temp[i], i, j, buffer[j]);
            j++;
        } 
        else { 
            printf("flag = 1\n");
            flag = 1;
            break;
        }
    }
    
    
    memcpy(&blockpointer[fat_index], tempor, BLOCK_SIZE);
    
   
    if (!flag) {   
        
        long next = is_fat_empty();
        
        printf("call file_write_proc(%ld, %s)\n", next, buffer + j);
        
        fatpointer->fat[fat_index] = next; 
        fatpointer->fat[next] = 0; 

        file_write_proc(next, buffer + j);
    }
}


void file_write_proc(long fat_in, char * buffer) {
//     printf("writing %s to blocks[%ld]\n\n", buffer, fat);
    
    long ext; 
    
    block temp; 
    memcpy(temp, &blockpointer[fat_in], BLOCK_SIZE);
    
    int flag = 0; 
    
    int i; 
    for (i = 0; i < BLOCK_SIZE; i++) {
        
        if (buffer[i] != '\0') {
            temp[i] = buffer[i]; 
//             printf("(temp[%d] = %c) = (buffer[%d] = %c)\n", i, temp[i], i, buffer[i]);
        }
        else {
          
            printf("flag = 1\n");
            flag = 1; 
            break; 
        }
    }

    memcpy(&blockpointer[fat_in], temp, BLOCK_SIZE);
    
   
    if (!flag) {
        ext = is_fat_empty(); 
        fatpointer->fat[fat_in] = ext;
        fatpointer->fat[ext] = 0; 
      
        return file_write_proc(ext, buffer + BLOCK_SIZE);
    }
}


void file_read(metadata * filepointer) {
    

    char * arr = malloc(8);
    
   
    long fat_index = filepointer->fileID;
    
    block temp; 
    
    do {
    
        memcpy(temp, &blockpointer[fat_index], BLOCK_SIZE);
        
        strcat(arr, temp);
    }
    while ( (fat_index = fatpointer->fat[fat_index]) != 0 );
    

    printf("%s\n", arr);
}
    
    








