#include <stdio.h>
#include <string.h>
#include "BigFloat.h"

template <int M, int E>
BigFloat<M, E>::Exponent::Exponent(int val) {
	printf("Constructor of Exponent %p was called\n", this);
	mExp = new char[E];
	*this = val;
}

template <int M, int E>
BigFloat<M, E>::Exponent::Exponent(const Exponent& e) {
	printf("Copy Constructor of Exponent %p was called\n", this);
	mExp = new char[E];
	memcpy(mExp, e.mExp, E);
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
int BigFloat<M, E>::Exponent::getVal() {
	int val = 0;
	for (int i = 0; i < E; ++i) {
		val += mExp[E - 1 - i];
		val <<= 8;
	}
	return val - mBias;
}

// Copy constructor
template <int M, int E>
BigFloat<M, E>::BigFloat(const BigFloat<M, E>& b) {
	printf("Copy constructor for BigFloat %p was called\n", this);
	mExp = b.mExp;
	mMantisse = b.mMantisse;
	mSgn = b.mSgn;
}

template <int M, int E>
BigFloat<M, E>::BigFloat(double init) {
	printf("Constructor for BigFloat %p was called\n", this);
	mSgn = init >= 0 ? 1 : -1;
	mExp = new char[E];
	mMantisse = new char[M];


}

template <int M, int E>
BigFloat<M, E>::~BigFloat() {
	printf("Destructor for BigFloat %p was called\n", this);
	mExp.~Exponent();
	mMantisse.~Mantisse();
}

template <int M, int E>
char* BigFloat<M, E>::toString() {

}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator=(double d) {

}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator-() const {
	BigFloat b(*this);
	b.setSign(-mSgn);
	return b;
}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator+(const BigFloat<M, E>& b) const {
	BigFloat a(*this);
	a += b;
	return a;
}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator-(const BigFloat<M, E>& b) const {

}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator* (const BigFloat<M, E>& b) const {

}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator/ (const BigFloat<M, E>& b) const {

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
	return mMantisse == b.mMantisse && mExp == b.mExp;
}

template <int M, int E>
bool BigFloat<M, E>::absGreaterThan (const BigFloat<M, E>& b) const {

}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::abs() const {
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
	BigFloat<> a, b;
	printf("smaller: %d " a < b);
	return 0;
}
