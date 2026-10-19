// HE THONG QUAN LY MANG XA HOI
// 1. Nguoi dung  : Hash Table + AVL Tree
// 2. Moi quan he : Graph (danh sach ke) + Hash Set
// 3. Bai viet    : Doubly Linked List + Max-Heap (Priority Queue)
// 4. Tuong tac   : N-ary Tree (binh luan long nhau) + Linked List (ds like)
// 5. Tim kiem    : Trie (autocomplete) + BFS
// 6. Tin nhan    : Queue (thong bao chua doc) + Doubly Linked List (lich su chat)
#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<cctype>
using namespace std;

const int MAXU = 100;    // id nguoi dung tu 1 den MAXU-1
const int HSIZE = 101;   // so bucket cua bang bam
const int MAXP = 1000;   // so bai viet toi da trong heap

// ====================== TIEN ICH ======================
typedef struct user{
    int id;
    char username[30];
    char password[30];
    char fullname[50];
    char bio[100];
}User;

User *gUsers[MAXU];      // tra cuu nhanh User theo id

const char *nameOf(int id){
    if(id > 0 && id < MAXU && gUsers[id] != NULL) return gUsers[id]->username;
    return "?";
}

int maxInt(int a, int b){
    return a > b ? a : b;
}

void copyStr(char *dst, const char *src, int n){
    strncpy(dst, src, n - 1);
    dst[n - 1] = '\0';
}

void toLowerStr(const char *src, char *dst, int n){
    int i = 0;
    for(; src[i] != '\0' && i < n - 1; i++) dst[i] = (char)tolower((unsigned char)src[i]);
    dst[i] = '\0';
}

// ====================== DANH SACH LIEN KET DON (int) ======================
typedef struct node{
    int data;
    struct node *next;
}Node;

typedef struct Slist{
    Node *head;
    Node *tail;
    int size;
    void init();
    Node *createNode(int x);
    void addFirst(int x);
    void addLast(int x);
    bool contains(int x);
    bool removeValue(int x);
    void clear();
    bool isempty();
    void print();
}Slist;

void Slist::init(){
    size = 0;
    head = tail = NULL;
}

Node *Slist::createNode(int x){
    Node *p = new Node();
    if(p == NULL){
        cout<<"cap phat bo nho loi";
        return NULL;
    }
    else{
        p->data = x;
        p->next = NULL;
        return p;
    }
}

void Slist::addFirst(int x){
    Node *t = createNode(x);
    if(size == 0){
        head = tail = t;
    }
    else{
        t->next = head;
        head = t;
    }
    size++;
}

void Slist::addLast(int x){
    Node *t = createNode(x);
    if(size == 0){
        head = tail = t;
    }
    else{
        tail->next = t;
        tail = t;
    }
    size++;
}

bool Slist::contains(int x){
    for(Node *i = head; i != NULL; i = i->next){
        if(i->data == x) return true;
    }
    return false;
}

bool Slist::removeValue(int x){
    Node *prev = NULL;
    Node *p = head;
    while(p != NULL && p->data != x){
        prev = p;
        p = p->next;
    }
    if(p == NULL) return false;
    if(prev == NULL) head = p->next;
    else prev->next = p->next;
    if(p == tail) tail = prev;
    delete p;
    size--;
    return true;
}

void Slist::clear(){
    while(head != NULL){
        Node *t = head;
        head = head->next;
        delete t;
    }
    tail = NULL;
    size = 0;
}

bool Slist::isempty(){
    return size == 0;
}

void Slist::print(){
    for(Node *i = head; i != NULL; i = i->next){
        cout<<nameOf(i->data);
        if(i->next != NULL) cout<<", ";
    }
}

// ====================== HANG DOI SO NGUYEN (cho BFS) ======================
typedef struct IntQueue{
    int a[MAXU];
    int front, rear;
    void init();
    void push(int x);
    int pop();
    bool isempty();
}IntQueue;

void IntQueue::init(){
    front = 0;
    rear = 0;
}

void IntQueue::push(int x){
    a[rear++] = x;
}

int IntQueue::pop(){
    return a[front++];
}

bool IntQueue::isempty(){
    return front == rear;
}

// ====================== 1. QUAN LY NGUOI DUNG ======================
// ---- Bang bam: tra cuu User theo username ----
typedef struct hnode{
    User *u;
    struct hnode *next;
}HNode;

typedef struct HashTable{
    HNode *bucket[HSIZE];
    void init();
    int hashFunc(const char *s);
    bool insert(User *u);
    User *find(const char *username);
}HashTable;

void HashTable::init(){
    for(int i = 0; i < HSIZE; i++) bucket[i] = NULL;
}

int HashTable::hashFunc(const char *s){
    unsigned int h = 5381;
    while(*s){
        h = h * 33 + (unsigned char)*s;
        s++;
    }
    return (int)(h % HSIZE);
}

bool HashTable::insert(User *u){
    if(find(u->username) != NULL) return false;
    HNode *n = new HNode();
    n->u = u;
    int h = hashFunc(u->username);
    n->next = bucket[h];
    bucket[h] = n;
    return true;
}

User *HashTable::find(const char *username){
    for(HNode *p = bucket[hashFunc(username)]; p != NULL; p = p->next){
        if(strcmp(p->u->username, username) == 0) return p->u;
    }
    return NULL;
}

// ---- Cay AVL: danh sach nguoi dung sap xep theo username ----
typedef struct avlnode{
    User *u;
    int height;
    struct avlnode *left;
    struct avlnode *right;
}AVLNode;

typedef struct AVLTree{
    AVLNode *root;
    void init();
    int height(AVLNode *p);
    int balance(AVLNode *p);
    AVLNode *rotateRight(AVLNode *y);
    AVLNode *rotateLeft(AVLNode *x);
    AVLNode *insert(AVLNode *p, User *u);
    void add(User *u);
    void inorder(AVLNode *p);
    void print();
}AVLTree;

void AVLTree::init(){
    root = NULL;
}

int AVLTree::height(AVLNode *p){
    return p == NULL ? 0 : p->height;
}

