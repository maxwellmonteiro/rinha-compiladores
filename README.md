# Introdução

Este código é uma implementação de um compilador source-to-source para participar do desafio <a href="https://raw.githubusercontent.com/aripiprazole/rinha-de-compiler">rinha de compiladores</a>. O qual recebe uma AST (Abstract Syntax Tree) como parâmentro em formato json, conforme o padrão estabelecido no desafio, e traduz para código em linguagem C, que por sua vez é compilado para binário e executado.

## Como Executar

Há duas formas para executar o código disponibilizado, com docker ou sem.

### Sem docker

No diretório raiz do projeto em um ambiente Linux e com GCC instalado execute os comandos:

- **make**
- **./rinha-compiladores \<caminho arquivo json da AST\>**

### Com docker

*O desafio espera que o arquivo json esteja exatamente do seguinte local no container: /var/rinha/source.rinha.json*

#### No diretório raiz do projeto em um ambiente com Docker instalado execute os comandos:

- **docker build -t rinha-compiladores .**
- **docker run -v \<caminho do arquivo source.rinha.json no host\>:/var/rinha/source.rinha.json rinha-compiladores**

#### Ou apenas use o docker-compose

- **docker-compose up**

#### Caso queira executar o código com qualquer arquivo json basta iniciar o docker no modo interativo mapeando o diretório com os arquivos

- **docker run -v \<caminho do diretorio com arquivos json no host\>:\<caminho do diretorio para onde será mapeado no guest\> -it rinha-compiladores /bin/bash**
- **./rinha-compiladores \<caminho arquivo json da AST no guest\>**

