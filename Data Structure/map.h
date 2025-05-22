//
// Created by JaneZ on 2025/5/6.
//

#ifndef MAP_H
#define MAP_H
#include "../Data Structure/exceptions.h"
#include "../Data Structure/utility.h"

namespace sjtu {
    template<class Key, class T, class Compare = std::less<Key> >
    class map {
    public:
        typedef pair<const Key, T> value_type;

        enum TreeType { LEFT, RIGHT };

        class iterator;
        class const_iterator;

    private:
        struct AVLnode {
            value_type data;
            AVLnode *left;
            AVLnode *right;
            AVLnode *parent;
            int height;

            AVLnode(const value_type &element, AVLnode *l = nullptr, AVLnode *r = nullptr, AVLnode *p = nullptr,
                    int h = 1): data(element), left(l), right(r), parent(p), height(h) {
            }
        };

        AVLnode *root;
        size_t Size;

        AVLnode *buildTree(AVLnode *other, AVLnode *father = nullptr) {
            if (other == nullptr) {
                return nullptr;
            }
            AVLnode *newNode = new AVLnode(other->data, nullptr, nullptr, nullptr, other->height);
            if (other->left != nullptr) {
                newNode->left = buildTree(other->left, newNode);
            }
            if (other->right != nullptr) {
                newNode->right = buildTree(other->right, newNode);
            }
            newNode->parent = father;
            return newNode;
        }

        void Clear(AVLnode *&t) {
            if (t == nullptr) {
                return;
            }
            if (t->left != nullptr) {
                Clear(t->left);
            }
            if (t->right != nullptr) {
                Clear(t->right);
            }
            delete t;
        }

        //得到子树高度
        int Height(AVLnode *t) {
            if (t == nullptr) {
                return 0;
            }
            return t->height;
        }

        //四种旋转方式
        void LL(AVLnode *&t) {
            AVLnode *newRoot = t->left;
            t->left = newRoot->right;
            newRoot->right = t;
            newRoot->parent = t->parent;
            t->parent = newRoot;
            if (t->left != nullptr) {
                t->left->parent = t;
            }
            t->height = std::max(Height(t->left), Height(t->right)) + 1;
            newRoot->height = std::max(Height(t), Height(newRoot->left)) + 1;
            t = newRoot;
        }

        void RR(AVLnode *&t) {
            AVLnode *newRoot = t->right;
            t->right = newRoot->left;
            newRoot->left = t;
            newRoot->parent = t->parent;
            t->parent = newRoot;
            if (t->right != nullptr) {
                t->right->parent = t;
            }
            t->height = std::max(Height(t->left), Height(t->right)) + 1;
            newRoot->height = std::max(Height(t), Height(newRoot->right)) + 1;
            t = newRoot;
        }

        void LR(AVLnode *&t) {
            RR(t->left);
            LL(t);
        }

        void RL(AVLnode *&t) {
            LL(t->right);
            RR(t);
        }

        AVLnode *findKey(const Key &k) {
            AVLnode *r = root;
            while (r != nullptr) {
                if (Compare()(r->data.first, k)) {
                    r = r->right;
                } else if (Compare()(k, r->data.first)) {
                    r = r->left;
                } else {
                    break;
                }
            }
            return r;
        }

        const AVLnode *findKey(const Key &k) const {
            const AVLnode *r = root;
            while (r != nullptr) {
                if (Compare()(r->data.first, k)) {
                    r = r->right;
                } else if (Compare()(k, r->data.first)) {
                    r = r->left;
                } else {
                    break;
                }
            }
            return r;
        }

