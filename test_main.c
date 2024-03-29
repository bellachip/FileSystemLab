#include "test_main.h"


//my mmap function
//mmap fun used in this function
char *do_mapping(char *drivep)
{
/*
        char *disk;
    unsigned char *pmap; //return value of mmap 
    int fd; //fd for disk 
    fd = open(drivep, O_RDWR, NULL);
    if(fd < 0){
        printf("fd open failed\n");
        exit(1);
    }

    // printf("hello\n");
    struct stat statbuf;
    if (fstat(fd, &statbuf)== -1){ //get stat buffer for the fd 
         fprintf(stderr, "stat fdd: %s\n", strerror(errno));
         return 1;
     }

     //printf("good\n");

    printf("statbuf size = %d\n", statbuf.st_size); //size of the fd 
    //printf("%d\n", sizeof(fdTable.blockTable));
   //will return the address so ur going to pass this in the functions 
    pmap = (unsigned char*)mmap(NULL, statbuf.st_size , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
    if(pmap == MAP_FAILED){
        printf("error = %d EBADF\n", errno);
      
        //printf("error = %s\n", errno);
        printf("mapping failed\n");
        exit(1);
    }

     //close the fd for the disk 
    if (close(fd)){
        printf("close fd failed\n");
        exit(1);
    }

    memset(pmap, '\0', statbuf.st_size);
*/
    char *pmap;//char poitners returns the value of mmmap
    int fd; //fd for disk 

    //open disk with read and right permission 
    fd = open(drivep, O_RDWR, NULL);
    if (fd == -1)
    {
        printf("failed to open disk\n");
        exit(EXIT_FAILURE);
    }

    struct stat statbuff;// stat buffer to obtain size of the disk 

    //get the buffer for the file 
    if (fstat(fd, &statbuff) == -1)
    {
        printf("fstat failed\n");
        exit(EXIT_FAILURE);
    }

    //mmap the disk file 
    if ((pmap = mmap(NULL, statbuff.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        printf("mmap failed\n");
        exit(EXIT_FAILURE);
    }

    //close the file descriptor for the disk 
    if (close(fd))
    {
        printf("close file failed\n");
        exit(EXIT_FAILURE);
    }

    memset(pmap, '\0', statbuff.st_size);

    printf("drive size: %d mapping success\n", (int)statbuff.st_size);

    return pmap;
}

//set all to -1 empty
void format(char *pmap)
{
    fatpointer = (FAT *)(&pmap[0]);
    int i = 0;
    while (i < NUM_BLOCKS)
    {
        fatpointer->file_allocation_table[i++] = -1;
    }
    printf("fat table is inititialized\n");
}

//set root function
void set_rt(void)
{
    struct directory *root = initialize_directory();
    memcpy(&blocks[0], root, sizeof(*root));
    fatpointer->file_allocation_table[0] = 0;
    printf("setting root\n");
}


int main(int argc, char **argv)
{
    char *disk;

    //disk is specified in argv[1]
    if ((disk = argv[1]) == NULL)
    {
        printf("syntax: You should enter ./filsystemTester <virtualdisk or filename> \n");
        exit(EXIT_FAILURE);
    }

    //mmap the disk 
    char *pmap = do_mapping(disk);
    //setting upthe first part of mmap mmoery to fat pointer
    format(pmap);


    //setting up the remaining mmapd memory to an array of block
    blocks = (block *)(&pmap[0] + sizeof(FAT));

    set_rt(); //set root 
    f_test(); //test function 

    exit(EXIT_SUCCESS);
}

