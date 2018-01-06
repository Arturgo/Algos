#ifndef NUMBER_THEORY_H
#define NUMBER_THEORY_H

template<class T>
T pgcd(T a, T b) {
	if(a == 0)
		return b;
	return pgcd<T>(b % a, a);
}

#endif
