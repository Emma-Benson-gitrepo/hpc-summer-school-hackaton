#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
 
#define FILENAME "test.txt"
#define PATTERN "16:4. And the third poured out his vial upon the rivers and the"
 
char* allocate_buffer_of(size_t length)
{
    return (char*)malloc(sizeof(char) * length);
}
 
int main(int argc, char* argv[])
{
    double start = omp_get_wtime();
    FILE* f = fopen(FILENAME, "r");
    if(f == NULL)
    {
        printf("The file loading failed.\n");
    }
    fseek(f, 0L, SEEK_END);
    size_t file_size = ftell(f);
    char* buffer = allocate_buffer_of(file_size + 1);
    buffer[file_size] = '\0';
 
    fseek(f, 0L, SEEK_SET);
    fread(buffer, 1, file_size, f);
 
    int patternSize = sizeof(PATTERN)/sizeof(char)-1;
    printf("Pattern:\n%s, chars:%d\n", PATTERN,patternSize);

    for(int i = 0;i<file_size;i++) 
    {
        if(buffer[i] == PATTERN[0] && buffer[i-1] == '\n')
        {
            for(int j = 1; j<patternSize;j++)
            {
                if(buffer[i+j] != PATTERN[j])
                {
                    break;
                } 
                if(j==patternSize-1 && buffer[i+j+1] == '\n')
                {
                    printf("match\n");
                    i+=j;
                }
            }
        }
    }
 
    fclose(f);
    double end = omp_get_wtime();
    printf("time to run: %f\n",end-start);

    return EXIT_SUCCESS;
}