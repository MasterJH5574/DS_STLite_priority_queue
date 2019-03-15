#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

template<typename T, class Compare = std::less<T> >
class priority_queue {
private:
    struct node {
        node *ls, *rs;
        T *data;
        int dis;

        node() {
            dis = -1;
            data = nullptr;
            ls = rs = nullptr;
        }

        node(const node &o) {
            dis = o.dis;
            data = new T(*o.data);
            ls = rs = nullptr;
        }

        node(const T &x, const int &d) {
            data = new T(x);
            dis = d;
            ls = rs = nullptr;
        }
    } *root;

    size_t size1;

    node* copy_construct(node* o) {
        if (o == nullptr)
            return nullptr;
        node *p = new node(*o);
        p->ls = copy_construct(o->ls);
        p->rs = copy_construct(o->rs);
        return p;
    }

    void deconstruct(node *o) {
        if (o == nullptr)
            return;
        deconstruct(o->ls);
        deconstruct(o->rs);
        delete o->data;
        delete o;
    }

    void swap(node *&x, node *&y) {
        node *p = x;
        x = y, y = p;
    }

    int get_dis(node *p) const {
        return p == nullptr ? -1 : p->dis;
    }

    node* merge_heap(node *x, node *y) {
        if (x == nullptr)
            return y;
        if (y == nullptr)
            return x;
        Compare cmp;
        if (cmp(*x->data, *y->data))
            swap(x, y);
        x->rs = merge_heap(x->rs, y);
        if (get_dis(x->ls) < get_dis(x->rs))
            swap(x->ls, x->rs);
        x->dis = x->ls != nullptr ? x->ls->dis + 1 : 0;
        return x;
    }

public:
	priority_queue() {
	    root = nullptr;
	    size1 = 0;
	}

	priority_queue(const priority_queue &o) {
	    root = copy_construct(o.root);
	    size1 = o.size1;
	}

	~priority_queue() {
	    deconstruct(root);
	}

	priority_queue &operator=(const priority_queue &o) {
        if (this == &o)
            return *this;
        deconstruct(root);
        root = copy_construct(o.root);
        size1 = o.size1;
        return *this;
	}

	// throw container_is_empty if empty() returns true;
	const T& top() const {
	    if (empty())
            throw container_is_empty();
	    return *root->data;
	}

	void push(const T &x) {
        node *p = new node(x, 0);
        root = merge_heap(root, p);
        size1++;
	}

	// throw container_is_empty if empty() returns true;
	void pop() {
        if (empty())
            throw container_is_empty();
        node *l = root->ls;
        node *r = root->rs;
        delete root->data;
        delete root;

        root = merge_heap(l, r);
        size1--;
	}

	size_t size() const {
        return size1;
	}

	bool empty() const {
        return size1 == 0;
	}

	void merge(priority_queue &o) {
	    //priority_queue o = O;
	    root = merge_heap(root, o.root);
	    size1 += o.size1;
	    o.size1 = 0;
	    o.root = nullptr;
	}
};

}

#endif
