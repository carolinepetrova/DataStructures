#include "pch.h"
#include <iostream>

const size_t SIZE = 3;

template <typename T>
struct Node {
	T arr[SIZE];
	int capacity;
	Node<T> * next;
	Node() {
		capacity = 0;
		next = nullptr;
	}
};

template <typename T>
class UnrolledLinkedList {
private:
	Node<T> * root;
	//copy and delete

public:
	//iterator
	struct UnrolledLinkedListIterator {
	private:
		Node<T> * iter;
		int counter;
	public:
		friend class UnrolledLinkedList<T>;
		UnrolledLinkedListIterator(Node<T> * iter, int counter = 0) : iter(iter), counter(counter) {}
		UnrolledLinkedListIterator(UnrolledLinkedListIterator const & other) {}
		UnrolledLinkedListIterator & operator++() {
			if(counter !=SIZE - 1)
			++counter;
			else {
				iter = iter->next;
				counter = 0;
			}
			return *this;
		}
		UnrolledLinkedListIterator & operator+(int num) {
			int counter2 = 0;
			while (num != counter2) {
				++counter2;
				iter;
				operator++();
			}
			
			return *this;
		}
		UnrolledLinkedListIterator & operator++(int) {
			UnrolledLinkedListIterator dummy = *this;
			++(*this);
			return dummy;
		}
		T & operator*() {
			return iter->arr[counter];
		}
		//UnrolledLinkedListIterator & operator+(int num) {}
		bool operator==(const UnrolledLinkedListIterator & other) {
			return this->iter == other.iter;
		}
		bool operator!=(const UnrolledLinkedListIterator & other) {
			return !(*this == other);
		}

	};

	//big 4
	UnrolledLinkedList<T>() {
		root = new Node<T>();
	}
	UnrolledLinkedList<T>(UnrolledLinkedList<T> const & other) {}
	UnrolledLinkedList<T> & operator=(UnrolledLinkedList<T> const & other) {}
	~UnrolledLinkedList<T>() {}

	//methods
	void expandAndAdd(int nodeNum, const T & elem) {
		Node<T> * node = new Node<T>();
		Node<T> * currentNode = new Node<T>();
		if (root->next == nullptr) {
			currentNode = root;
			root->next = node;
		}
		else {
			currentNode = lookup(nodeNum-1);
			if (currentNode->next != nullptr)
				node->next = currentNode->next;
			currentNode->next = node;
		}
		//double size = SIZE;
		for (int i = ceil(SIZE / 2); i < SIZE; i++) {
			node->arr[node->capacity] = currentNode->arr[i];
			currentNode->arr[i] = NULL;
			--(currentNode->capacity);
			++(node->capacity);
		}
		node->arr[node->capacity] = elem;
		++(node->capacity);
	}
	Node<T> * lookup(int index) const {
		Node<T> * currentNode = new Node<T>();
		currentNode = root;
		while (currentNode != NULL && index > 0) {
			currentNode = currentNode->next;
			--index;
		}
		return currentNode;
	}

	void push_back(const T & elem) {
		if (root->next == nullptr) {
			if (root->capacity < SIZE) {
				root->arr[root->capacity] = elem;
				++(root->capacity);
			}
			else expandAndAdd(1, elem);
		}
		else {
			Node<T> * lastNode = new Node<T>();
			lastNode = lookup(size());
			if (lastNode->capacity < SIZE) {
				push_to_node(size(), elem);
			}
			else expandAndAdd(size(), elem);
		}
	}
	void push_to_node(int nodeNum, T elem) {
		Node<T> * node = new Node<T>;
		node = root;
		node = lookup(nodeNum);
		if (node->capacity < SIZE) {
			node->arr[node->capacity] = elem;
			++(node->capacity);
		}
		else expandAndAdd(nodeNum, elem);
	}
	void push_in_node(int i, int j,const T & elem) {
		Node<T> * node = new Node<T>();
		node = lookup(i-1);
		if (node->capacity < SIZE) {
			if(node->arr[j - 1] == NULL)
				++(node->capacity);
			node->arr[j-1] = elem;
			
		}
		else expandAndAdd(i, elem);
	} //TODO
	void delete_occurances(T elem) {
		auto iter = this->begin();
		for (iter; iter != nullptr; ++iter) {
			if (elem == *iter)
				*iter = NULL;
		}
	}
	bool empty() {}
	size_t size() const {
		size_t size = 0;
		Node<T> * node = new Node<T>();
		node = root;
		while (node->next != nullptr) {
			++size;
			node = node->next;

		}
		return size;
	}
	UnrolledLinkedListIterator begin() const {
		return UnrolledLinkedListIterator(root);
	}
	UnrolledLinkedListIterator end() const {
		return UnrolledLinkedListIterator(lookup(this->size()));
	}
};

int main() {
	UnrolledLinkedList<int> l1;
	l1.push_back(3);
	l1.push_back(4);
	l1.push_back(5);
	l1.push_back(6);
	l1.push_in_node(1, 2, 8);
	l1.delete_occurances(5);
	auto iter = l1.begin();
	auto endIter = l1.end();
	//int k = 0;
	//int counter = 0;
	//for (k; k <= l1.size(); k++) {
	//	std::cout << "Elements in box " << k+1 << ": ";
	//	for (iter; iter != nullptr; ++iter) {
	//		if (*iter != NULL) {
	//			std::cout << *iter << " ";
	//			++counter;
	//		}
	//		else {
	//			iter = iter + (SIZE- counter);
	//			counter = 0;
	//			break;
	//		}
	//		
	//		
	//	}
	//	std::cout << std::endl;
	//	
	//}
	for (iter; iter != nullptr; ++iter) {
		std::cout << *iter << " ";
	}
//	l1.push_in_node(2, 2, 9);
	//  std::cout << "Hello World!\n"; 
}

