#include "number_theory.h"
#include <iostream>
using namespace std;

int main() {
	buint a(10), b(12);
	buint sum = a + b;
	cout << sum.to_ulong() << endl;
}
