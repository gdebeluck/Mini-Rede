#include "Mini-Rede.h"

// ═══════════════════════════════════════════════════════════════
// IMPRESSÃO
// ═══════════════════════════════════════════════════════════════

void imprimirUsuario(Usuario* u, std::ostream& saida) {
    saida << "USER " << u->id << " " << u->username << " " << u->nomeCompleto << "\n";
}

void imprimirPublicacao(Publicacao* p, std::ostream& saida) {
    saida << "POST " << p->id << " " << p->idAutor << " " << p->timestamp
          << " " << p->curtidas << " " << p->texto << "\n";
}

// ═══════════════════════════════════════════════════════════════
// HASH
// ═══════════════════════════════════════════════════════════════

int calcularHash(const char username[]) {
    int h = 0;
    for (int i = 0; username[i] != '\0'; i++)
        h = (h * 31 + (unsigned char)username[i]) % TAM_HASH;
    return h;
}

void inserirNaHash(TabelaHash& tabela, Usuario* usuario) {
    int pos = calcularHash(usuario->username);
    NodoHash* novo = new NodoHash;
    strcpy(novo->username, usuario->username);
    novo->usuario = usuario;
    novo->prox    = tabela.tabela[pos];
    tabela.tabela[pos] = novo;
}

Usuario* buscarNaHash(TabelaHash& tabela, const char username[]) {
    int pos = calcularHash(username);
    NodoHash* atual = tabela.tabela[pos];
    while (atual != nullptr) {
        if (strcmp(atual->username, username) == 0)
            return atual->usuario;
        atual = atual->prox;
    }
    return nullptr;
}

void liberarHash(TabelaHash& tabela) {
    for (int i = 0; i < TAM_HASH; i++) {
        NodoHash* atual = tabela.tabela[i];
        while (atual != nullptr) {
            NodoHash* prox = atual->prox;
            delete atual;
            atual = prox;
        }
        tabela.tabela[i] = nullptr;
    }
}

// ═══════════════════════════════════════════════════════════════
// ÁRVORE BST
// ═══════════════════════════════════════════════════════════════

void inserirNaArvore(NodoArvore*& raiz, Usuario* usuario) {
    if (raiz == nullptr) {
        raiz = new NodoArvore;
        raiz->usuario = usuario;
        raiz->esq     = nullptr;
        raiz->dir     = nullptr;
        return;
    }
    if (usuario->id < raiz->usuario->id)
        inserirNaArvore(raiz->esq, usuario);
    else
        inserirNaArvore(raiz->dir, usuario);
}

Usuario* buscarNaArvore(NodoArvore* raiz, int id) {
    if (raiz == nullptr) return nullptr;
    if (id == raiz->usuario->id) return raiz->usuario;
    if (id < raiz->usuario->id)  return buscarNaArvore(raiz->esq, id);
    return buscarNaArvore(raiz->dir, id);
}

void listarArvoreEmOrdem(NodoArvore* raiz, std::ostream& saida) {
    if (raiz == nullptr) return;
    listarArvoreEmOrdem(raiz->esq, saida);
    imprimirUsuario(raiz->usuario, saida);
    listarArvoreEmOrdem(raiz->dir, saida);
}

void liberarArvore(NodoArvore* raiz) {
    if (raiz == nullptr) return;
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    // O Usuario* é liberado separadamente (evitar double free)
    delete raiz;
}

// ═══════════════════════════════════════════════════════════════
// FILA DE NOTIFICAÇÕES
// ═══════════════════════════════════════════════════════════════

void enfileirar(Fila& fila, Notificacao notif) {
    NodoFila* novo = new NodoFila;
    novo->dado = notif;
    novo->prox = nullptr;
    if (fila.fim == nullptr) {
        fila.inicio = novo;
        fila.fim    = novo;
    } else {
        fila.fim->prox = novo;
        fila.fim       = novo;
    }
}

