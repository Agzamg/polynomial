#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class Polynomial {
private:
    map<size_t, T> mn;
public:
    void cut_zeroes() {
        for (auto i = mn.begin(); i != mn.end();) {
            if (i->second == T()) {
                i = mn.erase(i);
            } else {
                i++;
            }
        }
    }

    Polynomial<T>(const vector<T> &vec) {
        for (size_t i = 0; i < vec.size(); ++i) {
            mn[i] = vec[i];
        }
        cut_zeroes();
    }

    Polynomial<T>(const T &zero = T()) {
        mn[0] = (zero);
        cut_zeroes();
    }

    template<typename it>
    Polynomial<T>(it f, it l) {
        size_t c = 0;
        while (f != l) {
            mn[c] = *f;
            ++c;
            ++f;
        }
        cut_zeroes();
    }

    typename map<size_t, T>::const_iterator begin() const {
        return mn.begin();
    }

    typename map<size_t, T>::const_iterator end() const {
        return mn.end();
    }

    typename map<size_t, T>::const_reverse_iterator rbegin() const {
        return mn.rbegin();
    }

    typename map<size_t, T>::const_reverse_iterator rend() const {
        return mn.rend();
    }

    T operator[](size_t a) const {
        if (mn.count(a) == 0) {
            return T();
        }
        return mn.at(a);
    }

    int Degree() const {
        if (mn.empty()) {
            return -1;
        }
        return (mn.rbegin())->first;
    }

    bool operator==(const Polynomial<T> &buff) const {
        return mn == buff.mn;
    }

    bool operator==(const T &buff) const {
        return mn == Polynomial(buff).mn;
    }

    bool operator!=(const Polynomial<T> &buff) const {
        return !(mn == buff.mn);
    }

    bool operator!=(const T &buff) const {
        return !(mn == Polynomial(buff).mn);
    }

    Polynomial &operator+=(const Polynomial &sec) {
        for (auto i = sec.mn.begin();
             i != sec.mn.end(); ++i) {
            mn[i->first] += i->second;
        }
        cut_zeroes();
        return *this;
    }

    Polynomial &operator-=(const Polynomial &sec) {
        for (auto i = sec.mn.begin();
             i != sec.mn.end(); ++i) {
            mn[i->first] -= i->second;
        }
        cut_zeroes();
        return *this;
    }

    Polynomial &operator*=(const Polynomial &sec) {
        Polynomial<T> ans;
        for (auto i = mn.begin(); i != mn.end(); ++i) {
            for (auto j = sec.mn.begin(); j != sec.mn.end(); ++j) {
                ans.mn[i->first + j->first] += i->second * j->second;
            }
        }
        *this = ans;
        cut_zeroes();
        return *this;
    }

    T operator()(const T &x) const {
        T ans;
        for (auto it = mn.begin(); it != mn.end(); ++it) {
            T mult = T(1);
            for (int j = 0; j != it->first; j++) {
                mult *= x;
            }
            ans += it->second * mult;
        }
        return ans;
    }

    Polynomial<T> &operator/=(const Polynomial<T> &sec) {
        Polynomial ans, q(*this);
        if ((*this).Degree() < sec.Degree()) {
            *this = Polynomial<T>();
            return *this;
        }
        while (q.Degree() >= sec.Degree()) {
            int pos = q.Degree() - sec.Degree();
            ans.mn[pos] = q.rbegin()->second / sec.rbegin()->second;
            Polynomial<T> buff(ans);
            buff.mn[pos] *= T(2);
            buff -= ans;
            q -= buff * sec;
        }
        mn = ans.mn;
        cut_zeroes();
        return *this;
    }

};

template<typename T>
Polynomial<T> operator%(const Polynomial<T> &f, const Polynomial<T> &s) {
    Polynomial<T> buff(f);
    buff /= s;
    buff *= s;
    return f - buff;
}

template<typename T>
Polynomial<T> operator/(const Polynomial<T> &f, const Polynomial<T> &s) {
    Polynomial<T> ans(f);
    ans /= s;
    return ans;
}

template<typename T>
Polynomial<T> operator,(const Polynomial<T> &f, const Polynomial<T> &s) {
    if (s == T()) {
        return (f / Polynomial<T>(f[f.Degree()]));
    }
    return (s, f % s);
}

template<typename T>
Polynomial<T> operator+(const Polynomial<T> &f, const Polynomial<T> &s) {
    Polynomial<T> ans(f);
    ans += s;
    return ans;
}

template<typename T>
Polynomial<T> operator+(const Polynomial<T> &f, const T &s) {
    Polynomial<T> ans(f);
    ans += s;
    return ans;
}

template<typename T>
Polynomial<T> operator+(const T &f, const Polynomial<T> &s) {
    return s + f;
}

template<typename T>
Polynomial<T> operator-(const Polynomial<T> &f, const Polynomial<T> &s) {
    Polynomial<T> ans(f);
    ans -= s;
    return ans;
}

template<typename T>
Polynomial<T> operator-(const Polynomial<T> &f, const T &s) {
    Polynomial<T> ans(f);
    ans -= s;
    return ans;
}

template<typename T>
Polynomial<T> operator-(const T &f, const Polynomial<T> &s) {
    Polynomial<T> ans(f);
    return ans - s;
}

template<typename T>
Polynomial<T> operator*(const Polynomial<T> &f, const Polynomial<T> &s) {
    auto buff = f;
    buff *= s;
    return buff;
}

template<typename T>
Polynomial<T> operator*(const Polynomial<T> &f, const T &s) {
    return f * Polynomial<T>(s);
}

template<typename T>
Polynomial<T> operator*(const T &f, const Polynomial<T> &s) {
    return Polynomial<T>(f) * s;
}

template<typename T>
ostream &operator<<(ostream &stream, const Polynomial<T> &polynomial) {
    if (polynomial.Degree() == -1) {
        stream << 0;
        return stream;
    }
    if (polynomial.rbegin()->first == 0) {
        stream << polynomial[0];
        return stream;
    }
    for (auto it = polynomial.rbegin(); it != polynomial.rend(); ++it) {
        if (it->first == 0) {
            if (it->second < T()) {
                stream << it->second;
            } else {
                stream << '+' << it->second;
            }
            continue;
        }
        if (it->second > T()) {
            if (it != polynomial.rbegin()) {
                stream << '+';
            }
            if (it->second != T(1)) {
                stream << it->second << '*';
            }
        } else {
            if (it->second == T(-1)) {
                stream << '-';
            } else {
                stream << it->second << '*';
            }
        }
        stream << 'x';
        if (it->first != 1) {
            stream << '^' << it->first;
        }
    }
    return stream;
}

template<typename T>
Polynomial<T> operator&(const Polynomial<T> &f, const Polynomial<T> &s) {
    auto ans = Polynomial<T>();
    for (auto it = f.begin(); it != f.end(); it++) {
        Polynomial<T> buff = Polynomial<T>(it->second);
        for (size_t i = 0; i != it->first; ++i) {
            buff *= s;
        }
        ans += buff;
    }
    return ans;
}