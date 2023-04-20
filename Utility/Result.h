//
// Created by jantypas on 4/19/23.
//

#ifndef ONYXSIM_RESULT_H
#define ONYXSIM_RESULT_H
#include <variant>

/**
 * Result template -- A template to capture success and failure results from a function
 *
 * @tparam S - The structure we get on success
 * @tparam E - The structure we return on error
 *
 * Structures will need a copy constructor
 */
template <class S, class E> class Result {
private :
    bool isResultOK = false;        // Did we have an error
    std::variant<S, E> container;   // We store our success and error objects here
public :
    /**
     * Use this function to set the success object
     * @param s
     */
    void SUCCESS(S s) {
        isResultOK = true;  // No errors!
        container = s;      // Copy the object
    };
    /**
     * Use this function to set the error object
     * @param e
     */
    void ERROR(E e) {
        isResultOK = false; // We got an error
        container = e;      // Copy the object
    };
    /**
     * Did we have an error
     * @return boolean based on the error status
     */
    bool isOK() {
        return isResultOK;
    };
    /**
     * Return a copy of the error object.
     * NOTE: If the object does not exist because we did not have error,
     * we will get an index error.
     * @return
     */
    E GetError() {
        return std::get<E>(container);
    };
    /**
     * Return a copy of the success object.
     * NOTE: If the object does not exist because we did have error,
     * we will get an index error.
     * @return
     */
    S GetSuccess() {
        return std::get<S>(container);
    };
};

#endif //ONYXSIM_RESULT_H
