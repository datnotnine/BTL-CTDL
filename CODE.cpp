// HE THONG QUAN LY MANG XA HOI
// 1. Nguoi dung  : Hash Table + AVL Tree (+ luu tai khoan da dang nhap)
// 2. Moi quan he : Graph (danh sach ke) + Hash Set
// 3. Bai viet    : Doubly Linked List (danh so #1, #2... rieng tung tai khoan)
// 4. Tuong tac   : N-ary Tree (binh luan long nhau) + Linked List (ds like)
// 5. Ban chung   : Giao hai tap ke, goi y ket ban, BFS (khoang cach ket noi)
// 6. Tin nhan    : Queue (thong bao chua doc) + Doubly Linked List (lich su chat)
#include<iostream>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<cctype>
using namespace std;

const int MAXU = 100;    // id nguoi dung tu 1 den MAXU-1
const int HSIZE = 101;   // so bucket cua bang bam

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
    bool remove(const char *username);
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
    void freeAll(AVLNode *p);
    void reset();
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
    int id;                    // so thu tu bai (#) RIENG cua tac gia
    int authorId;
    char content[200];
    int commentCount;          // dem binh luan rieng cua bai (c1, c2, ...)
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

// ====================== 6. THONG BAO (HANG DOI FIFO) & TIN NHAN (DSLK DOI) ======================
typedef struct notif{
    char text[300];
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
    copyStr(n->text, t, 300);
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
    copyStr(out, t->text, 300);
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

// ====================== HAM BO SUNG CHO HASHTABLE / AVL ======================
bool HashTable::remove(const char *username){
    int h = hashFunc(username);
    HNode *prev = NULL;
    HNode *p = bucket[h];
    while(p != NULL && strcmp(p->u->username, username) != 0){
        prev = p;
        p = p->next;
    }
    if(p == NULL) return false;
    if(prev == NULL) bucket[h] = p->next;
    else prev->next = p->next;
    delete p;
    return true;
}

void AVLTree::freeAll(AVLNode *p){
    if(p == NULL) return;
    freeAll(p->left);
    freeAll(p->right);
    delete p;
}

void AVLTree::reset(){
    freeAll(root);
    root = NULL;
}

// ====================== SOCIAL NETWORK (lop dieu phoi) ======================
const int MAXSAVED = 5;    // so tai khoan duoc luu tren may (nhu Facebook)

typedef struct SocialNetwork{
    HashTable ht;
    AVLTree avl;
    Slist friends[MAXU];       // do thi vo huong (ban be)
    HashSet friendSet;
    PostList posts[MAXU];
    int postCounter[MAXU];     // dem bai viet RIENG cho tung tai khoan (#1, #2, ...)
    NotifQueue notifs[MAXU];
    Conv *convHead;
    int userCount;
    int saved[MAXSAVED];       // cac tai khoan da dang nhap
    int savedCount;

    void init();
    bool valid(int id);
    void notify(int to, const char *text);

    // 1. Nguoi dung
    int registerUser(const char *username, const char *password, const char *fullname, const char *bio);
    int login(const char *username, const char *password);
    void saveAccount(int id);
    void printProfile(User *u);
    void searchUser(const char *username);
    void rebuildAVL();
    int changeUsername(int id, const char *newName);
    int changePassword(int id, const char *oldPw, const char *newPw);
    bool changeBio(int id, const char *bio);
    bool changeFullname(int id, const char *fullname);

    // 2. Moi quan he
    bool addFriend(int a, int b);
    bool unfriend(int a, int b);
    void printFriends(int id);

    // 3. Bai viet
    int createPost(int author, const char *content);
    bool deletePost(int author, int postNo);
    Post *findPost(int authorId, int postNo);
    void printPost(Post *p, int viewer = 0);
    void printPostFull(Post *p, int viewer);
    void printUserPosts(int id, int viewer);

    // 4. Tuong tac
    bool likePost(int u, int authorId, int postNo);
    bool unlikePost(int u, int authorId, int postNo);
    void printLikes(int authorId, int postNo);
    bool addComment(int u, int authorId, int postNo, int parentId, const char *text);

    // 5. Ban chung & goi y
    void mutualFriends(int a, int b);
    int bfs(int s, int t);
    void suggestFriends(int u);

    // 6. Tin nhan & thong bao
    Conv *findConv(int a, int b, bool create);
    bool sendMessage(int from, int to, const char *text);
    void showAllConversations(int id);
    void showRecent(int a, int b, int n);
    void showNotifications(int id);
}SocialNetwork;

void SocialNetwork::init(){
    ht.init();
    avl.init();
    friendSet.init();
    for(int i = 0; i < MAXU; i++){
        friends[i].init();
        posts[i].init();
        notifs[i].init();
        postCounter[i] = 0;
        gUsers[i] = NULL;
    }
    convHead = NULL;
    userCount = 0;
    savedCount = 0;
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
    return u->id;
}

int SocialNetwork::login(const char *username, const char *password){
    char uname[30];
    toLowerStr(username, uname, 30);
    User *u = ht.find(uname);
    if(u == NULL || strcmp(u->password, password) != 0) return 0;
    return u->id;
}

// Luu tai khoan vua dang nhap len dau danh sach (khong trung lap)
void SocialNetwork::saveAccount(int id){
    int pos = -1;
    for(int i = 0; i < savedCount; i++){
        if(saved[i] == id) pos = i;
    }
    if(pos == -1){
        if(savedCount < MAXSAVED) savedCount++;
        pos = savedCount - 1;
    }
    for(int i = pos; i > 0; i--) saved[i] = saved[i - 1];
    saved[0] = id;
}

void SocialNetwork::printProfile(User *u){
    cout<<"  +----------------------------------------------------"<<endl;
    cout<<"  |  @"<<u->username<<"   (ID "<<u->id<<")"<<endl;
    cout<<"  |  Ho ten    : "<<u->fullname<<endl;
    cout<<"  |  Gioi thieu: "<<u->bio<<endl;
    cout<<"  |  Ban be: "<<friends[u->id].size<<"    Bai viet: "<<posts[u->id].size<<endl;
    cout<<"  +----------------------------------------------------"<<endl;
}

void SocialNetwork::searchUser(const char *username){
    char uname[30];
    toLowerStr(username, uname, 30);
    User *u = ht.find(uname);
    if(u != NULL) printProfile(u);
    else cout<<"  Khong tim thay nguoi dung '"<<uname<<"'"<<endl;
}

// Cay AVL khong co phep xoa, nen khi doi username ta dung lai cay tu dau
void SocialNetwork::rebuildAVL(){
    avl.reset();
    for(int i = 1; i <= userCount; i++){
        if(gUsers[i] != NULL) avl.add(gUsers[i]);
    }
}

// Tra ve 0 = thanh cong, 1 = khong hop le, 2 = da co nguoi dung
int SocialNetwork::changeUsername(int id, const char *newName){
    char nn[30];
    toLowerStr(newName, nn, 30);
    if(strlen(nn) == 0 || strlen(newName) >= 30) return 1;
    if(strcmp(nn, gUsers[id]->username) == 0) return 0;
    if(ht.find(nn) != NULL) return 2;
    ht.remove(gUsers[id]->username);       // xoa khoi bang bam bang ten cu
    copyStr(gUsers[id]->username, nn, 30);
    ht.insert(gUsers[id]);                 // them lai bang ten moi
    rebuildAVL();
    return 0;
}

// Tra ve 0 = thanh cong, 1 = sai mat khau cu, 2 = mat khau moi rong
int SocialNetwork::changePassword(int id, const char *oldPw, const char *newPw){
    if(strcmp(gUsers[id]->password, oldPw) != 0) return 1;
    if(strlen(newPw) == 0) return 2;
    copyStr(gUsers[id]->password, newPw, 30);
    return 0;
}

bool SocialNetwork::changeBio(int id, const char *bio){
    if(strlen(bio) == 0) return false;
    copyStr(gUsers[id]->bio, bio, 100);
    return true;
}

bool SocialNetwork::changeFullname(int id, const char *fullname){
    if(strlen(fullname) == 0) return false;
    copyStr(gUsers[id]->fullname, fullname, 50);
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
    char buf[300];
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

void SocialNetwork::printFriends(int id){
    cout<<"  Ban be cua "<<nameOf(id)<<" ("<<friends[id].size<<"): ";
    if(friends[id].size == 0) cout<<"(chua co ban be)";
    friends[id].print();
    cout<<endl;
}

// ---------- 3. BAI VIET (danh so rieng cho tung tai khoan) ----------
int SocialNetwork::createPost(int author, const char *content){
    if(!valid(author) || strlen(content) == 0) return 0;
    Post *p = new Post();
    p->id = ++postCounter[author];         // #1, #2, ... cua rieng tac gia
    p->authorId = author;
    copyStr(p->content, content, 200);
    p->commentCount = 0;
    p->likes.init();
    p->comments = new Comment();           // nut goc gia, id = 0
    p->comments->id = 0;
    p->prev = p->next = NULL;
    posts[author].addFirst(p);             // bai moi nhat o dau danh sach
    return p->id;
}

bool SocialNetwork::deletePost(int author, int postNo){
    if(!valid(author)) return false;
    return posts[author].removeById(postNo);
}

Post *SocialNetwork::findPost(int authorId, int postNo){
    if(!valid(authorId)) return NULL;
    return posts[authorId].findById(postNo);
}

void SocialNetwork::printPost(Post *p, int viewer){
    cout<<"  +-- Bai #"<<p->id<<"  |  "<<nameOf(p->authorId)<<endl;
    cout<<"  |   "<<p->content<<endl;
    cout<<"  |   Like: "<<p->likes.size<<"    Binh luan: "<<countComments(p->comments->child);
    if(viewer != 0 && p->likes.contains(viewer)) cout<<"    [BAN DA LIKE]";
    cout<<endl;
    cout<<"  +----------------------------------------------------"<<endl;
}

// Bai viet kem toan bo cay binh luan
void SocialNetwork::printPostFull(Post *p, int viewer){
    printPost(p, viewer);
    if(p->comments->child == NULL) cout<<"  (chua co binh luan)"<<endl;
    else printComments(p->comments->child, 1);
}

void SocialNetwork::printUserPosts(int id, int viewer){
    cout<<"  Bai viet cua "<<nameOf(id)<<" ("<<posts[id].size<<" bai, moi nhat truoc):"<<endl<<endl;
    if(posts[id].size == 0) cout<<"  (chua co bai viet)"<<endl;
    for(Post *p = posts[id].head; p != NULL; p = p->next){
        printPostFull(p, viewer);
        cout<<endl;
    }
}

// ---------- 4. TUONG TAC ----------
bool SocialNetwork::likePost(int u, int authorId, int postNo){
    Post *p = findPost(authorId, postNo);
    if(p == NULL || !valid(u)) return false;
    if(p->likes.contains(u)) return false;
    p->likes.addLast(u);
    if(p->authorId != u){
        char buf[300];
        sprintf(buf, "%s da thich bai viet #%d cua ban: \"%.60s\"", nameOf(u), postNo, p->content);
        notify(p->authorId, buf);
    }
    return true;
}

bool SocialNetwork::unlikePost(int u, int authorId, int postNo){
    Post *p = findPost(authorId, postNo);
    if(p == NULL) return false;
    return p->likes.removeValue(u);
}

void SocialNetwork::printLikes(int authorId, int postNo){
    Post *p = findPost(authorId, postNo);
    if(p == NULL){
        cout<<"  Khong tim thay bai viet"<<endl;
        return;
    }
    printPost(p);
    cout<<"  Nhung nguoi da like ("<<p->likes.size<<"): ";
    if(p->likes.size == 0) cout<<"(chua co ai)";
    p->likes.print();
    cout<<endl;
}

bool SocialNetwork::addComment(int u, int authorId, int postNo, int parentId, const char *text){
    Post *p = findPost(authorId, postNo);
    if(p == NULL || !valid(u) || strlen(text) == 0) return false;
    Comment *parent = (parentId == 0) ? p->comments : findComment(p->comments, parentId);
    if(parent == NULL) return false;
    Comment *c = new Comment();
    c->id = ++p->commentCount;             // binh luan danh so rieng trong tung bai
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
        char buf[300];
        sprintf(buf, "%s da binh luan bai viet #%d (\"%.40s\") cua ban: \"%.100s\"",
                nameOf(u), postNo, p->content, c->content);
        notify(p->authorId, buf);
    }
    return true;
}

// ---------- 5. BAN CHUNG & GOI Y ----------
void SocialNetwork::mutualFriends(int a, int b){
    int count = 0;
    cout<<"  Ban chung cua "<<nameOf(a)<<" va "<<nameOf(b)<<": ";
    for(Node *p = friends[a].head; p != NULL; p = p->next){
        if(friendSet.contains(p->data * MAXU + b)){
            if(count > 0) cout<<", ";
            cout<<nameOf(p->data);
            count++;
        }
    }
    if(count == 0) cout<<"(khong co)";
    cout<<endl;
}

// BFS: khoang cach ket noi ngan nhat giua s va t (-1 neu khong co duong)
// (Chuc nang van con trong code, chi bo phan hien thi tren menu)
int SocialNetwork::bfs(int s, int t){
    if(!valid(s) || !valid(t)) return -1;
    int dist[MAXU];
    for(int i = 0; i < MAXU; i++) dist[i] = -1;
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
                q.push(v);
            }
        }
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
    cout<<"  Goi y ket ban cho "<<nameOf(u)<<":"<<endl;
    int shown = 0;
    for(int r = 0; r < 5; r++){
        int best = -1;
        for(int i = 1; i < MAXU; i++){
            if(score[i] > 0 && (best == -1 || score[i] > score[best])) best = i;
        }
        if(best == -1) break;
        cout<<"    "<<nameOf(best)<<" ("<<score[best]<<" ban chung)"<<endl;
        score[best] = 0;
        shown++;
    }
    if(shown == 0) cout<<"    (chua co goi y)"<<endl;
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
    m->next = NULL;
    m->prev = c->tail;
    if(c->tail == NULL) c->head = m;
    else c->tail->next = m;
    c->tail = m;
    c->size++;
    char buf[300];
    sprintf(buf, "Ban co tin nhan moi tu %s", nameOf(from));
    notify(to, buf);
    return true;
}

