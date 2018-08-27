#include <stdio.h>
#include <string>
#include <string.h>
#include "BigFloat.h"

template <int M, int E>
BigFloat<M, E>::Exponent::Exponent(int val) {
	printf("Constructor of Exponent %p was called\n", this);
	mExp = new u_char[E];
	*this = val;
}

// Exponent - Copy Constructor
template <int M, int E>
BigFloat<M, E>::Exponent::Exponent(const Exponent& e) 
{
	printf("Copying Exponent %p in new object %p\n", &e, this);
	mExp = new u_char[E];
	memcpy(mExp, e.mExp, E);
}

// Exponent - Move Constructor
template <int M, int E>
BigFloat<M, E>::Exponent::Exponent(Exponent&& e)
{
	printf("Moving Exponent %p in new object %p\n", &e, this);
	mExp = e.mExp;
	e.mExp = nullptr;
}

//Copy operator
template <int M, int E>
typename BigFloat<M, E>::Exponent& BigFloat<M, E>::Exponent::operator=(const Exponent& e) {
	printf("Copying Exponent %p to %p\n", &e, this);
	memcpy(mExp, e.mExp, E);
	return *this;
}

//Move operator
template <int M, int E>
typename BigFloat<M, E>::Exponent& BigFloat<M, E>::Exponent::operator=(Exponent&& e) {
	printf("Moving Exponent %p to %p\n", &e, this);
	char *temp = mExp;
	mExp = e.mExp;
	e.mExp = temp;
	return *this;
}

