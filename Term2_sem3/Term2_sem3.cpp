// test_term2_sem3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <string>
#include <iostream>
using namespace std;

// структура эл-та дерева
struct Tree
{
	string data;
	Tree *left;
	Tree *right;
};

//структура эл-та стека который содержит указатель на элемент дерева
struct Stack
{
	Tree *data;
	Stack *next;
};

// структура очереди
struct Queue
{
	Stack *head;
	Stack *end;
};

Queue *initQueue() {
	Queue *q = new Queue{};
	q->head = new Stack{ nullptr, nullptr };
	q->end = q->head;
	return q;
}

void push(Queue *q, Tree *val) {
	q->end->next = new Stack{ val, nullptr };
	q->end = q->end->next;
}

void push(Queue *q, char ch, int count)
{
	q->end->next = new Stack;
	q->end->next->data = new Tree;
	q->end->next->next = nullptr;
	q->end->next->data->data.assign(1, ch);
	q->end->next->data->data += " " + to_string(count);
	q->end->next->data->left = nullptr;
	q->end->next->data->right = nullptr;
	q->end = q->end->next;
}

Tree *peek(Queue *q) {
	if (q->head != nullptr && q->head->next != nullptr)
		return q->head->next->data;
	return nullptr;
}

Stack *pop(Queue *q) {
	Stack *popped = nullptr;
	if (q->head->next != nullptr) {
		popped = q->head->next;
		q->head->next = q->head->next->next;
		if (popped == q->end) {
			q->end = q->head;
		}
	}
	return popped;
}


void bfs(Tree *head) {
	Queue *s = initQueue();
	push(s, head);

	while (peek(s) != nullptr) {
		Tree *current = pop(s)->data;
		cout << current->data << endl;
		if (current->left != nullptr) {
			push(s, current->left);
		}
		if (current->right != nullptr) {
			push(s, current->right);
		}
	}
}

//считает вес
int value(Stack *elem)
{
	if (!elem) return NULL;
	int val = 0;
	for (int i = 2; i < elem->data->data.size(); ++i)
		val += (elem->data->data[i] - '0');
	return val;
}

int value(Tree *elem)
{
	int val = 0;
	for (int i = 2;i < elem->data.size();++i)
		val += (elem->data[i] - '0');
	return val;
}

//возвращает самый маленький по весу эл-т
Stack *pop_smallest(Queue *head)
{
	if (head->head->next == nullptr) { return nullptr; }
	Stack *adress_prev_small = head->head, *current = head->head;
	while (current->next)//ищем самый маленький кроме head
	{
		if (value(current->next) < value(adress_prev_small->next))
		{
			adress_prev_small = current;
		}
		current = current->next;
	}
	if (head->end == adress_prev_small->next)
	{
		head->end = adress_prev_small;
	}
	current = adress_prev_small;
	adress_prev_small = adress_prev_small->next;
	current->next = adress_prev_small->next;
	adress_prev_small->next = nullptr;
	return adress_prev_small;
}

Queue *create_queue_from_string(string str)
{
	bool b = false;
	while (!b)//упорядочиваем строку для облегчения подсчета
	{
		b = true;
		for (int i = 0;i < str.size() - 1;++i)
		{
			if (str[i] > str[i + 1])
			{
				char tmp = str[i];
				str[i] = str[i + 1];
				str[i + 1] = tmp;
				b = false;
			}
		}
	}

	Queue *head = initQueue();
	char tmp = str[0];
	int count = 1;
	for (int i = 1;i < str.size();++i)// создаем стек элементов
	{
		if (tmp == str[i]) ++count;
		else
		{
			push(head, tmp, count);
			tmp = str[i];
			count = 1;
		}
	}
	push(head, tmp, count);
	return head;
}

//делаем дерево из стринг
Tree *make_tree(string str)
{
	Queue *q = create_queue_from_string(str);//делаем стек из стринг
	Tree *head;//указатель для создания нового эл-та
	while (q->head != nullptr)
	{
		head = new Tree;//создаем новый элемент дерева
		head->left = pop_smallest(q)->data;
		head->right = pop_smallest(q)->data;
		head->data = "  " + to_string(value(head->left) + value(head->right));
		if (!q->head->next)//если стек оказался пуст то возвращаем последний созданный
		{
			head->data = " ";// обнуляем дату для упрощения послд кодирования
			return head;
		}
		push(q, head);//иначе продолжаем формировать дерево
	}
}

string get_head_data(const string str)
{
	string result = "";
	if (str.size() > 2)
	{
		result = str.substr(2, str.size() - 2);
	}
	return result;
}

void cipher_tree(Tree *head)
{
	if (head->left)
	{
		head->left->data = head->left->data.substr(0, 2) + get_head_data(head->data) + "0";
		cipher_tree(head->left);
	}
	if (head->right)
	{
		head->right->data = head->right->data.substr(0, 2) + get_head_data(head->data) + "1";
		cipher_tree(head->right);
	}
}

void show_tree(Tree *head)
{
	if (!head) return;
	cout << head->data << endl;
	show_tree(head->left);
	show_tree(head->right);
}

///////// make cipher string ///////

struct Search_stack
{
	char data;
	string cipher;
	Search_stack* next;
};

void push(Search_stack*& head, string str)
{
	Search_stack* new_elem = new Search_stack;
	new_elem->data = str[0];
	new_elem->cipher = str.substr(2, str.size() - 2);
	new_elem->next = head;
	head = new_elem;
}

Search_stack* create(Tree* head)
{
	Search_stack* head_stack = nullptr;
	Queue* s = initQueue();
	push(s, head);
	string str;
	while (peek(s) != nullptr) {
		Tree* current = pop(s)->data;
		str =  current->data;
		if (str[0] != ' ') push(head_stack, str);
		if (current->left != nullptr) {
			push(s, current->left);
		}
		if (current->right != nullptr) {
			push(s, current->right);
		}
	}
	return head_stack;
}

string search_cipher(Search_stack* head, char ch)
{
	while (head != nullptr)
	{
		if (head->data == ch) return head->cipher;
		head = head->next;
	}
	return "Error!";
}

string make_cipher_str(Search_stack* head, string str)
{
	string result;
	for (int i = 0; i < str.size(); ++i)
	{
		result += search_cipher(head, str[i]);
	}
	return result;
}

int main()
{
	string str;
	cin >> str;
	Tree *head = make_tree(str);
	cipher_tree(head);
	//show_tree(head);
	bfs(head);
	cout << endl;
	Search_stack* shead = create(head);
	string result = make_cipher_str(shead, str);
	cout << result << endl;
	cout << str << endl;

	return 0;
}