# Multiplicação de Polinômios

Este repositório contém as implementações de diferentes algoritmos para a multiplicação de polinômios, desenvolvidos como parte de um trabalho acadêmico.

## 🚀 Como Executar

Todo o código desenvolvido pode ser encontrado neste repositório. Para facilitar a compilação e execução, foi disponibilizado um arquivo `Makefile`.

Para executar qualquer um dos algoritmos, utilize o seguinte comando no seu terminal:

```shell
make <nome-do-algoritmo> file=<nome-do-arquivo-de-entrada>
```
## Exemplo de Uso

Para executar a versão paralela da FFT com o arquivo de entrada `poly_big.txt`, o comando seria:

```shell
make fft_par file=poly_big.txt
```

## ⚙️ Algoritmos Disponíveis

Você pode escolher uma das seguintes implementações para executar:

* `naive`: Implementação sequencial da multiplicação de polinômios (ingênua).
* `naive_par`: Implementação paralela da versão ingênua.
* `naive_bib`: Implementação paralela da versão ingênua com uso de biblioteca externa.
* `fft`: Implementação sequencial utilizando a Transformada Rápida de Fourier (FFT).
* `fft_par`: Implementação paralela do algoritmo com FFT.

## 📁 Arquivos de Entrada

Os arquivos de entrada para teste devem estar no diretório `input/`. Os arquivos disponíveis são:

* `poly_small.txt`
* `poly_small2.txt`
* `poly_medium.txt`
* `poly_medium2.txt`
* `poly_big.txt`
* `poly_big2.txt`