bool desenfileirar(Fila& fila, Notificacao& notif) {
    if (fila.inicio == nullptr) return false;
    NodoFila* remover = fila.inicio;
    notif       = remover->dado;
    fila.inicio = remover->prox;
    if (fila.inicio == nullptr) fila.fim = nullptr;
    delete remover;
    return true;
}

bool filaVazia(const Fila& fila) {
    return fila.inicio == nullptr;
}

void liberarFila(Fila& fila) {
    Notificacao n;
    while (desenfileirar(fila, n)) {}
}

// ═══════════════════════════════════════════════════════════════
// LISTA DE SEGUINDO
// ═══════════════════════════════════════════════════════════════

void inserirSeguindo(NodoSeguindo*& lista, int idUsuario) {
    NodoSeguindo* novo = new NodoSeguindo;
    novo->idUsuario = idUsuario;
    novo->prox      = lista;
    lista           = novo;
}

bool estaSeguindo(NodoSeguindo* lista, int idUsuario) {
    while (lista != nullptr) {
        if (lista->idUsuario == idUsuario) return true;
        lista = lista->prox;
    }
    return false;
}

// Coleta ids de seguindo em vetor e imprime em ordem crescente
void listarSeguindoEmOrdem(NodoSeguindo* lista, NodoArvore* raizUsuarios, std::ostream& saida) {
    // Conta quantos há
    int count = 0;
    NodoSeguindo* atual = lista;
    while (atual != nullptr) { count++; atual = atual->prox; }

    if (count == 0) return;

    // Coloca ids em vetor auxiliar
    int* ids = new int[count];
    atual = lista;
    for (int i = 0; i < count; i++) {
        ids[i] = atual->idUsuario;
        atual  = atual->prox;
    }

    // Insertion sort por id crescente
    for (int i = 1; i < count; i++) {
        int chave = ids[i];
        int j = i - 1;
        while (j >= 0 && ids[j] > chave) {
            ids[j + 1] = ids[j];
            j--;
        }
        ids[j + 1] = chave;
    }

    // Imprime
    for (int i = 0; i < count; i++) {
        Usuario* u = buscarNaArvore(raizUsuarios, ids[i]);
        if (u != nullptr) imprimirUsuario(u, saida);
    }

    delete[] ids;
}

void liberarListaSeguindo(NodoSeguindo*& lista) {
    while (lista != nullptr) {
        NodoSeguindo* prox = lista->prox;
        delete lista;
        lista = prox;
    }
}

// ═══════════════════════════════════════════════════════════════
// LISTA DE QUEM CURTIU
// ═══════════════════════════════════════════════════════════════

void inserirQuemCurtiu(NodoQuemCurtiu*& lista, int idUsuario) {
    NodoQuemCurtiu* novo = new NodoQuemCurtiu;
    novo->idUsuario = idUsuario;
    novo->prox      = lista;
    lista           = novo;
}

bool jaCurtiu(NodoQuemCurtiu* lista, int idUsuario) {
    while (lista != nullptr) {
        if (lista->idUsuario == idUsuario) return true;
        lista = lista->prox;
    }
    return false;
}

void liberarListaQuemCurtiu(NodoQuemCurtiu*& lista) {
    while (lista != nullptr) {
        NodoQuemCurtiu* prox = lista->prox;
        delete lista;
        lista = prox;
    }
}

// ═══════════════════════════════════════════════════════════════
// LISTA DE PUBLICAÇÕES
// ═══════════════════════════════════════════════════════════════

void inserirPublicacao(NodoPublicacaoLista*& lista, Publicacao* pub) {
    NodoPublicacaoLista* novo = new NodoPublicacaoLista;
    novo->pub  = pub;
    novo->prox = lista;
    lista      = novo;
}

Publicacao* buscarPublicacao(NodoPublicacaoLista* lista, int idPost) {
    while (lista != nullptr) {
        if (lista->pub->id == idPost) return lista->pub;
        lista = lista->prox;
    }
    return nullptr;
}

