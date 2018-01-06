#ifndef NUMBER_THEORY_H
#define NUMBER_THEORY_H

#include <deque>
#include <iostream>
using namespace std;

/* Big Unsigned Ints */
const size_t __chunksize = sizeof(unsigned int) * 8;
const size_t __chunksizelog = __builtin_log(__chunksize);
const size_t __chunkmask = __chunksize - 1;
const unsigned long long __least_bits = (1ll << __chunksize) - 1;

class buint {
private:
	deque<unsigned int> chunks;
public:
	buint();
	buint(unsigned long long val);
	
	size_t nbChunks() const;
	deque<unsigned int>& chunksRef();
	const deque<unsigned int>& chunksCRef() const;
	unsigned int getChunk(size_t pos) const;
	void setChunk(size_t pos, unsigned int val);
	void prepareChunks(size_t size);
	void clean();
	size_t size() const;
	bool get(size_t pos) const;
	void set(size_t pos, bool val);
	
	unsigned long long to_ulong() const;
};

buint& operator += (buint& a, const buint& b);
buint operator + (const buint &a, const buint &b);
buint& operator |= (buint &a, const buint &b);
buint operator | (const buint &a, const buint &b);
buint& operator &= (buint &a, const buint &b);
buint operator & (const buint &a, const buint &b);
buint& operator ^= (buint &a, const buint &b);
buint operator ^ (const buint &a, const buint &b);
buint& operator <<= (buint &a, size_t dec);
buint operator << (const buint &a, size_t dec);

buint::buint() {
}

buint::buint(unsigned long long val) {
	unsigned long long chunk1 = val >> __chunksize;
	unsigned int chunk0 = val & __least_bits;
	setChunk(0, chunk0);
	setChunk(1, chunk1);
	clean();
}

inline size_t buint::nbChunks() const {
	return chunks.size();
}

inline deque<unsigned int>& buint::chunksRef() {
	return chunks;
}

inline const deque<unsigned int>& buint::chunksCRef() const {
	return chunks;
}

inline unsigned int buint::getChunk(size_t pos) const {
	if(pos >= nbChunks())
		return 0;
	return chunks[pos];
}

inline void buint::setChunk(size_t pos, unsigned int val) {
	if(pos >= nbChunks()) {
		chunks.resize(pos + 1, 0);
	}
	
	chunks[pos] = val;
	
	clean();
}

inline void buint::prepareChunks(size_t size) {
	chunks.resize(max(size, nbChunks()));
}

inline void buint::clean() {
	size_t cur = nbChunks();
	while(cur != 0 && chunks[cur - 1] == 0)
		cur--;
	if(cur != nbChunks())
		chunks.resize(cur);
}

inline size_t buint::size() const {
	if(chunks.empty())
		return 0;
	return __chunksize * nbChunks() - __builtin_clz(chunks.back());
}

inline bool buint::get(size_t pos) const {
	size_t chunk = pos >> __chunksizelog;
	return (getChunk(chunk) >> (pos & __chunkmask)) & 1;
}

inline void buint::set(size_t pos, bool val) {
	if(get(pos) == val)
		return;
	
	int chunk = pos >> __chunksizelog;
	setChunk(chunk, getChunk(chunk) ^ (1 << (pos & __chunkmask)));
	clean();
}

inline unsigned long long buint::to_ulong() const {
	return ((unsigned long long)getChunk(1) << __chunksize) + getChunk(0);
}

inline buint& operator += (buint &a, const buint &b) {
	a.prepareChunks(max(a.nbChunks(), b.nbChunks()) + 1);
	size_t r = 0;
	size_t cur = 0;
	
	deque<unsigned int>& chunksA = a.chunksRef();
	const deque<unsigned int>& chunksB = b.chunksCRef();
	
	for(;cur < b.nbChunks();cur++) {
		size_t s = r + chunksA[cur] + chunksB[cur];
		r = s >> __chunksize;
		chunksA[cur] = s & __least_bits;
	}
	
	if(r != 0) {
		chunksA[cur] = r;
	}
	a.clean();
	return a;
}

inline buint operator + (const buint &a, const buint &b) {
	buint c = a;
	c += b;
	return c;
}

inline buint& operator |= (buint &a, const buint &b) {
	a.prepareChunks(max(a.nbChunks(), b.nbChunks()) + 1);
	
	deque<unsigned int>& chunksA = a.chunksRef();
	const deque<unsigned int>& chunksB = b.chunksCRef();
	
	for(size_t cur = 0;cur < b.nbChunks();cur++) {
		chunksA[cur] |= chunksB[cur];
	}
	a.clean();
	return a;
}

inline buint operator | (const buint &a, const buint &b) {
	buint c = a;
	c |= b;
	return c;
}

inline buint& operator &= (buint &a, const buint& b) {
	deque<unsigned int>& chunksA = a.chunksRef();
	const deque<unsigned int>& chunksB = b.chunksCRef();
	
	chunksA.resize(chunksB.size());
	for(size_t cur = 0;cur < b.nbChunks();cur++) {
		chunksA[cur] &= chunksB[cur];
	}
	a.clean();	
	return a;
}

inline buint operator & (const buint &a, const buint &b) {
	buint c = a;
	c &= b;
	return c;
}

inline buint& operator ^= (buint &a, const buint &b) {
	a.prepareChunks(max(a.nbChunks(), b.nbChunks()) + 1);
	
	deque<unsigned int>& chunksA = a.chunksRef();
	const deque<unsigned int>& chunksB = b.chunksCRef();
	
	for(size_t cur = 0;cur < b.nbChunks();cur++) {
		chunksA[cur] ^= chunksB[cur];
	}
	a.clean();
	return a;
}

inline buint operator ^ (const buint &a, const buint &b) {
	buint c = a;
	c ^= b;
	return c;
}

inline buint& operator <<= (buint &a, size_t dec) {
	deque<unsigned int>& chunksA = a.chunksRef();
	chunksA.push_back(0);
	
	int mod = dec % __chunksize;
	if(mod != 0) {
		unsigned long long r = 0;
		for(size_t iChunk = 0;iChunk < a.nbChunks();iChunk++) {
			unsigned long long s = (chunksA[iChunk] << mod) | r;
			r = s >> __chunksize;
			chunksA[iChunk] = s & __least_bits;
		}
	}
	
	for(size_t n = 0; n < dec / __chunksize;n++) {
		chunksA.push_front(0);
	}
	
	a.clean();
	return a;
}

inline buint operator << (const buint &a, size_t dec) {
	buint c = a;
	c <<= dec;
	return c;
}

inline size_t log2(buint& a) {
	return a.size() - 1;
}

/*PGCD and PPCM*/

template<class T>
T pgcd(T a, T b) {
	if(a == 0)
		return b;
	return pgcd<T>(b % a, a);
}

template<class T>
inline T ppcm(T a, T b) {
	return a / pgcd<T>(a, b) * b;
}

/*Factorial*/
template<class T>
inline T factorial(T a) {
	T prod = 1;
	for(T fact = 1;fact <= a;fact++)
		prod *= fact;
	return prod;
}

#endif
