# AES Implementation in C

[![](https://img.shields.io/badge/cryptography-AES-brightgreen.svg)]()
## Advanced Encryption Standard
[Quick start at AES](https://www.youtube.com/watch?v=gP4PqVGudtg)
## Overall AES - Encryption
![Overall AES with all rounds](https://imgur.com/x19qHr6.png)

![Layers of encryption for each round](https://imgur.com/Cz93hqs.png)
### Byte Substitution Layer
Este é a primeira camada de cada round. No AES trabalhamos com blocos fixos de 16 bytes, desse modo temos 16 S-Boxes idênticas que recebem 8 bits como entrada e retornam outros 8 bits. A substituição pode ser feita através de uma lookup table que foi pre computada. A matemática por trás das S-Boxes pode ser visualizada  na página 102 do livro **Understanding Cryptography by Christof Paar**. O livro é grátis e pode ser encontrado [aqui](http://www.crypto-textbook.com).

![Lookup Table](https://imgur.com/HKy7P9g.png)

### ShiftRows Sublayer
Aqui basicamente colocamos os dados em uma matriz e cada linha é deslocada uma certa quantidade de colunas. Isso aumenta a difusão do AES.

![Shiftrows sublayer](https://imgur.com/F71akuO.png)

### MixColumn Sublayer
Esta é uma transformação da matriz de estado (que representa os 16 bytes sendo manipulados) que mistura cada coluna. Desse modo todo byte de entrada influenciará nos 4 bytes de saida dessa subcamada. Aqui toda a matemática é feita usando o conceito de *Finite Fields* ou *Galois Field* com os elementos de GF(2⁸).

O que fazemos é pegar cada coluna da matriz de estado resultante da operação anterior (ShiftRows) e multiplicarmos por uma matriz, e o resultado substituímos na coluna original da matriz de estado.

![MixColumn Sublayer](https://imgur.com/KgBwwTU.png)  

Observe que a matriz com as constantes é definida na especificação do AES. Exemplo, para encontrarmos C0 fariamos:
C0 = 02 * B0 + 03 * B5 + 01 * B10 + 01 * B15
As multiplicações são feitas usando os elementos de *Galois field*, depois somamos cada termo (somas em GF são operações XOR bit a bit) e por final, se necessário, fazemos a redução com o polinômio P(x) =  x⁸+x⁴+x³+x+1.

### Key Schedule
Usamos a key original de tamanho 128, 192 ou 256 bits (nesta implementação usamos apenas 128 bits) e encontramos as subkeys que serão usadas em cada round. Cada round possui um coeficiente constante para o calculo da subkey. A ideia é que se trabalhamos com 16 bytes e temos 10 rounds para o caso de 128 bits, então precisamos de 10 subkeys + key orignal, que no total são 11 keys. Para o calculo das subkeys colocamos em um array **W** de 44 posições onde cada posição possui 4 bytes e é denominada WORD. O calculo/expansão de cada word ocorre da seguinte forma:
![Key schedule](https://imgur.com/gONpOcQ.png)  

Observe que a key original é dividida em 4 bytes que são colocados em W[0] ... W[3]. Na função **g** observa-se que recebemos 4 bytes, realizamos uma rotação de bites, operamos a substituição (S-Box) e fazemos a adição XOR com o *RC[i]* onde **i** significa o round e *RC[i]*  é o coeficiente do round que é definido pelo AES como:  

![Round Coefficient](https://imgur.com/iAtn2vc.png)  

O resultado de toda essa função **g** é utilizado na composição da **RootWord** que se encontra nos indices raizes de cada subkey como: W[4], W[8], W[12] ... W[40].

Após calculamos as subkeys elas podem ser usadas em cada **Key Addition Layer**, onde fazemos a soma XOR de cada WORD da subkey do round com a matriz estado (que representa o estado na cifra).

## Overall AES - Decryption
Para descriptografar em AES devemos inverter todas as camadas.
De modo geral tem-se algo como demostrado na figura:
![Decryption](https://imgur.com/hxkMB02.png)

A etapa das camadas também ocorrem na ordem inversa:  

![Decryption layers order](https://imgur.com/xRIOop0.png)

Para mais informações veja o livro: [Understanding Cryptography by Christof Paar](http://www.crypto-textbook.com)

## Run the implementation
Acesse o diretório do projeto.
#### Linux

    ./run.sh

#### Windows ou Linux

    make

    ./main

ou

    gcc -o main main.c headers/aes/aes.c headers/utils/utils.c -lm -I.

    ./main

Coloque o arquivo em plaintext na pasta assets.

A estrutura de pastas deve ficar como algo assim:  

![](https://imgur.com/ToD0aJW.png)

Ao executarmos a aplicação teremos a opção de Encrypt (Criptografar) e Decrypt (Descriptografar). É importante que todos os caracteres do arquivo em plaintext não excedam a tabela ASCII (ou que o código ultrapasse 127), caracteres com acentuação não são aceitos. Nossa plaintext para esse exemplo é o arquivo **teste.txt** e possui o seguinte conteúdo:
![](https://imgur.com/5kqhBTF.png)

Executando a aplicação escolhemos a opção 1, digitamos a senha e o nome do arquivo que se encontra na pasta **assets**:
![](https://imgur.com/LX4HbW4.png)

Após isso ele vai criptografar todo o texto em blocos de 16 bytes:
![](https://imgur.com/TgABLr2.png)

Esse processo gera um novo arquivo com a extensão **.enc** de *encrypted* que possui o mesmo nome do arquivo original. O conteudo do arquivo **teste.enc** para este exemplo é:

    a764c432d692554e0d1dcc296c436e462840fb80f8fdaec86962d4617b7d57ed5542ad1075453176890b30a74a3f0fd742df76a1e1787642b844ab8461dfede908922cc9f7d1aac3b213482ba6a3c7b60e456d562165526d9cf9bf58899d96745b20099b9055cf1e066a820d6c7a323fa00fc9e2f874d00e5813646a2ca93d2933dd160b97e2197fc6c2bd17d97208a6669cfafcccb37a5a93de49d0ffdce6383af7f909e071bcc516199b5afd8ac5f822bafc63cf409051c8b364e4de1159ce4673ad870f1227a0e44bec49c8b84ad2cab63788c1d799674710bf3ed7ee4dbd2dedeff886f3de2c92c8f3f885bd9f8fa5b803eb1edf75fbf92966a143482472c3dc82500bb905f95ca36c3ef5571a19073627ef0138877a0c2687db27ccf5a1a112c6d314ff50cb56eeed5c57d4498bfb05d39fa9a302b13c1a03049e0e370c4bd293d0ec5cf6e9f2ec0f4acf79b386b23d55e09f0d2aefb9ac4db76e2a7ff8c7a490ed8ec6adb524f28a867362a01b7e9f9c22eb7f11eeb8672318089f21840ec0945c9d0194f9eb35c20617f205b6

Esta é o texto criptografado com a chave **1a456s87fg4ta78s**. Para descriptografarmos fazemos o mesmo processo, agora colocando a opção 2 e o arquivo é o **teste.enc**:![](https://imgur.com/0hxxocu.png)

Ele vai descriptografar os blocos

![](https://imgur.com/zb6BxDs.png)

E ao final gerará um arquivo com o texto descriptografado e com a extensão **.dec** de *decrypted*. Pode-se observar que o arquivo teste.txt e o teste.dec possuem conteúdos idênticos.

### Compare Files
Parte da tarefa é desenvolver uma função que compara se dois arquivos são idênticos. Desse modo, no final do modo de *Decrypt* deve-se digitar o nome dos dois arquivos que serão comparados. Ambos os arquivos deverão estar na pasta **/assets**. Sabemos que os arquivos que devem possuir o mesmo conteúdo no nosso exemplo é o **teste.txt** e o **teste.dec**. Observe que ele retornou que ambos os arquivos são idênticos.


![](https://imgur.com/PpIQDiu.png)

## Bibliografia
- http://www.crypto-textbook.com
- https://sites.math.washington.edu/~morrow/336_12/papers/juan.pdf
- https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
- https://www.youtube.com/watch?v=gP4PqVGudtg
- https://en.wikipedia.org/wiki/Finite_field_arithmetic#Rijndael's_finite_field
- https://en.wikipedia.org/wiki/Multiplication_algorithm#Peasant_or_binary_multiplication
- http://www.herongyang.com/Cryptography/AES-Example-Vector-of-AES-Encryption.html
