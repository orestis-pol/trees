/*
 * C++ AVL Tree implementation
 * Written by orestisp
 * std06176@di.uoa.gr
 */



#include <iostream>
#include <fstream>

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
		node *tnode;
		const T* tdata;
		unsigned int size_var;
		void AVL_clear(node*);
		inline void AVL_LL_rotate(node*&);
		inline void AVL_RR_rotate(node*&);
		inline void AVL_LR_rotate(node*&);
		inline void AVL_RL_rotate(node*&);
		bool AVL_insert(node*&);
		bool AVL_delete(node*&);
		bool AVL_delmin(node*&);
		void AVL_copy(node*&, node*);
		void AVL_print(node*) const;
		void AVL_display(node*, std::ofstream&, unsigned int*) const;
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
		AVL<T>& print(void) const;
		void display(std::ofstream&) const;
};


template <class T>
void AVL<T>::AVL_clear(node* p) {
	if (p->left) AVL_clear(p->left);
	if (p->right) AVL_clear(p->right);
	delete p;
}


template <class T>
inline void AVL<T>::AVL_LL_rotate(node*& p) {
	node *t = p;
	p = t->left;
	t->left = p->right;
	p->right = t;
	t->balance = -(++p->balance);
}


template <class T>
inline void AVL<T>::AVL_RR_rotate(node*& p) {
	node *t = p;
	p = t->right;
	t->right = p->left;
	p->left = t;
	t->balance = -(--p->balance);
}


template <class T>
inline void AVL<T>::AVL_LR_rotate(node*& p) {
	node *t = p, *l = t->left;
	p = l->right;
	l->right = p->left;
	t->left = p->right;
	p->right = t;
	p->left = l;
	if (p->balance != 1) {
		l->balance = 0;
		t->balance = -p->balance;
	} else {
		l->balance = -1;
		t->balance = 0;
	}
	p->balance = 0;
}


template <class T>
inline void AVL<T>::AVL_RL_rotate(node*& p) {
	node *t = p, *l = t->right;
	p = l->left;
	l->left = p->right;
	t->right = p->left;
	p->left = t;
	p->right = l;
	if (p->balance != -1) {
		l->balance = 0;
		t->balance = -p->balance;
	} else {
		l->balance = 1;
		t->balance = 0;
	}
	p->balance = 0;
}


template <class T>
bool AVL<T>::AVL_insert(node*& p) {
	if (!p) {
		p = new node(*tdata);
		size_var++;
		return true;
	}
	if (*tdata < p->data) {
		if (!AVL_insert(p->left))
			return false;
		if (p->balance != -1)
			return --p->balance;
		if (p->left->balance != 1)
			AVL_LL_rotate(p);
		else
			AVL_LR_rotate(p);
		return false;
	}
	if (p->data < *tdata) {
		if (!AVL_insert(p->right))
			return false;
		if (p->balance != 1)
			return ++p->balance;
		if (p->right->balance != -1)
			AVL_RR_rotate(p);
		else
			AVL_RL_rotate(p);
		return false;
	}
	return false;
}


template <class T>
bool AVL<T>::AVL_delete(node*& p) {
	if (!p) return false;
	if (*tdata < p->data) {
		if (!AVL_delete(p->left))
			return false;
		if (p->balance != 1)
			return !++p->balance;
		if (p->right->balance != -1) {
			AVL_RR_rotate(p);
			return !p->balance;
		}
		AVL_RL_rotate(p);
		return true;
	}
	if (p->data < *tdata) {
		if (!AVL_delete(p->right))
			return false;
		if (p->balance != -1)
			return !--p->balance;
		if (p->left->balance != 1) {
			AVL_LL_rotate(p);
			return !p->balance;
		}
		AVL_LR_rotate(p);
		return true;
	}
	size_var--;
	tnode = p;
	if (!p->left) p = p->right;
	else if (!p->right) p = p->left;
	else {
		bool r = AVL_delmin(p->right);
		tnode->balance = p->balance;
		tnode->left = p->left;
		tnode->right = p->right;
		delete p;
		p = tnode;
		if (!r) return false;
		if (p->balance != -1)
			return !--p->balance;
		if (p->left->balance != 1) {
			AVL_LL_rotate(p);
			return !p->balance;
		}
		AVL_LR_rotate(p);
		return true;
	}
	delete tnode;
	return true;
}


template <class T>
bool AVL<T>::AVL_delmin(node*& p) {
	if (p->left) {
		if (!AVL_delmin(p->left))
			return false;
		if (p->balance != 1)
			return !++p->balance;
		if (p->right->balance != -1) {
			AVL_RR_rotate(p);
			return !p->balance;
		}
		AVL_RL_rotate(p);
		return true;
	}
	tnode = p;
	p = p->right;
	return true;
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
void AVL<T>::AVL_display(node* p, std::ofstream& out, unsigned int *c) const {
	unsigned int n = (*c)++;
	out << "node" << n << "[label = \"<f0> |<f1> " << p->data << "|<f2> \"];\n";
	if (p->left) {
		out << "\"node" << n << "\":f0 -> \"node" << *c <<  "\":f1;\n";
		AVL_display(p->left, out, c);
	}
	if (p->right) {
		out << "\"node" << n << "\":f2 -> \"node" << *c <<  "\":f1;\n";
		AVL_display(p->right, out ,c);
	}
}


template <class T>
AVL<T>::AVL(void):
	root(0), size_var(0) {}


template <class T>
AVL<T>::AVL(const AVL& param):
	root(0), size_var(param.size_var) {
	if (param.root) {
		try {
			AVL_copy(root, param.root);
		} catch (...) {
			clear();
			throw;
		}
	}
}


template <class T>
AVL<T>::~AVL(void) {
	clear();
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
	if (root) AVL_clear(root);
	root = 0;
	size_var = 0;
	return *this;
}


template <class T>
bool AVL<T>::find(const T& d) const {
	node *p = root;
	while (p)
		if (d < p->data)
			p = p->left;
		else if (p->data < d)
			p = p->right;
		else return true;
	return false;
}


template <class T>
AVL<T>& AVL<T>::insert(const T& d) {
	tdata = &d;
	AVL_insert(root);
	return *this;
}


template <class T>
AVL<T>& AVL<T>::extract(const T& d) {
	tdata = &d;
	AVL_delete(root);
	return *this;
}


template <class T>
AVL<T>& AVL<T>::print(void) const {
	if (root) AVL_print(root);
	std::cout << std::endl;
	return *this;
}


template <class T>
void AVL<T>::display(std::ofstream& out) const {
	unsigned int c = 0;
	out << "digraph G {\n";
	out << "node [shape = record,height=.1];\n";
	if (root) AVL_display(root, out, &c);
	out << "}\n";
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
	ofstream out("avl.dot");
	tree.display(out);
	out.close();
	system("dot avl.dot -Tpng -o avl.png");
	cout << "Created tree image at avl.png!" << endl;
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
