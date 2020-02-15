#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#define NANOS ((unsigned long)1000000000)

long NUM_DE_PRIMOS = 0; //variável global para guardar o número de primos encontrados menor que a raiz de n


/*
Implementação simples do crivo sem optimizações para ser usada como 
auxiliar no crivo segmentado, retorna um vetor de inteiros com os primos 
encontrados
*/
long* erastsimples( long n, int my_rank ){ //crivo normal que retorna vetor de primos
    
    char* primos = malloc(n * sizeof(char));
    for (long i = 0; i < n; i++){
        primos[i] = 1;
    }

    primos[0] = 0, primos[1] = 0;
    
    for (long i = 2; i * i< n; i++){ 
        if (primos[i]){
            for (long j = i*i; j < n; j+=i){
                primos[j] = 0;
            }
        }
    }

   
    for (long i = 0; i < n; i++){ //determinar número de primos encontrados
        if (primos[i]){
            NUM_DE_PRIMOS++;
        }
    }
    
    long* primos_f = malloc(NUM_DE_PRIMOS * sizeof(long)); //vetor para armazenar os primos encontrados
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

void segerast (long n , int comm_sz, int my_rank){
    long limite = floor(sqrt(n)) + 1;
    //numero de vetores que cada processador vai trabalhar
    long num_blocos = floor(n/(comm_sz*limite)) + 1; 
    long* primos_f = NULL;
    primos_f = erastsimples(limite, my_rank); //obter vetor de primos < raiz de n
    
    /*
    limites inferiores e superiores para o while abaixo, determina o tamanho do vetor a ser processado
    de cada vez, idealmente o tamanho seria pequeno o suficiente para caber na cache
    */
    long my_inferior = limite + my_rank*limite*num_blocos;
    long my_superior = my_inferior + limite;
    
    long my_pontofinal =limite + num_blocos*limite*(my_rank+1);
    
    long my_total = 0;
    long total;


    while (my_inferior < my_pontofinal){ 
        
         /*
            "inferior" no while e o if abaixo garantem que não importando o valor do limite
            todo intervalo seja checado e todos os primos encontrados, outras formas de iterar 
            testadas perdem valores por buracos que aparecem no intervalo.
        */

        if(my_superior >= n){
            my_superior = n;
        }

        char e_primo[limite+1];
        
        for (long i = 0; i < limite+1; i++){
            e_primo[i] = 1;
        }
        
        for (long i = 0; i < NUM_DE_PRIMOS; i++){
            
            //encontrar o menor multiplo do primo da iteração no corte
            long menor_mul = floor(my_inferior/primos_f[i])*primos_f[i];
            if (menor_mul < my_inferior){
                menor_mul+=primos_f[i];
            }

            //marcar o resto dos multiplos do primo da iteração
            for (long j = menor_mul; j < my_superior; j+= primos_f[i]){
                e_primo[j-my_inferior] = 0;
                //printf("\n marcando %d",j-inferior);
            }
        }
        
        for (long i = my_inferior; i < my_superior; i++){
            if (e_primo[i - my_inferior]){
                my_total++;
                //printf(" %ld ", i);
                //para obter os 20 últimos, retire comentário acima e copie os ultimos 20 numeros printados

            }
        }





        my_inferior += limite;
        my_superior += limite;
    }

    MPI_Reduce(&my_total, &total,1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(my_rank == 0){
        total += NUM_DE_PRIMOS;
        printf("\n O número de primos menores que %ld é %ld \n",n,total);
    }

    free(primos_f);
}

int main (int argc, char* argv[]){
    int comm_sz;
    int my_rank;
    long n = strtol(argv[1], NULL, 10);
    double my_inicio, my_final, my_tpassado, tpassado;
    
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Barrier(MPI_COMM_WORLD);
    my_inicio = MPI_Wtime();
    
    segerast(n, comm_sz, my_rank);
    
    my_final = MPI_Wtime();
    my_tpassado = my_final - my_inicio;
    MPI_Reduce(&my_tpassado, &tpassado, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (my_rank == 0){
        printf(" Tempo passado em segundos : %e \n", tpassado);
    }
    MPI_Finalize();

}