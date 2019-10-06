#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define MAX_STRING 10240

int **map , *a , *b;

//Get the parameter of the first line of input (row, column, prime)
void get_parameter(char *text, int *row, int *column, int *prime)
{
    char *ptr, *token = " ";

    ptr = strtok(text, token);
    *row = atoi(ptr);

    ptr = strtok(NULL, token);
    *column = atoi(ptr);

    ptr = strtok(NULL, token);
    *prime = atoi(ptr);

    return;
}

//Add the ASCII of each character in the string
int hash(char *string) 
{
  int sum = 0, i = 0;

  for (i = 0; i < strlen(string); i++)
        sum += tolower(string[i]);

  return sum;
}

//Create a 2D array with row rows and column columns for pointer map
//Create an array with row elements uniformly chosen from [1, p-1] for pointer a and b
//a[i] and b[i] is independent
void init(int **map_array, int row, int column, int *a_array, int *b_array, int prime) 
{
    int *in_map , i=0 , j=0;

    //Create a 2D array
    map_array = (int**)malloc(row*sizeof(int *));
    in_map = (int*)malloc(row*column*sizeof(int));

    for(i = 0; i < row; i++, in_map += column)
    {
        map_array[i]=in_map;
        for(j=0;j<column;j++)
            map_array[i][j]=i;
    }
    map = map_array;

    //Create two arrays containing random numbers
    srand(time(NULL));
    a_array = (int*)malloc(row*sizeof(int));
    b_array = (int*)malloc(row*sizeof(int));

    for(i=0;i<row;i++)
    {
        a_array[i]=(rand()%prime);
            if(a_array[i]==0)   //Replace 0 in the array with 1
                a_array[i]=1;
        b_array[i]=(rand()%prime);
            if(b_array[i]==0)   //Replace 0 in the array with 1
                b_array[i]=1;
    }
    a=a_array;
    b=b_array;

    return;
}

//Enter a string to find its hash value
int myhash(char *str, int count, int row, int column, int prime, int *a, int *b)
{
    int key = hash(str);

    return (a[count] * key + b[count]) % prime % column;
}

//Enter a string, according to the hash value, find the smallest field in the hash table, then add one to them.
void insert(int **map, int row, int column, int prime, char *str, int *a, int *b)
{
    int min = 999999 , count = 0;

    //Find the smallest value in the hash table
    for (count=0 ; count<row ; count++)
    {
        if(map[count][myhash(str, count, row, column, prime ,a, b)] <min)
            min = map[count][myhash(str, count, row, column, prime, a, b)];
    }

    //Add one to the smallest value
    for (count=0 ; count<row ; count++)
    {
        if(min == map[count][myhash(str, count, row, column, prime, a, b)])
            map[count][myhash(str, count, row, column, prime, a, b)]++;
    }

    return;
}

//Enter a string, find the smallest value in the hash table
void query(int **map, int row, int column, int prime, char *str, int *a, int *b)
{
    int min = 99999 , count = 0;

    //Find the smallest value in the hash table
    for (count=0 ; count<row ; count++)
    {
        if(map[count][myhash(str, count, row, column, prime ,a, b)] <min)
            min = map[count][myhash(str, count, row, column, prime, a, b)];
    }

    printf("%d\n",min);

    return;
}

//Release the memory from malloc
void free_table(int **map, int row, int column, int *a, int *b)
{
    free(map[0]);
    free(map);
    free(a);
    free(b);
}


int main() 
{
    char text[MAX_STRING] ;
    int i = 0 ,j=0 , row =0 , column=0 , prime=0;

    FILE *fp;
    fp = fopen("input.txt", "r+");

    fgets(text, MAX_STRING, fp);
    get_parameter(text,&row,&column,&prime);

    init(map , row , column , a , b ,prime);

    char *ptr, *token = " ";

    while(fgets(text, MAX_STRING, fp)!=NULL)
    {
        //Remove the newline symbol from the string
        if(text[strlen(text)-1]=='\n')
            text[strlen(text)-1]='\0';

        //Empty string skips processing
        if(strlen(text)==0)
            continue;

        //Take words from the string for processing
        ptr = strtok(text, token);
        while(ptr != NULL)
        {
            //Remove the symbol at the end of the string
            if(ptr[strlen(ptr)-1]=='.' || ptr[strlen(ptr)-1]==',')
                ptr[strlen(ptr)-1]='\0';

            //If the string is not empty, insert it into the hash table.
            if(ptr[0]!='\0')
                insert(map, row, column, prime, ptr, a, b);

            ptr = strtok(NULL, token);
        }
    }

    //Query mode
    printf("Please enter the word you want to query.\n");
    printf("Enter 'exit' to leave the query mode\n");
    while(1)
    {
        char keyword[32];

        printf("# ");
        scanf("%s",keyword);

        if(strcmp(keyword,"exit")==0)
            break;
        else
            query(map, row, column, prime, keyword, a, b);
    }

    fclose(fp);
    free_table(map, row, column, a, b);

  return 0;
}