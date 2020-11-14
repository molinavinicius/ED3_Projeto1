#include <stdio.h>
#include <stdlib.h>
#include"functions.h"

int main(){

    char row[100]; 
    scanf("%[^\n]",row);
    char func = row[0];

    int  opt;
    char csv[20];
    char bin[20];
    char indexa[25];
    char field[25];
    char value[40];
    int n;
    
    switch (func){
        case '1':
            sscanf(row, "%d %s", &opt, csv);
            func1(csv);
            break;
        
        case '2':
            sscanf(row, "%d %s", &opt, bin);
            func2(bin);
            break;
        
        case '3':
            sscanf(row, "%d %s %s %s %s", &opt, bin, indexa, field, value);
    
            int* result = func3(bin, indexa, field, value);
            int size = result[0];
            for (int k=1; k<= size; k++){
                printf("%d ", result[k]);
            }
            free(result);
            break;
        
        case '4':
            /* code */
            break;
        case '5':
            sscanf(row, "%d %s %s %d", &opt, bin, indexa, &n);
            func5(bin, indexa, n);
            break;

        default:
            break;
    }
    return 0;
}