#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define NANOS ((unsigned long)1000000000)


char* erast( char* primos, long n ){
    
    primos = malloc(n * sizeof(char));
    printf("memoria alocada com sucesso");
    for (int i = 0; i < n; i++){
        primos[i] = 1;
        if ((i % 2) == 0 ){ //deixar todos os pares já marcados
            primos[i] = 0;
        }
    }

    primos[0] = 0, primos[1] = 0;
    
    for (int i = 3; i*i < n; i+=2){ //somente verificar ímpares
        if (primos[i]){
            for (int j = i*i; j < n; j+=i){
                primos[j] = 0;
            }
        }
    }

    return primos;
}




int main(int argc, char *argv[]){
    
    long n;  //= strtol(argv[1], NULL, 10);
    scanf("%ld",&n);
    
    char* primos = NULL;
    long total_primos = 1;

    clock_t start = clock();
    primos = erast(primos,n);

    for (int i = 0; i < n; i++) {
        if(primos[i]){
            total_primos++;

        }
    }

    free(primos);
    printf("\n O número de primos menores que %ld é %ld \n",n,total_primos);

    double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf ("Tempo total: %lf Resolução: %ld ns\n", elapsedTime, NANOS/CLOCKS_PER_SEC);

    return 0;
}