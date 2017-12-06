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
    static DerivativeMember *tmp_member;
    static std::vector<std::string> tmp_parts;

public:
    int multiply;
    int degree;

    std::string toString() {
        if (0 == degree) return std::to_string(multiply);
        if (1 == degree) return std::to_string(multiply) + "*x";

        return std::to_string(multiply) + "*x^" + std::to_string(degree);
//        if (degree > 1) return multiply + "*x^" + degree;
//        else return multiply + "*x";
    }

    static DerivativeMember *fromString(std::string str) {
        tmp_member = new DerivativeMember();
        tmp_parts = strSplit(str, {"*x^", "*x"});
        tmp_member->multiply = std::atoi(tmp_parts[0].c_str());
        tmp_member->degree = (tmp_parts.size() > 1) ? std::atoi(tmp_parts[1].c_str()) : 1;
        return tmp_member;
    }
};

DerivativeMember *DerivativeMember::tmp_member;
std::vector<std::string> DerivativeMember::tmp_parts;

//template<class TBase>
class DerivativeVector {
private:
    std::list<DerivativeMember *> m_parts;

    static std::vector<std::string> strSplit(std::string str) {
        std::vector<std::string> delimiters = {"+", "-"};
        std::vector<std::string> r;

        size_t pos = 0;
        size_t p;
        std::string delimiter;

        std::vector<std::string>::iterator it;

        do {
            pos = str.length();

            for (it = delimiters.begin(); it != delimiters.end(); it++) {
                p = str.find(*it);
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

//        part.erase(remove_if(part.begin(), part.end(), isspace), part.end());
        std::vector<std::string> parts = DerivativeVector::strSplit(part);
//        std::vector<std::string> parts;

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

    void runDerivative() {
        for (auto it = m_parts.begin(); it != m_parts.end();) {
            DerivativeMember *m = (*it);
            if (0 == m->degree) {
                it = m_parts.erase(it);
                continue;
            }

            m->multiply *= m->degree;
            m->degree--;

            it++;
        }
    }

    void simplify() {
        DerivativeMember *m;
        std::list<DerivativeMember *> tmp_parts;

        DerivativeMember *fixed;

        for (auto it1 = m_parts.begin(); it1 != m_parts.end();) {

            fixed = (*it1);

            for (auto it2 = m_parts.begin(); it2 != m_parts.end();) {
                if (it1 == it2) {
                    ++it2;
                    continue;
                }

                if ((*it1)->degree == (*it2)->degree) {
                    fixed->multiply += (*it2)->multiply;
                    it2 = m_parts.erase(it2);
                    continue;
                }

                ++it2;
            }

            it1 = m_parts.erase(it1);

            tmp_parts.push_back(fixed);
        }

        for (auto it1 = m_parts.begin(); it1 != m_parts.end(); it1++) {
            delete *it1;
        }
        m_parts.clear();
        m_parts = tmp_parts;
    }

    std::string toString() {
        std::string r;
        for (auto it = m_parts.begin(); it != m_parts.end(); it++) {
//            r += ((r.length() > 0 && (*it)->multiply > 0) ? "+" : "") + (*it)->toString();
            r += (it != m_parts.begin() ? "+" : "") + (*it)->toString();
        }
        return r;
    }
};

std::string derivative(std::string polynomial) {
    return (std::string) "";
}

int main(int argc, char **argv) {
    auto *v = new DerivativeVector;
    v->appendAll("200*x^99 + 200*x^5 + 10*x^10 + 2*x^5");
    v->simplify();
    v->runDerivative();
    std::cout << v->toString();
    delete (v);
    return 0;
}