        AVLnode *insertElem(const value_type &elem, AVLnode *&t, AVLnode *&father, TreeType a, bool &isInsert) {
            AVLnode *pos = nullptr;
            if (t == nullptr) {
                t = new AVLnode(elem, nullptr, nullptr, father);
                if (father != nullptr) {
                    if (a == LEFT) {
                        father->left = t;
                    } else {
                        father->right = t;
                    }
                }
                Size++;
                isInsert = true;
                pos = t;
            } else if (Compare()(t->data.first, elem.first)) {
                pos = insertElem(elem, t->right, t, RIGHT, isInsert);
                if (Height(t->right) - Height(t->left) == 2) {
                    if (Compare()(t->right->data.first, elem.first)) {
                        RR(t);
                    } else {
                        RL(t);
                    }
                }
            } else if (Compare()(elem.first, t->data.first)) {
                pos = insertElem(elem, t->left, t, LEFT, isInsert);
                if (Height(t->left) - Height(t->right) == 2) {
                    if (Compare()(t->left->data.first, elem.first)) {
                        LR(t);
                    } else {
                        LL(t);
                    }
                }
            } else {
                isInsert = false;
                pos = t;
            }
            t->height = std::max(Height(t->left), Height(t->right)) + 1;

            return pos;
        }

        bool removeElem(const Key &k, AVLnode *&t) {
            if (t == nullptr) {
                return true;
            }
            if (!Compare()(t->data.first, k) && !Compare()(k, t->data.first)) {
                if (t->left == nullptr && t->right == nullptr) {
                    Size--;
                    if (t->parent != nullptr) {
                        AVLnode *p = t->parent;
                        if (t->parent->left == t) {
                            delete t;
                            p->left = nullptr;

                            t = nullptr;
                        } else {
                            delete t;
                            p->right = nullptr;
                            t = nullptr;
                        }
                        return false;
                    } else {
                        delete t;
                        t = nullptr;
                        return true;
                    }
                } else if (t->left == nullptr) {
                    Size--;
                    AVLnode *oldNode = t;
                    AVLnode *f = t->parent;
                    t = t->right;
                    t->parent = f;
                    delete oldNode;
                    oldNode = nullptr;
                    return false;
                } else if (t->right == nullptr) {
                    Size--;
                    AVLnode *oldNode = t;
                    AVLnode *f = t->parent;
                    t = t->left;
                    t->parent = f;
                    delete oldNode;
                    oldNode = nullptr;
                    return false;
                } else {
                    int H = Height(t);
                    AVLnode *tmp = t->right;
                    bool flag = false;
                    while (tmp->left != nullptr) {
                        tmp = tmp->left;
                        flag = true;
                    }
                    if (flag) {
                        AVLnode *oldTmpRight = tmp->right;
                        if (tmp->parent->left == tmp) {
                            tmp->parent->left = t;
                        } else {
                            tmp->parent->right = t;
                        }
                        AVLnode *oldTmpParent = tmp->parent;
                        tmp->parent = t->parent;
                        t->parent = oldTmpParent;
                        tmp->left = t->left;
                        tmp->right = t->right;
                        t->left->parent = tmp;
                        t->right->parent = tmp;
                        //至此tmp已经全部替换为t
                        t->right = oldTmpRight;
                        t->left = nullptr;
                        if (oldTmpRight != nullptr) {
                            t->right->parent = t;
                        }
                    } else {
                        AVLnode *oldTmp = tmp->right;
                        tmp->parent = t->parent;
                        if (tmp->right != nullptr) {
                            tmp->right->parent = t;
                        }
                        tmp->left = t->left;
                        tmp->left->parent = tmp;
                        tmp->right = t;
                        t->parent = tmp;
                        t->left = nullptr;
                        t->right = oldTmp;
                    }
                    t->height = tmp->height;
                    tmp->height = H;
                    t = tmp;
                    if (removeElem(k, t->right)) {
                        return true;
                    }
                    return adjust(t, RIGHT);
                }
            } else if (Compare()(t->data.first, k)) {
                if (removeElem(k, t->right)) {
                    return true;
                }
                return adjust(t, RIGHT);
            } else {
                if (removeElem(k, t->left)) {
                    return true;
                }
                return adjust(t, LEFT);
            }
        }

        bool adjust(AVLnode *&t, TreeType a) {
            if (a == LEFT) {
                if (Height(t->right) - Height(t->left) == 1) {
                    return true;
                }
                if (Height(t->left) == Height(t->right)) {
                    t->height--;
                    return false;
                }
                if (Height(t->right->left) > Height(t->right->right)) {
                    RL(t);
                    return false;
                }
                RR(t);
                if (Height(t->left) == Height(t->right)) {
                    return false;
                }
                return true;
            } else {
                if (Height(t->left) - Height(t->right) == 1) {
                    return true;
                }
                if (Height(t->left) == Height(t->right)) {
                    t->height--;
                    return false;
                }
                if (Height(t->left->right) > Height(t->left->left)) {
                    LR(t);
                    return false;
                }
                LL(t);
                if (Height(t->left) == Height(t->right)) {
                    return false;
                }
                return true;
            }
        }

