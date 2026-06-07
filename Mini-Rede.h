#ifndef MINI_REDE_H
#define MINI_REDE_H

#include <iostream>

const int TAM_USERNAME = 50;
const int TAM_NOME = 100;
const int TAM_TEXTO = 280;
const int TAM_COMANDO = 30;
const int TAM_HASH = 101;  

// TODO: definir as structs principais do trabalho.
//
// Sugestao de structs que provavelmente serao necessarias:
// - Usuario
// - Publicacao
// - MiniRede
// - nos para lista encadeada
// - nos para arvore binaria de usuarios por id
// - nos para tabela hash de usernames
// - nos para fila de notificacoes
//
// Os campos de cada struct fazem parte do projeto dos alunos.

struct MiniRede {
    Nodo_Arvore* raizUsuarios;
    Tabela_Hash  tabelaUsernames; // Hash para busca rápida por username
    Nodo_Lista_Publicacoes*  publicacoes;     // Lista encadeada de todas as publicações
    int         totalUsuarios;
    int         totalPublicacoes;
};

struct Usuario {
    int id;
    char username[TAM_USERNAME];
    char nome_completo[TAM_NOME];

    Nodo_Seguindo seguindo;
    Nodo_Seguindo posts;
    Fila notificação;
};

struct Nodo_Seguindo {
    int ID_Usuario;
    Nodo_Seguindo* prox;
};

struct Nodo_Curtidas {
    int ID_Usuario;
    Nodo_Curtidas* prox;
};

struct Nodo_Lista_Publicacoes {
    Publicacao* publicacao;
    Nodo_Lista_Publicacoes* prox;
};

struct Publicacao {
    int ID;
    int ID_Autor;
    char texto[TAM_TEXTO];
    int n_curtidas;
    Nodo_Curtidas* curtidas;
};

struct Notificacao {
    char tipo[10];
    int ID_Remetente;
    int ID_post;
};

struct Nodo_Fila{
    Notificacao dado;
    Nodo_Fila* prox;
};

struct Fila {
    Nodo_Fila* inicio;
    Nodo_Fila* fim;
};

struct Nodo_Arvore {
    Usuario* usuario;
    Nodo_Arvore* esq;
    Nodo_Arvore* dir;
};

struct Nodo_Hash {
    char username[TAM_USERNAME];
    Usuario* usuario;
    Nodo_Hash* prox;
};

struct Tabela_Hash {
    Nodo_Hash* tabela[TAM_HASH];
};

void inicializarMiniRede(MiniRede& rede);
void liberarMiniRede(MiniRede& rede);
void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida);

void cadastrarUsuario(MiniRede& rede, int id, const char username[], const char nomeCompleto[], std::ostream& saida);
void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida);
void buscarUsuarioPorUsername(MiniRede& rede, const char username[], std::ostream& saida);
void listarUsuarios(MiniRede& rede, std::ostream& saida);

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida);
void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida);

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream& saida);
void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida);

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void listarTopPosts(MiniRede& rede, int k, std::ostream& saida);

// TODO: declarar aqui as funcoes auxiliares escolhidas pelo grupo.
//
// Exemplos de responsabilidades auxiliares:
// - buscar usuario por id
// - buscar usuario por username
// - buscar publicacao por id
// - inserir/listar/liberar arvore
// - inserir/buscar/liberar tabela hash
// - enfileirar/desenfileirar notificacoes
// - manipular listas encadeadas
// - ordenar vetores auxiliares para feed e ranking

#endif
