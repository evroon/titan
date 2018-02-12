#pragma once

#include "utility/StringUtils.h"

class String;

class Real
{
public:
	Real() { i = 0; t = INT; }
	Real(signed integer) { i = integer; t = INT; }
	Real(unsigned integer) { i = integer; t = INT; }
	Real(float doub) { f = doub; t = FLOAT; }

	Real(const String &text);

	String to_string() const;

	Real& operator+=(const Real &n) { if (t == INT) i += (n.t == INT ? n.i : (int)n.f); else f += (n.t == INT ? n.i : n.f); return *this; }
	Real& operator-=(const Real &n) { if (t == INT) i -= (n.t == INT ? n.i : (int)n.f); else f -= (n.t == INT ? n.i : n.f); return *this; }
	Real& operator*=(const Real &n) { if (t == INT) i *= (n.t == INT ? n.i : (int)n.f); else f *= (n.t == INT ? n.i : n.f); return *this; }
	Real& operator/=(const Real &n) { if (t == INT) i /= (n.t == INT ? n.i : (int)n.f); else f /= (n.t == INT ? n.i : n.f); return *this; }

	Real operator+(const Real &n) const { return Real((t == INT ? i : f) + (n.t == INT ? n.i : n.f)); }
	Real operator-(const Real &n) const { return Real((t == INT ? i : f) - (n.t == INT ? n.i : n.f)); }
	Real operator*(const Real &n) const { return Real((t == INT ? i : f) * (n.t == INT ? n.i : n.f)); }
	Real operator/(const Real &n) const { return Real((t == INT ? i : f) / (n.t == INT ? n.i : n.f)); }

	bool operator>(const Real &n) const { return (t == INT ? i : f) > (n.t == INT ? n.i : n.f); }
	bool operator>=(const Real &n) const { return (t == INT ? i : f) >= (n.t == INT ? n.i : n.f); }
	bool operator==(const Real &n) const { return (t == INT ? i : f) == (n.t == INT ? n.i : n.f); }
	bool operator!=(const Real &n) const { return (t == INT ? i : f) != (n.t == INT ? n.i : n.f); }
	bool operator<(const Real &n) const { return (t == INT ? i : f) < (n.t == INT ? n.i : n.f); }
	bool operator<=(const Real &n) const { return (t == INT ? i : f) <= (n.t == INT ? n.i : n.f); }

	bool operator==(const float &r) const { return (t == INT ? i : f) == (t == INT ? (int)r : (float)r); }
	bool operator==(const int &r) const { return (t == INT ? i : f) == (t == INT ? (int)r : (float)r); }

	operator int() const { return t == INT ? i : (int)f; }
	operator float() const { return t == FLOAT ? f : (float) i; }

	operator String() const;

	union { int i; float f; };
	enum Type { INT, FLOAT };

	int t = INT;
};