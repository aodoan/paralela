#resultados vao para a saida padrao.
NUMELEMENTOS=20000000
K=200

echo "Testes para ${NUMELEMENTOS} elementos com k = ${K} "
for i in {1..8} 
do
    if [ $i -eq 1 ]; then
        echo "teste sequencial"
    fi

    if [ $i -gt 1 ]; then
        echo "teste com ${i} threads"
    
    fi

    for n in {1..10} 
    do
        ./acharKMenores $NUMELEMENTOS $K $i
    done

done