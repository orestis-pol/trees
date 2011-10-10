/*
 * C++ AVL Tree implementation
 * Written by orestisp
 * std06176@di.uoa.gr
 */



#include <iostream>


template <class T>
class AVL {
	private:
		struct node {
			T data;
			int balance:2;
			node *left;
			node *right;
			node(const T& d, int b = 0):
				data(d), balance(b), left(0), right(0) {}
		};
		node *root;
		node ***pstack;
		bool *dstack;
		unsigned int size_var;
		inline void AVL_LL_rotate(node**);
		inline void AVL_RR_rotate(node**);
		inline void AVL_LR_rotate(node**);
		inline void AVL_RL_rotate(node**);
		void AVL_copy(node*&, node*);
		void AVL_print(node*) const;
	public:
		AVL(void);
		AVL(const AVL&);
		~AVL(void);
		bool empty(void) const;
		unsigned int size(void) const;
		AVL<T>& clear(void);
		bool find(const T&) const;
		AVL<T>& insert(const T&);
		AVL<T>& extract(const T&);
		void print(void) const;
};


template <class T>
inline void AVL<T>::AVL_LL_rotate(node** p) {
	node *t = *p;
	*p = t->left;
	t->left = (*p)->right;
	(*p)->right = t;
	t->balance = -(++(*p)->balance);
}


template <class T>
inline void AVL<T>::AVL_RR_rotate(node** p) {
	node *t = *p;
	*p = t->right;
	t->right = (*p)->left;
	(*p)->left = t;
	t->balance = -(--(*p)->balance);
}


template <class T>
inline void AVL<T>::AVL_LR_rotate(node** p) {
	node *t = *p, *l = t->left;
	*p = l->right;
	l->right = (*p)->left;
	t->left = (*p)->right;
	(*p)->right = t;
	(*p)->left = l;
	if ((*p)->balance != 1) {
		l->balance = 0;
		t->balance = -(*p)->balance;
	} else {
		l->balance = -1;
		t->balance = 0;
	}
	(*p)->balance = 0;
}


template <class T>
inline void AVL<T>::AVL_RL_rotate(node** p) {
	node *t = *p, *l = t->right;
	*p = l->left;
	l->left = (*p)->right;
	t->right = (*p)->left;
	(*p)->left = t;
	(*p)->right = l;
	if ((*p)->balance != -1) {
		l->balance = 0;
		t->balance = -(*p)->balance;
	} else {
		l->balance = 1;
		t->balance = 0;
	}
	(*p)->balance = 0;
}


template <class T>
void AVL<T>::AVL_copy(node*& p, node* rp) {
	p = new node(rp->data, rp->balance);
	if (rp->left) AVL_copy(p->left, rp->left);
	if (rp->right) AVL_copy(p->right, rp->right);
}


template <class T>
void AVL<T>::AVL_print(node* p) const {
	if (p->left) AVL_print(p->left);
	std::cout << p->data << ' ';
	if (p->right) AVL_print(p->right);
}


template <class T>
AVL<T>::AVL(void):
	root(0), size_var(0) {
	pstack = new node**[sizeof(unsigned int)*12];
	try {
		dstack = new bool[sizeof(unsigned int)*12];
	} catch (...) {
		delete[] pstack;
		throw;
	}
}


template <class T>
AVL<T>::AVL(const AVL& param):
	root(0), size_var(param.size_var) {
	pstack = new node**[sizeof(unsigned int)*12];
	try {
		dstack = new bool[sizeof(unsigned int)*12];
	} catch (...) {
		delete[] pstack;
		throw;
	}
	if (param.root) {
		try {
			AVL_copy(root, param.root);
		} catch (...) {
			delete[] pstack;
			delete[] dstack;
			clear();
			throw;
		}
	}
}


template <class T>
AVL<T>::~AVL(void) {
	delete[] dstack;
	clear();
	delete[] pstack;
}


template <class T>
bool AVL<T>::empty(void) const {
	return size_var == 0;
}


template <class T>
unsigned int AVL<T>::size(void) const {
	return size_var;
}


template <class T>
AVL<T>& AVL<T>::clear(void) {
	node ***s = pstack, **p;
	if (!root) return *this;
	*(++s) = &root;
	while (s != pstack) {
		p = *s;
		if ((*p)->left) *(++s) = &((*p)->left);
		else if ((*p)->right) *(++s) = &((*p)->right);
		else {
			delete *p;
			*p = 0;
			s--;
		}
	}
	root = 0;
	size_var = 0;
	return *this;
}


template <class T>
bool AVL<T>::find(const T& d) const {
	node *p = root;
	while (p)
		if (d < p->data) p = p->left;
		else if (!(d == p->data)) p = p->right;
		else return true;
	return false;
}


