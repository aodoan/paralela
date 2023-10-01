# Autores
# Claudinei Aparecido Alduan Filho GRR20203920
# Rodrigo Saviam Soffner GRR20205092
#resultados vao para a saida padrao.
NUMELEMENTOS=10000000000
K=100

for i in {1..8} 
do
    echo "Executando 10 vezes com ${NUMELEMENTOS} elementos e ${i} threads:"
    # if [ $i -eq 1 ]; then
    #     echo "teste sequencial"
    # fi

    # if [ $i -gt 1 ]; then
    #     echo "teste com ${i} threads"
    # fi

    for n in {1..10} 
    do
        ./acharKMenores $NUMELEMENTOS $K $i
    done
done