//
// Created by goforbroke on 05.12.17.
//

// g++ --std=c++11 -o stepic-derivative stepic-derivative.cpp

#include <iostream>
#include <list>
#include <vector>

std::vector<std::string> strSplit(std::string str, std::vector<std::string> delimiters) {
    std::vector<std::string> r;

    size_t pos = 0;
    std::string delimiter = "";

    do {
        pos = str.length();

        for (auto it = delimiters.begin(); it != delimiters.end(); it++) {
            size_t p = str.find(*it);
            if (p < pos && p != std::string::npos) {
                pos = p;
                delimiter = *it;
            }
        }

        r.push_back(str.substr(0, pos));
        str.erase(0, pos + delimiter.length());
    } while (str.length() > 0);

    return r;
}

template<typename T, typename P>
T remove_if(T beg, T end, P pred) {
    T dest = beg;
    for (T itr = beg; itr != end; ++itr)
        if (!pred(*itr))
            *(dest++) = *itr;
    return dest;
}

class DerivativeMember {
public:
    int multiply;
    int degree;

    std::string toString(void) {
        if (degree > 1) return multiply + "*x^" + degree;
        else return multiply + "*x";
    }

    static DerivativeMember *fromString(std::string basic_string) {
        DerivativeMember *m = new DerivativeMember();
        std::vector<std::string> p = strSplit(basic_string, {"x^", "x"});
        m->multiply = std::atoi(p[0].c_str());
        m->degree = std::atoi(p[1].c_str());
        return m;
    }
};

//template<class TBase>
class DerivativeVector {
private:
    std::list<DerivativeMember *> m_parts;

    static std::vector<std::string> strSplit(std::string str) {
        std::vector<std::string> delimiters = {"+", "-"};
        std::vector<std::string> r;

        size_t pos = 0;
        std::string delimiter = "";

        do {
            pos = str.length();

            for (auto it = delimiters.begin(); it != delimiters.end(); it++) {
                size_t p = str.find(*it);
                if (p < pos && p != std::string::npos) {
                    pos = p;
                    delimiter = *it;
                }
            }

            r.push_back((delimiter == "-" ? "-" : "") + str.substr(0, pos));
            str.erase(0, pos + delimiter.length());
        } while (str.length() > 0);

        return r;
    }

public:
    void appendAll(std::string part) {

        part.erase(remove_if(part.begin(), part.end(), isspace), part.end());
        std::vector<std::string> parts = DerivativeVector::strSplit(part);

        for (auto it = parts.begin(); it != parts.end(); it++) {
            m_parts.push_back(DerivativeMember::fromString(*it));
        }

        // remove spaces
        // split by "+"
        // split by "-"
        // append
    }

    void append(std::string part) {
        m_parts.push_back(
                DerivativeMember::fromString(part)
        );
    }

    void simplify() {
        for (auto it1 = m_parts.begin(); it1 != m_parts.end(); it1++) {
            for (auto it2 = m_parts.begin(); it2 != m_parts.end(); it2++) {
                if (it1 == it2) continue;

                if ((*it1)->degree == (*it2)->degree) {
                    DerivativeMember *m = new DerivativeMember();
                    m->multiply = (*it1)->multiply + (*it2)->multiply;
                    m->degree = (*it1)->degree;
                    m_parts.push_back(m);

                    it1 = m_parts.erase(it1);
                    it2 = m_parts.erase(it2);

                    continue;
                }

                ++it1;
                ++it2;
            }
        }
    }

    std::string toString(void) {
        std::string r;
        for (auto it = m_parts.begin(); it != m_parts.end(); it++) {
//            r += ((r.length() > 0 && (*it)->multiply > 0) ? "+" : "") + (*it)->toString();
            r += "+" + (*it)->toString();
        }
        return r;
    }
};

std::string derivative(std::string polynomial) {
    return (std::string) "";
}

int main(int argc, char **argv) {
    DerivativeVector *v = new DerivativeVector;
    v->appendAll("200*x^99+200*x");

    std::cout << v->toString();

    return 0;
}