// Hien tat ca cuoc tro chuyen cua tai khoan nay (khong can nhap username)
void SocialNetwork::showAllConversations(int id){
    int count = 0;
    for(Conv *c = convHead; c != NULL; c = c->next){
        if(c->a != id && c->b != id) continue;
        int other = (c->a == id) ? c->b : c->a;
        cout<<"  +-- Cuoc tro chuyen voi "<<nameOf(other)<<" ("<<c->size<<" tin nhan)"<<endl;
        for(Msg *m = c->head; m != NULL; m = m->next){
            cout<<"  |   "<<nameOf(m->from)<<": "<<m->text<<endl;
        }
        cout<<"  +----------------------------------------------------"<<endl<<endl;
        count++;
    }
    if(count == 0) cout<<"  (ban chua nhan tin voi ai)"<<endl;
}

void SocialNetwork::showRecent(int a, int b, int n){
    Conv *c = findConv(a, b, false);
    cout<<endl<<"  "<<n<<" tin nhan gan nhat (cuon nguoc tu cuoi):"<<endl<<endl;
    if(c == NULL){
        cout<<"  (chua co tin nhan)"<<endl;
        return;
    }
    int i = 0;
    for(Msg *m = c->tail; m != NULL && i < n; m = m->prev, i++){
        cout<<"  "<<nameOf(m->from)<<": "<<m->text<<endl;
    }
}

