#include "filesystems.h"

void test(void) {
    
//     file_create("dir1", "", 1, 0);
    file_create(NULL, "dir1", "", 1);
//     printdir(0);
    
    char * path = malloc(8);
    strcpy(path, "/dir1/");
    
    file_create(path, "file1", "txt", 0);
    file_create(path, "dir2", "", 1);
    file_create(path, "file2", "", 0);
    file_create(path, "file3", "", 0);
    
    char * path1 = malloc(16);
    
    strcpy(path1, "/dir1/dir2/");
    
    file_create(path1, "file_in_dir2", "doc", 0);
    file_create(path1, "dir_in_dir2", "", 1);
    
    printf("path:\n");
    print_dir(path);
    printf("path1:\n");
    print_dir(path1);

    printdir(1);
    
        
    metadata * fp = open(path, "file1", "txt");
    
    printf("fat = %ld, path/filename.extension = %s/%s.%s\n", fp->fileID, path, fp->filename, fp->extension);
    
    char * str = malloc(10000);
    int i;
    for (i = 0; i < 300; i = i+3)
        str[i] = 'a';
    for (i = 1; i < 300; i = i+3)
        str[i] = 'b';
    for (i = 2; i < 300; i = i+3)
        str[i] = 'c';

    printf("str = %s\n", str);
    
    file_write(fp, str);
    printf("\nfile_read1\n");
    file_read(fp);
    
    file_write(fp, str);
    printf("\nfile_read2\n");
    file_read(fp);
    
    file_write(fp, str);
    printf("\nfile_read3\n");
    file_read(fp);
  
    
    strcpy(path1, "/dir1/dir2/");
    metadata * fp1 = open(path1, "file_in_dir2", "doc");
    
    char * str1 = malloc(10000);
    strcpy(str1, "Green anarchism (or eco-anarchism) is a school of thought within anarchism which puts a particular emphasis on environmental issues. A green anarchist theory is normally one that extensionends anarchist ideology beyond a critique of human interactions, and includes a critique of the interactions between humans and non-humans as well.[1] This often culminates in an anarchist revolutionary praxis that is not merely dedicated to human liberation, but also to some form of nonhuman liberation,[2] and that aims to bring about an environmentally sustainable anarchist society.\nImportant early influences were Henry David Thoreau, Leo Tolstoy[3] and Élisée Reclus.[4] In the late 19th century there emerged anarcho-naturism as the fusion of anarchism and naturist philosophies within individualist anarchist circles in France, Spain, Cuba,[5] and Portugal.[3][6] Important contemporary currents (some of which may be mutually exclusive) include anarcho-primitivism, which offers a critique of technology and argues that anarchism is best suited to uncivilised ways of life; veganarchism, which argues that human liberation and animal liberation are inseparable;[7] and social ecology, which argues that the hierarchical domination of nature by human stems from the hierarchical domination of human by human.[8]");
    file_write(fp1, str1);
    file_read(fp1);
    
    file_write(fp1, str);
    file_read(fp1);
        
    printf("~~~~~~~before delete\n");
    print_dir(path1);
    file_delete(path1, "file_in_dir2", "doc");
    
    printf("~~~~~~~after delete\n");
    print_dir(path1);
    
    free(str);
    free(str1);
    exit(EXIT_SUCCESS);
}

