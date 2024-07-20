// align_helpers.h
#ifndef ALIGN_HELPERS_H
#define ALIGN_HELPERS_H

#include <iostream>
#include <string>
#include <iomanip>

/*
Template for auto-aligning (left, center, right) output stream (act like std::left or std::right)
Originaly copied from: https://stackoverflow.com/questions/14861018/center-text-in-fixed-width-field-with-stream-manipulators-in-c
Then expanded to support left and right alignment with GPT 4o for it originally only support center alignment
*/

// Center alignment helper
template<typename charT, typename traits = std::char_traits<charT> >
class center_helper {
    std::basic_string<charT, traits> str_;
public:
    center_helper(std::basic_string<charT, traits> str) : str_(str) {}
    template<typename a, typename b>
    friend std::basic_ostream<a, b>& operator<<(std::basic_ostream<a, b>& s, const center_helper<a, b>& c);
};

template<typename charT, typename traits = std::char_traits<charT> >
center_helper<charT, traits> centered(std::basic_string<charT, traits> str) {
    return center_helper<charT, traits>(str);
}

center_helper<std::string::value_type, std::string::traits_type> centered(const std::string& str) {
    return center_helper<std::string::value_type, std::string::traits_type>(str);
}

template<typename charT, typename traits>
std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& s, const center_helper<charT, traits>& c) {
    std::streamsize w = s.width();
    if (w > c.str_.length()) {
        std::streamsize left = (w + c.str_.length()) / 2;
        s.width(left);
        s << c.str_;
        s.width(w - left);
        s << "";
    } else {
        s << c.str_;
    }
    return s;
}

// Left alignment helper
template<typename charT, typename traits = std::char_traits<charT> >
class left_helper {
    std::basic_string<charT, traits> str_;
public:
    left_helper(std::basic_string<charT, traits> str) : str_(str) {}
    template<typename a, typename b>
    friend std::basic_ostream<a, b>& operator<<(std::basic_ostream<a, b>& s, const left_helper<a, b>& l);
};

template<typename charT, typename traits = std::char_traits<charT> >
left_helper<charT, traits> lefted(std::basic_string<charT, traits> str) {
    return left_helper<charT, traits>(str);
}

left_helper<std::string::value_type, std::string::traits_type> lefted(const std::string& str) {
    return left_helper<std::string::value_type, std::string::traits_type>(str);
}

template<typename charT, typename traits>
std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& s, const left_helper<charT, traits>& l) {
    std::streamsize w = s.width();
    s << std::left << std::setw(w) << l.str_;
    return s;
}

// Right alignment helper
template<typename charT, typename traits = std::char_traits<charT> >
class right_helper {
    std::basic_string<charT, traits> str_;
public:
    right_helper(std::basic_string<charT, traits> str) : str_(str) {}
    template<typename a, typename b>
    friend std::basic_ostream<a, b>& operator<<(std::basic_ostream<a, b>& s, const right_helper<a, b>& r);
};

template<typename charT, typename traits = std::char_traits<charT> >
right_helper<charT, traits> righted(std::basic_string<charT, traits> str) {
    return right_helper<charT, traits>(str);
}

right_helper<std::string::value_type, std::string::traits_type> righted(const std::string& str) {
    return right_helper<std::string::value_type, std::string::traits_type>(str);
}

template<typename charT, typename traits>
std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& s, const right_helper<charT, traits>& r) {
    std::streamsize w = s.width();
    s << std::right << std::setw(w) << r.str_;
    return s;
}

#endif // ALIGN_HELPERS_H