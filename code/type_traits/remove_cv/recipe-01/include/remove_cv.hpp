#pragma once

namespace mini_stl {

template <typename T> struct remove_cv                   { typedef T type; };
template <typename T> struct remove_cv<const T>          { typedef T type; };
template <typename T> struct remove_cv<volatile T>       { typedef T type; };
template <typename T> struct remove_cv<const volatile T> { typedef T type; };
 
template <typename T> struct remove_const                { typedef T type; };
template <typename T> struct remove_const<const T>       { typedef T type; };
 
template <typename T> struct remove_volatile             { typedef T type; };
template <typename T> struct remove_volatile<volatile T> { typedef T type; };

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

}   // namespace mini_stl
