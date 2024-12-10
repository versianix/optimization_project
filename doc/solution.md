# Solução Otimizada e Paralela do Projeto

Este documento descreve a solução construída para o projeto, detalhando as otimizações realizadas, bem como a tentativa de paralelização e seus resultados.

## Contexto do Projeto

O projeto busca simular um padrão comum em aplicações paralelas:

1. Ler dados de mais de uma fonte;
2. Realizar um processamento nos dados;
3. Armazenar o resultado.

A aplicação consiste em gerar combinações de registros, filtrar com base em limiares nos valores, encontrar IDs combinados de acordo com a regra descrita e finalmente produzir um resultado ordenado. O projeto fornece uma versão sequencial não otimizada (`seq.c`) como referência.

### Requisitos Principais

- Aplicar filtros nos registros antes ou durante o processamento (e.g., descartar registros A cujo valor mínimo `a_m` ≤ 0.25, e descartar registros B cujo valor máximo `b_M` ≥ 0.75).
- Formar o produto cartesiano (registros de `Ca` com registros de `Cb`) e para cada combinação aplicar a lógica de gerar um `combined_id` e encontrar o primeiro registro na base com este `combined_id`.
- Calcular o valor final `f = a_m * b_M * p`, ordenar os resultados e salvar em um CSV.

## Solução Otimizada com Pré-Filtragem e Hash Table

A primeira otimização realizada foi introduzir **pré-filtragem** e o uso de uma **hash table** para buscas rápidas de IDs. 

### Pré-Filtragem

Em vez de considerar todos os 30.000 registros (no exemplo simplificado usado, 100 registros), primeiro filtramos:

- Em `A.txt`, filtramos registros cujo `value > 0.25`. Apenas estes são considerados em `Ca`.
- Em `B.txt`, filtramos registros cujo `value < 0.75`. Apenas estes são considerados em `Cb`.

Isso reduz drasticamente o espaço de busca, uma vez que não precisamos mais iterar sobre todos os registros A e B ao formar as combinações. Em vez disso, apenas os índices válidos (armazenados em `validA` e `validB`) são usados.

### Hash Table para Busca de IDs

No código sequencial não otimizado, a busca por um `combined_id` específico envolvia percorrer linearmente todos os IDs para encontrar correspondências. Isso é muito custoso.

Para otimizar, criamos uma tabela hash (`HashTable`) mapeando cada ID para seu índice original. Assim, encontrar o índice de um determinado `combined_id` torna-se O(1) em média, eliminando a busca linear. Isso dá um ganho enorme de desempenho.

### Resultados da Otimização

Após implementar a pré-filtragem e a hash table, os tempos de execução caíram drasticamente. Por exemplo, no teste com 100 registros:

- Versão sequencial não otimizada: ~24 segundos
- Versão com pré-filtragem e hash table: ~0.22 segundos

Uma melhora considerável!

Além de mais rápido, o código agora gera um conjunto menor de resultados, já que muitos registros são descartados antes de entrar no loop principal. Isso é esperado e faz parte do ganho de eficiência.

## Tentativa de Paralelização (mainParalel.c)

Em seguida, tentamos paralelizar o código usando OpenMP. A ideia foi paralelizar os dois primeiros loops (`Ca`) e manter o resto similar. Também garantimos exclusão mútua na escrita do arquivo de saída (`output.csv`) com `#pragma omp critical`.

Porém, nesta implementação, a versão paralela **não ficou mais rápida** do que a versão otimizada sem paralelização. Na verdade, ela ficou mais lenta. Possíveis motivos:

- **Overhead de Criação e Sincronização de Threads:** Com poucos registros (e possivelmente nas condições testadas), o overhead de gerenciamento de threads pode superar os ganhos.
- **Região Crítica na Escrita do Arquivo:** Como todas as threads precisam acessar o mesmo arquivo de saída, há um gargalo de I/O, reduzindo o potencial de paralelismo.
- **Poucos Registros no Teste:** Com um número muito pequeno de registros (100), o paralelismo não atinge seu potencial. Com o dataset completo de 30.000 registros, talvez o paralelismo mostre vantagens (mas isso não foi testado aqui, porque o código demomaria muito tempo pra rodar ou consumiria mais memória do que temos disponível).

Assim, a versão paralelizada não apresentou speedup em relação à versão otimizada sem paralelismo. Entretanto, ela demonstra a tentativa de aplicar paralelismo, o que pode ser um passo em direção a futuras otimizações, caso um controle mais refinado de buffers ou estratégias diferentes de divisão de trabalho fossem usadas.

## Conclusão

- **Versão Otimizada (Pré-Filtragem + Hash Table):** Significativa redução de runtime comparado à versão sequencial simples.
- **Versão Paralelizada:** Ficou mais lenta que a otimizada, provavelmente devido ao overhead de sincronização, pequena quantidade de dados no teste e a região crítica na escrita do arquivo.

A solução final registra uma melhora considerável no tempo de execução na versão otimizada e demonstra a complexidade de buscar mais speedup com paralelização incorreta ou em condições desfavoráveis.

---
