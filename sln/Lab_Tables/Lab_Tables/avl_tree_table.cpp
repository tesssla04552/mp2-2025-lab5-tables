#include "avl_tree_table.h"
#include <queue>
#include <algorithm>
#include <iostream>
AVLTreeTable::AVLNode::AVLNode(const std::string& k, const Polynomials& p) : key(k), height(1), left(nullptr), right(nullptr) {
	chain.push_back(p);
}
AVLTreeTable::AVLTreeTable() : root(nullptr), ComparisonCount(0), RotationCount(0), ChainProbeCount(0) {}
AVLTreeTable::~AVLTreeTable() {
	Clear(root);
}
void AVLTreeTable::Clear(AVLNode* node) {
	if (node == nullptr) { return; }
	Clear(node->left);
	Clear(node->right);
	delete node;
}
int AVLTreeTable::Height(AVLNode* node) const {
	return (node == nullptr) ? 0 : node->height;
}
int AVLTreeTable::BalanceFactor(AVLNode* node) const {
	if (node == 0) { return 0; }
	return Height(node->left) - Height(node->right);
}
void AVLTreeTable::UpdateHeight(AVLNode* node) {
	if (node != nullptr) {
		node->height = 1 + std::max(Height(node->left), Height(node->right));
	}
}
AVLTreeTable::AVLNode* AVLTreeTable::RotateRight(AVLNode* y) {
	RotationCount++;
	AVLNode* x = y->left;
	AVLNode* T2 = x->right;
	x->right = y;
	y->left = T2;
	UpdateHeight(y);
	UpdateHeight(x);
	return x;
}
AVLTreeTable::AVLNode* AVLTreeTable::RotateLeft(AVLNode* x) {
	RotationCount++;
	AVLNode* y = x->right;
	AVLNode* T2 = y->left;
	y->left = x;
	x->right = T2;
	UpdateHeight(y);
	UpdateHeight(x);
	return y;
}
AVLTreeTable::AVLNode* AVLTreeTable::Balance(AVLNode* node) {
	if (node == nullptr) { return nullptr; }
	UpdateHeight(node);
	int bf = BalanceFactor(node);
	if (bf > 1) {
		if (BalanceFactor(node->left) >= 0) {
			return RotateRight(node);
		}
		else {
			node->left = RotateLeft(node->left);
			return RotateRight(node);
		}
	}
	if (bf < -1) {
		if (BalanceFactor(node->right) <= 0) {
			return RotateLeft(node);
		}
		else {
			node->right = RotateRight(node->right);
			return RotateLeft(node);
		}
	}
	return node;
}
AVLTreeTable::AVLNode* AVLTreeTable::Insert(AVLNode* node, const std::string& key, const Polynomials& value) {
	if (node == nullptr) { return new AVLNode(key, value); }
	ComparisonCount++;
	if (key < node->key) {
		node->left = Insert(node->left, key, value);
	}
	else if (key > node->key) {
		node->right = Insert(node->right, key, value);
	}
	else {
		node->chain.push_back(value);
		ChainProbeCount++;
		std::cout << "[AVLTree] Conflict: key '" << key << "' already exists. " << \
			"Added to the chain. Size of chain: " << node->chain.size() << std::endl;
		return node;
	}
	return Balance(node);
}
void AVLTreeTable::Insert(const std::string& key, const Polynomials& value) {
	root = Insert(root, key, value);
}
AVLTreeTable::AVLNode* AVLTreeTable::FindMin(AVLNode* node) {
	if (node == nullptr) { return nullptr; }
	while (node->left != nullptr) {
		node = node->left;
	}
	return node;
}
Polynomials* AVLTreeTable::Search(AVLNode* node, const std::string& key) {
	if (node == nullptr) { return nullptr; }
	ComparisonCount++;
	if (key < node->key) {
		return Search(node->left, key);
	}
	else if (key > node->key) {
		return Search(node->right, key);
	}
	else {
		if (node->chain.empty()) {
			return nullptr;
		}
		ChainProbeCount++;
		return &(node->chain.front());
	}
}
Polynomials* AVLTreeTable::Search(const std::string& key) {
	return Search(root, key);
}
AVLTreeTable::AVLNode* AVLTreeTable::remove(AVLNode* node, const std::string& key) {
	if (node == nullptr) {
		return nullptr;
	}
	ComparisonCount++;
	if (key < node->key) {
		node->left = remove(node->left, key);
	}
	else if (key > node->key) {
		node->right = remove(node->right, key);
	}
	else {
		if (node->chain.size() > 1) {
			ChainProbeCount++;
			node->chain.pop_front();
			std::cout << "[AVLTree] An item has been removed from the key chain '" << key << \
				"'. Left in the chain: " << node->chain.size() << std::endl;
			return node;
		}
		if (node->left == nullptr || node->right == nullptr) {
			AVLNode* tmp = (node->left != nullptr) ? node->left : node->right;
			if (tmp == nullptr) {
				delete node;
				return nullptr;
			}
			else {
				delete node;
				return tmp;
			}
		}
		AVLNode* successor = FindMin(node->right);
		node->key = successor->key;
		node->chain = successor->chain;
		node->right = remove(node->right, successor->key);
	}
	return Balance(node);
}
bool AVLTreeTable::Remove(const std::string& key) {
	if (Search(key) == nullptr) {
		return false;
	}
	root = remove(root, key);
	return true;
}
int AVLTreeTable::GetSize() const {
	int size = 0;
	std::queue<AVLNode*> q;
	if (root != nullptr) {
		q.push(root);
	}
	while (!q.empty()) {
		AVLNode* current = q.front();
		q.pop();
		size++;
		if (current->left != nullptr) { q.push(current->left); }
		if (current->right != nullptr) { q.push(current->right); }
	}
	return size;
}
void AVLTreeTable::PrintAll() const {
	if (root == nullptr) {
		std::cout << "[AVLTree] The tree is empty" << std::endl;
		return;
	}
	std::cout << "\n===== AVL TREE WITH CHAINS =====" << std::endl;
	std::queue<std::pair<AVLNode*, int>> q;
	q.push({ root, 0 });
	int currentLevel = 0;
	while (!q.empty()) {
		auto front = q.front();
		AVLNode* node = front.first;
		int level = front.second;
		q.pop();
		if (level > currentLevel) {
			std::cout << std::endl;
			currentLevel = level;
		}
		std::cout << " [" << node->key << " (h=" << node->height << ", chain:" << node->chain.size() << ")]";
		if (node->left != nullptr) { q.push({ node->left, level + 1 }); }
		if (node->right != nullptr) { q.push({ node->right, level + 1 }); }
	}
	std::cout << "\nHeight of tree: " << Height(root) << std::endl;
	std::cout << "Number of nodes: " << GetSize() << std::endl;
}