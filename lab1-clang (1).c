#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <float.h>
#include "ipp.h"
/*ippcore, ippvm */
float X;
int main(int argc, char* argv[])
{
    int i, N;
    int A = 432;
    struct timeval T1, T2;
    long delta_ms;
    N = atoi(argv[1]); /* N equals first console parameter */
    gettimeofday(&T1, NULL); /* Remember current time T1 */
    for (i=0; i<50;  i++) /*50 amount of experiments*/
    {
        unsigned int seed = i; /* Инициализировать начальное значение ГСЧ (рнг) A = 432
        Заполнить массив исходнымх данных размером N
        Решить поставленную задачу, заполнить массив с результатами
        Таблица map №1 = 7  №2 = 8 вариант ! 1 + ((432 mod 47) mod 7) = 3! 1 + ((432 mod 47) mod 8) = 2!
        Таблица Merge = 6                   1 + ((432 mod 47) mod 6) = 4!
        Таблица Sort = 7                    1 + ((432 mod 47) mod 7) = 3!

        */
        float M1[N];
        for (int j=0; j<N; j++)
        {
            M1[j] = (float) rand_r(&seed) / (float) RAND_MAX * (A - 1) + 1; // NOLINT(cert-msc30-c, cert-msc50-cpp)
        }

        float M2[N/2];

        for (int k=0; k<N/2; k++)
        {
            M2[k] = (float) rand_r(&seed) / (float) RAND_MAX * (A * 10 - A) + A; // NOLINT(cert-msc30-c, cert-msc50-cpp)
        }

        /*for  (int b = 0; b < N; b++) //map
        {
            M1[b] = exp(sqrt(M1[b]));
        }*/
        ippsSqrt_32f_A24(M1, M1, N);
        ippsExp_32f_A24(M1, M1, N);

        float M2Copied[N/2];
        ippsCopy_32f(M2, M2Copied, N/2);

        ippsAdd_32f(M2Copied, M2, M2, N/2-1);
        float e = exp(1);
        ippsMulC_32f(M2, e, M2, N/2);
        ippsSqrt_32f_A24(M2, M2, N/2);
        /*for (int k=0; k<N/2; k++)
        {
            M2Copied[k] = M2[k];
        }

        float prev = M2Copied[0];
        fwsAdd_64f(prev, M2Copied )
        for (int l = 1; l < N/2; l++)
        {
            M2[l] = sqrt((M2Copied[l]+prev)*exp(1));
            prev = M2Copied[l];
        }

        for (int g = 0; g < N/2; g++) //Merge
        {
            M2[g] = fabsf(M1[g]-M2[g]);
        }*/
        ippsSub_32f(M1, M2, M2, N/2);
        ippsAbs_32f(M2, M2, N/2);
        for (int o = 0; o < N/2; o++) //Sort
        {
            int minPosition = o;
            for (int j = o + 1; j < N/2; j++)
                if (M2[minPosition] > M2[j])
                    minPosition = j;
            float tmp = M2[minPosition];
            M2[minPosition] = M2[o];
            M2[o] = tmp;
        }

        float minV = M2[0];
        for (int j=0; j < N / 2; j++){
            if ((M2[j] < minV) && (M2[j] != 0)){
                minV = M2[j];
            }
        }


        for (int k = 0; k <N/2; k++) {
            if ((int)(M2[k]/minV) % 2 == 0)
                X = X + sin(M2[k]);
        }
        /* Отсортировать массив с результатами узкащанным методом*/
    }
    gettimeofday(&T2, NULL); /* Запомнить текущее время Т2*/
    delta_ms = 1000*(T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec)/1000;
    printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms);
    printf("\nX=%f", X);
    return 0;
}
