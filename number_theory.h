#ifndef NUMBER_THEORY_H
#define NUMBER_THEORY_H

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

#endif