int AVLTree::balance(AVLNode *p){
    return p == NULL ? 0 : height(p->left) - height(p->right);
}

AVLNode *AVLTree::rotateRight(AVLNode *y){
    AVLNode *x = y->left;
    AVLNode *t = x->right;
    x->right = y;
    y->left = t;
    y->height = 1 + maxInt(height(y->left), height(y->right));
    x->height = 1 + maxInt(height(x->left), height(x->right));
    return x;
}

AVLNode *AVLTree::rotateLeft(AVLNode *x){
    AVLNode *y = x->right;
    AVLNode *t = y->left;
    y->left = x;
    x->right = t;
    x->height = 1 + maxInt(height(x->left), height(x->right));
    y->height = 1 + maxInt(height(y->left), height(y->right));
    return y;
}

AVLNode *AVLTree::insert(AVLNode *p, User *u){
    if(p == NULL){
        AVLNode *n = new AVLNode();
        n->u = u;
        n->height = 1;
        n->left = n->right = NULL;
        return n;
    }
    int c = strcmp(u->username, p->u->username);
    if(c < 0) p->left = insert(p->left, u);
    else if(c > 0) p->right = insert(p->right, u);
    else return p;

    p->height = 1 + maxInt(height(p->left), height(p->right));
    int b = balance(p);
    if(b > 1 && strcmp(u->username, p->left->u->username) < 0) return rotateRight(p);
    if(b < -1 && strcmp(u->username, p->right->u->username) > 0) return rotateLeft(p);
    if(b > 1 && strcmp(u->username, p->left->u->username) > 0){
        p->left = rotateLeft(p->left);
        return rotateRight(p);
    }
    if(b < -1 && strcmp(u->username, p->right->u->username) < 0){
        p->right = rotateRight(p->right);
        return rotateLeft(p);
    }
    return p;
}

void AVLTree::add(User *u){
    root = insert(root, u);
}

void AVLTree::inorder(AVLNode *p){
    if(p == NULL) return;
    inorder(p->left);
    cout<<"  "<<p->u->username<<" - "<<p->u->fullname<<endl;
    inorder(p->right);
}

void AVLTree::print(){
    inorder(root);
}

// ====================== 2. HASH SET (kiem tra nhanh trang thai ban be / theo doi) ======================
typedef struct snode{
    int key;
    struct snode *next;
}SNode;

typedef struct HashSet{
    SNode *bucket[HSIZE];
    void init();
    bool contains(int k);
    bool add(int k);
    bool remove(int k);
}HashSet;

void HashSet::init(){
    for(int i = 0; i < HSIZE; i++) bucket[i] = NULL;
}

bool HashSet::contains(int k){
    for(SNode *p = bucket[k % HSIZE]; p != NULL; p = p->next){
        if(p->key == k) return true;
    }
    return false;
}

bool HashSet::add(int k){
    if(contains(k)) return false;
    SNode *n = new SNode();
    n->key = k;
    n->next = bucket[k % HSIZE];
    bucket[k % HSIZE] = n;
    return true;
}

bool HashSet::remove(int k){
    SNode *prev = NULL;
    SNode *p = bucket[k % HSIZE];
    while(p != NULL && p->key != k){
        prev = p;
        p = p->next;
    }
    if(p == NULL) return false;
    if(prev == NULL) bucket[k % HSIZE] = p->next;
    else prev->next = p->next;
    delete p;
    return true;
}

// ====================== 4. BINH LUAN: CAY N PHAN (con trai - anh em phai) ======================
typedef struct comment{
    int id;
    int authorId;
    char content[150];
    struct comment *child;     // binh luan tra loi dau tien
    struct comment *sibling;   // binh luan cung cap tiep theo
}Comment;

void freeComments(Comment *p){
    if(p == NULL) return;
    freeComments(p->child);
    freeComments(p->sibling);
    delete p;
}

Comment *findComment(Comment *p, int id){
    if(p == NULL) return NULL;
    if(p->id == id) return p;
    Comment *r = findComment(p->child, id);
    if(r != NULL) return r;
    return findComment(p->sibling, id);
}

int countComments(Comment *p){
    if(p == NULL) return 0;
    return 1 + countComments(p->child) + countComments(p->sibling);
}

void printComments(Comment *p, int level){
    for(Comment *c = p; c != NULL; c = c->sibling){
        cout<<"  ";
        for(int i = 1; i < level; i++) cout<<"      ";
        if(level > 1) cout<<"`-> ";
        cout<<"[c"<<c->id<<"] "<<nameOf(c->authorId)<<": "<<c->content<<endl;
        printComments(c->child, level + 1);
    }
}

// ====================== 3. BAI VIET: DANH SACH LIEN KET DOI ======================
typedef struct post{
    int id;
    int authorId;
    char content[200];
    int time;
    Slist likes;               // danh sach id nguoi da like
    Comment *comments;         // goc cay binh luan (nut gia id = 0)
    struct post *prev;
    struct post *next;
}Post;

typedef struct PostList{
    Post *head;
    Post *tail;
    int size;
    void init();
    void addFirst(Post *p);
    Post *findById(int id);
    bool removeById(int id);
}PostList;

void PostList::init(){
    size = 0;
    head = tail = NULL;
}

void PostList::addFirst(Post *p){
    p->prev = NULL;
    p->next = head;
    if(head == NULL) tail = p;
    else head->prev = p;
    head = p;
    size++;
}

Post *PostList::findById(int id){
    for(Post *p = head; p != NULL; p = p->next){
        if(p->id == id) return p;
    }
    return NULL;
}

bool PostList::removeById(int id){
    Post *p = findById(id);
    if(p == NULL) return false;
    if(p->prev != NULL) p->prev->next = p->next;
    else head = p->next;
    if(p->next != NULL) p->next->prev = p->prev;
    else tail = p->prev;
    freeComments(p->comments);
    p->likes.clear();
    delete p;
    size--;
    return true;
}

// ---- Max-Heap: tron bai viet cho Newsfeed, bai moi nhat len dau ----
typedef struct MaxHeap{
    Post *a[MAXP];
    int n;
    void init();
    void push(Post *p);
    Post *pop();
    bool isempty();
}MaxHeap;

