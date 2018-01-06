#ifndef NUMBER_THEORY_H
#define NUMBER_THEORY_H

#include <deque>
using namespace std;

const size_t __chunksize = sizeof(unsigned int) * 8;
const size_t __chunksizelog = __builtin_log(__chunksize);
const size_t __chunkmask = __chunksize - 1;

class bint {
private:
	deque<unsigned int> chunks;
public:
	size_t size();
	bool get(size_t pos);
	void set(size_t pos, bool val);
	
	friend bint operator + (const bint& a, const bint &b);
};

size_t bint::size() {
	if(chunks.empty())
		return 0;
	return __chunksize * chunks.size() - __builtin_clzll(chunks.back());
}

bool bint::get(size_t pos) {
	size_t chunk = pos >> __chunksizelog;
	if(chunk >= chunks.size())
		return 0;
	return (chunks[chunk] >> (pos & __chunkmask)) & 1;
}

void bint::set(size_t pos, bool val) {
	if(get(pos) == val)
		return;
	size_t chunk = pos >> __chunksizelog;
	if(chunk >= chunks.size())
		chunks.resize(chunk + 1, 0);
	
	chunks[chunk] ^= (1 << (pos & __chunkmask));
	
	size_t cur = size();
	while(cur != 0 && chunks[cur - 1] == 0)
		cur--;
	chunks.resize(cur);
}

bint operator + (const bint &a, const bint &b) {
	if(a.chunks.size() > b.chunks.size())
		return b + a;
	
	bint c = b;
	
	size_t r = 0;
	for(size_t cur = 0;cur < a.chunks.size();cur++) {
		size_t s = r + a.chunks[cur] + c.chunks[cur];
		r = s >> __chunksize;
		c.chunks[cur] = s - (r << __chunksize);
	}
	
	if(r != 0) {
		if(a.chunks.size() < b.chunks.size())
			c.chunks[a.chunks.size()] = r;
		else
			c.chunks.push_back(r);
	}
	
	size_t cur = c.size();
	while(cur != 0 && c.chunks[cur - 1] == 0)
		cur--;
	c.chunks.resize(cur);
	
	return c;
}

size_t log2(bint& a) {
	return a.size() - 1;
}

template<class T>
T pgcd(T a, T b) {
	if(a == 0)
		return b;
	return pgcd<T>(b % a, a);
}

template<class T>
T ppcm(T a, T b) {
	return a / pgcd<T>(a, b) * b;
}

template<class T>
T factorial(T a) {
	T prod = 1;
	for(T fact = 1;fact <= a;fact++)
		prod *= fact;
	return prod;
}

#endif
