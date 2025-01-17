#include "MinHeap.hpp"

void MinHeap::push(const State *s) {
	states.push_back(s);
	const size_t index = states.size() - 1;
	lookup[s] = index;
	heapifyUp(index);
}

void MinHeap::heapifyUp(size_t index) {
	size_t parent = parentIndex(index);
	
	while (index > 0 && states[parent]->score.f > states[index]->score.f) {
		swap(states[parent], states[index]);
		lookup[states[parent]] = parent;
		lookup[states[index]] = index;
		index = parent;
		parent = parentIndex(index);
	}
}

constexpr size_t MinHeap::parentIndex(const size_t index) {
	return (index - 1) / 2;
}

const State *MinHeap::pop() {
	const State *ret = states.front();
	states[0] = states.back();
	lookup[states[0]] = 0;
	states.pop_back();
	lookup.erase(ret);
	heapifyDown();
	
	return ret;
}

void MinHeap::heapifyDown() {
	size_t index = 0;
	size_t lCIndex = leftChildIndex(index);
	size_t rCIndex = lCIndex + 1;
	
	while (lCIndex < states.size()) {
		const size_t smallerChildIndex = (rCIndex < states.size() && states[rCIndex]->score.f < states[lCIndex]->score.f) ? rCIndex : lCIndex;
		
		if (states[index]->score.f < states[smallerChildIndex]->score.f)
			return;
		
		swap(states[index], states[smallerChildIndex]);
		lookup[states[index]] = index;
		lookup[states[smallerChildIndex]] = smallerChildIndex;
		index = smallerChildIndex;
		lCIndex = leftChildIndex(index);
		rCIndex = lCIndex + 1;
	}
}

constexpr size_t MinHeap::leftChildIndex(const size_t index) {
	return (index * 2) + 1;
}

void MinHeap::decreaseKey(const State *s) {
	heapifyUp(lookup.at(s));
}

void MinHeap::clear() {
	states.clear();
	lookup.clear();
}

bool MinHeap::isEmpty() {
	return states.empty();
}
