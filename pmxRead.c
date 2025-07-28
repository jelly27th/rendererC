#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef  unsigned char uint8_t;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t;
typedef  unsigned long long uint64_t;
typedef  float float32_t;
typedef  double float64_t;

typedef struct
{
    uint32_t len;
    wchar_t *data_wide; // For wide character support
} pmx_text_t;

typedef struct
{
    uint32_t count;
    uint32_t size;
} pmx_vertex_t;

typedef struct
{
    uint8_t sign[4]; // PMX file signature
    float32_t version; // PMX file version
    uint8_t goalCount; // Number of goals
    uint8_t goalType[8]; // Goal types
    pmx_text_t localModelName; // Local model name
    pmx_text_t generalModelName; // General model name
    pmx_text_t localModelComment; // Local model comment
    pmx_text_t generalModelComment; // General model comment
    pmx_vertex_t vertex; // Vertex data
} pmx_t;

pmx_t pmx;
unsigned char buffer[65536];
void read_file(char *filename)
{
    setlocale(LC_ALL, "");// suggested print to windows terminal

    FILE *fd = fopen(filename,"rb"); 
    if(fd == NULL)
    {
        perror("open failed!");
        exit(1);        //出错、退出
    }


    fread(&(pmx.sign[0]), sizeof(pmx.sign), 1, fd);

    fread(&(pmx.version), sizeof(pmx.version), 1, fd);
    
    fread(&(pmx.goalCount), sizeof(pmx.goalCount), 1, fd);
    
    fread(&(pmx.goalType[0]), sizeof(pmx.goalType), 1, fd);
    
    fread(&(pmx.localModelName.len), sizeof(pmx.localModelName.len), 1, fd);
    pmx.localModelName.data_wide = (wchar_t  *)malloc(sizeof(wchar_t ) * (pmx.localModelName.len / 2 + 1));
    fread(pmx.localModelName.data_wide, pmx.localModelName.len, 1, fd);
    pmx.localModelName.data_wide[pmx.localModelName.len / 2] = L'\0'; // Null-terminate the string
    
    fread(&(pmx.generalModelName.len), sizeof(pmx.generalModelName.len), 1, fd);
    pmx.generalModelName.data_wide = (wchar_t  *)malloc(sizeof(wchar_t ) * (pmx.generalModelName.len / 2 + 1));
    fread(pmx.generalModelName.data_wide, pmx.generalModelName.len, 1, fd);
    pmx.generalModelName.data_wide[pmx.generalModelName.len / 2] = L'\0'; // Null-terminate the string
    
    fread(&(pmx.localModelComment.len), sizeof(pmx.localModelComment.len), 1, fd);
    pmx.localModelComment.data_wide = (wchar_t  *)malloc(sizeof(wchar_t ) * (pmx.localModelComment.len / 2 + 1));
    fread(pmx.localModelComment.data_wide, pmx.localModelComment.len, 1, fd);
    pmx.localModelComment.data_wide[pmx.localModelComment.len / 2] = L'\0'; // Null-terminate the string
    wprintf(L"Local Model Comment: %ls\n", pmx.localModelComment.data_wide);
    
    fread(&(pmx.generalModelComment.len), sizeof(pmx.generalModelComment.len), 1, fd);
    pmx.generalModelComment.data_wide = (wchar_t  *)malloc(sizeof(wchar_t ) * (pmx.generalModelComment.len / 2 + 1));
    fread(pmx.generalModelComment.data_wide, pmx.generalModelComment.len, 1, fd);
    pmx.generalModelComment.data_wide[pmx.generalModelComment.len / 2] = L'\0'; // Null-terminate the string
    wprintf(L"General Model Comment: %ls\n", pmx.generalModelComment.data_wide);

    fread(&(pmx.vertex.count), sizeof(pmx.vertex.count), 1, fd);
    pmx.vertex.size = 3 * 4 + 3 * 4 + 2 * 4;
    if (pmx.goalType[1] !=0 ) {
      pmx.vertex.size += 4 * 4 * pmx.goalType[1];
    }
    pmx.vertex.size += 1;

    fread(&buffer[0],sizeof(buffer),1,fd);
    fclose(fd);
}

int main()
{
  read_file("C:\\Users\\dong\\Downloads\\xiao\\xiao.pmx");

  return 0;
}