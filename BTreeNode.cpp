#include "btreenode.h"
#include <iostream>

BTreeNode::BTreeNode(int _t, bool _leaf) {
	t = _t;
	leaf = _leaf;
	keys = new int[2 * t - 1];
	C = new BTreeNode *[2 * t];
	n = 0;
}

void BTreeNode::traverse() {
	int i;
	for (i = 0; i < n; i++) {
		if (!leaf) {
			C[i]->traverse();
		}
		std::cout << " " << keys[i];
	}
	if (!leaf) {
		C[i]->traverse();
	}
}

BTreeNode* BTreeNode::search(int k) {
	int i = 0;
	while (i < n  && k > keys[i]) {
		i++;
	}
	if (keys[i] == k) {
		return this;
	}
	if (leaf) {
		return NULL;
	}
	return C[i]->search(k);
}

int BTreeNode::findKey(int k)
{
	int idx = 0;
	while (idx<n && keys[idx] < k)
		++idx;
	return idx;
}
void BTreeNode::remove(int k)
{
	int idx = findKey(k);
	if (idx < n && keys[idx] == k)
	{
		if (leaf) {
			removeFromLeaf(idx);
		}
		else {
			removeFromNonLeaf(idx);
		}
	}
	else
	{
		if (leaf)
		{
			cout << "el numero " << k << " no existe en este arbolB\n";
			return;
		}
		bool flag = ((idx == n) ? true : false);
		if (C[idx]->n < t) {
			fill(idx);
		}
		if (flag && idx > n) {
			C[idx - 1]->remove(k);
		}
		else {
			C[idx]->remove(k);
		}
	}
}
void BTreeNode::removeFromLeaf(int idx){
	for (int i = idx + 1; i < n; ++i) {
		keys[i - 1] = keys[i];
	}
	n--;
}

void BTreeNode::removeFromNonLeaf(int idx)
{

	int k = keys[idx];

	if (C[idx]->n >= t)
	{
		int pred = getPred(idx);
		keys[idx] = pred;
		C[idx]->remove(pred);
	}

	else if (C[idx + 1]->n >= t)
	{
		int succ = getSucc(idx);
		keys[idx] = succ;
		C[idx + 1]->remove(succ);
	}

	else
	{
		merge(idx);
		C[idx]->remove(k);
	}
}

int BTreeNode::getPred(int idx)
{
	BTreeNode *cur = C[idx];
	while (!cur->leaf) {
		cur = cur->C[cur->n];
	}
	return cur->keys[cur->n - 1];
}

int BTreeNode::getSucc(int idx)
{
	BTreeNode *cur = C[idx + 1];
	while (!cur->leaf) {
		cur = cur->C[0];
	}
	return cur->keys[0];
}
void BTreeNode::fill(int idx)
{
	if (idx != 0 && C[idx - 1]->n >= t) {
		borrowFromPrev(idx);
	}
	else if (idx != n && C[idx + 1]->n >= t) {
		borrowFromNext(idx);
	}
	else
	{
		if (idx != n) {
			merge(idx);
		}
		else {
			merge(idx - 1);
		}
	}
}
void BTreeNode::borrowFromPrev(int idx)
{

	BTreeNode *child = C[idx];
	BTreeNode *sibling = C[idx - 1];

	for (int i = child->n - 1; i >= 0; --i) {
		child->keys[i + 1] = child->keys[i];
	}

	if (!child->leaf)
	{
		for (int i = child->n; i >= 0; --i) {
			child->C[i + 1] = child->C[i];
		}
	}

	child->keys[0] = keys[idx - 1];
	if (!leaf) {
		child->C[0] = sibling->C[sibling->n];
	}
	keys[idx - 1] = sibling->keys[sibling->n - 1];
	child->n += 1;
	sibling->n -= 1;
}
void BTreeNode::borrowFromNext(int idx)
{

	BTreeNode *child = C[idx];
	BTreeNode *sibling = C[idx + 1];

	child->keys[(child->n)] = keys[idx];

	if (!(child->leaf)) {
		child->C[(child->n) + 1] = sibling->C[0];
	}
	keys[idx] = sibling->keys[0];

	for (int i = 1; i < sibling->n; ++i) {
		sibling->keys[i - 1] = sibling->keys[i];
	}
	if (!sibling->leaf)
	{
		for (int i = 1; i <= sibling->n; ++i)
			sibling->C[i - 1] = sibling->C[i];
	}
	child->n += 1;
	sibling->n -= 1;
}

void BTreeNode::merge(int idx)
{
	BTreeNode *child = C[idx];
	BTreeNode *sibling = C[idx + 1];
	child->keys[t - 1] = keys[idx];
	for (int i = 0; i < sibling->n; ++i) {
		child->keys[i + t] = sibling->keys[i];
	}
	if (!child->leaf)
	{
		for (int i = 0; i <= sibling->n; ++i) {
			child->C[i + t] = sibling->C[i];
		}
	}
	for (int i = idx + 1; i < n; ++i) {
		keys[i - 1] = keys[i];
	}
	for (int i = idx + 2; i <= n; ++i) {
		C[i - 1] = C[i];
	}
	child->n += sibling->n + 1;
	n--;
	delete(sibling);
}
