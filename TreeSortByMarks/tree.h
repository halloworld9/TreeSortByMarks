#pragma once
#include <cmath>
//Дерево доделано с шаблонами,
// но работать будет только с типами данных, у которых переопределенны операторы сравнения.

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

/// Функция удаления элемента из списка
/// @param root корневой элемент, определяющий дерево или поддерево
/// @param value значение, которое необходимо удалить
/// @return true, если значение было удалено и false в противном случае
template <typename T>
bool remove(node<T>*& root, T value)
{
	// Если корневой элемент отсутствует, удалять нечего, выходим. 
	if (!root) return false;
	// Если в корневой элемент содержит удаляемое значение
	if (root->val == value)
	{
		// Сам корневой элемент удалять нельзя, потому что в этом случае
		// структура дерева сильно исказится (будет большой дисбаланс)
		// Поэтому нужно найти элемент, как можно ниже
		if (!root->left && !root->right)
		{
			// Однако, если этот элемент - лист его можно удалить безболезненно, что и делаем
			delete root;
			root = nullptr;
		}
		else {
			// Начинаем искать самый большой элемент слева от удаляемого или самый маленький справа
			// от удаляемого
			// Для начала определим какое из поддеревьев выше и запомним полученное значение
			auto right_dir = get_tree_height(root->left) > get_tree_height(root->right);
			// Если левое поддерево выше, пойдем в ЛЕВОМ поддереве направо (right_dir = true)
			// Если правое поддерево выше, пойдем в ПРАВОМ поддереве налево (right_dir = false)
			auto& sub_tree = right_dir ? root->left : root->right;
			// Находим элемент в поддереве, из которого будем переносить значение в тот узел, где
			// сейчас находится удаляемое значение
			move_node(root, sub_tree, root, right_dir);
			// После выполнения функции в root, где раньше лежало удаляемое значение
			// будет находиться значение из одного из нижележащих узлов
			// Производим балансировку дерева, в том числе корректируем высоту корневого узла,
			// поскольку он мог измениться после удаления
			balance(root);
		}
		// Сообщаем о том, что элемент был удален из дерева
		return true;
	}
	// Если удаляемое значение не было еще найдено, определяем направление,
	// в котором его можно будет искать 
	auto& sub = value < root->val ? root->left : root->right;
	// И вызываем функцию удаления рекурсивно
	bool r = remove(sub, value);
	// Если удаление произведено (r = true), выполняем балансировку и коррекцию высоты
	if (r) balance(root);
	return r;
}

/// Функция перемещения элемента дерева
/// @param rem_el Ссылка на узел, куда будет перемещаться значение из реально удаляемого элемента
/// @param root Ссылка на корень, определяющий дерево или поддерево
/// @param parent Ссылка на узел, который является родительским для корневого элемента
/// @param right Направление движения по поддерево (true - вправо, если двигаемся в левом поддереве и false - влево, если двигаемся в правом поддереве)
template <typename T>
void move_node(node<T>* rem_el, node<T>* root, node<T>* parent, bool right)
{
	// Проверяем есть ли узел правее данного, если двигаемся вправо
	// или левее данного, если двигаемся влево
	if (right && !root->right || !right && !root->left)
	{
		// Дошли до максимального или минимального элемента в поддереве
		// У самого правого (левого) элемента могу быть дочерни элемента слева (справа)
		// Переносим ссылку на них в родительский узел относительно удаляемого (root) узла
		if (right) parent->right = root->left;
		else parent->left = root->right;
		// Переносим значения из удаляемого узла в тот узел, где находятся удаляемые значения
		rem_el->val = root->val;
		// Не забываем и про количество, которое также хранится в структуре
		rem_el->count = root->count;
		// Удаляем узел из дерева
		delete root;
		return;
	}
	// Если до самого правого (левого) узла еще не дошли, вызываем move_node рекурсивно
	move_node(rem_el, right ? root->right : root->left, root, right);
	// Проводим балансировку в дереве, поскольку высоты могли измениться после удаления.
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