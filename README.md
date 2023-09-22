# Introdução

Este código é uma implementação de um compilador source-to-source para participar do desafio <a href="https://github.com/aripiprazole/rinha-de-compiler">rinha de compiladores</a>. O qual recebe uma AST (Abstract Syntax Tree) como parâmentro em formato json, conforme o padrão estabelecido no desafio, e traduz para código em linguagem C, que por sua vez é compilado para binário e executado.

## Dependências

- <a href="https://github.com/akheron/jansson">Jansson</a> (o binário dessa lib já está no diretório ./lib, mas você pode baixá-la e fazer a build se preferir)

## Como Executar

Há duas formas para executar o código disponibilizado: com docker ou sem.

### Sem docker

**Pré-requisitos:** Em um ambiente Linux com GCC instalado.

```console
make 
```
```console
./rinha-compiladores arquivo-da-ast.json
```

### Com docker

**Pré-requisitos:** Docker instalado. 

*O desafio espera que o arquivo json esteja exatamente no seguinte caminho do container: /var/rinha/source.rinha.json*

#### No diretório raiz do projeto execute os comandos:

```console
docker build -t rinha-compiladores .
```
```console
docker run -v <caminho arquivo source.rinha.json no host>:/var/rinha/source.rinha.json rinha-compiladores
```

#### Ou apenas use o docker-compose

*Lembre-se de alterar o caminho do arquivo json no campo **-volumes** do docker-compose.yaml*

```console
docker-compose up
```

#### Caso queira executar o código com qualquer arquivo json basta iniciar o docker no modo interativo mapeando o diretório com os arquivos

```console
docker run -it -v <diretorio com arquivos json no host>:<diretorio para onde será mapeado no guest> rinha-compiladores /bin/bash
```
```console
./rinha-compiladores <caminho arquivo json da AST no guest>
```

