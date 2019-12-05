#include "filesystems.h"

/*--------------------------------------------*/
struct directory * initialize_directory(void){
    struct directory * dirptr = malloc(sizeof(*dirptr));
    int i; 
    for (int i=0; i<8; i++){
        strcpy(dirptr->entry[i].filename, "");
        strcpy(dirptr->entry[i].extension, "");
        dirptr->entry[i].fileID= -1;
        dirptr->entry[i].is_dir= 0;
        dirptr->entry[i].time = (time_t) NULL;
    }
    return dirptr; //returns pointer to entries
}

long is_fat_empty(void){
    long i=1;
    while (i<NUM_BLOCKS){
        if (fatpointer-> fat[i] == -1){
            return i;
        }
        i++;
    }
    return 0;
}


int is_entry_empty(struct directory *dirpointer){
    for (int i=0; i< ENTRY_PER_DIR; i++){
        if (dirpointer->entry[i].filename == -1){
            return i; 
        }
    }
    return -1;
}

int is_entry_empty_a(long dir_loc){
    struct directory *dirpointer = malloc(sizeof(*dirpointer));
    memcpy(dirpointer, &blockpointer[dir_loc], sizeof(*dirpointer));

    for (int i=0; i < ENTRY_PER_DIR; i++){
        if (dirpointer->entry[i].fileID == -1){
            return i;
        }
    }
    return -1;
}

int file_location(char *filename, char *extension, struct directory *(dirpointer)){
    
    for(int i =0; i< ENTRY_PER_DIR; i++){
        if(!strcmp(dirpointer->entry[i].filename, filename) && !strcmp(dirpointer->entry[i].extension, extension)){
            if(!dirpointer -> entry[i].is_dir){
                printf("%s.%s is a file\n", filename, extension);
                return i;
            } else {
                printf("%s.%s is a directory\n", filename, extension);
                return i;
            }
        }
    } 
    return -1;
}

int file_location_a(char *filename, char *extension, long dir_loc){
    struct directory *dirpointer = malloc (sizeof(*dirpointer));

    memcpy(dirpointer, &blockpointer[dir_loc], sizeof(*dirpointer));

    for (int i=0; i < ENTRY_PER_DIR; i++){
        if (!strcmp(dirpointer->entry[i].filename, filename)&& !strcmp(dirpointer->entry[i].extension, extension)){
            if (!dirpointer-> entry[i].is_dir){
                printf("%s.%s is a file\n", filename, extension);
                return i;
            } else {
                printf("%s.%s is a dir\n", filename, extension);
            }
        }
    }
    return -1; 
}

char **get_folders(char *path_t){
    char *path = malloc(sizeof(path));
    strcpy(path, path_t);

    int i =0;
    char **buffer = malloc (64 * sizeof(char*));
    if (buffer[0] = strtok(path, "/") == NULL){
        printf("in get_folders, strtok() failed\n");
        return NULL; 
    }

    while((buffer[++i] = strtok(NULL, "/")) != NULL){
        ;
    }
    return buffer;
}

long get_directory_location(char *path){
    printf("get_directory_location(%s)\n", path);

    //delcare and allocate array to hold the func get_folders(path)
    char ** folders = malloc (64 * sizeof(char*)); 

    //declare and allocate directory pointer 
    struct directory *dirpointer = malloc(sizeof(*dirpointer));
    long location_in_disk = 0; //start at the root 

    int location_in_directory; //which entry number in the directory

    char *folder = malloc(32);//declare and allocate string for single folder 

    int n; 
    folders = get_folders(path);

    int i=0;

    while((folder =folders[i])!= NULL){
        memcpy(dirpointer, &blockpointer[location_in_disk], sizeof(*dirpointer)); //point to the address of the block that contains the directory 

        if ((location_in_directory = file_location(folder, "", dirpointer))== -1){
            printf("failed");
            return -1;


        }

        print_file_entry(&dirpointer -> entry[location_in_directory]);
        
        n = dirpointer->entry[location_in_directory].is_dir;

        //if this entry is unused or a file, the path is invalid 
        if (!n || n==-1){
            printf("path: %s is invalid; %s is not a directory\n", path, folder);
            return -1;
        }

        location_in_disk = dirpointer->entry[location_in_directory].fileID;
        i++; //folder array counter
    }
    return location_in_disk; //prints the index in disk

}

void print_file_entry(metadata *metaptr){
    printf("file %s.%s \t", metaptr->filename, metaptr->extension);
    printf("file location: %ls \t is_dir: %d\t", metaptr->fileID, metaptr->is_dir);
    printf("last modified: %ls\n", metaptr->time);
}

void print_dir(char *path){
    long directory_location = get_directory_location(path);

    printdir(directory_location);


}

void printdir(long dir_loc){
    struct directory *dirpointer = malloc (sizeof(*dirpointer));
    memcpy(dirpointer, &blockpointer[dir_loc], sizeof(*dirpointer));

    int i; 
    for (int i = 0; i< ENTRY_PER_DIR; i++){
        printf("file %d: name: %s.%s \t", i, dirpointer->entry[i].filename, dirpointer->entry[i].extension);
        printf("fileID: %ld \t", dirpointer->entry[i].fileID);
        printf("is_dir: %d\n", dirpointer->entry[i].is_dir);
        printf("was last modified at %ld\n", dirpointer->entry[i].time);
    }

    return;
}
