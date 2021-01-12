#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>

#define SRAND_VALUE 1985
#define DIM 2048
#define GERACOES 2000
#define NTHREADS 8

int **grid, **newgrid;

int mod (int a, int b) {
    int r = a % b;

    return r < 0 ? r + b : r;
}

int getNeighbors (int** grid, int i, int j) {
    int qt_vizinhos_vivos = 0;

    if ((i == 0) || (i == (DIM - 1)) || (j == 0) || (j == (DIM - 1))) {
        if (grid[mod((i-1), DIM)][mod((j-1), DIM)] == 1) qt_vizinhos_vivos++;
        if (grid[mod((i-1), DIM)][mod((j), DIM)] == 1) qt_vizinhos_vivos++;
        if (grid[mod((i-1), DIM)][mod((j+1), DIM)] == 1) qt_vizinhos_vivos++;
        if (grid[mod((i), DIM)][mod((j-1), DIM)] == 1) qt_vizinhos_vivos++;
        if (grid[mod((i), DIM)][mod((j+1), DIM)] == 1) qt_vizinhos_vivos++;
        if (grid[mod((i+1), DIM)][mod((j-1), DIM)] == 1) qt_vizinhos_vivos++;
        if (grid[mod((i+1), DIM)][mod((j), DIM)] == 1) qt_vizinhos_vivos++;
        if (grid[mod((i+1), DIM)][mod((j+1), DIM)] == 1) qt_vizinhos_vivos++;
    } else {
        if (grid[(i-1)][(j-1)] == 1) qt_vizinhos_vivos++;
        if (grid[(i-1)][(j)] == 1) qt_vizinhos_vivos++;
        if (grid[(i-1)][(j+1)] == 1) qt_vizinhos_vivos++;
        if (grid[(i)][(j-1)] == 1) qt_vizinhos_vivos++;
        if (grid[(i)][(j+1)] == 1) qt_vizinhos_vivos++;
        if (grid[(i+1)][(j-1)] == 1) qt_vizinhos_vivos++;
        if (grid[(i+1)][(j)] == 1) qt_vizinhos_vivos++;
        if (grid[(i+1)][(j+1)] == 1) qt_vizinhos_vivos++;
    }
    return qt_vizinhos_vivos;
}

int **Aloca_grid () {
    int **grid, k;

    grid = (int **) calloc (DIM, sizeof(int *));

    if (grid == NULL) return NULL;

    for (k = 0; k < DIM; k++) {
        grid[k] = (int*) calloc (DIM, sizeof(int));
        if (grid[k] == NULL) return NULL;
    }

    return grid; 
}

void Inicia_grid (int** grid) {
    int i, j;

    if (grid == NULL) return;

    srand (SRAND_VALUE);

    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            grid[i][j] = rand() % 2;
        }
    }
}

void Libera_grid (int **grid) {
    int  k;

    if (grid == NULL) return;

    for (k = 0; k < DIM; k++) free (grid[k]);

    free (grid);
}

int Conta_celulas_vivas (int **grid) {
    int i, j, qt = 0;

    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            if (grid[i][j] == 1) qt++;
        }
    }

    return qt;
}

void Aplica_regras (int **grid, int **newgrid, int a, int b) {
    int i, j, qt_vizinhos_vivos;

    for (i = a; i < b; i++) {
        for (j = 0; j < DIM; j++) {
            qt_vizinhos_vivos = getNeighbors(grid, i, j);
            if (grid[i][j] == 0) {
                if (qt_vizinhos_vivos == 3) newgrid[i][j] = 1;
            } else if (grid[i][j] == 1) {
                if (qt_vizinhos_vivos >= 4) newgrid[i][j] = 0;
                else if ((qt_vizinhos_vivos == 2) || (qt_vizinhos_vivos == 3)) newgrid[i][j] = 1;
                else if (qt_vizinhos_vivos < 2) newgrid[i][j] = 0;
            }
        }
    }
}

void Copia_grid (int **grid, int **newgrid, int a, int b) {
    int i, j;

    for (i = a; i < b; i++) {
        for (j = 0; j < DIM; j++) {
            grid[i][j] = newgrid[i][j];
        }
    }
}

int main (int argc, char *argv[]) {
    double inicio, fim;
    int i, j, k, qt = 0;

    grid = Aloca_grid ();
    newgrid = Aloca_grid ();
    Inicia_grid(grid);

    printf("OMP-REDUCTION | GERACOES: %d | THREADS: %d\n", GERACOES, NTHREADS);
    printf("Geração inicial: %d\n", Conta_celulas_vivas(grid));

    k = DIM/NTHREADS;
    
    omp_set_dynamic(0);
    omp_set_num_threads(NTHREADS);
    
    //inicio = omp_get_wtime();
    #pragma omp parallel private (i) shared (grid, newgrid)
    {
        int id = omp_get_thread_num();
        int a = ((k*(id+1))-k);
        int b = (k*(id+1));
        for (i = 0; i < GERACOES; i++) {
            #pragma omp barrier
            Aplica_regras(grid, newgrid, a, b);
            #pragma omp barrier
            Copia_grid(grid, newgrid, a, b);
        }
    }
    //fim = omp_get_wtime();

    //printf("Geração %d: %d\n", GERACOES, Conta_celulas_vivas(newgrid));

    inicio = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, 1) collapse (2) reduction(+ : qt)
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            if (newgrid[i][j] == 1) {
                qt++;
            }
        }
    }
    fim = omp_get_wtime();

    printf("Geração %d: %d\n", GERACOES, qt);

    Libera_grid (newgrid);
    Libera_grid (grid);

    double tempo_total = fim - inicio;
    printf("Wall Time: %.3f segundos\n", tempo_total);

    return 0;
}