void MaxHeap::init(){
    n = 0;
}

void MaxHeap::push(Post *p){
    if(n >= MAXP) return;
    int i = n++;
    a[i] = p;
    while(i > 0){
        int par = (i - 1) / 2;
        if(a[par]->time >= a[i]->time) break;
        Post *t = a[par]; a[par] = a[i]; a[i] = t;
        i = par;
    }
}

Post *MaxHeap::pop(){
    Post *top = a[0];
    a[0] = a[--n];
    int i = 0;
    while(true){
        int l = 2 * i + 1, r = 2 * i + 2, big = i;
        if(l < n && a[l]->time > a[big]->time) big = l;
        if(r < n && a[r]->time > a[big]->time) big = r;
        if(big == i) break;
        Post *t = a[big]; a[big] = a[i]; a[i] = t;
        i = big;
    }
    return top;
}

bool MaxHeap::isempty(){
    return n == 0;
}

// ====================== 5. TRIE (goi y tu khoa / hashtag) ======================
typedef struct tnode{
    struct tnode *child[128];
    bool isEnd;
    int count;
}TNode;

typedef struct Trie{
    TNode *root;
    bool showCount;
    void init(bool sc);
    TNode *createNode();
    void insert(const char *s);
    void collect(TNode *p, char *buf, int len, int &found);
    void autocomplete(const char *prefix);
}Trie;

void Trie::init(bool sc){
    showCount = sc;
    root = createNode();
}

TNode *Trie::createNode(){
    TNode *p = new TNode();
    for(int i = 0; i < 128; i++) p->child[i] = NULL;
    p->isEnd = false;
    p->count = 0;
    return p;
}

void Trie::insert(const char *s){
    TNode *p = root;
    for(int i = 0; s[i] != '\0'; i++){
        int c = ((unsigned char)tolower((unsigned char)s[i])) & 127;
        if(p->child[c] == NULL) p->child[c] = createNode();
        p = p->child[c];
    }
    p->isEnd = true;
    p->count++;
}

void Trie::collect(TNode *p, char *buf, int len, int &found){
    if(p->isEnd){
        buf[len] = '\0';
        cout<<"  "<<buf;
        if(showCount) cout<<" ("<<p->count<<" bai)";
        cout<<endl;
        found++;
    }
    for(int i = 0; i < 128; i++){
        if(p->child[i] != NULL && len < 60){
            buf[len] = (char)i;
            collect(p->child[i], buf, len + 1, found);
        }
    }
}

void Trie::autocomplete(const char *prefix){
    TNode *p = root;
    char buf[64];
    int len = 0;
    for(int i = 0; prefix[i] != '\0' && len < 60; i++){
        int c = ((unsigned char)tolower((unsigned char)prefix[i])) & 127;
        if(p->child[c] == NULL){
            cout<<"  (khong co goi y)"<<endl;
            return;
        }
        p = p->child[c];
        buf[len++] = (char)c;
    }
    int found = 0;
    collect(p, buf, len, found);
    if(found == 0) cout<<"  (khong co goi y)"<<endl;
}

// ====================== 6. THONG BAO (HANG DOI FIFO) & TIN NHAN (DSLK DOI) ======================
typedef struct notif{
    char text[200];
    struct notif *next;
}Notif;

typedef struct NotifQueue{
    Notif *front;
    Notif *rear;
    int size;
    void init();
    void enqueue(const char *t);
    bool dequeue(char *out);
    bool isempty();
}NotifQueue;

void NotifQueue::init(){
    front = rear = NULL;
    size = 0;
}

void NotifQueue::enqueue(const char *t){
    Notif *n = new Notif();
    copyStr(n->text, t, 200);
    n->next = NULL;
    if(rear == NULL) front = rear = n;
    else{
        rear->next = n;
        rear = n;
    }
    size++;
}

bool NotifQueue::dequeue(char *out){
    if(front == NULL) return false;
    Notif *t = front;
    copyStr(out, t->text, 200);
    front = front->next;
    if(front == NULL) rear = NULL;
    delete t;
    size--;
    return true;
}

bool NotifQueue::isempty(){
    return size == 0;
}

typedef struct msg{
    int from;
    char text[200];
    int time;
    struct msg *prev;
    struct msg *next;
}Msg;

typedef struct conv{
    int a, b;                  // a < b
    Msg *head;
    Msg *tail;
    int size;
    struct conv *next;
}Conv;

// ====================== SOCIAL NETWORK (lop dieu phoi) ======================
typedef struct SocialNetwork{
    HashTable ht;
    AVLTree avl;
    Trie userTrie;
    Trie tagTrie;
    Slist friends[MAXU];       // do thi vo huong (ban be)
    Slist following[MAXU];     // do thi co huong (theo doi)
    HashSet friendSet;
    HashSet followSet;
    PostList posts[MAXU];
    NotifQueue notifs[MAXU];
    Conv *convHead;
    int userCount, postCounter, commentCounter, timer;

    void init();
    bool valid(int id);
    void notify(int to, const char *text);

    // 1. Nguoi dung
    int registerUser(const char *username, const char *password, const char *fullname, const char *bio);
    int login(const char *username, const char *password);
    void printProfile(User *u);
    void searchUser(const char *username);
    bool updateProfile(int id, const char *fullname, const char *bio);

    // 2. Moi quan he
    bool addFriend(int a, int b);
    bool unfriend(int a, int b);
    bool follow(int a, int b);
    bool unfollow(int a, int b);
    void printFriends(int id);
    void printFollowing(int id);

    // 3. Bai viet & Newsfeed
    int createPost(int author, const char *content);
    bool deletePost(int author, int postId);
    Post *findPost(int postId);
    void printPost(Post *p);
    void printUserPosts(int id);
    void pushPosts(MaxHeap &h, int uid);
    void newsfeed(int id, int k);

    // 4. Tuong tac
    bool likePost(int u, int postId);
    bool unlikePost(int u, int postId);
    void printLikes(int postId);
    bool addComment(int u, int postId, int parentId, const char *text);
    void showComments(int postId);

    // 5. Tim kiem & goi y
    void mutualFriends(int a, int b);
    int bfs(int s, int t, bool showPath);
    void suggestFriends(int u);

    // 6. Tin nhan & thong bao
    Conv *findConv(int a, int b, bool create);
    bool sendMessage(int from, int to, const char *text);
    void showHistory(int a, int b);
    void showRecent(int a, int b, int n);
    void showNotifications(int id);
}SocialNetwork;

