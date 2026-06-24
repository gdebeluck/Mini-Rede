#ifndef MINI_REDE_H
#define MINI_REDE_H

#include <iostream>
#include <cstring>

const int TAM_USERNAME = 50;
const int TAM_NOME     = 100;
const int TAM_TEXTO    = 280;
const int TAM_COMANDO  = 30;
const int TAM_HASH     = 101;

// ─────────────────────────────────────────────
// Notificação e Fila
// ─────────────────────────────────────────────

struct Notificacao {
    char tipo[10];      // "FOLLOW" ou "LIKE"
    int  idRemetente;
    int  idPost;        // usado apenas para LIKE
};

struct NodoFila {
    Notificacao dado;
    NodoFila*   prox;
};

struct Fila {
    NodoFila* inicio;
    NodoFila* fim;
};

// ─────────────────────────────────────────────
// Listas encadeadas específicas
// ─────────────────────────────────────────────

struct NodoSeguindo {
    int          idUsuario;
    NodoSeguindo* prox;
};

struct NodoQuemCurtiu {
    int            idUsuario;
    NodoQuemCurtiu* prox;
};

// Declaração antecipada
struct Publicacao;

struct NodoPublicacaoLista {
    Publicacao*          pub;
    NodoPublicacaoLista* prox;
};

// ─────────────────────────────────────────────
// Publicação
// ─────────────────────────────────────────────

struct Publicacao {
    int             id;
    int             idAutor;
    int             timestamp;
    char            texto[TAM_TEXTO];
    int             curtidas;
    NodoQuemCurtiu* quemCurtiu;
};

// ─────────────────────────────────────────────
// Usuário
// ─────────────────────────────────────────────

struct Usuario {
    int                  id;
    char                 username[TAM_USERNAME];
    char                 nomeCompleto[TAM_NOME];
    NodoSeguindo*        seguindo;
    NodoPublicacaoLista* posts;
    Fila                 notificacoes;
};

// ─────────────────────────────────────────────
// Árvore BST de usuários por id
// ─────────────────────────────────────────────

struct NodoArvore {
    Usuario*    usuario;
    NodoArvore* esq;
    NodoArvore* dir;
};

// ─────────────────────────────────────────────
// Tabela Hash de usuários por username
// ─────────────────────────────────────────────

struct NodoHash {
    char      username[TAM_USERNAME];
    Usuario*  usuario;
    NodoHash* prox;
};

struct TabelaHash {
    NodoHash* tabela[TAM_HASH];
};

// ─────────────────────────────────────────────
// MiniRede principal
// ─────────────────────────────────────────────

struct MiniRede {
    NodoArvore*          raizUsuarios;
    TabelaHash           tabelaUsernames;
    NodoPublicacaoLista* publicacoes;
    int                  totalUsuarios;
    int                  totalPublicacoes;
};

// ─────────────────────────────────────────────
// Funções principais (interface pública)
// ─────────────────────────────────────────────

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

// ─────────────────────────────────────────────
// Funções auxiliares
// ─────────────────────────────────────────────

// Hash
int       calcularHash(const char username[]);
void      inserirNaHash(TabelaHash& tabela, Usuario* usuario);
Usuario*  buscarNaHash(TabelaHash& tabela, const char username[]);
void      liberarHash(TabelaHash& tabela);

// Árvore BST
void      inserirNaArvore(NodoArvore*& raiz, Usuario* usuario);
Usuario*  buscarNaArvore(NodoArvore* raiz, int id);
void      listarArvoreEmOrdem(NodoArvore* raiz, std::ostream& saida);
void      liberarArvore(NodoArvore* raiz);

// Fila
void      enfileirar(Fila& fila, Notificacao notif);
bool      desenfileirar(Fila& fila, Notificacao& notif);
bool      filaVazia(const Fila& fila);
void      liberarFila(Fila& fila);

// Listas de seguindo
void      inserirSeguindo(NodoSeguindo*& lista, int idUsuario);
bool      estaSeguindo(NodoSeguindo* lista, int idUsuario);
void      listarSeguindoEmOrdem(NodoSeguindo* lista, TabelaHash& tabela, NodoArvore* raiz, std::ostream& saida);
void      liberarListaSeguindo(NodoSeguindo*& lista);

// Listas de quem curtiu
void      inserirQuemCurtiu(NodoQuemCurtiu*& lista, int idUsuario);
bool      jaCurtiu(NodoQuemCurtiu* lista, int idUsuario);
void      liberarListaQuemCurtiu(NodoQuemCurtiu*& lista);

// Lista de publicações
void      inserirPublicacao(NodoPublicacaoLista*& lista, Publicacao* pub);
Publicacao* buscarPublicacao(NodoPublicacaoLista* lista, int idPost);
void      liberarListaPublicacoes(NodoPublicacaoLista*& lista, bool liberarPub);

// Ordenação (insertion sort em vetor auxiliar)
void      ordenarFeed(Publicacao** vetor, int n);
void      ordenarTopPosts(Publicacao** vetor, int n);

// Impressão
void      imprimirUsuario(Usuario* u, std::ostream& saida);
void      imprimirPublicacao(Publicacao* p, std::ostream& saida);

#endif