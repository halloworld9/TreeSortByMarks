#pragma once
#include <cmath>
//������ �������� � ���������,
// �� �������� ����� ������ � ������ ������, � ������� ��������������� ��������� ���������.

template <typename T>
struct node
{
	T val;
	int count = 1;
	int height = 1;
	node<T>* left = nullptr;
	node<T>* right = nullptr;
};

template <typename T>
struct tree
{
	node<T>* root = nullptr;

};


template <typename T>
bool add(tree<T>& t, T value)
{
	return add(t.root, value);
}

template <typename T>
node<T>* get(tree<T> t, T value)
{
	return get(t.root, value);
}

template <typename T>
void remove(tree<T>& t, T value)
{
	remove(t.root, value);
}

/// ������� �������� �������� �� ������
/// @param root �������� �������, ������������ ������ ��� ���������
/// @param value ��������, ������� ���������� �������
/// @return true, ���� �������� ���� ������� � false � ��������� ������
template <typename T>
bool remove(node<T>*& root, T value)
{
	// ���� �������� ������� �����������, ������� ������, �������. 
	if (!root) return false;
	// ���� � �������� ������� �������� ��������� ��������
	if (root->val == value)
	{
		// ��� �������� ������� ������� ������, ������ ��� � ���� ������
		// ��������� ������ ������ ��������� (����� ������� ���������)
		// ������� ����� ����� �������, ��� ����� ����
		if (!root->left && !root->right)
		{
			// ������, ���� ���� ������� - ���� ��� ����� ������� �������������, ��� � ������
			delete root;
			root = nullptr;
		}
		else {
			// �������� ������ ����� ������� ������� ����� �� ���������� ��� ����� ��������� ������
			// �� ����������
			// ��� ������ ��������� ����� �� ����������� ���� � �������� ���������� ��������
			auto right_dir = get_tree_height(root->left) > get_tree_height(root->right);
			// ���� ����� ��������� ����, ������ � ����� ��������� ������� (right_dir = true)
			// ���� ������ ��������� ����, ������ � ������ ��������� ������ (right_dir = false)
			auto& sub_tree = right_dir ? root->left : root->right;
			// ������� ������� � ���������, �� �������� ����� ���������� �������� � ��� ����, ���
			// ������ ��������� ��������� ��������
			move_node(root, sub_tree, root, right_dir);
			// ����� ���������� ������� � root, ��� ������ ������ ��������� ��������
			// ����� ���������� �������� �� ������ �� ����������� �����
			// ���������� ������������ ������, � ��� ����� ������������ ������ ��������� ����,
			// ��������� �� ��� ���������� ����� ��������
			balance(root);
		}
		// �������� � ���, ��� ������� ��� ������ �� ������
		return true;
	}
	// ���� ��������� �������� �� ���� ��� �������, ���������� �����������,
	// � ������� ��� ����� ����� ������ 
	auto& sub = value < root->val ? root->left : root->right;
	// � �������� ������� �������� ����������
	bool r = remove(sub, value);
	// ���� �������� ����������� (r = true), ��������� ������������ � ��������� ������
	if (r) balance(root);
	return r;
}

/// ������� ����������� �������� ������
/// @param rem_el ������ �� ����, ���� ����� ������������ �������� �� ������� ���������� ��������
/// @param root ������ �� ������, ������������ ������ ��� ���������
/// @param parent ������ �� ����, ������� �������� ������������ ��� ��������� ��������
/// @param right ����������� �������� �� ��������� (true - ������, ���� ��������� � ����� ��������� � false - �����, ���� ��������� � ������ ���������)
template <typename T>
void move_node(node<T>* rem_el, node<T>* root, node<T>* parent, bool right)
{
	// ��������� ���� �� ���� ������ �������, ���� ��������� ������
	// ��� ����� �������, ���� ��������� �����
	if (right && !root->right || !right && !root->left)
	{
		// ����� �� ������������� ��� ������������ �������� � ���������
		// � ������ ������� (������) �������� ���� ���� ������� �������� ����� (������)
		// ��������� ������ �� ��� � ������������ ���� ������������ ���������� (root) ����
		if (right) parent->right = root->left;
		else parent->left = root->right;
		// ��������� �������� �� ���������� ���� � ��� ����, ��� ��������� ��������� ��������
		rem_el->val = root->val;
		// �� �������� � ��� ����������, ������� ����� �������� � ���������
		rem_el->count = root->count;
		// ������� ���� �� ������
		delete root;
		return;
	}
	// ���� �� ������ ������� (������) ���� ��� �� �����, �������� move_node ����������
	move_node(rem_el, right ? root->right : root->left, root, right);
	// �������� ������������ � ������, ��������� ������ ����� ���������� ����� ��������.
	balance(root);
}

template <typename T>
void drop(tree<T>& t)
{
	drop(t.root);
}

template <typename T>
void drop(node<T>*& root)
{
	if (root->left) drop(root->left);
	if (root->right) drop(root->right);
	delete root;
	root = nullptr;
}

template <typename T>
node<T>*& get(node<T>*& root, T value)
{
	if (!root || root->val == value) return root;
	return get(value < root->val ? root->left : root->right, value);
}

template <typename T>
bool add(node<T>*& root, T value)
{
	if (!root) {
		node<T>* new_node = new node<T>;
		new_node->val = value;
		root = new_node;
		return true;
	}
	if (root->val == value)
	{
		root->count++;
		return false;
	}
	const bool res = add(value < root->val ? root->left : root->right, value);
	balance(root);
	root->height = get_tree_height(root);
	return res;
}

template <typename T>
int get_tree_height(const node<T>* root)
{
	if (!root) return 0;
	const auto lh = root->left ? root->left->height : 0;
	const auto rh = root->right ? root->right->height : 0;
	return 1 + (lh > rh ? lh : rh);
}
template <typename T>
void small_left_rotate(node<T>*& root)
{
	node<T>* new_root = root->right;
	root->right = new_root->left;
	new_root->left = root;
	root->height = get_tree_height(root);
	root = new_root;
	root->height = get_tree_height(root);
}

template <typename T>
void small_right_rotate(node<T>*& root)
{
	node<T>* new_root = root->left;
	root->left = new_root->right;
	new_root->right = root;
	root->height = get_tree_height(root);
	root = new_root;
	root->height = get_tree_height(root);
}

template <typename T>
void large_left_rotate(node<T>*& root)
{
	small_right_rotate(root->right);
	small_left_rotate(root);
}

template <typename T>
void large_right_rotate(node<T>*& root)
{
	small_left_rotate(root->left);
	small_right_rotate(root);
}

template <typename T>
void balance(node<T>*& root)
{
	auto lh = get_tree_height(root->left);
	auto rh = get_tree_height(root->right);
	if (abs(lh - rh) >= 2)
	{
		if (lh > rh)
		{
			if (get_tree_height(root->left->left) > get_tree_height(root->left->right))
				small_right_rotate(root);
			else
				large_right_rotate(root);
		}
		else
		{
			if (get_tree_height(root->right->right) > get_tree_height(root->right->left))
				small_left_rotate(root);
			else
				large_left_rotate(root);
		}
	}
	else
	{
		root->height = get_tree_height(root);
	}
}