void SocialNetwork::init(){
    ht.init();
    avl.init();
    userTrie.init(false);
    tagTrie.init(true);
    friendSet.init();
    followSet.init();
    for(int i = 0; i < MAXU; i++){
        friends[i].init();
        following[i].init();
        posts[i].init();
        notifs[i].init();
        gUsers[i] = NULL;
    }
    convHead = NULL;
    userCount = postCounter = commentCounter = timer = 0;
}

bool SocialNetwork::valid(int id){
    return id > 0 && id < MAXU && gUsers[id] != NULL;
}

void SocialNetwork::notify(int to, const char *text){
    if(valid(to)) notifs[to].enqueue(text);
}

// ---------- 1. NGUOI DUNG ----------
int SocialNetwork::registerUser(const char *username, const char *password, const char *fullname, const char *bio){
    char uname[30];
    toLowerStr(username, uname, 30);
    if(strlen(uname) == 0 || strlen(username) >= 30) return 0;
    if(userCount >= MAXU - 1) return 0;
    if(ht.find(uname) != NULL) return 0;
    User *u = new User();
    u->id = ++userCount;
    copyStr(u->username, uname, 30);
    copyStr(u->password, password, 30);
    copyStr(u->fullname, fullname, 50);
    copyStr(u->bio, bio, 100);
    gUsers[u->id] = u;
    ht.insert(u);
    avl.add(u);
    userTrie.insert(u->username);
    return u->id;
}

int SocialNetwork::login(const char *username, const char *password){
    char uname[30];
    toLowerStr(username, uname, 30);
    User *u = ht.find(uname);
    if(u == NULL || strcmp(u->password, password) != 0) return 0;
    return u->id;
}

void SocialNetwork::printProfile(User *u){
    cout<<"  +----------------------------------------------------"<<endl;
    cout<<"  |  @"<<u->username<<"   (ID "<<u->id<<")"<<endl;
    cout<<"  |  Ho ten    : "<<u->fullname<<endl;
    cout<<"  |  Gioi thieu: "<<u->bio<<endl;
    cout<<"  |  Ban be: "<<friends[u->id].size<<"    Dang theo doi: "<<following[u->id].size
        <<"    Bai viet: "<<posts[u->id].size<<endl;
    cout<<"  +----------------------------------------------------"<<endl;
}

void SocialNetwork::searchUser(const char *username){
    char uname[30];
    toLowerStr(username, uname, 30);
    User *u = ht.find(uname);
    if(u != NULL){
        printProfile(u);
    }
    else{
        cout<<"Khong tim thay. Co the ban muon tim:"<<endl;
        userTrie.autocomplete(uname);
    }
}

bool SocialNetwork::updateProfile(int id, const char *fullname, const char *bio){
    if(!valid(id)) return false;
    if(strlen(fullname) > 0) copyStr(gUsers[id]->fullname, fullname, 50);
    if(strlen(bio) > 0) copyStr(gUsers[id]->bio, bio, 100);
    return true;
}

// ---------- 2. MOI QUAN HE ----------
bool SocialNetwork::addFriend(int a, int b){
    if(a == b || !valid(a) || !valid(b)) return false;
    if(friendSet.contains(a * MAXU + b)) return false;
    friends[a].addLast(b);
    friends[b].addLast(a);
    friendSet.add(a * MAXU + b);
    friendSet.add(b * MAXU + a);
    char buf[200];
    sprintf(buf, "%s da ket ban voi ban", nameOf(a));
    notify(b, buf);
    return true;
}

bool SocialNetwork::unfriend(int a, int b){
    if(!valid(a) || !valid(b)) return false;
    if(!friendSet.contains(a * MAXU + b)) return false;
    friends[a].removeValue(b);
    friends[b].removeValue(a);
    friendSet.remove(a * MAXU + b);
    friendSet.remove(b * MAXU + a);
    return true;
}

bool SocialNetwork::follow(int a, int b){
    if(a == b || !valid(a) || !valid(b)) return false;
    if(followSet.contains(a * MAXU + b)) return false;
    following[a].addLast(b);
    followSet.add(a * MAXU + b);
    char buf[200];
    sprintf(buf, "%s da theo doi ban", nameOf(a));
    notify(b, buf);
    return true;
}

bool SocialNetwork::unfollow(int a, int b){
    if(!valid(a) || !valid(b)) return false;
    if(!followSet.contains(a * MAXU + b)) return false;
    following[a].removeValue(b);
    followSet.remove(a * MAXU + b);
    return true;
}

void SocialNetwork::printFriends(int id){
    cout<<"Danh sach ban be cua "<<nameOf(id)<<" ("<<friends[id].size<<"): ";
    friends[id].print();
    cout<<endl;
}

void SocialNetwork::printFollowing(int id){
    cout<<nameOf(id)<<" dang theo doi ("<<following[id].size<<"): ";
    following[id].print();
    cout<<endl;
}

// ---------- 3. BAI VIET & NEWSFEED ----------
int SocialNetwork::createPost(int author, const char *content){
    if(!valid(author) || strlen(content) == 0) return 0;
    Post *p = new Post();
    p->id = ++postCounter;
    p->authorId = author;
    copyStr(p->content, content, 200);
    p->time = ++timer;
    p->likes.init();
    p->comments = new Comment();         // nut goc gia, id = 0
    p->comments->id = 0;
    p->prev = p->next = NULL;
    posts[author].addFirst(p);           // bai moi nhat o dau danh sach

    // tach hashtag dua vao Trie
    for(int i = 0; p->content[i] != '\0'; i++){
        if(p->content[i] == '#'){
            char tag[32];
            int k = 0, j = i + 1;
            while(p->content[j] != '\0' && (isalnum((unsigned char)p->content[j]) || p->content[j] == '_') && k < 30){
                tag[k++] = p->content[j];
                j++;
            }
            tag[k] = '\0';
            if(k > 0) tagTrie.insert(tag);
            i = j - 1;
        }
    }
    return p->id;
}

