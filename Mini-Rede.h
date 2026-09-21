#ifndef MINI_REDE_H
#define MINI_REDE_H

#include <iostream>
#include <cstring>

const int TAM_USERNAME = 50;
const int TAM_NOME     = 100;
const int TAM_TEXTO    = 280;
const int TAM_COMANDO  = 30;
const int TAM_HASH     = 101;

struct Notificacao {
    char tipo[15];      // "FOLLOW", "LIKE" ou "COMMENT"
    int  idRemetente;
    int  idPost;        // usado para LIKE e COMMENT
    int  idComentario;  // usado apenas para COMMENT
};

struct NodoFila {
    Notificacao dado;
    NodoFila*   prox;
};

struct Fila {
    NodoFila* inicio;
    NodoFila* fim;
};

struct Comentario {
    int  id;
    int  idAutor;
    int  idPost;
    char texto[TAM_TEXTO];
};

struct NodoComentario {
    Comentario*    comentario;
    NodoComentario* prox;
};

struct NodoSeguindo {
    int           idUsuario;
    NodoSeguindo* prox;
};

struct NodoQuemCurtiu {
    int             idUsuario;
    NodoQuemCurtiu* prox;
};

struct Publicacao;

struct NodoPublicacaoLista {
    Publicacao*          pub;
    NodoPublicacaoLista* prox;
};

struct Publicacao {
    int             id;
    int             idAutor;
    int             timestamp;
    char            texto[TAM_TEXTO];
    int             curtidas;
    NodoQuemCurtiu* quemCurtiu;
    NodoComentario* comentarios;  // lista de comentários
    int             totalComentarios;
};

struct Usuario {
    int                  id;
    char                 username[TAM_USERNAME];
    char                 nomeCompleto[TAM_NOME];
    NodoSeguindo*        seguindo;
    NodoPublicacaoLista* posts;
    Fila                 notificacoes;
};

struct NodoArvore {
    Usuario*    usuario;
    NodoArvore* esq;
    NodoArvore* dir;
};

struct NodoHash {
    char      username[TAM_USERNAME];
    Usuario*  usuario;
    NodoHash* prox;
};

struct TabelaHash {
    NodoHash* tabela[TAM_HASH];
};

struct MiniRede {
    NodoArvore*          raizUsuarios;
    TabelaHash           tabelaUsernames;
    NodoPublicacaoLista* publicacoes;
    int                  totalUsuarios;
    int                  totalPublicacoes;
    int                  totalComentarios;  // contador global de ids de comentários
};

void inicializarMiniRede(MiniRede& rede);
void liberarMiniRede(MiniRede& rede);
void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida);

void cadastrarUsuario(MiniRede& rede, int id, const char username[], const char nomeCompleto[], std::ostream& saida);
void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida);
void buscarUsuarioPorUsername(MiniRede& rede, const char username[], std::ostream& saida);
void listarUsuarios(MiniRede& rede, std::ostream& saida);

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida);
void pararDeSeguir(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida);  // EXTRA: UNFOLLOW
void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida);

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream& saida);
void removerPublicacao(MiniRede& rede, int idPost, int idAutor, std::ostream& saida);     // EXTRA: REMOVE_POST
void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida);

void comentarPublicacao(MiniRede& rede, int idUsuario, int idPost, const char texto[], std::ostream& saida); // EXTRA: COMMENT
void listarComentarios(MiniRede& rede, int idPost, std::ostream& saida);                  // EXTRA: LIST_COMMENTS

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void listarTopPosts(MiniRede& rede, int k, std::ostream& saida);

int       calcularHash(const char username[]);
void      inserirNaHash(TabelaHash& tabela, Usuario* usuario);
Usuario*  buscarNaHash(TabelaHash& tabela, const char username[]);
void      liberarHash(TabelaHash& tabela);

void      inserirNaArvore(NodoArvore*& raiz, Usuario* usuario);
Usuario*  buscarNaArvore(NodoArvore* raiz, int id);
void      listarArvoreEmOrdem(NodoArvore* raiz, std::ostream& saida);
void      liberarArvore(NodoArvore* raiz);

void      enfileirar(Fila& fila, Notificacao notif);
bool      desenfileirar(Fila& fila, Notificacao& notif);
bool      filaVazia(const Fila& fila);
void      liberarFila(Fila& fila);

void      inserirSeguindo(NodoSeguindo*& lista, int idUsuario);
bool      removerSeguindo(NodoSeguindo*& lista, int idUsuario);  // EXTRA: para UNFOLLOW
bool      estaSeguindo(NodoSeguindo* lista, int idUsuario);
void      listarSeguindoEmOrdem(NodoSeguindo* lista, NodoArvore* raiz, std::ostream& saida);
void      liberarListaSeguindo(NodoSeguindo*& lista);

void      inserirQuemCurtiu(NodoQuemCurtiu*& lista, int idUsuario);
bool      jaCurtiu(NodoQuemCurtiu* lista, int idUsuario);
void      liberarListaQuemCurtiu(NodoQuemCurtiu*& lista);

void        inserirPublicacao(NodoPublicacaoLista*& lista, Publicacao* pub);
Publicacao* buscarPublicacao(NodoPublicacaoLista* lista, int idPost);
bool        removerDaListaPublicacoes(NodoPublicacaoLista*& lista, int idPost, bool liberarPub); // EXTRA
void        liberarListaPublicacoes(NodoPublicacaoLista*& lista, bool liberarPub);

void        inserirComentario(NodoComentario*& lista, Comentario* c);
void        liberarListaComentarios(NodoComentario*& lista);

void      ordenarFeed(Publicacao** vetor, int n);
void      ordenarTopPosts(Publicacao** vetor, int n);

void      imprimirUsuario(Usuario* u, std::ostream& saida);
void      imprimirPublicacao(Publicacao* p, std::ostream& saida);

#endif