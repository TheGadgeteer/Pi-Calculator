#include <stdio.h>
#include <string>
#include <string.h>
#include "BigFloat.h"

template <int M, int E>
BigFloat<M, E>::Exponent::Exponent(int val) {
	//printf("Constructor of Exponent %p was called\n", this);
	mExp = new char[E];
	*this = val;
}

//Copy constructor
template <int M, int E>
typename BigFloat<M, E>::Exponent& BigFloat<M, E>::Exponent::operator=(const Exponent& e) {
	printf("Copying Exponent %p to %p\n", &e, this);
	mExp = new char[E];
	memcpy(mExp, e.mExp, E);
	return *this;
}

//Move constructor
template <int M, int E>
typename BigFloat<M, E>::Exponent& BigFloat<M, E>::Exponent::operator=(Exponent&& e) {
	printf("Moving Exponent %p to %p\n", &e, this);
	mExp = e.mExp;
	e.mExp = NULL;
	return *this;
}

template <int M, int E>
typename BigFloat<M, E>::Exponent& BigFloat<M, E>::Exponent::operator=(int val) {
	val += mBias;
	for (int i = 0; i < E; ++i) {
		mExp[E - i - 1] = (val >> i*8) & 0xff;
	}
}

template <int M, int E>
typename BigFloat<M, E>::Exponent& BigFloat<M, E>::Exponent::operator+=(int i) {
	*this = i + getVal();
}

template <int M, int E>
typename BigFloat<M, E>::Exponent& BigFloat<M, E>::Exponent::operator-=(int i) {
	*this = getVal() - i;
}

template <int M, int E>
bool BigFloat<M, E>::Exponent::operator<(const Exponent& e) const {
	for (int i = 0; i < E; ++i) {
		if (mExp[i] < e.mExp[i])
			return true;
	}
	return false;
}

template <int M, int E>
bool BigFloat<M, E>::Exponent::operator==(const Exponent& e) const {
	for (int i = E - 1; i >= 0; --i)
		if (mExp[i] != e.mExp[i])
			return false;
	return true;
}

template <int M, int E>
int BigFloat<M, E>::Exponent::getVal() {
	int val = 0;
	for (int i = 0; i < E; ++i) {
		val += mExp[E - 1 - i];
		val <<= 8;
	}
	return val - mBias;
}




template <int M, int E>
BigFloat<M, E>::Mantisse::Mantisse(double val) {
	//printf("Constructor of Mantisse %p was called\n", this);
	mMantisse = new char[M];
	*this = val;
}

template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator=(const Mantisse& m) {
	printf("Copying Mantisse %p to %p\n", &m, this);
	mMantisse = new char[M];
	memcpy(mMantisse, m.mMantisse, M);
	return *this;
}

template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator=(Mantisse&& m) {
		printf("Moving Mantisse %p to %p\n", &m, this);
		mMantisse = m.mMantisse;
		m.mMantisse = NULL;
		return *this;
}

//Value is assumed to be between 0 and 2
template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator=(double val) {
	const double powerof2 = (double) (1 << 8);
	double temp = powerof2 * 2.;
	for (int i = 0; i < M; ++i) {
		mMantisse[i] = (char)(val * temp);
		temp *= powerof2;
	}
}

template <int M, int E>
bool BigFloat<M, E>::Mantisse::operator<(const Mantisse& m) const {
	for (int i = 0; i < M; ++i) {
		if (mMantisse[i] < m.mMantisse[i])
			return true;
	}
	return false;
}

template <int M, int E>
bool BigFloat<M, E>::Mantisse::operator==(const Mantisse& m) const {
	for (int i = 0; i < M; ++i)
		if (mMantisse[i] != m.mMantisse[i])
			return false;
	return true;
}

template <int M, int E>
double BigFloat<M, E>::Mantisse::getVal() {
	const double powerof2= (double) (1 << 8);
	double temp = powerof2 * 2.;
	double val = (double)mMantisse[0] / temp;
	for (int i = 1; i < M; ++i) {
		temp *= powerof2;
		val += (double)mMantisse[i] / temp;
	}
	return val;
}