bool SocialNetwork::deletePost(int author, int postId){
    if(!valid(author)) return false;
    return posts[author].removeById(postId);
}

Post *SocialNetwork::findPost(int postId){
    for(int u = 1; u <= userCount; u++){
        Post *p = posts[u].findById(postId);
        if(p != NULL) return p;
    }
    return NULL;
}

void SocialNetwork::printPost(Post *p){
    cout<<"  +-- Bai #"<<p->id<<"  |  "<<nameOf(p->authorId)<<"  |  luc "<<p->time<<endl;
    cout<<"  |   "<<p->content<<endl;
    cout<<"  |   Like: "<<p->likes.size<<"    Binh luan: "<<countComments(p->comments->child)<<endl;
    cout<<"  +----------------------------------------------------"<<endl;
}

void SocialNetwork::printUserPosts(int id){
    cout<<"Bai viet cua "<<nameOf(id)<<" (moi nhat truoc):"<<endl;
    if(posts[id].size == 0) cout<<"  (chua co bai viet)"<<endl;
    for(Post *p = posts[id].head; p != NULL; p = p->next){
        printPost(p);
    }
}

void SocialNetwork::pushPosts(MaxHeap &h, int uid){
    for(Post *p = posts[uid].head; p != NULL; p = p->next){
        h.push(p);
    }
}

void SocialNetwork::newsfeed(int id, int k){
    bool seen[MAXU];
    for(int i = 0; i < MAXU; i++) seen[i] = false;
    MaxHeap h;
    h.init();
    seen[id] = true;
    pushPosts(h, id);
    for(Node *p = friends[id].head; p != NULL; p = p->next){
        if(!seen[p->data]){
            seen[p->data] = true;
            pushPosts(h, p->data);
        }
    }
    for(Node *p = following[id].head; p != NULL; p = p->next){
        if(!seen[p->data]){
            seen[p->data] = true;
            pushPosts(h, p->data);
        }
    }
    cout<<"  Newsfeed cua "<<nameOf(id)<<" (bai moi nhat o tren):"<<endl<<endl;
    if(h.isempty()) cout<<"  (chua co bai viet nao)"<<endl;
    for(int i = 0; i < k && !h.isempty(); i++){
        printPost(h.pop());
    }
}

// ---------- 4. TUONG TAC ----------
bool SocialNetwork::likePost(int u, int postId){
    Post *p = findPost(postId);
    if(p == NULL || !valid(u)) return false;
    if(p->likes.contains(u)) return false;
    p->likes.addLast(u);
    if(p->authorId != u){
        char buf[200];
        sprintf(buf, "%s da thich bai viet #%d cua ban", nameOf(u), postId);
        notify(p->authorId, buf);
    }
    return true;
}

bool SocialNetwork::unlikePost(int u, int postId){
    Post *p = findPost(postId);
    if(p == NULL) return false;
    return p->likes.removeValue(u);
}

void SocialNetwork::printLikes(int postId){
    Post *p = findPost(postId);
    if(p == NULL){
        cout<<"Khong tim thay bai viet"<<endl;
        return;
    }
    cout<<"Bai #"<<postId<<" co "<<p->likes.size<<" like: ";
    p->likes.print();
    cout<<endl;
}

bool SocialNetwork::addComment(int u, int postId, int parentId, const char *text){
    Post *p = findPost(postId);
    if(p == NULL || !valid(u) || strlen(text) == 0) return false;
    Comment *parent = (parentId == 0) ? p->comments : findComment(p->comments, parentId);
    if(parent == NULL) return false;
    Comment *c = new Comment();
    c->id = ++commentCounter;
    c->authorId = u;
    copyStr(c->content, text, 150);
    c->child = c->sibling = NULL;
    if(parent->child == NULL) parent->child = c;
    else{
        Comment *t = parent->child;
        while(t->sibling != NULL) t = t->sibling;
        t->sibling = c;
    }
    if(p->authorId != u){
        char buf[200];
        sprintf(buf, "%s da binh luan bai viet #%d cua ban", nameOf(u), postId);
        notify(p->authorId, buf);
    }
    return true;
}

void SocialNetwork::showComments(int postId){
    Post *p = findPost(postId);
    if(p == NULL){
        cout<<"Khong tim thay bai viet"<<endl;
        return;
    }
    printPost(p);
    if(p->comments->child == NULL) cout<<"  (chua co binh luan)"<<endl;
    printComments(p->comments->child, 1);
}

// ---------- 5. TIM KIEM & GOI Y ----------
void SocialNetwork::mutualFriends(int a, int b){
    int count = 0;
    cout<<"Ban chung cua "<<nameOf(a)<<" va "<<nameOf(b)<<": ";
    for(Node *p = friends[a].head; p != NULL; p = p->next){
        if(friendSet.contains(p->data * MAXU + b)){
            cout<<nameOf(p->data)<<" ";
            count++;
        }
    }
    if(count == 0) cout<<"(khong co)";
    cout<<endl;
}

// BFS tren do thi ban be: khoang cach ket noi ngan nhat
int SocialNetwork::bfs(int s, int t, bool showPath){
    if(!valid(s) || !valid(t)) return -1;
    int dist[MAXU], par[MAXU];
    for(int i = 0; i < MAXU; i++){
        dist[i] = -1;
        par[i] = -1;
    }
    IntQueue q;
    q.init();
    dist[s] = 0;
    q.push(s);
    while(!q.isempty()){
        int u = q.pop();
        if(u == t) break;
        for(Node *p = friends[u].head; p != NULL; p = p->next){
            int v = p->data;
            if(dist[v] == -1){
                dist[v] = dist[u] + 1;
                par[v] = u;
                q.push(v);
            }
        }
    }
    if(dist[t] != -1 && showPath){
        int path[MAXU], len = 0;
        for(int x = t; x != -1; x = par[x]) path[len++] = x;
        cout<<"Duong ket noi: ";
        for(int i = len - 1; i >= 0; i--){
            cout<<nameOf(path[i]);
            if(i > 0) cout<<" -> ";
        }
        cout<<endl;
    }
    return dist[t];
}