void SocialNetwork::showNotifications(int id){
    cout<<"  Thong bao cua "<<nameOf(id)<<":"<<endl<<endl;
    if(notifs[id].isempty()){
        cout<<"  (khong co thong bao moi)"<<endl;
        return;
    }
    char buf[300];
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
    rowTwo("  9  Xem ban be", "");
    border('-');
    rowText("[3] BAI VIET & TUONG TAC");
    rowTwo("  10 Dang bai", "11 Xoa bai");
    rowTwo("  12 Xem bai & tuong tac", "13 Xem danh sach like");
    border('-');
    rowText("[4] BAN CHUNG & GOI Y");
    rowTwo("  14 Ban chung", "15 Goi y ket ban");
    border('-');
    rowText("[5] TIN NHAN & THONG BAO");
    rowTwo("  16 Gui tin nhan", "17 Lich su chat");
    rowTwo("  18 Xem thong bao", "");
    border('-');
    rowText("  0  Thoat chuong trinh");
    border('=');
}

void printTitle(int ch){
    const char *titles[19] = {
        "", "DANG KY NGUOI DUNG", "DANG NHAP", "TIM NGUOI DUNG", "CAP NHAT HO SO",
        "DANH SACH NGUOI DUNG (CAY AVL)", "HO SO CUA TOI", "KET BAN", "HUY KET BAN",
        "DANH SACH BAN BE", "DANG BAI MOI", "XOA BAI VIET", "XEM BAI VIET (LIKE & BINH LUAN)",
        "DANH SACH LIKE", "BAN CHUNG", "GOI Y KET BAN", "GUI TIN NHAN", "LICH SU CHAT",
        "THONG BAO (HANG DOI)"
    };
    char buf[100];
    cout<<endl;
    border('=');
    if(ch >= 1 && ch <= 18) sprintf(buf, "  >> %s", titles[ch]);
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

    sn.createPost(1, "Hom nay hoc cau truc du lieu that vui");   // dat #1
    sn.createPost(2, "Cuoi tuan di choi Da Lat");                 // giang #1
    sn.createPost(3, "On thi cau truc du lieu va giai thuat");    // duy #1
    sn.createPost(4, "Bat dau hoc C++");                          // long #1
    sn.createPost(3, "Da hoan thanh bai tap lon");                // duy #2

    sn.likePost(2, 1, 1);       // giang like bai dat #1
    sn.likePost(3, 1, 1);       // duy like bai dat #1
    sn.likePost(1, 3, 1);       // dat like bai duy #1
    sn.addComment(2, 1, 1, 0, "Chuc mung ban!");
    sn.addComment(1, 1, 1, 1, "Cam on Giang nhe");
    sn.addComment(3, 1, 1, 0, "Hoc cung nhau nhe");
    sn.sendMessage(1, 2, "Chao Giang, cuoi tuan di cafe khong?");
    sn.sendMessage(2, 1, "Duoc, 9h nhe!");
    sn.sendMessage(1, 2, "Ok hen gap");
}

