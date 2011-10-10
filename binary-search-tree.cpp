/*
 * C++ Binary Search Tree implementation
 * Written by orestisp
 * std06176@di.uoa.gr
 */



#include <iostream>
#include <fstream>


template <class T>
class BST {
	private:
		struct node {
			T data;
			node *left;
			node *right;
			node(const T& d):
				data(d), left(0), right(0) {}
		};
		node *root;
		node **array;
		unsigned int size_var;
		void BST_clear(node*);
		void BST_copy(node*&, node*);
		void BST_to_array(node*, node***);
		void BST_from_array(node*&, unsigned int, unsigned int);
		void BST_print(node*) const;
		void BST_display(node*, std::ofstream&, unsigned int*) const;
	public:
		BST(void);
		BST(const BST&);
		~BST(void);
		bool empty(void) const;
		unsigned int size(void) const;
		BST<T>& clear(void);
		bool find(const T&) const;
		BST<T>& insert(const T&);
		BST<T>& extract(const T&);
		BST<T>& balance(void);
		BST<T>& print(void) const;
		void display(std::ofstream&) const;
};


template <class T>
void BST<T>::BST_clear(node* p) {
	if (p->left) BST_clear(p->left);
	if (p->right) BST_clear(p->right);
	delete p;
}


template <class T>
void BST<T>::BST_copy(node*& p, node* rp) {
	p = new node(rp->data);
	if (rp->left) BST_copy(p->left, rp->left);
	if (rp->right) BST_copy(p->right, rp->right);
}


template <class T>
void BST<T>::BST_to_array(node* p, node*** a) {
	if (p->left) BST_to_array(p->left, a);
	*(*a)++ = p;
	if (p->right) BST_to_array(p->right, a);
}


template <class T>
void BST<T>::BST_from_array(node*& p, unsigned int low, unsigned int high) {
	if (low > high) p = 0;
	else if (low == high) {
		p = array[low];
		p->left = p->right = 0;
	} else {
		unsigned int mid = low+high;
		if (mid >= 2) mid >>= 1;
		p = array[mid];
		BST_from_array(p->left, low, mid-1);
		BST_from_array(p->right, mid+1, high);
	}
}


template <class T>
void BST<T>::BST_print(node* p) const {
	if (p->left) BST_print(p->left);
	std::cout << p->data << ' ';
	if (p->right) BST_print(p->right);
}


template <class T>
void BST<T>::BST_display(node* p, std::ofstream& out, unsigned int *c) const {
	unsigned int n = (*c)++;
	out << "node" << n << "[label = \"<f0> |<f1> " << p->data << "|<f2> \"];\n";
	if (p->left) {
		out << "\"node" << n << "\":f0 -> \"node" << *c <<  "\":f1;\n";
		BST_display(p->left, out, c);
	}
	if (p->right) {
		out << "\"node" << n << "\":f2 -> \"node" << *c <<  "\":f1;\n";
		BST_display(p->right, out ,c);
	}
}


template <class T>
BST<T>::BST(void):
	root(0), size_var(0) {}


template <class T>
BST<T>::BST(const BST& param):
	root(0), size_var(param.size_var) {
	if (param.root) {
		try {
			BST_copy(root, param.root);
		} catch (...) {
			clear();
			throw;
		}
	}
}


template <class T>
BST<T>::~BST(void) {
	clear();
}


template <class T>
bool BST<T>::empty(void) const {
	return size_var == 0;
}


template <class T>
unsigned int BST<T>::size(void) const {
	return size_var;
}


template <class T>
BST<T>& BST<T>::clear(void) {
	if (root) BST_clear(root);
	root = 0;
	size_var = 0;
	return *this;
}


template <class T>
bool BST<T>::find(const T& d) const {
	node *p = root;
	while (p)
		if (d < p->data) 
			p = p->left;
		else if (!(d == p->data)) 
			p = p->right;
		else return true;
	return false;
}


template <class T>
BST<T>& BST<T>::insert(const T& d) {
	node **p = &root;
	while (*p)
		if (d < (*p)->data) 
			p = &((*p)->left);
		else if (!(d == (*p)->data)) 
			p = &((*p)->right);
		else return *this;
	*p = new node(d);
	size_var++;
	return *this;
}


template <class T>
BST<T>& BST<T>::extract(const T& d) {
	node *t, **p = &root;
	while (*p)
		if (d < (*p)->data) 
			p = &((*p)->left);
		else if (!(d == (*p)->data)) 
			p = &((*p)->right);
		else break;
	if (!*p) return *this;
	size_var--;
	if (!(*p)->left) {
		t = *p;
		*p = (*p)->right;
		delete t;
	} else if (!(*p)->right) {
		t = *p;
		*p = (*p)->left;
		delete t;
	} else {
		node **r = p;
		p = &((*p)->right);
		while ((*p)->left)
			p = &((*p)->left);
		t = *p;
		*p = (*p)->right;
		t->left = (*r)->left;
		t->right = (*r)->right;
		delete *r;
		*r = t;
	}
	return *this;
}


template <class T>
BST<T>& BST<T>::balance(void) {
	node **t;
	if (size_var <= 2) return *this;
	array = t = new node* [size_var];
	BST_to_array(root, &t);
	BST_from_array(root, 0, size_var-1);
	delete[] array;
	return *this;
}


template <class T>
BST<T>& BST<T>::print(void) const {
	if (root) BST_print(root);
	std::cout << std::endl;
	return *this;
}


template <class T>
void BST<T>::display(std::ofstream& out) const {
	unsigned int c = 0;
	out << "digraph G {\n";
	out << "node [shape = record,height=.1];\n";
	if (root) BST_display(root, out, &c);
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
	BST<int> tree;
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
	ofstream out("bst.dot");
	tree.display(out);
	out.close();
	system("dot bst.dot -Tpng -o bst.png");
	cout << "Created tree image at bst.png!" << endl;
//	tree.balance();
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
