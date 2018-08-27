// Float type with Exponent Bias Shift, Normalized Mantisse. The leading 1 is kept. Saved in Big Endian.
#include <string.h>

typedef unsigned char u_char;

template <int M=4, int E=2>
class BigFloat {
private:

	class Exponent;
	class Mantisse;

	char mSgn;
	Exponent mExp;
	Mantisse mMantisse;

	void setSign(int sgn);
public:
	BigFloat(double init=0.);
	BigFloat(const BigFloat<M, E>& b);
	BigFloat(BigFloat<M, E>&& b);
	~BigFloat();

	std::string toString(int maxLen = 100);
	BigFloat<M, E>& operator= (double d);
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
	bool operator< (const BigFloat<M, E>& b) const;
	bool operator> (const BigFloat<M, E>& b) const { return (b < *this); }
	bool operator== (const BigFloat<M, E>& b) const;
	bool operator!= (const BigFloat<M, E>& b) const { return !(operator==(b)); }
	bool operator<= (const BigFloat<M, E>& b) const { return (operator<(b) || operator==(b));}
	bool operator>= (const BigFloat<M, E>& b) const { return (operator>(b) || operator==(b));}
	bool absGreaterThan (const BigFloat<M, E>& b) const;
	BigFloat<M, E> abs() const;
	double getDouble();

};

template <int M, int E>
class BigFloat<M, E>::Exponent {
private:
	u_char *mExp;
	static const unsigned int mBias = (1 << (E * 8 - 1)) - 1;
public:
	static const int MAX = mBias;
	static const int MIN = -(int)mBias;

	Exponent(int val=0);
	Exponent(const Exponent& e);
	Exponent(Exponent&& e);
	~Exponent() { delete[] mExp; printf("Destructed Exponent %p\n", this);}

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
	~Mantisse() { delete[] mMantisse; printf("Destructed Mantisse %p\n", this);}

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

	double getVal();
};
