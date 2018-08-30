// Float type with Exponent Bias Shift, Normalized Mantisse. The leading 1 is kept. Saved in Big Endian.
#include <string.h>

typedef unsigned char u_char;

template <int M=8, int E=2>
class BigFloat {
	template <int M, int E>
	friend void printBits(const BigFloat<M,E>&); //TODO: delete later
private:
	class Exponent;
	class Mantisse;

	char mSgn;
	Exponent mExp;
	Mantisse mMantisse;

	BigFloat<M, E>& BigFloat<M, E>::addAbs(const BigFloat<M, E>& b);
	BigFloat<M, E>& BigFloat<M, E>::subAbs(const BigFloat<M, E>& b);
public:
	BigFloat(double init=0.);
	BigFloat(long long init) { *this = init; }
	BigFloat(int init) : BigFloat((long long)init) {}
	BigFloat(const BigFloat<M, E>& b);
	BigFloat(BigFloat<M, E>&& b);
	~BigFloat();

	const Exponent& getExp() const { return mExp; }
	const Mantisse& getMantisse() const { return mMantisse;  }
	char getSign() { return mSgn; }
	void setSign(int sgn);

	void assignFraction(long long numerator, long long denominator);
	void BigFloat<M, E>::toString(char *out, int maxLen) const;
	BigFloat<M, E>& operator= (double d);
	BigFloat<M, E>& operator= (long long val);
	BigFloat<M, E>& operator= (int val) { return operator=(val); }
	BigFloat<M, E>& operator= (const BigFloat<M, E>& b);
	BigFloat<M, E>& operator= (BigFloat<M, E>&& b);
	BigFloat<M, E> operator- () const;
	BigFloat<M, E> operator+ (const BigFloat<M, E>& b) const;
	BigFloat<M, E> operator- (const BigFloat<M, E>& b) const;
	BigFloat<M, E> operator* (const BigFloat<M, E>& b) const;
	BigFloat<M, E> operator/ (const BigFloat<M, E>& b) const;
	BigFloat<M, E>& operator+= (const BigFloat<M, E>& b);
	BigFloat<M, E>& operator-= (const BigFloat<M, E>& b);
	BigFloat<M, E>& operator*= (const BigFloat<M, E>& b);
	BigFloat<M, E>& operator/= (const BigFloat<M, E>& b);
	BigFloat<M, E>& operator<<= (int shift) { mExp += shift; }
	BigFloat<M, E>& operator>>= (int shift) { mExp -= shift; }
	bool operator< (const BigFloat<M, E>& b) const;
	bool operator> (const BigFloat<M, E>& b) const { return (b < *this); }
	bool operator== (const BigFloat<M, E>& b) const;
	bool operator!= (const BigFloat<M, E>& b) const { return !(operator==(b)); }
	bool operator<= (const BigFloat<M, E>& b) const { return (operator<(b) || operator==(b));}
	bool operator>= (const BigFloat<M, E>& b) const { return (operator>(b) || operator==(b));}
	operator double() const { return this->getDouble(); }
	operator int() const { return this->floor(); }

	bool absSmallerThan(const BigFloat<M, E>& b) const;
	BigFloat<M, E> abs() const;
	double getDouble() const;
	int floor() const;
	BigFloat<M, E>& substractFloor();  //needed for toString() function


};

template <int M, int E>
class BigFloat<M, E>::Exponent {
private:
	u_char *mExp;
	static const unsigned int mBias = (1 << (E * 8 - 1)) - 1;
public:
	static const int MAX = mBias + 1;
	static const int MIN = -(int)mBias;

	Exponent(int val=0);
	Exponent(const Exponent& e);
	Exponent(Exponent&& e);
	~Exponent() { delete[] mExp; printf("\tDestructed Exponent %p\n", this);}

	Exponent& operator=(int val);
	Exponent& operator=(const Exponent& e);
	Exponent& operator=(Exponent&& e);
	Exponent& operator+=(int i);
	Exponent& operator+=(const Exponent& e) { *this += e.getVal(); }
	Exponent& operator-=(int i);
	Exponent& operator-=(const Exponent& e) { *this += e.getVal(); }
	bool operator<(const Exponent& e) const;
	bool operator>(const Exponent& e) const {return e < *this;}
	bool operator==(const Exponent& e) const;
	bool operator!=(const Exponent& e) const {return !(*this == e);}
	bool operator<=(const Exponent& e) const {return *this < e || *this == e;}
	bool operator>=(const Exponent& e) const {return *this > e || *this == e;}
	operator int() const { return this->getVal(); }

	int getVal() const;
};

// Only takes Values in [0, 2). Stored in Big Endian. Format:  x.xxxxx
template <int M, int E>
class BigFloat<M, E>::Mantisse{
	friend class BigFloat;
private:
	u_char *mMantisse;

	Mantisse& operator>>=(int shift);   //The shift operations are private as they only need to be used for the arithmetic operations
	Mantisse& operator<<=(int shift);

public:
	Mantisse(double val=0.);
	Mantisse(const Mantisse& m);
	Mantisse(Mantisse&& m);
	~Mantisse() { delete[] mMantisse; printf("\tDestructed Mantisse %p\n", this);}

	Mantisse& operator=(double val);
	Mantisse& operator=(const Mantisse& m);
	Mantisse& operator=(Mantisse&& m);

	bool operator<(const Mantisse& e) const;
	bool operator>(const Mantisse& e) const {return e < *this;}
	bool operator==(const Mantisse& e) const;
	bool operator!=(const Mantisse& e) const {return !(*this == e);}
	bool operator<=(const Mantisse& e) const {return *this < e || *this == e;}
	bool operator>=(const Mantisse& e) const {return *this > e || *this == e;}
	u_char& operator[](std::size_t idx) { return mMantisse[idx]; }
	const u_char& operator[](std::size_t idx) const { return mMantisse[idx]; }

	// The Index 0 is the most left, the Index M*8 - 1 the most right.
	int getBit(int idx) const { return (mMantisse[idx / 8] & (1 << (7 - (idx % 8)))) != 0; }
	void setBit(int idx) { mMantisse[idx / 8] |= (1 << (7 - (idx % 8))); }
	void clearBit(const int idx) { mMantisse[idx / 8] &= 0xff - (1 << (7 - (idx % 8))); }

	double getVal() const;
};
