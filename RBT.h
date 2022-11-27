#pragma once

template <typename T> class RBST;
template <typename T> class  node;

template <typename T>class node
{
public:

	node* left_; 
	node* right_; 
	short key; 
	bool color; 
	T value;
	node* parent_; 

	node(T val)
	{
		value = val;
		std::hash<T> hash;
		key = hash(val);
		left_ = NULL;
		right_ = NULL;
		color = true;

	}

private:
	void add(short hashed, T newVal, node* root)
	{
		if (hashed == root->key) {
			return;
		}
		else if (hashed < root->key) {
			if (root->left_ == nullptr) {
				root->left_ = new node(newVal);
				root->left_->parent_ = root;
			}
			else {
				add(hashed, newVal, root->left_);
			}
		}
		else {
			if (root->right_ == nullptr) {
				root->right_ = new node(newVal);
				root->right_->parent_ = root;
			}
			else {
				add(hashed, newVal, root->right_);
			}
		}

		if (root->right_ && root->left_) {
			if (root->right_->color && !root->left_->color) {
				root = rotL(root);
			}
			else if (root->left_->left_) {
				if (root->left_->color && root->left_->left_->color) {
					root = rotR(root);
				}
			}
			else if (root->left_->color && root->right_->color) {
				flipColors(root);
			}
		}
	}

	bool is_present(T w)
	{
		std::hash<T> want;
		int hashed = want(w);
		if (hashed == key) {
			return true;
		}
		if (hashed < key) {
			if (left_ != nullptr && left_->is_present(w)) {
				return true;
			}
		}
		else {
			if (right_ != nullptr && right_->is_present(w)) {
				return true;
			}
		}
		return false;
	}

	void remove(node*& root, short k)
	{
		node* del = root;

		while (del->key != k) {
			if (k < del->key) {
				del = del->left_;
			}
			else {
				del = del->right_;
			}
		}

		if (del->right_ == nullptr && del->left_ == nullptr) { //no children
			if (del != root) {
				if (del->parent_->left_ == del) {
					del->parent_->left_ = nullptr;
				}
				else {
					del->parent_->right_ = nullptr;
				}
			}
			else {
				root = nullptr;
			}
			std::cout << "\nDeleting " << del->value << std::endl;
			delete del;
		}
		else if (del->right_ && del->left_) { //2 children
			T min = right_->minimum();
			std::hash<T> hash;
			remove(root, hash(min));
			del->value = min;
			del->key = hash(min);

		}
		else { //1 child 
			node* child;
			if (del->left_) {
				child = del->left_;
			}
			else {
				child = del->right_;
			}
			if (del != root) {
				if (del->parent_->left_ == del) {
					del->parent_->left_ = child;
				}
				else {
					del->parent_->right_ = child;
				}
			}
			else {
				root = child;
			}
			std::cout << "\nDeleting " << del->value << std::endl;
			delete del;

		}
	}

	node* rotL(node*& p)
	{
		node* n = p->right_;
		node* pa = p->parent_;
		if (n != nullptr) {
			p->right_ = n->left_;
			n->left_ = p;
			p->parent_ = n;
			if (p->right_ != nullptr) {
				p->right_->parent_ = p;
			}
			if (pa != nullptr) {
				if (p == pa->left_) {
					pa->left_ = n;
				}
				else if (p == pa->right_) {
					pa->right_ = n;
				}
			}
			n->parent_ = pa;
		}
		return n;
	}

	node* rotR(node*& p)
	{
		node* n = p->left_;
		node* pa = n->parent_;
		if (n != nullptr) {
			p->left_ = n->right_;
			n->right_ = p;
			p->parent_ = n;
			if (p->left_ != nullptr) {
				p->left_->parent_ = p;
			}
			if (p != nullptr) {
				if (p == pa->left_) {
					pa->left_ = n;
				}
				else if (p == pa->right_) {
					pa->right_ = n;
				}
			}
			n->parent_ = pa;
		}
		return n;
	}

	void flipColors(node*& h)
	{
		if (!h->color && h->left_->color && h->right_->color) {
			h->color = true;
			h->left_->color = false;
			h->right_->color = false;
		}
	}

	T minimum()
	{
		if (left_ == nullptr) {
			return value;
		}
		else {
			return left_->minimum();
		}
	}

	void display()
	{
		if (left_ != nullptr) {
			left_->display();
		}
		std::cout << value << " ";
		if (right_ != nullptr) {
			right_->display();
		}
	}

	template <typename U> friend class  RBST;
};

template <typename T>
class RBST
{
public:
	RBST(T val)
		:root_(new node<T>(val)){}

	~RBST()
	{
		delete root_;
	}

	void insert(T val) {
		std::hash<T> hash;
		root_->add(hash(val), val, root_);
	}

	bool search(T val)
	{
		return root_->is_present(val);
	}

	void del(T val) {
		std::hash<T> hash;
		if (root_->is_present(val)) {
			root_->remove(root_, hash(val));
		}
	}

	node<T>* root() {
		return root_;
	}

	void print()
	{
		root_->display();
	}

	node<T>* root_;
	template <typename U> friend class  node;
};
