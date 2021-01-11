## JOGO DA VIDA OMP
O jogo da vida é um autômato celular desenvolvido pelo matemático britânico John Horton Conway em 1970. O jogo foi criado de modo a reproduzir, através de regras simples, as alterações e mudanças de seres vivos. As regras definidas são aplicadas a cada nova "geração", assim a partir de uma matriz bi-dimensional é possível perceber as mudanças a cada nova geração.

Esta implementação em questão foi desenvolvida em C com a paralelização em OMP.

Os valores do tamanho do tabuleiro, número de gerações e número de threads podem ser alterados no algoritmo, mudando o valor das seguintes constantes, respectivamente: DIM, GERACOES e NTHREADS.

# Requerimento | Dependências
A seguinte biblioteca precisa estar instalada localmente:
* [fopenmp](https://www.openmp.org/)

# Compilação
Para compilar qualquer versão do programa, deve-se fazer o link da seguinte biblioteca: `-fopenmp`.

# Execução
Para executar qualquer versão do programa, basta digitar: `./jogodavidaomp-critical`, `./jogodavidaomp-reduction` ou `./jogodavidaomp-outro`.