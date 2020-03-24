#ifndef MinHeap_hpp
#define MinHeap_hpp

#include <unordered_map>
#include "./State.hpp"

using namespace std;

class MinHeap {
public:
	void push(const State *s);
	const State *pop();
	void decreaseKey(const State *s);
	void clear();
	bool isEmpty();
	
private:
	vector<const State *> states;
	unordered_map<const State *, size_t> lookup;
	
	void heapifyUp(size_t index);
	void heapifyDown();
	
	constexpr size_t parentIndex(const size_t index);
	constexpr size_t leftChildIndex(const size_t index);
};

#endif
