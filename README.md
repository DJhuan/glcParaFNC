# glcParaFNC

Este repositório é dedicado ao trabalho prático da disciplina de Linguagens Formais e Autômatos.

## Integrantes

- Lívia Della Garza Silva
- Jhuan Carlos Sabaini Dassie
- Marina Hermógenes Siqueira

## Instruções de compilação

```
g++ -o glcParaFNC main.cpp Glc.cpp ArranjoBin.cpp
```

## Executando o programa

```
./glcParaFNC nome_arquivo_entrada.txt nome_arquivo_saida.txt
```

## Funcionamento

- Leitura do arquivo
  - Parsing das entradas
- Transformações em FNC
  - Remoção do símbolo inicial
  - Remoção do lambda
  - Remoção de regras de cadeia
  - REACH
  - TERM
  - FNC
- Escrita do arquivo
