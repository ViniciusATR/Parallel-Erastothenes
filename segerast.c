#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define NANOS ((unsigned long)1000000000)

long NUM_DE_PRIMOS = 0;

/*
Implementação simples do crivo sem optimizações para ser usada como 
auxiliar no crivo segmentado, retorna um vetor de inteiros com os primos 
encontrados
*/
long* erastsimples( int n ){
    
    char* primos = malloc(n* sizeof(char));
    printf(" "); //por algum motivo esse printf conserta a função
    for (long i = 0; i < n; i++){
        primos[i] = 1;
    }

    primos[0] = 0, primos[1] = 0;
    
    for (long i = 2; i < sqrt(n) ; i++){ 
        if (primos[i]){
            for (int j = i*i; j < n; j+=i){
                primos[j] = 0;
            }
        }
    }

   
    for (long i = 0; i < n; i++){ //determinar número de primos encontrados
        if (primos[i]){
            NUM_DE_PRIMOS++;
        }
    }
    
    long* primos_f = malloc(NUM_DE_PRIMOS * sizeof(int)); //vetor para armazenar os primos encontrados
    long contador = 0;
    
    for (long i = 0; i < n; i++){
        if(primos[i]){
            primos_f[contador] = i;
            contador++;
        }
    }

    free(primos);
    return primos_f;
}

void segerast (long n ){
    //
    long limite = floor(sqrt(n)) + 1;
    long* primos_f = erastsimples(limite);
    
    /*
    limites inferiores e superiores para o while abaixo, determina o tamanho do vetor a ser processado
    de cada vez, idealmente o tamanho seria pequeno o suficiente para caber na cache
    */
    long inferior = limite;
    long superior = limite*2;

    long total_primos = NUM_DE_PRIMOS;

    while (inferior < n){
        
        /*
            "inferior" no while e o if abaixo garantem que não importando o valor do limite
            todo intervalo seja checado e todos os primos encontrados, outras formas de iterar 
            testadas perdem valores.
        */

        if (superior >= n){
            superior = n;
        }

        char e_primo[limite+1];
        
        for (long i = 0; i < limite+1; i++){
            e_primo[i] = 1;
        }
        
        for (long i = 0; i < NUM_DE_PRIMOS; i++){
            
            
            //encontrar o menor multiplo do primo da iteração no corte
            long menor_mul = floor(inferior/primos_f[i])*primos_f[i];
            //garantir que o menor multiplo esteja no intervalo sendo processado
            if (menor_mul < inferior){
                menor_mul+=primos_f[i];
            }

            //marcar o resto dos multiplos do primo da iteração
            for (long j = menor_mul; j < superior; j+= primos_f[i]){
                e_primo[j-inferior] = 0;
            }
        }
        
        for (long i = inferior; i < superior; i++){
            if (e_primo[i - inferior]){
                total_primos++;
            }
            
        }

        inferior += limite;
        superior += limite;
    }
    printf("\n O número de primos menores que %ld é %ld \n",n,total_primos);
    free(primos_f);

}

int main(int argc, char *argv[]){
    
    long n = strtol(argv[1], NULL, 10);
    //scanf("%ld",&n);

    clock_t start = clock();
    segerast(n);
    double elapsedTime = (double)(clock() - start) / CLOCKS_PER_SEC;
    
    printf ("Tempo total: %lf Resolução: %ld ns\n", elapsedTime, NANOS/CLOCKS_PER_SEC);
    
    return 0;
}