// ====================== XU LY TUNG CHUC NANG ======================
bool gSkipPause = false;   // true = chuc nang tu quan ly man hinh, khong can "Nhan Enter"

// Hien thi danh sach nguoi dung de CHON BANG SO (khong can go username)
// mode 0: danh sach tron | mode 1: kem trang thai ban be | mode 2: kem so bai viet
// Tra ve id nguoi duoc chon, hoac 0 neu huy / chon sai
int pickUser(SocialNetwork &sn, int cur, bool withSelf, int mode, const char *header){
    cout<<"  "<<header<<endl<<endl;
    int shown = 0;
    for(int id = 1; id <= sn.userCount; id++){
        if(!sn.valid(id)) continue;
        if(id == cur && !withSelf) continue;
        char status[60];
        status[0] = '\0';
        if(mode == 1){
            if(id == cur) copyStr(status, "[CHINH BAN]", 60);
            else if(sn.friendSet.contains(cur * MAXU + id)) copyStr(status, "[DA LA BAN]", 60);
            else copyStr(status, "[CHUA LA BAN]", 60);
        }
        else if(mode == 2){
            sprintf(status, "(%d bai viet)%s", sn.posts[id].size, id == cur ? "  <- ban" : "");
        }
        printf("   [%2d] %-10s %-20s %s\n", id, gUsers[id]->username, gUsers[id]->fullname, status);
        shown++;
    }
    if(shown == 0){
        err("Chua co nguoi dung nao de chon");
        return 0;
    }
    cout<<endl;
    int pick = readInt("Chon so (0 = huy): ");
    if(pick == 0) return 0;
    if(!sn.valid(pick) || (pick == cur && !withSelf)){
        err("Lua chon khong hop le");
        return 0;
    }
    return pick;
}

