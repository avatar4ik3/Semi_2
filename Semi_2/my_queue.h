#pragma once
#ifndef MY_QUEUE
#define MY_QUEUE_H
#include <ostream>
template<typename T>
class my_queue
{
private:
	//коменты на русском = сломаный гит
	//элементы очереди  - отдельный класс
	class Node {
	public:
		Node *pnext; // указатель на некст элемент
		T elem; // текущий элемет
		Node(T &_elem, Node *_pnext = nullptr):elem(_elem), pnext(_pnext) {};
	};
	Node *head;
	unsigned size;
	T buff;
public:
	//инициализация
	my_queue() :size(0), head(nullptr),buff(0) {};
	//деструктор удаляет первый элемент пока есть элементы
	~my_queue() {
		while (size)pop();
	}
	//добавление элемента
	void push(T _elem) {
		head = new Node(_elem, head);
		size++;
	}
	//обращаемся к первому элементу очереди
	T front() {
		Node *pointer = head;
		while (pointer->pnext != nullptr) {
			pointer = pointer->pnext;
		}
		buff = pointer->elem;
		pop();
		return buff;
	}
	T front_safe() {
		Node *pointer = head;
		while (pointer->pnext != nullptr)pointer = pointer->pnext;
		return  pointer->elem;
	}
	//обращаемся к последнему
	T back() {
		return head->elem;
	}
	//удаляем первый
	void pop() {
		Node *pointer = head;
		while (pointer->pnext != nullptr)pointer = pointer->pnext;
		if (pointer != head) {
			Node *del = pointer;
			pointer = head;
			while (pointer->pnext != del)pointer = pointer->pnext;
			pointer->pnext = nullptr;
			delete[] del;
		}
		else {
			delete[] head; 
			head = nullptr;
		}
		size--;
	}
	//пустая ли очередь
	bool empty(){ 
		if (size)return true;
		return false;
	}
	//возвращает размер очереди
	unsigned get_size() {
		return this->size;
	}
	//выводим всю очередь
	friend std::ostream & operator <<(std::ostream & _os,const my_queue<T> & _q) {
		_os << std::endl;
		Node *pointer;
		for (unsigned i = _q.size; i > 0; --i) {
			pointer = _q.head;
			for (unsigned j = 1; j < i; ++j) {
				pointer = pointer->pnext;
			}
			_os << pointer->elem << " ";
		}
		return _os;
	}
	//копирует матрицу
	my_queue <T> & operator =(const my_queue<T> & _q) {
		this->~my_queue();
		Node *pointer;
		for (unsigned i = _q.size; i > 0; --i) {
			pointer = _q.head;
			for (unsigned j = 1; j < i; ++j) {
				pointer = pointer->pnext;
			}
			this->push(pointer->elem);
		}
		return *this;
	}
	//инвертируем очередь вариант 1: через еще одну очередь (количество операций n + (n^2)/2)
	void invert() {
		my_queue<T> helper;
		Node *pointer = head;
		while (pointer != nullptr) {
			helper.push(pointer->elem);
			pointer = pointer->pnext;
		}
		for (unsigned i = size; i > 0; --i) {
			pointer = head;
			for (unsigned j = 1; j < i; ++j) {
				pointer = pointer->pnext;
			}
			pointer->elem = helper.front();
		}
	}
	//инвертируем очередь вариант 2 : через динамический массив(количество операций 2n)
	void inver_a() {
		T *arr = new T[size];
		unsigned i = 0;
		while(size) {
			arr[i] = front();
			i++;
		}
		while(i) {
			i--;
			push(arr[i]);
		}
		delete[] arr;
	}
	//сортируем вариант 1:через динамический массив вставками(количество операций 3n + (n^2)/2)
	//вставками т.к. кода мало писать надо
	friend void sort_a(my_queue <T> & _q, my_queue <T> & _p,my_queue<T> &res) {
		unsigned size1 = _q.size, size2 = _p.size;
		T *arr = new T[size1 + size2];
		unsigned i = _q.size - 1;
		while (_q.size) {
			arr[i] = _q.front();
			i--;
		}
		for (i = 0; i < size1; ++i) {
			_q.push(arr[i]);
		}
		i = _q.size + _p.size - 1;
		while (_p.size) {
			arr[i] = _p.front();
			i--;
		}
		for (i = _q.size; i < size1+size2; ++i) {
			_p.push(arr[i]);
		}
		for (i = 1; i < size1 + size2 ; ++i) {
			for (unsigned j = i; j > 0 && arr[j - 1] > arr[j]; --j) {
				T tmp = arr[j - 1];
				arr[j - 1] = arr[j];
				arr[j] = tmp;
			}
		}
		for (i = 0; i < size1 + size2; ++i) {
			res.push(arr[i]);
		}
		delete[]arr;
	}
};
#endif // !MY_QUEUE;