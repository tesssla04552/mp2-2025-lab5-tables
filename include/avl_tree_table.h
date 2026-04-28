#ifndef AVL_TREE_TABLE
#define AVL_TREE_TABLE
#include "table_struct.h"
#include <list>
#include <string>
#include <algorithm>
class AVLTreeTable : public Table<std::string, Polynomials> {
private:
	struct AVLNode {
		std::string key;
		std::list<Polynomials> chain;
		int height;
		AVLNode* left;
		AVLNode* right;
		AVLNode(const std::string& k, const Polynomials& p);
	};
	AVLNode* root;
	int ComparisonCount;
	int RotationCount;
	int ChainProbeCount;
	int Height(AVLNode* node) const;
	int BalanceFactor(AVLNode* node) const;
	void UpdateHeight(AVLNode* node);
	AVLNode* RotateRight(AVLNode* y);
	AVLNode* RotateLeft(AVLNode* x);
	AVLNode* Balance(AVLNode* node);
	AVLNode* Insert(AVLNode* node, const std::string& key, const Polynomials& value);
	Polynomials* Search(AVLNode* node, const std::string& key);
	AVLNode* FindMin(AVLNode* node);
	AVLNode* remove(AVLNode* node, const std::string& key);
	void Clear(AVLNode* node);
public:
	AVLTreeTable();
	~AVLTreeTable();
	void Insert(const std::string& key, const Polynomials& value) override;
	Polynomials* Search(const std::string& key) override;
	bool Remove(const std::string& key) override;
	int GetComparisonCount() const override { return ComparisonCount; }
	int GetRotationCount() const override { return RotationCount; }
	int GetChainProbeCount() const override { return ChainProbeCount; }
	void ResetStats() override {
		ComparisonCount = 0;
		RotationCount = 0;
		ChainProbeCount = 0;
	}
	std::string GetName() const override { return "AVLTreeWithChains"; }
	int GetSize() const;
	bool IsEmpty() const override { return root == nullptr; }
	void PrintAll() const;
	int GetHeight() const { return Height(root); }
};
#endif
