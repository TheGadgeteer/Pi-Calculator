#include <stdio.h>
#include <string>
#include <string.h>
#include "BigFloat.h" 

template <int M, int E>
BigFloat<M, E>::Exponent::Exponent(int val) {
	printf("\tConstructor of Exponent %p was called\n", this);
	mExp = new u_char[E];
	*this = val;
}

// Exponent - Copy Constructor
template <int M, int E>
BigFloat<M, E>::Exponent::Exponent(const Exponent& e) 
{
	printf("\tCopying Exponent %p in new object %p\n", &e, this);
	mExp = new u_char[E];
	memcpy(mExp, e.mExp, E);
}

// Exponent - Move Constructor
template <int M, int E>
BigFloat<M, E>::Exponent::Exponent(Exponent&& e)
{
	printf("\tMoving Exponent %p in new object %p\n", &e, this);
	mExp = e.mExp;
	e.mExp = nullptr;
}

//Copy operator
template <int M, int E>
typename BigFloat<M, E>::Exponent& BigFloat<M, E>::Exponent::operator=(const Exponent& e) {
	printf("\tCopying Exponent %p to %p\n", &e, this);
	memcpy(mExp, e.mExp, E);
	return *this;
}

//Move operator
template <int M, int E>
typename BigFloat<M, E>::Exponent& BigFloat<M, E>::Exponent::operator=(Exponent&& e) {
	printf("\tMoving Exponent %p to %p\n", &e, this);
	delete[] mExp;
	mExp = e.mExp;
	e.mExp = nullptr;
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
	return *this;
}

template <int M, int E>
typename BigFloat<M, E>::Exponent& BigFloat<M, E>::Exponent::operator-=(int i) {
	return *this = getVal() - i;
}

