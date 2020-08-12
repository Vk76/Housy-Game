#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
int main()
{
    ///variables
    int size = 0;  //size of the array in file.
    int judge[10]; //initialize  judge array
    int pass;
    int num = 0;

    //opening judge file
    FILE *fpj;
    fpj = fopen("J.txt", "r");

    //extracting data from file to array
    if (fpj == NULL)
    {
        printf("Couldn't open judge file\n");
    }
    else
    {
        int i = 0;
        int buff;
        while (fscanf(fpj, "%d ", &buff) == 1)
        {
            judge[i] = buff;
            size++;
            i++;
        }
        judge[i + 1] = -1; //last element to -1 for end
        size = size + 1;
    }

    ///shared part//////

    //shared variables
    key_t key1 = 5000; //key for current
    key_t key2 = 6000; //key for synchronisation
    key_t key3 = 7000; //key for terminate

    int shmid1 = shmget(key1, 4, 0666 | IPC_CREAT); //segment for current
    int shmid2 = shmget(key2, 4, 0666 | IPC_CREAT); //segment for synchronization
    int shmid3 = shmget(key3, 4, 0666 | IPC_CREAT); //segment for termination

    int *current = (int *)shmat(shmid1, (void *)0, 0);         //for current element in judge
    int *synchronization = (int *)shmat(shmid2, (void *)0, 0); //for synchronization element in judge
    int *terminate = (int *)shmat(shmid3, (void *)0, 0);       //for synchronization element in judge
    *terminate = 0;

    //entry code

    *synchronization = 0;
    pass = rand() % (size - 1);
    while (1)
    {

        while (*synchronization != 0)
            ;

        ///critical section

        if (num < size - 1)
        {

            *current = judge[pass];
            judge[pass] = -999;

            printf("\nNumber is %d\n", *current);
            srand(time(NULL));

            num++;

            while (num < size - 1)
            {

                pass = rand() % (size - 1);
                if (judge[pass] != -999)
                {
                    break;
                }
            }
        }
        else
        {
            *terminate = 1;
            *synchronization = 1;
            exit(0);
        }

        ///exit section

        *synchronization = 1;
        sleep(6);
    }

    return 0;
}

int find(int a[], int idx)
{
    if (a[idx] == -1)
        return 1;
    return 0;
}