template <class T>
AVL<T>& AVL<T>::insert(const T& d) {
	node ***s = pstack, **p = &root;
	bool *b = dstack;
	while (*p) {
		*(++s) = p;
		if ((*(++b) = (d < (*p)->data)))
			p = &((*p)->left);
		else if (!(d == (*p)->data))
			p = &((*p)->right);
		else return *this;
	}
	*p = new node(d);
	size_var++;
	while (s != pstack) {
		p = *s;
		if (*b) {
			if ((*p)->balance == -1) {
				if ((*p)->left->balance != 1)
					AVL_LL_rotate(p);
				else
					AVL_LR_rotate(p);
				return *this;
			}
			if (!--(*p)->balance) return *this;
		} else {
			if ((*p)->balance == 1) {
				if ((*p)->right->balance != -1)
					AVL_RR_rotate(p);
				else 
					AVL_RL_rotate(p);
				return *this;
			}
			if (!++(*p)->balance) return *this;
		}
		b--;
		s--;
	}
	return *this;
}


template <class T>
AVL<T>& AVL<T>::extract(const T& d) {
	node ***s = pstack, **p = &root, *t;
	bool *b = dstack;
	while (*p) {
		*(++s) = p;
		if ((*(++b) = (d < (*p)->data)))
			p = &((*p)->left);
		else if (!(d == (*p)->data))
			p = &((*p)->right);
		else break;
	}
	if (!(*p)) return *this;
	size_var--;
	if (!(*p)->left) {
		t = *p;
		*p = (*p)->right;
		delete t;
		b--;
		s--;
	} else if (!(*p)->right) {
		t = *p;
		*p = (*p)->left;
		delete t;
		b--;
		s--;
	} else {
		node **r = p, ***w = s+1;
		p = &((*p)->right);
		while ((*p)->left) {
			*(++s) = p;
			*(++b) = true;
			p = &((*p)->left);
		}
		t = *p;
		*p = (*p)->right;
		t->balance = (*r)->balance;
		t->left = (*r)->left;
		t->right = (*r)->right;
		delete *r;
		*r = t;
		*w = &(t->right);
	}
	while (s != pstack) {
		p = *s;
		if (*b) {
			if ((*p)->balance == 1) {
				if ((*p)->right->balance != -1) {
					AVL_RR_rotate(p);
					if ((*p)->balance) return *this;
				} else AVL_RL_rotate(p);
			} else if (++(*p)->balance) return *this;
		} else {
			if ((*p)->balance == -1) {
				if ((*p)->left->balance != 1) {
					AVL_LL_rotate(p);
					if ((*p)->balance) return *this;
				} else AVL_LR_rotate(p);
			} else if (--(*p)->balance) return *this;
		}
		b--;
		s--;
	}
	return *this;
}


template <class T>
void AVL<T>::print(void) const {
	if (root) AVL_print(root);
	std::cout << std::endl;
}



/* Testing main */

#include <cstdlib>
#include <ctime>
using namespace std;


int main(int argc, char **argv)
{
	int i, j, n;
	double t;
	AVL<int> tree;
	if (argc > 3) return EXIT_FAILURE;
	i = time(0);
	if (argc == 1) n = 20;
	else {
		n = atoi(argv[1]);
		if (argc == 3) i = atoi(argv[2]);
	}
	srand((unsigned int)i);
	cout << "Size is " << n << endl;
	cout << "Seed is " << i << endl;
	cout << "Inserting..." << endl;
	t = ((double)clock())/CLOCKS_PER_SEC;
	for (i = 1 ; i <= n ; i++) {
		j = rand()%n+1;
	//	cout << j << ' ';
		tree.insert(j);
	}
	t = ((double)clock())/CLOCKS_PER_SEC-t;
	cout << t << " secs" << endl;
//	cout << "\nPrinting tree..." << endl;
//	tree.print();
	cout << "Size of tree is: " << tree.size() << endl;
	cout << "Extracting..." << endl;
	t = ((double)clock())/CLOCKS_PER_SEC;
	for (i = 1 ; i <= n ; i++) {
		j = rand()%n+1;
	//	cout << j << ' ';
		tree.extract(j);
	}
	t = ((double)clock())/CLOCKS_PER_SEC-t;
	cout << t << " secs" << endl;
//	cout << "\nPrinting tree..." << endl;
//	tree.print();
	cout << "Size of tree is: " << tree.size() << endl;
	cout << "Clearing..." << endl;
	t = ((double)clock())/CLOCKS_PER_SEC;
	tree.clear();
	t = ((double)clock())/CLOCKS_PER_SEC-t;
	cout << t << " secs" << endl;
	return EXIT_SUCCESS;
}