    public:
        class iterator {
            friend class map;
            friend class const_iterator;

        private:
            map *AVL;
            AVLnode *p;

        public:
            iterator(): AVL(nullptr), p(nullptr) {
            }

            iterator(const iterator &other): AVL(other.AVL), p(other.p) {
            }

            iterator(map *m, AVLnode *a): AVL(m), p(a) {
            }

            iterator operator++(int) {
                iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            iterator &operator++() {
                if (p == nullptr) {
                    throw invalid_iterator();
                }
                if (p->right != nullptr) {
                    p = p->right;
                    while (p->left != nullptr) {
                        p = p->left;
                    }
                } else {
                    AVLnode *father = p->parent;
                    while (father != nullptr && p == father->right) {
                        p = father;
                        father = father->parent;
                    }
                    p = father;
                }
                return *this;
            }

            iterator operator--(int) {
                iterator tmp = *this;
                --(*this);
                return tmp;
            }

            iterator &operator--() {
                if (p == nullptr) {
                    p = AVL->root;
                    if (p == nullptr) {
                        throw invalid_iterator();
                    }
                    while (p->right != nullptr) {
                        p = p->right;
                    }
                } else if (p->left != nullptr) {
                    p = p->left;
                    while (p->right != nullptr) {
                        p = p->right;
                    }
                } else {
                    AVLnode *father = p->parent;
                    while (father != nullptr && p == father->left) {
                        p = father;
                        father = father->parent;
                    }
                    p = father;
                }
                return *this;
            }

            value_type &operator*() const {
                if (p == nullptr) {
                    throw invalid_iterator();
                }
                return p->data;
            }

            bool operator==(const iterator &rhs) const {
                return AVL == rhs.AVL && p == rhs.p;
            }

            bool operator==(const const_iterator &rhs) const {
                return AVL == rhs.AVLconst && p == rhs.p_const;
            }

            bool operator!=(const iterator &rhs) const {
                if (*this == rhs) {
                    return false;
                } else {
                    return true;
                }
            }

            bool operator !=(const const_iterator &rhs) const {
                if (*this == rhs) {
                    return false;
                } else {
                    return true;
                }
            }

            value_type *operator->() const noexcept {
                if (p == nullptr) {
                    throw invalid_iterator();
                }
                return &(p->data);
            }
        };

        class const_iterator {
            friend class map;
            friend class iterator;

        private:
            const map *AVLconst;
            const AVLnode *p_const;

        public:
            const_iterator(): AVLconst(nullptr), p_const(nullptr) {
            }

            const_iterator(const const_iterator &other): AVLconst(other.AVLconst), p_const(other.p_const) {
            }

            const_iterator(const iterator &other): AVLconst(other.AVL), p_const(other.p) {
            }

            const_iterator(const map *m, const AVLnode *a): AVLconst(m), p_const(a) {
            }

            const_iterator operator++(int) {
                const_iterator temp = *this;
                ++(*this);
                return temp;
            }

            const_iterator &operator++() {
                if (p_const == nullptr) {
                    throw invalid_iterator();
                }
                if (p_const->right != nullptr) {
                    p_const = p_const->right;
                    while (p_const->left != nullptr) {
                        p_const = p_const->left;
                    }
                } else {
                    const AVLnode *parent = p_const->parent;
                    while (parent != nullptr && p_const == parent->right) {
                        p_const = parent;
                        parent = parent->parent;
                    }
                    p_const = parent;
                }
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator temp = *this;
                --(*this);
                return temp;
            }

            const_iterator &operator--() {
                if (p_const == nullptr) {
                    p_const = AVLconst->root;
                    if (p_const == nullptr) {
                        throw invalid_iterator();
                    }
                    while (p_const->right != nullptr) {
                        p_const = p_const->right;
                    }
                } else if (p_const->left != nullptr) {
                    p_const = p_const->left;
                    while (p_const->right != nullptr) {
                        p_const = p_const->right;
                    }
                } else {
                    const AVLnode *parent = p_const->parent;
                    while (parent != nullptr && p_const == parent->left) {
                        p_const = parent;
                        parent = parent->parent;
                    }
                    p_const = parent;
                }
                return *this;
            }

