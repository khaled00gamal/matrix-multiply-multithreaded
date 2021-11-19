#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>



int matrix1[10][10];
int matrix2[10][10];
int matrix3[10][10];
int matrix3row[10][10];
int row1,col1,row2,col2;

typedef struct params{
int row,col;
}args;


void* mulElem(void*arg){
  struct params *p= (struct params*)arg;
 for(int i=0;i<col1;i++){
    matrix3[p->row][p->col]+=matrix1[p->row][i]*matrix2[i][p->col];
 }
}


void* mulRow(void*arg){               //----- = ------ * -
    int rowCount = (int*)arg;                                 //-
                                                       //-
   for(int i=0;i<col2;i++){
    for(int j=0;j<col1;j++){
        matrix3row[rowCount][i]+= matrix1[rowCount][j]*matrix2[j][i];
    }
   }
}


int main()
{

    //read first matrix size
    FILE *fp = fopen("matrixInput.txt","r");
    fscanf(fp,"%d %d",&row1,&col1);

    //read first matrix elements
    int matrix1Element;
    for(int i=0;i<row1;i++){
        for(int j=0;j<col1;j++){
            fscanf(fp,"%d",&matrix1Element);
            matrix1[i][j]=matrix1Element;
        }
    }

    //read second matrix size
    fscanf(fp,"%d %d",&row2,&col2);

    //read second matrix elements
    int matrix2Element;
    for(int i=0;i<row2;i++){
        for(int j=0;j<col2;j++){
            fscanf(fp,"%d",&matrix2Element);
            matrix2[i][j]=matrix2Element;
        }
    }

    //print matrix 1
    printf("first matrix\n\n");
    for(int i=0;i<row1;i++){
            printf("\n");
        for(int j=0;j<col1;j++){
            printf("%d\t",matrix1[i][j]);
        }
    }
     printf("\n\n\n");

    //print matrix 2
     printf("second matrix\n\n");
    for(int i=0;i<row2;i++){
            printf("\n");
        for(int j=0;j<col2;j++){
             printf("%d\t",matrix2[i][j]);
        }
    }
    printf("\n");


    if(col1 != row2){
        printf("multiplication cant happen, enter correct input.");
        return 0;
    }




    //-----------------------------------------------element--------------------------------------------------//


pthread_t element_threads[row1*col2];
int threadCount=0;
args* arguments;


clock_t time1 = clock();

//create thread for each element in result
for(int i=0;i<row1;i++){
    for(int j=0;j<col2;j++){
            arguments[threadCount].row=i;
            arguments[threadCount].col=j;
        pthread_create(&element_threads[threadCount++],NULL,mulElem,(void*)&arguments[threadCount]);
    }
}

clock_t time2 = clock();
    double totalTime = ((double)(time2-time1))/CLOCKS_PER_SEC;


//wait for threads
for (int i=0;i<(row1*col2);i++){
        pthread_join(element_threads[i],NULL);
    }


printf("\n result by element\n");
    for(int i=0;i<row1;i++){
        for(int j=0;j<col2;j++){
            printf("%d ",matrix3[i][j]);
        }
        printf("\n");
    }
    printf("time:%f \n\n",totalTime);



    //--------------------------------------------------end---------------------------------------------------//

    //--------------------------------------------------row--------------------------------------------//

   pthread_t row_threads[row1];

    clock_t time1_row = clock();

  //create a thread for each row
    for(int i=0;i<row1;i++){
        pthread_create(&row_threads[i],NULL,mulRow,(void*)(i));
    }

    clock_t time2_row = clock();
    double totalTime_row = ((double)(time2_row-time1_row))/CLOCKS_PER_SEC;


    //wait for threads
    for (int i=0;i<(row1);i++){
        pthread_join(row_threads[i],NULL);
    }

    //print result
    printf("\n result by row\n");
    for(int i=0;i<row1;i++){
        for(int j=0;j<col2;j++){
            printf("%d ",matrix3row[i][j]);
        }
         printf("\n");
    }
    printf("\n time:%f \n\n",totalTime_row);


    //---------------------------------------------end-------------------------------------------------------------//



    return 0;
}
