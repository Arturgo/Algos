#include "number_theory.h"
#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

vector<bool> vc;
vector<bool> va, vb;

buint a, b, c;

int main() {
	for(int i = 0;i < 1000000;i++) {
		va.push_back(rand() % 2);
		a.set(i, va.back());
		vb.push_back(rand() % 2);
		b.set(i, vb.back());
	}
	
	clock_t deb = clock();
	
	vc.resize(1000004);
	for(int i = 0;i < 1000000;i++) {
		vc[i] = va[i] | vb[i];
	}
	
	clock_t mil = clock();
	
	c = a | b;
	
	clock_t fin = clock();
	
	cout << (long long)(mil - deb) << " " << (long long)(fin - mil) << endl;
}
