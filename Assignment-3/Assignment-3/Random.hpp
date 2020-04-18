#ifndef Random_hpp
#define Random_hpp

struct Random {
	virtual int to(int limit) = 0;
	virtual float chance() = 0;
};

struct RandRandom : public Random {
	int to(int limit) {
		return rand() % limit;
	}
	
	float chance() {
		return rand() / static_cast<float>(RAND_MAX);
	}
};

#endif
