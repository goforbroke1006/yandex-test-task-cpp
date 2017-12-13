//
// Created by goforbroke on 13.12.17.
//

#include <iostream>

class StringPointer {
private:
    std::string *poiner;
public:
    std::string *operator->() { return poiner; }

    operator std::string *() {
        return poiner;
    }

    StringPointer(std::string *strPtr) {
        if (strPtr)
            poiner = new std::string(*strPtr);
        else
            poiner = new std::string("");
    }

    ~StringPointer() {
        poiner->erase();
        poiner->clear();
        poiner->shrink_to_fit();
        delete poiner;
    }
};

int main() {

    std::string s1 = "Hello, world!";

    StringPointer sp1(&s1);
    StringPointer sp2(NULL);

    std::cout << sp1->length() << std::endl;
    std::cout << *sp1 << std::endl;
    std::cout << sp2->length() << std::endl;
    std::cout << *sp2 << std::endl;

    return 0;
}