void SocialNetwork::suggestFriends(int u){
    int score[MAXU];
    for(int i = 0; i < MAXU; i++) score[i] = 0;
    for(Node *f = friends[u].head; f != NULL; f = f->next){
        for(Node *g = friends[f->data].head; g != NULL; g = g->next){
            int x = g->data;
            if(x != u && !friendSet.contains(u * MAXU + x)) score[x]++;
        }
    }
    cout<<"Goi y ket ban cho "<<nameOf(u)<<":"<<endl;
    int shown = 0;
    for(int r = 0; r < 5; r++){
        int best = -1;
        for(int i = 1; i < MAXU; i++){
            if(score[i] > 0 && (best == -1 || score[i] > score[best])) best = i;
        }
        if(best == -1) break;
        cout<<"  "<<nameOf(best)<<" ("<<score[best]<<" ban chung)"<<endl;
        score[best] = 0;
        shown++;
    }
    if(shown == 0) cout<<"  (chua co goi y)"<<endl;
}

// ---------- 6. TIN NHAN & THONG BAO ----------
Conv *SocialNetwork::findConv(int a, int b, bool create){
    int x = a < b ? a : b;
    int y = a < b ? b : a;
    for(Conv *c = convHead; c != NULL; c = c->next){
        if(c->a == x && c->b == y) return c;
    }
    if(!create) return NULL;
    Conv *c = new Conv();
    c->a = x;
    c->b = y;
    c->head = c->tail = NULL;
    c->size = 0;
    c->next = convHead;
    convHead = c;
    return c;
}

bool SocialNetwork::sendMessage(int from, int to, const char *text){
    if(from == to || !valid(from) || !valid(to) || strlen(text) == 0) return false;
    Conv *c = findConv(from, to, true);
    Msg *m = new Msg();
    m->from = from;
    copyStr(m->text, text, 200);
    m->time = ++timer;
    m->next = NULL;
    m->prev = c->tail;
    if(c->tail == NULL) c->head = m;
    else c->tail->next = m;
    c->tail = m;
    c->size++;
    char buf[200];
    sprintf(buf, "Ban co tin nhan moi tu %s", nameOf(from));
    notify(to, buf);
    return true;
}

void SocialNetwork::showHistory(int a, int b){
    Conv *c = findConv(a, b, false);
    cout<<"  Cuoc tro chuyen "<<nameOf(a)<<" <-> "<<nameOf(b)<<" (cu -> moi):"<<endl;
    if(c == NULL){
        cout<<"  (chua co tin nhan)"<<endl;
        return;
    }
    for(Msg *m = c->head; m != NULL; m = m->next){
        cout<<"  [t="<<m->time<<"] "<<nameOf(m->from)<<": "<<m->text<<endl;
    }
}

void SocialNetwork::showRecent(int a, int b, int n){
    Conv *c = findConv(a, b, false);
    cout<<endl<<"  "<<n<<" tin nhan gan nhat (cuon nguoc tu cuoi):"<<endl;
    if(c == NULL){
        cout<<"  (chua co tin nhan)"<<endl;
        return;
    }
    int i = 0;
    for(Msg *m = c->tail; m != NULL && i < n; m = m->prev, i++){
        cout<<"  [t="<<m->time<<"] "<<nameOf(m->from)<<": "<<m->text<<endl;
    }
}

void SocialNetwork::showNotifications(int id){
    cout<<"  Thong bao cua "<<nameOf(id)<<":"<<endl;
    if(notifs[id].isempty()){
        cout<<"  (khong co thong bao moi)"<<endl;
        return;
    }
    char buf[200];
    while(notifs[id].dequeue(buf)){
        cout<<"  * "<<buf<<endl;
    }
}