// Xem bai viet cua 1 nguoi: hien tung bai (kem like, binh luan), chon bai de Like / Binh luan
void viewPostsFlow(SocialNetwork &sn, int cur, int author){
    char listMsg[120];
    char c[200];
    listMsg[0] = '\0';
    while(true){
        clearScreen();
        printTitle(12);
        sn.printUserPosts(author, cur);
        if(listMsg[0] != '\0'){
            cout<<endl<<"  "<<listMsg<<endl;
            listMsg[0] = '\0';
        }
        cout<<endl<<"  Nhap so bai viet (#) de Like / Binh luan  (0 = quay lai menu)"<<endl;
        int no = readInt("Chon bai viet: ");
        if(no == 0) break;
        Post *p = sn.findPost(author, no);
        if(p == NULL){
            copyStr(listMsg, "[!]  Khong co bai viet nay, hay chon lai", 120);
            continue;
        }
        char msg[200];
        msg[0] = '\0';
        while(true){
            clearScreen();
            printTitle(12);
            sn.printPostFull(p, cur);
            if(msg[0] != '\0'){
                cout<<endl<<"  "<<msg<<endl;
                msg[0] = '\0';
            }
            bool liked = (cur != 0 && p->likes.contains(cur));
            cout<<endl<<"  --- Thao tac voi bai #"<<p->id<<" cua "<<nameOf(author)<<" ---"<<endl;
            cout<<"  1. "<<(liked ? "Bo like (ban da like bai nay)" : "Like bai viet nay")<<endl;
            cout<<"  2. Binh luan / tra loi binh luan"<<endl;
            cout<<"  0. Quay lai danh sach bai viet"<<endl<<endl;
            int act = readInt("Chon: ");
            if(act == 0) break;
            if(act == 1){
                if(cur == 0) copyStr(msg, "[!]  Ban can dang nhap de like", 200);
                else if(liked){
                    sn.unlikePost(cur, author, no);
                    copyStr(msg, "[OK] Da bo like bai viet", 200);
                }
                else{
                    sn.likePost(cur, author, no);
                    copyStr(msg, "[OK] Da like bai viet", 200);
                }
            }
            else if(act == 2){
                if(cur == 0) copyStr(msg, "[!]  Ban can dang nhap de binh luan", 200);
                else{
                    int pid = readInt("Tra loi binh luan so (0 = binh luan goc): ");
                    readStr("Noi dung binh luan: ", c, 200);
                    if(sn.addComment(cur, author, no, pid, c)) copyStr(msg, "[OK] Da binh luan", 200);
                    else copyStr(msg, "[!]  That bai (sai so binh luan hoac noi dung rong)", 200);
                }
            }
            else copyStr(msg, "[!]  Lua chon khong hop le", 200);
        }
    }
    gSkipPause = true;
}

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
        // Co tai khoan da luu -> cho chon luon, khong can nhap lai
        if(sn.savedCount > 0){
            cout<<"  Tai khoan da luu tren may nay:"<<endl<<endl;
            for(int i = 0; i < sn.savedCount; i++){
                User *u = gUsers[sn.saved[i]];
                cout<<"    ["<<i + 1<<"] @"<<u->username<<"  ("<<u->fullname<<")";
                if(sn.saved[i] == cur) cout<<"  <- dang dung";
                cout<<endl;
            }
            cout<<"    [0] Dang nhap bang tai khoan khac"<<endl<<endl;
            int pick = readInt("Chon: ");
            if(pick >= 1 && pick <= sn.savedCount){
                cur = sn.saved[pick - 1];
                sn.saveAccount(cur);
                sprintf(d, "Da vao tai khoan %s (%d thong bao chua doc)", nameOf(cur), sn.notifs[cur].size);
                ok(d);
                return;
            }
            if(pick != 0){
                err("Lua chon khong hop le");
                return;
            }
            cout<<endl;
        }
        readStr("Username: ", a, 100);
        readStr("Mat khau: ", b, 100);
        int id = sn.login(a, b);
        if(id > 0){
            cur = id;
            sn.saveAccount(cur);
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
        cout<<"  1. Doi ten tai khoan (username)"<<endl;
        cout<<"  2. Doi mat khau"<<endl;
        cout<<"  3. Cap nhat gioi thieu"<<endl;
        cout<<"  4. Cap nhat ho ten"<<endl;
        cout<<"  0. Quay lai"<<endl<<endl;
        int sel = readInt("Chon: ");
        if(sel == 1){
            readStr("Ten tai khoan moi: ", a, 100);
            int r = sn.changeUsername(cur, a);
            if(r == 0){
                sprintf(d, "Ten tai khoan hien tai: @%s", nameOf(cur));
                ok(d);
            }
            else if(r == 1) err("Ten tai khoan khong hop le (rong hoac qua dai)");
            else err("Ten tai khoan da co nguoi su dung");
        }
        else if(sel == 2){
            readStr("Mat khau cu: ", a, 100);
            readStr("Mat khau moi: ", b, 100);
            int r = sn.changePassword(cur, a, b);
            if(r == 0) ok("Da doi mat khau");
            else if(r == 1) err("Mat khau cu khong dung");
            else err("Mat khau moi khong duoc de trong");
        }
        else if(sel == 3){
            readStr("Gioi thieu moi: ", c, 200);
            if(sn.changeBio(cur, c)) ok("Da cap nhat gioi thieu");
            else err("Gioi thieu khong duoc de trong");
        }
        else if(sel == 4){
            readStr("Ho ten moi: ", c, 200);
            if(sn.changeFullname(cur, c)) ok("Da cap nhat ho ten");
            else err("Ho ten khong duoc de trong");
        }
        else if(sel != 0) err("Lua chon khong hop le");
        if(sel >= 1 && sel <= 4){
            cout<<endl;
            sn.printProfile(gUsers[cur]);
        }
    }
    else if(ch == 5){
        sn.avl.print();
    }
    else if(ch == 6){
        if(needLogin(cur)) return;
        sn.printProfile(gUsers[cur]);
    }
    else if(ch == 7 || ch == 8){
        if(needLogin(cur)) return;
        int t = pickUser(sn, cur, true, 1, ch == 7 ? "Chon nguoi ban muon KET BAN:" : "Chon nguoi ban muon HUY KET BAN:");
        if(t == 0) return;
        bool isFriend = sn.friendSet.contains(cur * MAXU + t);
        // Chi in ra 1 trong 3 trang thai: CHINH BAN / DA LA BAN / CHUA LA BAN
        if(t == cur){
            err("CHINH BAN: day la tai khoan cua ban, khong the thuc hien voi chinh minh");
        }
        else if(ch == 7){
            if(isFriend){
                sprintf(d, "DA LA BAN: ban va %s da la ban be roi", nameOf(t));
                err(d);
            }
            else{
                sn.addFriend(cur, t);
                sprintf(d, "Da ket ban voi %s (truoc do: CHUA LA BAN)", nameOf(t));
                ok(d);
            }
        }
        else{
            if(!isFriend){
                sprintf(d, "CHUA LA BAN: ban va %s chua la ban be nen khong the huy", nameOf(t));
                err(d);
            }
            else{
                sn.unfriend(cur, t);
                sprintf(d, "Da huy ket ban voi %s", nameOf(t));
                ok(d);
            }
        }
    }
    else if(ch == 9){
        if(needLogin(cur)) return;
        sn.printFriends(cur);
    }
    else if(ch == 10){
        if(needLogin(cur)) return;
        readStr("Noi dung bai viet: ", c, 200);
        int no = sn.createPost(cur, c);
        if(no > 0){
            sprintf(d, "Da dang bai #%d", no);
            ok(d);
        }
        else err("Noi dung rong");
    }
    else if(ch == 11){
        if(needLogin(cur)) return;
        if(sn.posts[cur].size == 0){
            err("Ban chua co bai viet nao de xoa");
            return;
        }
        sn.printUserPosts(cur, cur);
        int no = readInt("So bai viet can xoa (#, 0 = huy): ");
        if(no == 0) return;
        if(sn.deletePost(cur, no)) ok("Da xoa bai viet");
        else err("Khong tim thay bai viet cua ban");
    }
    else if(ch == 12){
        int author = pickUser(sn, cur, true, 2, "Xem bai viet cua ai?");
        if(author == 0) return;
        if(sn.posts[author].size == 0){
            err("Nguoi nay chua co bai viet nao");
            return;
        }
        viewPostsFlow(sn, cur, author);
    }
    else if(ch == 13){
        int author = pickUser(sn, cur, true, 2, "Xem danh sach like bai viet cua ai?");
        if(author == 0) return;
        if(sn.posts[author].size == 0){
            err("Nguoi nay chua co bai viet nao");
            return;
        }
        cout<<endl;
        for(Post *p = sn.posts[author].head; p != NULL; p = p->next){
            sn.printPost(p, cur);
        }
        int no = readInt("Chon bai viet (#) de xem danh sach like (0 = huy): ");
        if(no == 0) return;
        Post *p = sn.findPost(author, no);
        if(p == NULL){
            err("Khong tim thay bai viet nay");
            return;
        }
        cout<<endl;
        sn.printLikes(author, no);
    }
    else if(ch == 14){
        if(needLogin(cur)) return;
        int t = pickUser(sn, cur, false, 0, "Tim ban chung voi ai?");
        if(t == 0) return;
        cout<<endl;
        sn.mutualFriends(cur, t);
    }
    else if(ch == 15){
        if(needLogin(cur)) return;
        sn.suggestFriends(cur);
    }
    else if(ch == 16){
        if(needLogin(cur)) return;
        int t = pickUser(sn, cur, false, 0, "Gui tin nhan cho ai?");
        if(t == 0) return;
        readStr("Noi dung tin nhan: ", c, 200);
        if(sn.sendMessage(cur, t, c)){
            sprintf(d, "Da gui tin nhan cho %s", nameOf(t));
            ok(d);
        }
        else err("Gui that bai (noi dung rong)");
    }
    else if(ch == 17){
        if(needLogin(cur)) return;
        sn.showAllConversations(cur);
    }
    else if(ch == 18){
        if(needLogin(cur)) return;
        sn.showNotifications(cur);
    }
    else{
        err("Lua chon khong hop le, hay nhap so tu 0 den 18");
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
        gSkipPause = false;
        handle(sn, cur, ch);
        if(!gSkipPause) pauseScreen();
    }
    cout<<endl<<"  Tam biet!"<<endl;
    return 0;
}