            bool operator==(const const_iterator &rhs) const {
                return AVLconst == rhs.AVLconst && p_const == rhs.p_const;
            }

            bool operator==(const iterator &rhs) const {
                return AVLconst == rhs.AVL && p_const == rhs.p;
            }

            bool operator!=(const const_iterator &rhs) const {
                if (*this == rhs) {
                    return false;
                } else {
                    return true;
                }
            }

            bool operator!=(const iterator &rhs) const {
                if (*this == rhs) {
                    return false;
                } else {
                    return true;
                }
            }

            const value_type &operator*() const {
                if (p_const == nullptr) {
                    throw invalid_iterator();
                }
                return p_const->data;
            }

            const value_type *operator->() const noexcept {
                if (p_const == nullptr) {
                    throw invalid_iterator();
                }
                return &(p_const->data);
            }
        };

        map() {
            root = nullptr;
            Size = 0;
        }

        map(const map &other) {
            if (other.root == nullptr) {
                root = nullptr;
                Size = 0;
            } else {
                root = buildTree(other.root, nullptr);
                Size = other.Size;
            }
        }

        map &operator=(const map &other) {
            if (this == &other) {
                return *this;
            }
            clear();
            if (other.root != nullptr) {
                root = buildTree(other.root, nullptr);
                Size = other.Size;
            } else {
                root = nullptr;
                Size = 0;
            }
            return *this;
        }

        ~map() {
            Clear(root);
            root = nullptr;
            Size = 0;
        }

        T &at(const Key &key) {
            AVLnode *target = findKey(key);
            if (target == nullptr) {
                throw index_out_of_bound();
            }
            return target->data.second;
        }

        const T &at(const Key &key) const {
            const AVLnode *target = findKey(key);
            if (target == nullptr) {
                throw index_out_of_bound();
            }
            return target->data.second;
        }

        T &operator[](const Key &key) {
            AVLnode *target = findKey(key);
            if (target == nullptr) {
                value_type Pair = {key, T()};
                bool isinsert;
                AVLnode *father = nullptr;
                target = insertElem(Pair, root, father, LEFT, isinsert);
            }
            return target->data.second;
        }

        const T &operator[](const Key &key) const {
            return at(key);
        }

        iterator begin() {
            AVLnode *p = root;
            if (p != nullptr) {
                while (p->left != nullptr) {
                    p = p->left;
                }
            }
            return iterator(this, p);
        }

        const_iterator cbegin() const {
            const AVLnode *p = root;
            if (p != nullptr) {
                while (p->left != nullptr) {
                    p = p->left;
                }
            }
            return const_iterator(this, p);
        }

        iterator end() {
            return iterator(this, nullptr);
        }

        const_iterator cend() const {
            return const_iterator(this, nullptr);
        }

        bool empty() const {
            return root == nullptr;
        }

        size_t size() const {
            return Size;
        }

        void clear() {
            Clear(root);
            root = nullptr;
            Size = 0;
        }

        pair<iterator, bool> insert(const value_type &value) {
            bool isinsert;
            AVLnode *target;
            AVLnode *father = nullptr;
            target = insertElem(value, root, father, LEFT, isinsert);
            pair<iterator, bool> Pair = {iterator(this, target), isinsert};
            return Pair;
        }

        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            if (pos.AVL != this) {
                throw invalid_iterator();
            }
            if (pos.p == nullptr) {
                throw invalid_iterator();
            }
            removeElem(pos.p->data.first, root);
        }

        size_t count(const Key &key) const {
            const AVLnode *p = findKey(key);
            if (p == nullptr) {
                return 0;
            } else {
                return 1;
            }
        }

        iterator find(const Key &key) {
            AVLnode *p = findKey(key);
            return iterator(this, p);
        }

        const_iterator find(const Key &key) const {
            const AVLnode *p = findKey(key);
            return const_iterator(this, p);
        }
    };
}
#endif //MAP_H
