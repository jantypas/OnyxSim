//
// Created by jantypas on 4/19/23.
//

#ifndef ONYXSIM_RESULT_H
#define ONYXSIM_RESULT_H
#include <variant>

template <class S, class E> class Result {
private :
    bool isResultOK = false;
    std::variant<S, E> container;
public :
    void SUCCESS(S s) {
        isResultOK = true;
        container = s;
    };
    void ERROR(E e) {
        isResultOK = false;
        container = e;
    };
    bool isOK() {
        return isResultOK;
    };
    E GetError() {
        return std::get<E>(container);
    };
    S GetSuccess() {
        return std::get<S>(container);
    };
};

#endif //ONYXSIM_RESULT_H
