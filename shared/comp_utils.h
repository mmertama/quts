/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef COMP_UTILS_H
#define COMP_UTILS_H

#ifdef BROKEN_COMPILER

template <class LIST, typename ...T>
LIST c_conv(const T& ... argList) {
    return LIST(argList...);
}

template <class LIST, typename ... T>
LIST c_fromPair(const std::initializer_list<T...>& lst) {
    LIST l;
    for(const auto& i : lst) {
        l.insert(i.first, i.second);
    }
    return l;
}

template <class LIST, typename ... T>
LIST c_add(const std::initializer_list<T...>& lst) {
    LIST l;
    for(const auto& i : lst) {
        l.append(i);
    }
    return l;
}
#endif

#endif // COMP_UTILS_H