// Copy constructor
template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator=(const BigFloat<M, E>& b) {
	printf("Copying %p to %p\n", &b, this);
	mExp = b.mExp;
	mMantisse = b.mMantisse;
	mSgn = b.mSgn;
}

// Move constructor
template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator=(BigFloat<M, E>&& b)  {
	printf("Moving %p to %p\n", &b, this);
	mExp = std::move(b.mExp);
	mMantisse = std::move(b.mMantisse);
	mSgn = b.mSgn;
}

template <int M, int E>
BigFloat<M, E>::BigFloat(double initVal) : mExp(0){
	printf("Constructor for BigFloat %p was called\n", this);
	operator=(initVal);
}

template <int M, int E>
BigFloat<M, E>::~BigFloat() {
	printf("Destructor for BigFloat %p was called\n", this);
	mExp.~Exponent();
	mMantisse.~Mantisse();
}

template <int M, int E>
std::string BigFloat<M, E>::toString() {
	//TODO
	printf("in function toString()\n");
	std::string s = "Sign: " ;
	s += std::to_string(mSgn);
	s += " * ";
	s+= std::to_string(mMantisse.getVal());
	s += " * 2 ^ ";
	s += std::to_string(mExp.getVal());
	return s;
}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator=(double d) {
	mSgn = d >= 0 ? 1 : -1;
	d *= mSgn; //get absolute
	while (d < 1) {
		d *= 2.;
		mExp -= 1;
	}
	while (d > 2) {
		d /= 2;
		mExp += 1;
	}
	mMantisse = d;
}

template <int M, int E>
BigFloat<M, E> BigFloat<M, E>::operator-() const {
	BigFloat b(*this);
	b.setSign(-mSgn);
	return b;
}

template <int M, int E>
BigFloat<M, E> BigFloat<M, E>::operator+(const BigFloat<M, E>& b) const {
	BigFloat a(*this);
	a += b;
	return a;
}

template <int M, int E>
BigFloat<M, E> BigFloat<M, E>::operator-(const BigFloat<M, E>& b) const {

}

template <int M, int E>
BigFloat<M, E> BigFloat<M, E>::operator* (const BigFloat<M, E>& b) const {

}

template <int M, int E>
BigFloat<M, E> BigFloat<M, E>::operator/ (const BigFloat<M, E>& b) const {

}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator+= (const BigFloat<M, E>& b) {

}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator-= (const BigFloat<M, E>& b) {

}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator*= (const BigFloat<M, E>& b) {

}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator/= (const BigFloat<M, E>& b) {

}

template <int M, int E>
bool BigFloat<M, E>::operator< (const BigFloat<M, E>& b) const {
	if (b.mSgn == mSgn) {
		bool isSmaller;
		if (b.mExp == mExp)
			isSmaller = mMantisse < b.mMantisse;
		else
			isSmaller = mExp < b.mExp;
		if (mSgn == -1)
			return !isSmaller;
		else
			return isSmaller;
	}
	else {
		if (mSgn == 1)
			return false;
		else
			return true;
	}

}

template <int M, int E>
bool BigFloat<M, E>::operator== (const BigFloat<M, E>& b) const {
	return mMantisse == b.mMantisse && mExp == b.mExp && mSgn == b.mSgn;
}

template <int M, int E>
bool BigFloat<M, E>::absGreaterThan (const BigFloat<M, E>& b) const {

}

template <int M, int E>
BigFloat<M, E> BigFloat<M, E>::abs() const {
	BigFloat b(*this);
	b.setSign(0);
	return b;
}

//set to 1 or -1
template <int M, int E>
void BigFloat<M, E>::setSign(int sgn) {
	if (sgn == 1 || sgn == -1)
		mSgn = sgn;
}

int main() {
	BigFloat<> a(0.0024), b(30000.24);
	printf("BigFloat<> c  = -b\n");
	BigFloat<> c = -b;
	printf("a = -b\n");
	a = -b;
	printf("smaller: %d, equal: %d\n", a < b, a == b);
	std::string s =  a.toString();
	printf("%s\n", s.c_str());
	return 0;
}