// ====================== GIAO DIEN CONSOLE ======================
void clearScreen(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void border(char c){
    cout<<"+";
    for(int i = 0; i < 51; i++) cout<<c;
    cout<<"+"<<endl;
}

void rowText(const char *s){
    printf("| %-49s |\n", s);
}

void rowTwo(const char *l, const char *r){
    printf("| %-24s %-24s |\n", l, r);
}

void ok(const char *s){
    cout<<endl<<"  [OK] "<<s<<endl;
}

void err(const char *s){
    cout<<endl<<"  [!]  "<<s<<endl;
}

void readStr(const char *prompt, char *buf, int n){
    cout<<"  "<<prompt;
    cin.getline(buf, n);
    if(!cin.good()){
        cin.clear();
        buf[0] = '\0';
    }
}

int readInt(const char *prompt){
    char buf[32];
    readStr(prompt, buf, 32);
    return atoi(buf);
}

void pauseScreen(){
    char t[256];
    cout<<endl<<"  >> Nhan Enter de quay lai menu...";
    cin.getline(t, 256);
    if(!cin.good()) cin.clear();
}

bool needLogin(int cur){
    if(cur == 0){
        err("Ban can dang nhap truoc (chon 2 o menu)");
        return true;
    }
    return false;
}

void printMenu(const char *who, int unread){
    char buf[100];
    border('=');
    rowText("          MANG XA HOI MINI - BAI TAP LON CTDL&GT");
    border('=');
    sprintf(buf, "Tai khoan: %s  |  Thong bao moi: %d", who, unread);
    rowText(buf);
    border('-');
    rowText("[1] NGUOI DUNG");
    rowTwo("  1  Dang ky", "2  Dang nhap");
    rowTwo("  3  Tim nguoi dung", "4  Cap nhat ho so");
    rowTwo("  5  DS nguoi dung (A-Z)", "6  Xem ho so cua toi");
    border('-');
    rowText("[2] MOI QUAN HE");
    rowTwo("  7  Ket ban", "8  Huy ket ban");
    rowTwo("  9  Theo doi", "10 Bo theo doi");
    rowTwo("  11 Xem ban be", "12 Xem dang theo doi");
    border('-');
    rowText("[3] BAI VIET");
    rowTwo("  13 Dang bai", "14 Xoa bai");
    rowTwo("  15 Bai viet cua toi", "16 Newsfeed");
    border('-');
    rowText("[4] TUONG TAC");
    rowTwo("  17 Like", "18 Unlike");
    rowTwo("  19 Binh luan", "20 Xem binh luan");
    rowTwo("  21 Xem danh sach like", "");
    border('-');
    rowText("[5] TIM KIEM & GOI Y");
    rowTwo("  22 Goi y username", "23 Goi y hashtag");
    rowTwo("  24 Ban chung", "25 Khoang cach (BFS)");
    rowTwo("  26 Goi y ket ban", "");
    border('-');
    rowText("[6] TIN NHAN & THONG BAO");
    rowTwo("  27 Gui tin nhan", "28 Lich su chat");
    rowTwo("  29 Xem thong bao", "");
    border('-');
    rowText("  0  Thoat chuong trinh");
    border('=');
}

void printTitle(int ch){
    const char *titles[30] = {
        "", "DANG KY NGUOI DUNG", "DANG NHAP", "TIM NGUOI DUNG", "CAP NHAT HO SO",
        "DANH SACH NGUOI DUNG (CAY AVL)", "HO SO CUA TOI", "KET BAN", "HUY KET BAN",
        "THEO DOI", "BO THEO DOI", "DANH SACH BAN BE", "DANH SACH DANG THEO DOI",
        "DANG BAI MOI", "XOA BAI VIET", "BAI VIET CUA TOI", "NEWSFEED (MAX-HEAP)",
        "LIKE BAI VIET", "UNLIKE BAI VIET", "BINH LUAN", "XEM BINH LUAN (CAY N PHAN)",
        "DANH SACH LIKE", "GOI Y USERNAME (TRIE)", "GOI Y HASHTAG (TRIE)", "BAN CHUNG",
        "KHOANG CACH KET NOI (BFS)", "GOI Y KET BAN", "GUI TIN NHAN", "LICH SU CHAT",
        "THONG BAO (HANG DOI)"
    };
    char buf[100];
    cout<<endl;
    border('=');
    if(ch >= 1 && ch <= 29) sprintf(buf, "  >> %s", titles[ch]);
    else sprintf(buf, "  >> KHONG HOP LE");
    rowText(buf);
    border('=');
    cout<<endl;
}

void seed(SocialNetwork &sn){
    sn.registerUser("dat", "123", "Nguyen Tien Dat", "Sinh vien CNTT");
    sn.registerUser("giang", "123", "Tran Truong Giang", "Thich du lich");
    sn.registerUser("duy", "123", "Le Khanh Duy", "Dam me lap trinh");
    sn.registerUser("long", "123", "Pham Hoang Long", "Game thu");
    sn.registerUser("tuan", "123", "Vo Anh Tuan", "Yeu am nhac");
    sn.registerUser("huy", "123", "Do Quang Huy", "Nguoi moi");

    sn.addFriend(1, 2); sn.addFriend(2, 3); sn.addFriend(3, 4);
    sn.addFriend(1, 3); sn.addFriend(4, 5);
    sn.follow(1, 4); sn.follow(5, 1);

    sn.createPost(1, "Hom nay hoc #cautrucdulieu that vui");
    sn.createPost(2, "Cuoi tuan di choi #dulich #daklak");
    sn.createPost(3, "On thi #cautrucdulieu va #giaithuat");
    sn.createPost(4, "Bat dau hoc #cpp");
    sn.createPost(3, "Da hoan thanh bai tap lon #cpp");

    sn.likePost(2, 1); sn.likePost(3, 1); sn.likePost(1, 3);
    sn.addComment(2, 1, 0, "Chuc mung ban!");
    sn.addComment(1, 1, 1, "Cam on Giang nhe");
    sn.addComment(3, 1, 0, "Hoc cung nhau nhe");
    sn.sendMessage(1, 2, "Chao Giang, cuoi tuan di cafe khong?");
    sn.sendMessage(2, 1, "Duoc, 9h nhe!");
    sn.sendMessage(1, 2, "Ok hen gap");
}

// ====================== XU LY TUNG CHUC NANG ======================
void handle(SocialNetwork &sn, int &cur, int ch){
    char a[100], b[100], c[200], d[200];

    if(ch == 1){
        readStr("Username: ", a, 100);
        readStr("Mat khau: ", b, 100);
        readStr("Ho ten: ", c, 200);
        readStr("Gioi thieu: ", d, 200);
        int id = sn.registerUser(a, b, c, d);
        if(id > 0){
            sprintf(d, "Dang ky thanh cong, id = %d. Hay chon 2 de dang nhap.", id);
            ok(d);
        }
        else err("Dang ky that bai (username rong / qua dai / da ton tai)");
    }
    else if(ch == 2){
        readStr("Username: ", a, 100);
        readStr("Mat khau: ", b, 100);
        int id = sn.login(a, b);
        if(id > 0){
            cur = id;
            sprintf(d, "Dang nhap thanh cong! Xin chao %s (%d thong bao chua doc)", nameOf(cur), sn.notifs[cur].size);
            ok(d);
        }
        else err("Sai username hoac mat khau");
    }
    else if(ch == 3){
        readStr("Nhap username can tim: ", a, 100);
        cout<<endl;
        sn.searchUser(a);
    }
    else if(ch == 4){
        if(needLogin(cur)) return;
        readStr("Ho ten moi (bo trong = giu nguyen): ", c, 200);
        readStr("Gioi thieu moi (bo trong = giu nguyen): ", d, 200);
        sn.updateProfile(cur, c, d);
        ok("Da cap nhat ho so");
        cout<<endl;
        sn.printProfile(gUsers[cur]);
    }
    else if(ch == 5){
        sn.avl.print();
    }
    else if(ch == 6){
        if(needLogin(cur)) return;
        sn.printProfile(gUsers[cur]);
    }
    else if(ch >= 7 && ch <= 10){
        if(needLogin(cur)) return;
        readStr("Username nguoi kia: ", a, 100);
        toLowerStr(a, b, 100);
        User *t = sn.ht.find(b);
        if(t == NULL){
            err("Khong tim thay nguoi dung");
            return;
        }
        bool res = false;
        const char *msg = "";
        if(ch == 7){ res = sn.addFriend(cur, t->id); msg = "Da ket ban"; }
        else if(ch == 8){ res = sn.unfriend(cur, t->id); msg = "Da huy ket ban"; }
        else if(ch == 9){ res = sn.follow(cur, t->id); msg = "Da theo doi"; }
        else{ res = sn.unfollow(cur, t->id); msg = "Da bo theo doi"; }
        if(res) ok(msg);
        else err("Khong thuc hien duoc (da ton tai / chua co quan he / chinh ban)");
    }
    else if(ch == 11){
        if(needLogin(cur)) return;
        sn.printFriends(cur);
    }
    else if(ch == 12){
        if(needLogin(cur)) return;
        sn.printFollowing(cur);
    }
    else if(ch == 13){
        if(needLogin(cur)) return;
        readStr("Noi dung (dung #hashtag neu muon): ", c, 200);
        int id = sn.createPost(cur, c);
        if(id > 0){
            sprintf(d, "Da dang bai #%d", id);
            ok(d);
        }
        else err("Noi dung rong");
    }
    else if(ch == 14){
        if(needLogin(cur)) return;
        int id = readInt("ID bai viet can xoa: ");
        if(sn.deletePost(cur, id)) ok("Da xoa bai viet");
        else err("Khong tim thay bai viet cua ban");
    }
    else if(ch == 15){
        if(needLogin(cur)) return;
        sn.printUserPosts(cur);
    }
    else if(ch == 16){
        if(needLogin(cur)) return;
        int k = readInt("Xem toi da bao nhieu bai: ");
        if(k <= 0) k = 5;
        cout<<endl;
        sn.newsfeed(cur, k);
    }
    else if(ch == 17 || ch == 18){
        if(needLogin(cur)) return;
        int id = readInt("ID bai viet: ");
        bool res = (ch == 17) ? sn.likePost(cur, id) : sn.unlikePost(cur, id);
        if(res) ok(ch == 17 ? "Da like" : "Da bo like");
        else err("Khong thuc hien duoc (sai id / da like / chua like)");
    }
    else if(ch == 19){
        if(needLogin(cur)) return;
        int id = readInt("ID bai viet: ");
        int pid = readInt("Tra loi binh luan so (0 = binh luan goc): ");
        readStr("Noi dung: ", c, 200);
        if(sn.addComment(cur, id, pid, c)) ok("Da binh luan");
        else err("That bai (sai id bai viet / binh luan, hoac noi dung rong)");
    }
    else if(ch == 20){
        int id = readInt("ID bai viet: ");
        cout<<endl;
        sn.showComments(id);
    }
    else if(ch == 21){
        int id = readInt("ID bai viet: ");
        cout<<endl;
        sn.printLikes(id);
    }
    else if(ch == 22){
        readStr("Nhap tien to username: ", a, 100);
        cout<<endl<<"  Ket qua goi y:"<<endl;
        sn.userTrie.autocomplete(a);
    }
    else if(ch == 23){
        readStr("Nhap tien to hashtag (khong can dau #): ", a, 100);
        cout<<endl<<"  Ket qua goi y:"<<endl;
        sn.tagTrie.autocomplete(a);
    }
    else if(ch == 24 || ch == 25){
        if(needLogin(cur)) return;
        readStr("Username nguoi kia: ", a, 100);
        toLowerStr(a, b, 100);
        User *t = sn.ht.find(b);
        if(t == NULL){
            err("Khong tim thay nguoi dung");
            return;
        }
        cout<<endl;
        if(ch == 24) sn.mutualFriends(cur, t->id);
        else{
            int dist = sn.bfs(cur, t->id, true);
            if(dist < 0) cout<<"  Hai nguoi chua co ket noi nao"<<endl;
            else cout<<"  Khoang cach ket noi: "<<dist<<" buoc"<<endl;
        }
    }
    else if(ch == 26){
        if(needLogin(cur)) return;
        sn.suggestFriends(cur);
    }
    else if(ch == 27 || ch == 28){
        if(needLogin(cur)) return;
        readStr("Username nguoi kia: ", a, 100);
        toLowerStr(a, b, 100);
        User *t = sn.ht.find(b);
        if(t == NULL){
            err("Khong tim thay nguoi dung");
            return;
        }
        if(ch == 27){
            readStr("Noi dung tin nhan: ", c, 200);
            if(sn.sendMessage(cur, t->id, c)) ok("Da gui tin nhan");
            else err("Gui that bai (noi dung rong hoac gui cho chinh minh)");
        }
        else{
            cout<<endl;
            sn.showHistory(cur, t->id);
            sn.showRecent(cur, t->id, 2);
        }
    }
    else if(ch == 29){
        if(needLogin(cur)) return;
        sn.showNotifications(cur);
    }
    else{
        err("Lua chon khong hop le, hay nhap so tu 0 den 29");
    }
}

int main(){
    SocialNetwork sn;
    sn.init();
    int cur = 0;

    clearScreen();
    cout<<endl;
    border('=');
    rowText("          MANG XA HOI MINI - BAI TAP LON CTDL&GT");
    border('=');
    cout<<endl;
    if(readInt("Nap du lieu mau? (1 = co, 0 = khong): ") == 1){
        seed(sn);
        ok("Da nap 6 nguoi dung: dat, giang, duy, long, tuan, huy (mat khau: 123)");
        pauseScreen();
    }

    while(true){
        clearScreen();
        printMenu(cur == 0 ? "(chua dang nhap)" : nameOf(cur), cur == 0 ? 0 : sn.notifs[cur].size);
        int ch = readInt("Chon chuc nang: ");
        if(ch == 0) break;
        clearScreen();
        printTitle(ch);
        handle(sn, cur, ch);
        pauseScreen();
    }
    cout<<endl<<"  Tam biet!"<<endl;
    return 0;
}