template <int M, int E>
typename BigFloat<M, E>::Exponent& BigFloat<M, E>::Exponent::operator=(int val) {
	val += mBias;
	for (int i = 0; i < E; ++i) {
		mExp[E - i - 1] = (char)(val >> i*8);
	}
	return *this;
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
int BigFloat<M, E>::Exponent::getVal() const {
	int val = 0;
	for (int i = 0; i < E; ++i) {
		val |= ((unsigned int)mExp[E - 1 - i] << i * 8);
	}
	return val - mBias;
}

template <int M, int E>
BigFloat<M, E>::Mantisse::Mantisse(double val) {
	printf("Constructor of Mantisse %p was called\n", this);
	mMantisse = new u_char[M];
	*this = val;
}

// Mantisse - Copy Constructor
template <int M, int E>
BigFloat<M, E>::Mantisse::Mantisse(const Mantisse& m) {
	printf("Copying Mantisse %p into new Object %p\n", &m, this);
	mMantisse = new u_char[M];
	memcpy(mMantisse, m.mMantisse, M);
}

// Mantisse - Move Constructor
template <int M, int E>
BigFloat<M, E>::Mantisse::Mantisse(Mantisse&& m)
{
	printf("Moving Mantisse %p into new object %p\n", &m, this);
	mMantisse = m.mMantisse;
	m.mMantisse = nullptr;
}

template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator=(const Mantisse& m) {
	printf("Copying Mantisse %p to %p\n", &m, this);
	memcpy(mMantisse, m.mMantisse, M);
	return *this;
}

template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator=(Mantisse&& m) {
		printf("Moving Mantisse %p to %p\n", &m, this);
		char* temp = mMantisse;
		mMantisse = m.mMantisse;
		m.mMantisse = mMantisse;
		return *this;
}


template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator>>=(int shift)
{
	if (shift < 0)
		return operator<<=(-shift);

	//clear the bits that will be overwritten
	int i = M - 1, remaining = shift;
	while (remaining >= 8) {
		remaining -= 8;
		mMantisse[i--] = 0;
	}
	mMantisse[i] &= (0xff << remaining);

	for (int i = M * 8 - 1 - shift; i >= 0; --i) {
		int newPos = i + shift;
		int newChar = newPos / 8, oldChar = i / 8;
		int posInChar = newPos % 8, oldPos = i % 8;
		bool isSet = (mMantisse[oldChar] & (1 << oldPos)) > 0;
		if (isSet)
			mMantisse[newChar] |= (1 << posInChar);   //set Bit eventually
	}

	//clear the last bits
	i = 0; remaining = shift;
	while (remaining >= 8) {
		remaining -= 8;
		mMantisse[i++] = 0;
	}
	mMantisse[i] &= (0xff >> remaining);


	for (int i = 0; i < M; ++i)
		printf("%hhx ", mMantisse[i]);
	printf("\n");
	return *this;
	return *this;
}


template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator<<=(int shift)
{
	shift = 5;
	printf("bitshift test %d\n", shift);
	for (int x = 0; x < M; ++x) {
		for (int i = 0; i < 8; ++i) {
			printf("%i", (mMantisse[x] & (1 << (7 - i))) > 0);
		}
		printf(" ");
	}
	printf("\n");
	if (shift < 0)
		return operator>>=(-shift);

	for (int i = shift; i < M * 8; ++i) {
		int oldSegment = i / 8, oldPosInSeg =  7 - i % 8;   //posInSeg:  0 is the most right, 7 the most left
		int newSegment = (i - shift) / 8, newPosInSeg = 7 - (i - shift) % 8;

		if ((mMantisse[oldSegment] & (1 << oldPosInSeg)) != 0)
			mMantisse[newSegment] |= (1 << newPosInSeg);   // set Bit
		else
			mMantisse[newSegment] &= 0xff - (1 << newPosInSeg);  // clear Bit
		//printf("%d, %d  to %d, %d :  %d\tm%d & %d\n", oldSegment, oldPosInSeg, newSegment, newPosInSeg, isSet, mMantisse[oldSegment], (1 << oldPosInSeg));
	}

	//clear the last bits
	int i = M - 1;
	while (shift >= 8) {
		shift-= 8;
		mMantisse[i--] = 0;
	}
	mMantisse[i] &= (0xff << shift);

	for (int x = 0; x < M; ++x) {
		for (int i = 0; i < 8; ++i) {
			printf("%i", (mMantisse[x] & (1 << (7 - i))) > 0);
		}
		printf(" ");
	}
	printf("\n");
	return *this;
}

//Value is assumed to be between 0 and 2
template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator=(double val) {
	double save = val;
	const double powerof2 = (double) (1 << 8);
	val /= 2.;
	for (int i = 0; i < M; ++i) {
		val *= powerof2;
		mMantisse[i] = (char)(val);
		val -= mMantisse[i];
	}

	return *this;
}

template <int M, int E>
double BigFloat<M, E>::Mantisse::getVal() {
	const double powerof2 = (double) (1 << 8);
	double val = (double)mMantisse[M - 1];
	for (int i = M - 2; i >= 0; --i) {
		val /= powerof2;
		val += (double)mMantisse[i];
	}
	val /= powerof2 / 2.;
	return val;
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
BigFloat<M, E>::BigFloat(double initVal) {
	printf("Constructor for BigFloat %p was called\n", this);
	operator=(initVal);
}

template <int M, int E>
BigFloat<M, E>::~BigFloat() {
	printf("Destructor for BigFloat %p was called\n", this);
}


// BigFloat - Copy constructor
template <int M, int E>
BigFloat<M, E>::BigFloat(const BigFloat<M, E>& b) :
	mExp(b.mExp), mMantisse(b.mMantisse), mSgn(b.mSgn) {
		printf("Copied %p into new object %p\n", &b, this);
}

//  BigFloat - Move constructor
template<int M, int E>
BigFloat<M, E>::BigFloat(BigFloat<M, E>&& b) :
	mExp(std::move(b.mExp)), mMantisse(std::move(b.mMantisse)), mSgn(b.mSgn) {
	printf("Moved %p into new object %p\n", &b, this);
}

// BigFloat -Copy operator
template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator=(const BigFloat<M, E>& b) {
	printf("Copying %p to %p\n", &b, this);
	mExp = b.mExp;
	mMantisse = b.mMantisse;
	mSgn = b.mSgn;
	return *this;
}

// BigFloat - Move operator
template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator=(BigFloat<M, E>&& b)  {
	printf("Moving %p to %p\n", &b, this);
	mExp = std::move(b.mExp);
	mMantisse = std::move(b.mMantisse);
	mSgn = b.mSgn;
}

// a primitive attempt to convert the BigFloat into a String.
// Maximum ten digits before the floating point.
template <int M, int E>
std::string BigFloat<M, E>::toString(int maxLen) {
	int exp = mExp.getVal();
	std::string s = "";
	if (mSgn < 0)
		s += '-';
	//first, convert part in front of the floating point
	int numBits = 1 + exp;
	if (numBits > 64)
		numBits = 64;

	if (numBits <= 0){
		s += '0';
	} else {
		unsigned long long n = 0;
		n = mMantisse[0];
		numBits -= 8;
		int i = 1;
		while (numBits > 0) {
			n <<= 8;
			n |= mMantisse[i];
			++i;
			numBits -= 8;
		}
		n >>= -numBits;
		
		s += std::to_string(n);
	}
	//then the part after the dot
	s += ".";
	//TODO
	s += "xxxx";
	return s;
}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator=(double d) {
	int exp = 0;
	mSgn = d >= 0 ? 1 : -1;
	d *= mSgn; //get absolute
	 //shift bits until d = 1.xxx
	while (d < 1 && exp > Exponent::MIN) {
		d *= 2.;
		exp -= 1;
	}
	while (d >= 2 && exp < Exponent::MAX) {
		d /= 2;
		exp += 1;
	}
	mMantisse = d;
	mExp = exp;

	for (int x = 0; x < M; ++x) {
		for (int i = 0; i < 8; ++i) {
			printf("%i", (mMantisse[x] & (1 << (7 - i))) > 0);
		}
		printf(" ");
	}
	printf("\texp: %d\n", exp);
	return *this;
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
	BigFloat a(*this);
	a -= b;
	return a;
}

template <int M, int E>
BigFloat<M, E> BigFloat<M, E>::operator* (const BigFloat<M, E>& b) const {
	BigFloat a(*this);
	a *= b;
	return a;
}

template <int M, int E>
BigFloat<M, E> BigFloat<M, E>::operator/ (const BigFloat<M, E>& b) const {
	BigFloat a(*this);
	a /= b;
	return a;
}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator+= (const BigFloat<M, E>& b) {
	int exp1 = mExp.getVal();
	int exp2 = b.mExp.getVal();
	mMantisse <<= 4;
	return *this;
}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator-= (const BigFloat<M, E>& b) {
	//TODO
}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator*= (const BigFloat<M, E>& b) {
	//TODO
}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator/= (const BigFloat<M, E>& b) {
	//TODO
}

template <int M, int E>
bool BigFloat<M, E>::operator< (const BigFloat<M, E>& b) const {
	if (b.mSgn == mSgn) {
		if (mSgn > 0)
			return mExp < b.mExp || (mExp == b.mExp && mMantisse < b.mMantisse);
		else
			return mExp > b.mExp || (mExp == b.mExp && mMantisse > b.mMantisse);
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
	return mExp > b.mExp || (mExp == b.mExp && mMantisse > b.mMantisse);
}

template <int M, int E>
BigFloat<M, E> BigFloat<M, E>::abs() const {
	BigFloat b(*this);
	b.setSign(0);
	return b;
}

template<int M, int E>
double BigFloat<M, E>::getDouble()
{
	double d = mSgn * mMantisse.getVal();
	int exp = mExp.getVal();
	while (exp > 0) {
			d *= 2.;
			--exp;
	}
	while (exp < 0) {
		d /= 2.;
		++exp;
	}
	return d;
			
}

//set to 1 or -1
template <int M, int E>
void BigFloat<M, E>::setSign(int sgn) {
	if (sgn == 1 || sgn == -1)
		mSgn = sgn;
}

int main() {
	//BigFloat<> a(0.2432342);
	BigFloat<> b(30201516.45462);
	b += b;
	//BigFloat<> t(-304234);
	//printf("BigFloat<> c  = -a\n");
	//BigFloat<> c = -a;
//	printf("smaller: %d, equal: %d\n", a < b, a == b);
	//printf("%s, %f\n", a.toString().c_str(), a.getDouble());
	printf("%s, %f\n", b.toString().c_str(), b.getDouble());
	//printf("%s, %f\n", t.toString().c_str(), t.getDouble());
	//printf("%s, %f\n", c.toString().c_str(), c.getDouble());
	
	

	return 0;
}
