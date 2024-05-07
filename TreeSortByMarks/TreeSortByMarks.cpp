#include <iostream>
#include "tree.h"
#include <fstream>

using namespace std;

struct student {
	char* second_name = new char[1024];
	char* initials  = new char[1024];
	float average_mark;
};

tree<student> load(const char* file_name) {
	ifstream f(file_name);
	tree<student> tree;
	if (!f.is_open()) return tree;

	while (!f.eof()) {
		student s{};
		f >> s.second_name >> s.initials >> s.average_mark;
		add(tree, s);
	}
	return tree;
}

std::ostream& operator<<(std::ostream& o, const student& s)
{
	return o << "{second name: " << s.second_name << " average mark: " << s.average_mark << '}';
}

bool operator<(const student s1, const student s2) {
	return s1.average_mark < s2.average_mark;
}
bool operator>(const student s1, const student s2) {
	return s1.average_mark > s2.average_mark;
}
bool operator==(const student s1, const student s2) {
	return s1.average_mark == s2.average_mark;
}

template <typename T>
void show_tree(node<T>* root, T* compare) {
	if (!root) return;
	
	if (root->val > *compare) {
		cout << root->val << endl;
		show_tree(root->left, compare);
	}
	show_tree(root->right, compare);
}

template <typename T>
void show_tree(tree<T> tree, T* compare) {
	show_tree(tree.root, compare);
}


void main()
{
	cout << "Content-type: text/html; charset=windows-1251\n\n";

	ifstream f("index.tmpl");
	if (!f.is_open()) return;
	char* line = new char[1024];
	while (!f.eof()) {
		f.getline(line, 1024);
		if (!strcmp(line, "<!--CONTENT-->")) {
			auto tree = load("student.txt");
			student s{};
			s.average_mark = 2;
			show_tree(tree, &s);
		}
		else if (!strcmp(line, "<!--MENU-->")) {
			cout << "<div id='menu'>\n";
			cout << "<div id='menu-info'>Навигация по сайту</div>\n";
			cout << "<a href='/index.cgi'>Биография</a></br>\n";
			cout << "<a href='/rofls.cgi'>Анекдоты</a>\n";
			cout << "</div>" << endl;
		}
		else
			cout << line << endl;
	}
	delete[] line;
	f.close();
}
