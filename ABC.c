
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
void rmv(int a[], int index);
void displaya(int a[], int n);
void displayb(int a[], int n);
void displayc(int a[], int n);
int iscomplete(int a[], int n);
int search(int a[], int n, int ele);
int main()
{

    FILE *fpa;

    // Opening first file of A
    fpa = fopen("A.txt", "r");
    int sizea = 0; //Size of array of A
    int arrA[10];

    //extracting data from file to array
    if (fpa == NULL)
    {
        printf("Couldn't open judge file\n");
    }
    else
    {
        int i = 0;
        int buff;
        while (fscanf(fpa, "%d ", &buff) == 1)
        {
            arrA[i] = buff;
            sizea++;
            i++;
        }
        arrA[i + 1] = -1; //Last element to -1 for end
        sizea = sizea + 1;
    }
    //B
    FILE *fpb;
    // Opening first file of A
    fpb = fopen("B.txt", "r");
    int sizeb = 0;
    int arrB[10];

    //extracting data from file to array
    if (fpb == NULL)
    {
        printf("Couldn't open judge file\n");
    }
    else
    {
        int i = 0;
        int buff;
        while (fscanf(fpb, "%d ", &buff) == 1)
        {
            arrB[i] = buff;
            sizeb++;
            i++;
        }
        arrB[i + 1] = -1; //last element to -1 for end
        sizeb = sizeb + 1;
    }
    //C
    FILE *fpc;
    fpc = fopen("C.txt", "r");
    int sizec = 0;
    int arrC[10];

    //extracting data from file to array
    if (fpc == NULL)
    {
        printf("Couldn't open judge file\n");
    }
    else
    {
        int i = 0;
        int buff;
        while (fscanf(fpc, "%d ", &buff) == 1)
        {
            arrC[i] = buff;
            sizec++;
            i++;
        }
        arrC[i + 1] = -1; //last element to -1 for end
        sizec = sizec + 1;
    }

    ///shared part//////

    //shared variables
    key_t key1 = 5000; //key for current
    key_t key2 = 6000; //key for synchronisation
    key_t key3 = 7000; //key for terminate

    int shmid1 = shmget(key1, 4, 0666 | IPC_CREAT);      //segment for current
    int shmid2 = shmget(key2, 4, 0666 | IPC_CREAT);      //segment for synchronization
    int shmid3 = shmget(key3, 4, 0666 | IPC_CREAT);      //segment for termination
    int *terminate = (int *)shmat(shmid3, (void *)0, 0); //for synchronization element in judge
    *terminate = 0;

    int *current = (int *)shmat(shmid1, (void *)0, 0);         //for current element in judge
    int *synchronization = (int *)shmat(shmid2, (void *)0, 0); //for synchronization element in judge

    int printa = 0, printb = 0, printc = 0;

    //entry code
    while (1)
    {

        while (*synchronization != 1)
            ;
        //critical section

        //check for termination
        if (*terminate == 1)
            exit(0);

        int indexa, indexb, indexc;

        int curr = *current;

        printf("\nNew number came is %d\n", curr);

        indexa = search(arrA, sizea - 1, curr);
        if (indexa != -1)
            rmv(arrA, indexa);
        if (!iscomplete(arrA, sizea - 1))
        {

            displaya(arrA, sizea - 1);
        }
        else if (printa == 0)
        {
            printf("\nA got access to pizza\n");
            printa++;
        }

        indexb = search(arrB, sizeb - 1, curr);
        if (indexb != -1)
            rmv(arrB, indexb);
        if (!iscomplete(arrB, sizeb - 1))
        {

            displayb(arrB, sizeb - 1);
        }
        else if (printb == 0)
        {
            printf("\nB got access to pizza\n");
            printb++;
        }

        indexc = search(arrC, sizec - 1, curr);
        if (indexc != -1)
            rmv(arrC, indexc);
        if (!iscomplete(arrC, sizec - 1))
        {

            displayc(arrC, sizec - 1);
        }
        else if (printc == 0)
        {

            printf("\nC got access to pizza\n");
            printc++;
        }
        //exit section

        *synchronization = 0;

        sleep(3);
    }

    ///////////////////////////////////////////////////
    return 0;
}

//function to search specific element
int search(int a[], int n, int ele)
{

    for (int i = 0; i < n; i++)
        if (a[i] == ele)
            return i;

    return -1;
}

//fuction to update values
void rmv(int a[], int index)
{

    a[index] = -99;
}

void displaya(int a[], int n)
{

    printf("\n     ----------------------\n");
    printf("A-->|");
    for (int i = 0; i < n; i++)
        if (a[i] == -99)
            printf("   |");
        else
            printf(" %d |", a[i]);
    printf("\n     -----------------------\n");
}
void displayb(int a[], int n)
{

    printf("\n     ----------------------\n");
    printf("B-->|");
    for (int i = 0; i < n; i++)
        if (a[i] == -99)
            printf("   |");
        else
            printf(" %d |", a[i]);
    printf("\n     -----------------------\n");
}
void displayc(int a[], int n)
{

    printf("\n     ----------------------\n");
    printf("C-->|");
    for (int i = 0; i < n; i++)
        if (a[i] == -99)
            printf("   |");
        else
            printf(" %d |", a[i]);
    printf("\n     -----------------------\n");
}

int iscomplete(int a[], int n)
{

    for (int i = 0; i < n; i++)
        if (a[i] != -99)
            return 0;

    return 1;
}
