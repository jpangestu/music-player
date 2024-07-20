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

template<typename charT, typename traits = std::char_traits<charT>>
class align_helper {
    std::basic_string<charT, traits> str_;
public:
    align_helper(std::basic_string<charT, traits> str) : str_(str) {}

    template<typename a, typename b>
    friend std::basic_ostream<a, b>& operator<<(std::basic_ostream<a, b>& s, const align_helper<a, b>& c);
};

template<typename charT, typename traits = std::char_traits<charT>>
inline align_helper<charT, traits> centered(std::basic_string<charT, traits> str) {
    return align_helper<charT, traits>(str);
}

template<typename charT, typename traits = std::char_traits<charT>>
inline align_helper<charT, traits> lefted(std::basic_string<charT, traits> str) {
    return align_helper<charT, traits>(str);
}

template<typename charT, typename traits = std::char_traits<charT>>
inline align_helper<charT, traits> righted(std::basic_string<charT, traits> str) {
    return align_helper<charT, traits>(str);
}

// redeclare for std::string directly so we can support anything that implicitly converts to std::string
inline align_helper<std::string::value_type, std::string::traits_type> centered(const std::string& str) {
    return align_helper<std::string::value_type, std::string::traits_type>(str);
}

inline align_helper<std::string::value_type, std::string::traits_type> lefted(const std::string& str) {
    return align_helper<std::string::value_type, std::string::traits_type>(str);
}

inline align_helper<std::string::value_type, std::string::traits_type> righted(const std::string& str) {
    return align_helper<std::string::value_type, std::string::traits_type>(str);
}

template<typename charT, typename traits>
inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& s, const align_helper<charT, traits>& c) {
    std::streamsize w = s.width();
    if (w > c.str_.length()) {
        if (typeid(c) == typeid(lefted(c.str_))) {
            s << c.str_;
            s.width(w - c.str_.length());
            s << "";
        } else if (typeid(c) == typeid(righted(c.str_))) {
            s.width(w - c.str_.length());
            s << "";
            s << c.str_;
        } else {
            std::streamsize left = (w + c.str_.length()) / 2;
            s.width(left);
            s << c.str_;
            s.width(w - left);
            s << "";
        }
    } else {
        s << c.str_;
    }
    return s;
}

#endif // ALIGN_HELPERS_H