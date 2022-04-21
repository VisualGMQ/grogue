#pragma once
#include "pch.hpp"
#include "log.hpp"

namespace grogue::core {

class UTF8String;

class UTF8Char final {
public:
    friend class UTF8String;
    friend std::ostream& operator<<(std::ostream&, const UTF8Char&);

    UTF8Char();
    UTF8Char(const std::string&);
    UTF8Char(char value);
    UTF8Char(char value1, char value2);
    UTF8Char(char value1, char value2, char value3);
    UTF8Char(char value1, char value2, char value3, char value4);
    size_t CharNum() const;

private:
    char data_[4];
};

std::ostream& operator<<(std::ostream&, const UTF8Char&);

class UTF8String final {
public:
    UTF8String();
    explicit UTF8String(const std::string& str);
    explicit UTF8String(const char* str);
    UTF8String(const UTF8String&);
    UTF8String(UTF8String&&);

    ~UTF8String();

    size_t Size() const { return size_; }

    bool Empty() const { return Size() == 0; }

    UTF8Char& operator[](size_t idx) const {
        return *(chars_ + idx);
    }

    void Reset(const std::string& str);
    void Reset(const char* str, const size_t len);

    UTF8String& operator=(const char* str);
    UTF8String& operator=(const std::string& o);
    UTF8String& operator=(const UTF8String& o);
    UTF8String& operator=(UTF8String&& o);

    UTF8Char& At(size_t idx) const {
        if (idx < 0 || idx >= size_) {
            throw std::out_of_range("UTF8String index out of range");
        } else {
            return operator[](idx);
        }
    }

    std::string ToString() const;

private:
    UTF8Char* chars_;
    size_t size_;

    using CCharPtr = const char* ;
    void pushChars(CCharPtr& it, size_t& index, int num);
};

std::ostream& operator<<(std::ostream&, const UTF8String&);

}