// liberarPub=true libera o objeto Publicacao também (usado na MiniRede)
// liberarPub=false libera só o nodo (usado na lista de posts do usuário,
//   que aponta para o mesmo objeto já liberado pela MiniRede)
void liberarListaPublicacoes(NodoPublicacaoLista*& lista, bool liberarPub) {
    while (lista != nullptr) {
        NodoPublicacaoLista* prox = lista->prox;
        if (liberarPub) {
            liberarListaQuemCurtiu(lista->pub->quemCurtiu);
            delete lista->pub;
        }
        delete lista;
        lista = prox;
    }
}

// ═══════════════════════════════════════════════════════════════
// ORDENAÇÃO (insertion sort em vetor de ponteiros)
// ═══════════════════════════════════════════════════════════════

// Feed: timestamp decrescente; empate → menor post_id
void ordenarFeed(Publicacao** v, int n) {
    for (int i = 1; i < n; i++) {
        Publicacao* chave = v[i];
        int j = i - 1;
        while (j >= 0) {
            bool trocar = false;
            if (v[j]->timestamp < chave->timestamp) trocar = true;
            else if (v[j]->timestamp == chave->timestamp && v[j]->id > chave->id) trocar = true;
            if (!trocar) break;
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

// Top posts: curtidas decrescente; empate → menor post_id
void ordenarTopPosts(Publicacao** v, int n) {
    for (int i = 1; i < n; i++) {
        Publicacao* chave = v[i];
        int j = i - 1;
        while (j >= 0) {
            bool trocar = false;
            if (v[j]->curtidas < chave->curtidas) trocar = true;
            else if (v[j]->curtidas == chave->curtidas && v[j]->id > chave->id) trocar = true;
            if (!trocar) break;
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = chave;
    }
}

// ═══════════════════════════════════════════════════════════════
// INICIALIZAR / LIBERAR MINIREDE
// ═══════════════════════════════════════════════════════════════

void inicializarMiniRede(MiniRede& rede) {
    rede.raizUsuarios     = nullptr;
    rede.publicacoes      = nullptr;
    rede.totalUsuarios    = 0;
    rede.totalPublicacoes = 0;
    for (int i = 0; i < TAM_HASH; i++)
        rede.tabelaUsernames.tabela[i] = nullptr;
}

// Libera todos os usuários percorrendo a árvore em ordem
static void liberarUsuariosArvore(NodoArvore* raiz) {
    if (raiz == nullptr) return;
    liberarUsuariosArvore(raiz->esq);
    liberarUsuariosArvore(raiz->dir);
    // Libera estruturas internas do usuário
    liberarListaSeguindo(raiz->usuario->seguindo);
    liberarListaPublicacoes(raiz->usuario->posts, false); // posts liberados pela lista global
    liberarFila(raiz->usuario->notificacoes);
    delete raiz->usuario;
    delete raiz;
}

void liberarMiniRede(MiniRede& rede) {
    // Libera publicações (e quemCurtiu de cada uma)
    liberarListaPublicacoes(rede.publicacoes, true);
    // Libera hash (apenas nodos, Usuario* já serão liberados pela árvore)
    liberarHash(rede.tabelaUsernames);
    // Libera árvore + usuários
    liberarUsuariosArvore(rede.raizUsuarios);
    rede.raizUsuarios     = nullptr;
    rede.totalUsuarios    = 0;
    rede.totalPublicacoes = 0;
}

// ═══════════════════════════════════════════════════════════════
// CADASTRAR USUÁRIO
// ═══════════════════════════════════════════════════════════════

void cadastrarUsuario(MiniRede& rede, int id, const char username[], const char nomeCompleto[], std::ostream& saida) {
    if (buscarNaArvore(rede.raizUsuarios, id) != nullptr) {
        saida << "ERROR USER_EXISTS\n";
        return;
    }
    if (buscarNaHash(rede.tabelaUsernames, username) != nullptr) {
        saida << "ERROR USER_EXISTS\n";
        return;
    }

    Usuario* novo = new Usuario;
    novo->id = id;
    strcpy(novo->username, username);
    strcpy(novo->nomeCompleto, nomeCompleto);
    novo->seguindo             = nullptr;
    novo->posts                = nullptr;
    novo->notificacoes.inicio  = nullptr;
    novo->notificacoes.fim     = nullptr;

    inserirNaArvore(rede.raizUsuarios, novo);
    inserirNaHash(rede.tabelaUsernames, novo);
    rede.totalUsuarios++;

    saida << "USER_ADDED\n";
}

// ═══════════════════════════════════════════════════════════════
// BUSCAR USUÁRIO POR ID
// ═══════════════════════════════════════════════════════════════

void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida) {
    Usuario* u = buscarNaArvore(rede.raizUsuarios, id);
    if (u == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    imprimirUsuario(u, saida);
}

// ═══════════════════════════════════════════════════════════════
// BUSCAR USUÁRIO POR USERNAME
// ═══════════════════════════════════════════════════════════════

void buscarUsuarioPorUsername(MiniRede& rede, const char username[], std::ostream& saida) {
    Usuario* u = buscarNaHash(rede.tabelaUsernames, username);
    if (u == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    imprimirUsuario(u, saida);
}

// ═══════════════════════════════════════════════════════════════
// LISTAR USUÁRIOS (ordem crescente de id)
// ═══════════════════════════════════════════════════════════════

void listarUsuarios(MiniRede& rede, std::ostream& saida) {
    saida << "USERS_BEGIN\n";
    listarArvoreEmOrdem(rede.raizUsuarios, saida);
    saida << "USERS_END\n";
}

// ═══════════════════════════════════════════════════════════════
// SEGUIR USUÁRIO
// ═══════════════════════════════════════════════════════════════

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida) {
    Usuario* seguidor = buscarNaArvore(rede.raizUsuarios, idSeguidor);
    if (seguidor == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    Usuario* seguido = buscarNaArvore(rede.raizUsuarios, idSeguido);
    if (seguido == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    if (idSeguidor == idSeguido) {
        saida << "ERROR CANNOT_FOLLOW_SELF\n";
        return;
    }
    if (estaSeguindo(seguidor->seguindo, idSeguido)) {
        saida << "ERROR ALREADY_FOLLOWING\n";
        return;
    }

    inserirSeguindo(seguidor->seguindo, idSeguido);

    // Notificação para o usuário seguido
    Notificacao notif;
    strcpy(notif.tipo, "FOLLOW");
    notif.idRemetente = idSeguidor;
    notif.idPost      = -1;
    enfileirar(seguido->notificacoes, notif);

    saida << "FOLLOWED\n";
}

// ═══════════════════════════════════════════════════════════════
// LISTAR SEGUINDO (ordem crescente de id)
// ═══════════════════════════════════════════════════════════════

void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida) {
    Usuario* u = buscarNaArvore(rede.raizUsuarios, idUsuario);
    if (u == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    saida << "FOLLOWING_BEGIN\n";
    listarSeguindoEmOrdem(u->seguindo, rede.raizUsuarios, saida);
    saida << "FOLLOWING_END\n";
}

// ═══════════════════════════════════════════════════════════════
// CADASTRAR PUBLICAÇÃO
// ═══════════════════════════════════════════════════════════════

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream& saida) {
    Usuario* autor = buscarNaArvore(rede.raizUsuarios, idAutor);
    if (autor == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    if (buscarPublicacao(rede.publicacoes, idPost) != nullptr) {
        saida << "ERROR POST_EXISTS\n";
        return;
    }

    Publicacao* pub = new Publicacao;
    pub->id        = idPost;
    pub->idAutor   = idAutor;
    pub->timestamp = timestamp;
    strcpy(pub->texto, texto);
    pub->curtidas   = 0;
    pub->quemCurtiu = nullptr;

    // Insere na lista global
    inserirPublicacao(rede.publicacoes, pub);
    // Insere na lista de posts do autor
    inserirPublicacao(autor->posts, pub);
    rede.totalPublicacoes++;

    saida << "POST_ADDED\n";
}

// ═══════════════════════════════════════════════════════════════
// CURTIR PUBLICAÇÃO
// ═══════════════════════════════════════════════════════════════

void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida) {
    Usuario* usuario = buscarNaArvore(rede.raizUsuarios, idUsuario);
    if (usuario == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    Publicacao* pub = buscarPublicacao(rede.publicacoes, idPost);
    if (pub == nullptr) {
        saida << "ERROR POST_NOT_FOUND\n";
        return;
    }
    if (jaCurtiu(pub->quemCurtiu, idUsuario)) {
        saida << "ERROR ALREADY_LIKED\n";
        return;
    }

    inserirQuemCurtiu(pub->quemCurtiu, idUsuario);
    pub->curtidas++;

    // Notificação para o autor da publicação
    Usuario* autor = buscarNaArvore(rede.raizUsuarios, pub->idAutor);
    if (autor != nullptr) {
        Notificacao notif;
        strcpy(notif.tipo, "LIKE");
        notif.idRemetente = idUsuario;
        notif.idPost      = idPost;
        enfileirar(autor->notificacoes, notif);
    }

    saida << "LIKED\n";
}

// ═══════════════════════════════════════════════════════════════
// CONSULTAR NOTIFICAÇÕES
// ═══════════════════════════════════════════════════════════════

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida) {
    Usuario* u = buscarNaArvore(rede.raizUsuarios, idUsuario);
    if (u == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }

    saida << "NOTIFICATIONS_BEGIN\n";
    Notificacao notif;
    int count = 0;
    while (count < k && desenfileirar(u->notificacoes, notif)) {
        saida << "NOTIFICATION " << notif.tipo << " " << notif.idRemetente;
        if (strcmp(notif.tipo, "LIKE") == 0)
            saida << " " << notif.idPost;
        saida << "\n";
        count++;
    }
    saida << "NOTIFICATIONS_END\n";
}

// ═══════════════════════════════════════════════════════════════
// FEED
// ═══════════════════════════════════════════════════════════════

void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida) {
    Usuario* u = buscarNaArvore(rede.raizUsuarios, idUsuario);
    if (u == nullptr) {
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }

    // Conta publicações dos usuários seguidos
    int total = 0;
    NodoSeguindo* seg = u->seguindo;
    while (seg != nullptr) {
        Usuario* seguido = buscarNaArvore(rede.raizUsuarios, seg->idUsuario);
        if (seguido != nullptr) {
            NodoPublicacaoLista* p = seguido->posts;
            while (p != nullptr) { total++; p = p->prox; }
        }
        seg = seg->prox;
    }

    if (total == 0) {
        saida << "FEED_BEGIN\n";
        saida << "FEED_END\n";
        return;
    }

    // Monta vetor auxiliar
    Publicacao** vetor = new Publicacao*[total];
    int idx = 0;
    seg = u->seguindo;
    while (seg != nullptr) {
        Usuario* seguido = buscarNaArvore(rede.raizUsuarios, seg->idUsuario);
        if (seguido != nullptr) {
            NodoPublicacaoLista* p = seguido->posts;
            while (p != nullptr) {
                vetor[idx++] = p->pub;
                p = p->prox;
            }
        }
        seg = seg->prox;
    }

    ordenarFeed(vetor, total);

    saida << "FEED_BEGIN\n";
    int exibidos = 0;
    for (int i = 0; i < total && exibidos < k; i++) {
        imprimirPublicacao(vetor[i], saida);
        exibidos++;
    }
    saida << "FEED_END\n";

    delete[] vetor;
}

// ═══════════════════════════════════════════════════════════════
// TOP POSTS
// ═══════════════════════════════════════════════════════════════

void listarTopPosts(MiniRede& rede, int k, std::ostream& saida) {
    // Conta total de publicações
    int total = 0;
    NodoPublicacaoLista* p = rede.publicacoes;
    while (p != nullptr) { total++; p = p->prox; }

    if (total == 0) {
        saida << "TOP_POSTS_BEGIN\n";
        saida << "TOP_POSTS_END\n";
        return;
    }

    // Monta vetor auxiliar
    Publicacao** vetor = new Publicacao*[total];
    int idx = 0;
    p = rede.publicacoes;
    while (p != nullptr) {
        vetor[idx++] = p->pub;
        p = p->prox;
    }

    ordenarTopPosts(vetor, total);

    saida << "TOP_POSTS_BEGIN\n";
    int exibidos = 0;
    for (int i = 0; i < total && exibidos < k; i++) {
        imprimirPublicacao(vetor[i], saida);
        exibidos++;
    }
    saida << "TOP_POSTS_END\n";

    delete[] vetor;
}

// ═══════════════════════════════════════════════════════════════
// PROCESSAR COMANDOS
// ═══════════════════════════════════════════════════════════════

void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida) {
    char comando[TAM_COMANDO];

    while (entrada >> comando) {
        if (strcmp(comando, "END") == 0) break;

        if (strcmp(comando, "ADD_USER") == 0) {
            int  id;
            char username[TAM_USERNAME];
            char nomeCompleto[TAM_NOME];
            entrada >> id >> username >> nomeCompleto;
            cadastrarUsuario(rede, id, username, nomeCompleto, saida);

        } else if (strcmp(comando, "FIND_USER") == 0) {
            int id;
            entrada >> id;
            buscarUsuarioPorId(rede, id, saida);

        } else if (strcmp(comando, "FIND_USERNAME") == 0) {
            char username[TAM_USERNAME];
            entrada >> username;
            buscarUsuarioPorUsername(rede, username, saida);

        } else if (strcmp(comando, "LIST_USERS") == 0) {
            listarUsuarios(rede, saida);

        } else if (strcmp(comando, "FOLLOW") == 0) {
            int idSeguidor, idSeguido;
            entrada >> idSeguidor >> idSeguido;
            seguirUsuario(rede, idSeguidor, idSeguido, saida);

        } else if (strcmp(comando, "LIST_FOLLOWING") == 0) {
            int idUsuario;
            entrada >> idUsuario;
            listarSeguindo(rede, idUsuario, saida);

        } else if (strcmp(comando, "ADD_POST") == 0) {
            int  idPost, idAutor, timestamp;
            char texto[TAM_TEXTO];
            entrada >> idPost >> idAutor >> timestamp >> texto;
            cadastrarPublicacao(rede, idPost, idAutor, timestamp, texto, saida);

        } else if (strcmp(comando, "LIKE") == 0) {
            int idUsuario, idPost;
            entrada >> idUsuario >> idPost;
            curtirPublicacao(rede, idUsuario, idPost, saida);

        } else if (strcmp(comando, "GET_NOTIFICATIONS") == 0) {
            int idUsuario, k;
            entrada >> idUsuario >> k;
            consultarNotificacoes(rede, idUsuario, k, saida);

        } else if (strcmp(comando, "FEED") == 0) {
            int idUsuario, k;
            entrada >> idUsuario >> k;
            gerarFeed(rede, idUsuario, k, saida);

        } else if (strcmp(comando, "TOP_POSTS") == 0) {
            int k;
            entrada >> k;
            listarTopPosts(rede, k, saida);

        } else {
            saida << "ERROR INVALID_COMMAND\n";
        }
    }
}

// ═══════════════════════════════════════════════════════════════
// MAIN
// ═══════════════════════════════════════════════════════════════

int main() {
    MiniRede rede;
    inicializarMiniRede(rede);
    processarComandos(rede, std::cin, std::cout);
    liberarMiniRede(rede);
    return 0;
}