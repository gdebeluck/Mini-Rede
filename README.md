# Mini-Rede

Simulador de uma rede social desenvolvido em C++ como projeto acadêmico e de portfólio. O sistema foi pensado para modelar operações típicas de uma plataforma social, como cadastro de usuários, seguidores, publicações, curtidas, comentários, notificações e feed personalizado.

## Visão geral

O Mini-Rede é uma implementação em linguagem C++ de uma pequena rede social com foco em estruturas de dados e lógica de funcionamento. O projeto demonstra a aplicação prática de conceitos como:

- Árvores binárias de busca
- Tabela hash para busca por username
- Listas encadeadas
- Filas para notificações
- Ordenação de publicações por timestamp e curtidas

Além de exercitar programação orientada a objetos em nível de estruturas e manipulação de memória, o projeto também simula interações reais de uma rede social em escala reduzida.

## Funcionalidades

O sistema permite:

- Cadastro de usuários
- Busca por ID e por username
- Listagem de usuários
- Seguir e deixar de seguir usuários
- Visualização de quem o usuário segue
- Criação e remoção de postagens
- Curtir publicações
- Adicionar e listar comentários
- Consultar notificações recebidas
- Gerar feed de posts de usuários seguidos
- Listar os posts mais curtidos

## Estrutura do projeto

- `Mini-Rede.h`: definições das estruturas e protótipos das funções
- `Mini-Rede.cpp`: implementação da lógica da rede social
- `README.md`: documentação do projeto

## Arquitetura e estrutura de dados

A solução foi organizada para representar os principais elementos da rede social:

- `MiniRede`: estrutura principal da aplicação
- `Usuario`: dados do usuário e seus relacionamentos
- `Publicacao`: conteúdo, curtidas e comentários
- `Fila`: fila de notificações por usuário
- `NodoArvore`: árvore binária para busca por ID
- `TabelaHash`: tabela hash para busca por username
- `NodoSeguindo` / `NodoQuemCurtiu`: listas encadeadas para relações e curtidas

Essa combinação permite uma implementação eficiente para operações frequentes em um sistema social simplificado.

## Como executar

### Pré-requisitos

- Compilador C++ (g++, clang++ ou MinGW)
- Terminal ou prompt de comando

### Compilação

No diretório do projeto, execute:

```bash
g++ Mini-Rede.cpp -o Mini-Rede
```

### Execução

```bash
./Mini-Rede
```

No Windows, com MinGW:

```bash
Mini-Rede.exe
```

## Exemplo de entrada

```text
ADD_USER 1 joao Joao Silva
ADD_USER 2 maria Maria Souza
FOLLOW 1 2
ADD_POST 10 1 100 Olha esse projeto!
LIKE 2 10
GET_NOTIFICATIONS 2 10
FEED 1 5
TOP_POSTS 5
END
```

## Exemplo de saída

```text
USER_ADDED
USER_ADDED
FOLLOWED
POST_ADDED
LIKED
NOTIFICATIONS_BEGIN
NOTIFICATION LIKE 2 10
NOTIFICATIONS_END
FEED_BEGIN
POST 10 1 100 1 Olha esse projeto!
FEED_END
TOP_POSTS_BEGIN
POST 10 1 100 1 Olha esse projeto!
TOP_POSTS_END
```

## Aprendizados e destaque para portfólio

Este projeto demonstra habilidades importantes para quem busca um perfil como desenvolvedor em C++ e estruturas de dados:

- Modelagem de sistemas reais em código
- Domínio de estruturas de dados fundamentais
- Lógica de manipulação de relações entre entidades
- Organização de código em módulos separados
- Implementação de operações de alta complexidade com eficiência

É uma boa opção para compor um portfólio porque mostra capacidade de resolver problemas com abstrações bem definidas e estruturas robustas.

## Observações

O projeto foi desenvolvido em contexto acadêmico, mas pode servir como base para evoluções futuras, como:

- autenticação de usuários
- persistência em arquivo
- interface gráfica
- suporte a múltiplos tipos de mídia
- arquitetura mais modular e orientada a objetos

## Autor

Projeto desenvolvido como exercício de estrutura de dados e para apresentação no portfólio pessoal.

## Licença

Este projeto está disponível para fins de estudo e demonstração.