template <int M, int E>
bool BigFloat<M, E>::Exponent::operator<(const Exponent& e) const {
	for (int i = 0; i < E; ++i) {
		if (mExp[i] < e.mExp[i])
			return true;
		else if (mExp[i] > e.mExp[i])
			return false;
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

// Mantisse - Constructor
template <int M, int E>
BigFloat<M, E>::Mantisse::Mantisse(double val) {
	printf("\tConstructor of Mantisse %p was called\n", this);
	mMantisse = new u_char[M];
	*this = val;
}

// Mantisse - Copy Constructor
template <int M, int E>
BigFloat<M, E>::Mantisse::Mantisse(const Mantisse& m) {
	printf("\tCopying Mantisse %p into new Object %p\n", &m, this);
	mMantisse = new u_char[M];
	memcpy(mMantisse, m.mMantisse, M);
}

// Mantisse - Move Constructor
template <int M, int E>
BigFloat<M, E>::Mantisse::Mantisse(Mantisse&& m) {
	printf("\tMoving Mantisse %p into new object %p\n", &m, this);
	mMantisse = m.mMantisse;
	m.mMantisse = nullptr;
}

//Mantisse - Copy Operator
template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator=(const Mantisse& m) {
	printf("\tCopying Mantisse %p to %p\n", &m, this);
	memcpy(mMantisse, m.mMantisse, M);
	return *this;
}

//Mantisse - Move Operator
template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator=(Mantisse&& m) {
		printf("\tMoving Mantisse %p to %p\n", &m, this);
		delete[] mMantisse;
		mMantisse = m.mMantisse;
		m.mMantisse = nullptr;
		return *this;
}


template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator>>=(int shift)
{
	if (shift < 0)
		return operator<<=(-shift);

	for (int i = M * 8 - 1 - shift; i >= 0; --i) {
		int oldSegment = i / 8, oldPosInSeg = 7 - i % 8;   //posInSeg:  0 is the most right, 7 the most left
		int newSegment = (i + shift) / 8, newPosInSeg = 7 - (i + shift) % 8;

		if ((mMantisse[oldSegment] & (1 << oldPosInSeg)) != 0)
			mMantisse[newSegment] |= (1 << newPosInSeg);   // set Bit
		else
			mMantisse[newSegment] &= 0xff - (1 << newPosInSeg);  // clear Bit
	}
	//clear the bits in front
	
	int i = 0;
	while (shift >= 8 && i < M) {
		shift -= 8;
		mMantisse[i++] = 0;
	}
	if (i < M)
		mMantisse[i] &= (0xff >> shift);
	
	return *this;
}


template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator<<=(int shift)
{
	if (shift < 0)
		return operator>>=(-shift);

	for (int i = shift; i < M * 8; ++i) {
		int oldSegment = i / 8, oldPosInSeg =  7 - i % 8;   //posInSeg:  0 is the most right, 7 the most left
		int newSegment = (i - shift) / 8, newPosInSeg = 7 - (i - shift) % 8;

		if ((mMantisse[oldSegment] & (1 << oldPosInSeg)) != 0)
			mMantisse[newSegment] |= (1 << newPosInSeg);   // set Bit
		else
			mMantisse[newSegment] &= 0xff - (1 << newPosInSeg);  // clear Bit
	}
	//clear the last bits
	int i = M - 1;
	while (shift >= 8 && i >= 0) {
		shift-= 8;
		mMantisse[i--] = 0;
	}
	if (i >= 0)
		mMantisse[i] &= (0xff << shift);

	return *this;
}

//Value is assumed to be between 0 and 2
template <int M, int E>
typename BigFloat<M, E>::Mantisse& BigFloat<M, E>::Mantisse::operator=(double val) {
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
double BigFloat<M, E>::Mantisse::getVal() const {
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
		else if (mMantisse[i] > m.mMantisse[i])
			return false;
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

// BigFloat - Copy operator
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
	return *this;
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

	return *this;
}

template<int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator=(long long val) {
	if (val < 0) {
		mSgn = -1;
		val = -val;
	} else {
		mSgn = 1;
	}
	u_char *pVal = (u_char*)&val;
	int i = 0;
	int shift = 63;
	for (int n = 7; n >= 0; --n) {
		u_char c = *(pVal + n);
		if (c == 0 && i == 0) { 
			shift -= 8;
		} else {
			mMantisse[i++] = c;
			if (i >= M)
				break;
		}
	}
	while (i < M)
		mMantisse[i++] = 0;
	if (val == 0) {
		mExp = Exponent::MIN;
	} else {
		mExp = shift;
		shift = 0;
		while (mMantisse.getBit(shift) == 0)
			shift++;
		mMantisse <<= shift;
		mExp -= shift;
	}
	return *this;
}

// |numerator| <= |denominator|;  nominator != 0, denominator != 0
// ggt(numerator, denominator) = 1
template<int M, int E>
BigFloat<M, E>& BigFloat<M, E>::assignFraction(long long numerator, long long denominator) {
	if (numerator == 0 || denominator == 0)
		return *this;
	int sgn = 1;
	if (numerator < 0) {
		numerator = -numerator;
		if (denominator < 0) {
			denominator = -denominator;
		} else {
				sgn = -1;
		}
	} else if (denominator < 0) {
		denominator = -denominator;
		sgn = -1;
	}

	*this = denominator;

	// make ggt(denominator, 2) = 1
	int exp = mExp;
	while (mMantisse.getBit(exp) == 0)
		exp--;
	int shift = mExp - exp;
	mExp = exp;
	// find t so denominator*t = (2^p - 1)
	BigFloat<> temp(*this);
	int t = 1;
	while(true) {
		bool isOne = true;
		for (int i = 0; i <= (int)mExp; ++i) {
			if (mMantisse.getBit(i) != 1) {
				isOne = false;
				break;
			}
		}
		if (isOne) {
			break;
		} else {
			t++;
			*this += temp;
		}
	}
	int p = (int)mExp + 1;
	*this = numerator;
	temp = t;
	*this *= temp;
	printf("periode %d, t %d, exp %d\n", p, t, (int)mExp);
	*this >>= p;
	if (p == 1 && mMantisse.getBit(0) == 1) {   // as 0.1111111... = 1.0
		printf("correcting ..\n");
		for (int i = 0; i < M; ++i)
			mMantisse[i] = 0;
		mMantisse.setBit(0);
		mExp += 1;
	}
	else {
		for (int i = p; i < M * 8; ++i) {
			if (mMantisse.getBit(i - p))
				mMantisse.setBit(i);
			else
				mMantisse.clearBit(i);
		}
	}
	mExp -= shift;
	mSgn = sgn;
	return *this;
}

// Writes BigFloat as String into out.
// maxLen: Max number of digits including the string-ending '\0'.
template <int M, int E>
void BigFloat<M, E>::toString(char *out, int maxLen) const {
	int exp = -1, idx = 0;  //exponent to BASE, idx: next free index in out array
	const BigFloat<M, E> BASE = 10;
	BigFloat<M, E> f(*this);
	if (f.mSgn < 0) {
		out[idx++] = '-';
	}
	if ((int)f.mExp < 0) {
		out[idx++] = '0';
		out[idx++] = '.';
	} else {
		do {
			f /= BASE;
			exp++;
		} while ((int)f.mExp >= 0);
	}
	// Digits in front of point
	while (idx < maxLen - 1) {
		f *= BASE;
		out[idx++] = f.floor() + 48;
		f.substractFloor();
		if (exp >= 0) {  // exp-- only needed if exp is still >= 0.
			if (exp == 0)
				out[idx++] = '.';
			exp--;
		}
	}
	out[idx] = '\0';
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
	if (mSgn == b.mSgn)
		return addAbs(b);
	else // case 1 : this pos, b neg -> this + b = |this| - |b|,
			// case 2: this neg, b pos -> this + b = -||this| - |b||
		return subAbs(b);
}

//performs |*this| += |b|,  completely ignoring any signs
template<int M, int E>
BigFloat<M, E>& BigFloat<M, E>::addAbs(const BigFloat<M, E>& b) {
	int exp1 = mExp.getVal();
	int exp2 = b.mExp.getVal();
	int offset = exp1 - exp2;
	if (offset < 0) { // If exp1 < exp2 : Shift Mantisse
		mMantisse >>= (-offset);
		mExp = exp2;
		offset = 0;
	}

	int carryBit = 0;
	//i is the index of the current bit in b.mMantisse;  the most right position has index 0
	for (int i = M * 8 - 1; i >= 0; --i) {
		int val;
		if (i >= offset)
			val = mMantisse.getBit(i) + b.mMantisse.getBit(i - offset) + carryBit;
		else {
			if (carryBit == 0)
				break;
			val = mMantisse.getBit(i) + carryBit;
		}
		if (val > 1) {
			val -= 2;
			carryBit = 1;
		} else
			carryBit = 0;
		//set Bit in Mantisse according to the result
		if (val == 0)
			mMantisse.clearBit(i);
		else
			mMantisse.setBit(i);
	}
	if (carryBit > 0) {
		mMantisse >>= 1;
		mExp += 1;
		mMantisse.setBit(0);
	}
	return *this;
}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator-= (const BigFloat<M, E>& b) {
	if (mSgn == b.mSgn) { //case 1: both pos  -> this - b = |this| - |b|;   case 2: both neg -> -|this| - -|b| = - ||this| - |b|| 
		return subAbs(b);
	} else   // case 1:  this pos, b neg -> this = this + |b|;   case 2:  this neg, b pos -> this = -|this| - b = -|this + b|
		return addAbs(b);   
}

//performs |*this| -= |b|,  completely ignoring any signs
template<int M, int E>
BigFloat<M, E>& BigFloat<M, E>::subAbs(const BigFloat<M, E>& b) {
	if (this->absSmallerThan(b)) { //this = this - b = -|b - this| 
		BigFloat<M, E>* pTemp = new BigFloat<M, E>(b);
		pTemp->subAbs(*this);
		pTemp->setSign(-mSgn);  // switch sign
		*this = std::move(*pTemp);
		delete pTemp;
	} else {
		int exp1 = mExp.getVal();
		int exp2 = b.mExp.getVal();
		int offset = exp1 - exp2;

		int carryBit = 0;
		//i is the index of the current bit in b.mMantisse;  the most right position has index 0
		for (int i = M * 8 - 1; i >= 0; --i) {
			int val;
			if (i >= offset) {
				val = mMantisse.getBit(i) - b.mMantisse.getBit(i - offset) - carryBit;
			}
			else {
				if (carryBit == 0)
					break;
				val = mMantisse.getBit(i) - carryBit;
			}
			if (val < 0) {
				val += 2;
				carryBit = 1;
			}
			else {
				carryBit = 0;
			}
			//set Bit in Mantisse according to the result
			if (val == 0)
				mMantisse.clearBit(i);
			else
				mMantisse.setBit(i);
		}
		if (carryBit > 0)
			printf("Something went wrong.");
		int shift = 0;
		while (shift < M * 8 && mMantisse.getBit(shift) == 0)
			shift++;
		if (shift == M * 8)  // result is 0.0
			mExp = Exponent::MIN;
		else {
			mExp -= shift;
			mMantisse <<= shift;
		}
	}
	return *this;
}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator*= (const BigFloat<M, E>& b) {
	BigFloat<M, E> result(0.);
	result.setSign(mSgn == b.mSgn ? 1 : -1);
	int midExp = mExp.getVal() + b.mExp.getVal();
	for (int i = M*8 - 1; i >= 0; --i) {
		if (b.mMantisse.getBit(i)) {
			mExp = midExp - i;
			result.addAbs(*this);
		}
	}
	*this = std::move(result);
	return *this;
}

template <int M, int E>
BigFloat<M, E>& BigFloat<M, E>::operator/= (const BigFloat<M, E>& b) {
	//TODO

	return *this;
}

template <int M, int E>
bool BigFloat<M, E>::operator< (const BigFloat<M, E>& b) const {
	if (b.mSgn == mSgn) {
		if (mSgn > 0)  // both numbers positive
			return mExp < b.mExp || (mExp == b.mExp && mMantisse < b.mMantisse);
		else		   // both numbers negative
			return mExp > b.mExp || (mExp == b.mExp && mMantisse > b.mMantisse);
	}
	else {
		if (mSgn == 1)  // this positive, b negative
			return false;
		else			// this negative, b positive
			return true;
	}

}

template <int M, int E>
bool BigFloat<M, E>::operator== (const BigFloat<M, E>& b) const {
	return mMantisse == b.mMantisse && mExp == b.mExp && mSgn == b.mSgn;
}

template <int M, int E>
bool BigFloat<M, E>::absSmallerThan(const BigFloat<M, E>& b) const {
	return mExp < b.mExp || (mExp == b.mExp && mMantisse < b.mMantisse);
}
template <int M, int E>
BigFloat<M, E> BigFloat<M, E>::abs() const {
	BigFloat b(*this);
	b.setSign(0);
	return b;
}

template<int M, int E>
double BigFloat<M, E>::toDouble() const {
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

template<int M, int E>
int BigFloat<M, E>::floor() const {
	int exp = mExp.getVal();
	exp = mExp;
	int ret = 0;
	int idx = 0;
	while (exp >= 0 && idx < M) {
		ret <<= 8;
		ret |= (unsigned int)mMantisse[idx++];
		exp -= 8;
	}
	ret >>= -exp - 1;
	return mSgn * ret;
}

template<int M, int E>
BigFloat<M, E>& BigFloat<M, E>::substractFloor() {
	int exp = mExp;
	if (exp >= 0) {
		mMantisse <<= exp + 1;
		mExp = -1;
	}
	return *this;
}

//set to 1 or -1
template <int M, int E>
void BigFloat<M, E>::setSign(int sgn) {
	if (sgn == 1 || sgn == -1)
		mSgn = sgn;
}


template<int M, int E>
void printBits(const BigFloat<M, E>& b) {
	printf("%f:\t", b.toDouble());
	for (int x = 0; x < M; ++x) {
		for (int i = 0; i < 8; ++i) {
			printf("%i", (b.mMantisse[x] & (1 << (7 - i))) > 0);
		}
		printf(" ");
	}
	printf("\texp: %d\n", b.mExp.getVal());
}


void checkMem() {
	// Send all reports to STDOUT
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	printf("Checked for Memory Leaks: %d\n", _CrtDumpMemoryLeaks());
	//system("pause");
}

int main() {
	for (int i = 1; i < 50; ++i) {
		printf("\n%d: %f\n\n", i, BigFloat<>().assignFraction(1, i).toDouble());
	}
	return 0;
	BigFloat<> a(340000.00031);
	BigFloat<> b(-304234);
	BigFloat<> c(1.003125);
	BigFloat<> d(-0.01);
	BigFloat<> e(0.9734);
	BigFloat<> f = -a;
	//printf("\nresults: %f, %f ; %f, %f\n\n", (a - b).toDouble(), (b - a).toDouble(), (-b - -a).toDouble(), (-a - -b).toDouble());  // correct

	printf("\nresults: %f, %f\n", (a * b).toDouble(), (b * a).toDouble());
	printf("\nresults: %f, %f\n", (d * d).toDouble(), (e * f).toDouble());
	//printf("\nresults: %f, %f, %f, %f\n", (t + c).toDouble(), (c + t).toDouble(), (a + d).toDouble(), (a + c).toDouble()); //correct

	const int LEN = 20;
	char numbers[5][LEN];
	/*a.toString(numbers[0], LEN);
	b.toString(numbers[1], LEN);
	c.toString(numbers[2], LEN);
	d.toString(numbers[3], LEN);*/
	e.toString(numbers[4], LEN);

	/*printf("%s, %f\n", numbers[0], a.toDouble());
	printf("%s, %f\n", numbers[1], b.toDouble());
	printf("%s, %f\n", numbers[2], c.toDouble());
	printf("%s, %f\n", numbers[3], d.toDouble());*/
	printf("%s, %f\n", numbers[4], e.toDouble());
	

	printf("\nEND\n\n");
	return 0;
}
