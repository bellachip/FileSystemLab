#include "filesystems.h"

char * do_mapping(char *drivep){
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

    //strncpy(pmap+10, "hello", 5);
   // printf("pmap = %s\n", pmap);
    printf("size: %d \n disk map success!!\n", (int)statbuf.st_size);

    return pmap;

}


//set all to empty  format the drive 
void format(char *address) {
    fatpointer = (FAT *)(&address[0]);
    int i=0; 
    while(i < NUM_BLOCKS){
        fatpointer->fat[i++] = -1;
    }
    printf("all initialized to -1");
}


//p
void init_root(void){
    //create directory pointer to indicate root 
    struct directory *root = initialize_directory(); //initialize directory to root 

    memcpy(&blockpointer[0], root, sizeof(*root));

    fatpointer->fat[0] = 0;
    printf("root initialized\n");
}

int main (int argc, char **argv) {
    char *drive;

    if((drive =argv[1]) == NULL) {
        printf("syntax error, you should type in './test <mydisk>': ./test file\n");
        exit(EXIT_FAILURE);
    }

    char *pmap = do_mapping(drive);
    //set mmap to fat pointer
    format(pmap);

    //set rest mmao mem to block arr 
    blockpointer = (block*)(&pmap[0] +sizeof(FAT));

    init_root();
    test();

   


      exit(EXIT_SUCCESS);
}





    // fd *fd ;
    
    // fd = open("Drive2MB", O_RDWR);

    // if(fd == NULL) {
    //     printf("cannot open");
    // }

    // struct stat statbuf; 
    // if (fstat (fd, &statbuf)){
    //     perror("couldn't get fd size.\n");
    // }
    // printf("fd size is %d\n", statbuf.st_size);

    // //mmap 
    // char *dir;
    // dir= mmap(0, statbuf.st_size , PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //returns pointer to a block of a memory, which is a big array of memory and my fd is going to be in that memory 
    // return 0;







 






