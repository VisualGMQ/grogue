#include "core/utf8string.hpp"

namespace grogue::core {

UTF8Char::UTF8Char() {
    memset(data_, 0, sizeof(data_));
}

UTF8Char::UTF8Char(char value)
: UTF8Char(value, 0, 0, 0) {}

UTF8Char::UTF8Char(char value1, char value2)
: UTF8Char(value1, value2, 0, 0) {}

UTF8Char::UTF8Char(char value1, char value2, char value3)
: UTF8Char(value1, value2, value3, 0) {}

UTF8Char::UTF8Char(char value1, char value2, char value3, char value4) {
    data_[0] = value1;
    data_[1] = value2;
    data_[2] = value3;
    data_[3] = value4;
}

size_t UTF8Char::CharNum() const {
    if ((data_[0] & 0xF0) == 0xF0) return 4;
    else if ((data_[0] & 0xE0) == 0xE0) return 3;
    else if ((data_[0] & 0xC0) == 0xC0) return 2;
    else if ((data_[0] & 0x80) == 0) return 1;
    LOG_ERROR("invalid char num in UTF8Char");
    return -1;
}

std::ostream& operator<<(std::ostream& o, const UTF8Char& c) {
    for (auto& data : c.data_) {
        o << data;
    }
    return o;
}

UTF8Char::UTF8Char(const std::string& s) {
    size_t i = 0;
    while (i < s.size()) {
        data_[i++] = s[i];
    }
    while (i < 4) {
        data_[i++] = 0;
    }
}

UTF8String::UTF8String(): chars_(nullptr), size_(0) {}

UTF8String::UTF8String(const char* str): chars_(nullptr) {
    *this = str;
}

UTF8String::UTF8String(const std::string& str): chars_(nullptr) {
    *this = str;
}

UTF8String::UTF8String(const UTF8String& o): chars_(nullptr) {
    *this = o;
}

UTF8String::UTF8String(UTF8String&& o): chars_(nullptr) {
    operator=(std::move(o));
}

UTF8String& UTF8String::operator=(UTF8String&& o) {
    if (&o == this) return *this;
    size_ = o.size_;
    if (chars_) delete[] chars_;
    chars_ = o.chars_;
    o.chars_ = nullptr;
    return *this;
}

UTF8String& UTF8String::operator=(const UTF8String& o) {
    if (&o == this) return *this;
    if (chars_) delete[] chars_;
    chars_ = new UTF8Char[o.size_];
    size_ = o.size_;
    for (size_t i = 0; i < o.Size(); i++) {
        chars_[i] = o.chars_[i];
    }
    return *this;
}

UTF8String& UTF8String::operator=(const std::string& o) {
    Reset(o);
    return *this;
}

UTF8String& UTF8String::operator=(const char* str) {
    Reset(str, std::strlen(str));
    return *this;
}

void UTF8String::Reset(const std::string& str) {
    Reset(str.data(), str.length());
}

void UTF8String::Reset(const char* str, const size_t len) {
    if (chars_) {
        delete[] chars_;
    }

    size_ = 0;
    const char* it = str;

    while (it < str + len) {
        auto& c = *it;
        if ((c & 0xF0) == 0xF0)      { size_ += 1; it += 4; }
        else if ((c & 0xE0) == 0xE0) { size_ += 1; it += 3; }
        else if ((c & 0xC0) == 0xC0) { size_ += 1; it += 2; }
        else if ((c & 0x80) == 0)    { size_ += 1; it += 1; }
        else {
            LOG_ERROR("invalid check bits in utf8string");
            break;
        }
    }
    
    if (it == str + len) {
        chars_ = new UTF8Char[size_];

        it = str;
        size_t index = 0;
        while (it < str + len) {
            if ((*it & 0xF0) == 0xF0) {
                pushChars(it, index, 4);
            } else if ((*it & 0xE0) == 0xE0) {
                pushChars(it, index, 3);
            } else if ((*it & 0xC0) == 0xC0) {
                pushChars(it, index, 2);
            } else if ((*it & 0x80) == 0) {
                pushChars(it, index, 1);
            } else {
                LOG_ERROR("invalid check bits in utf8string");
                break;
            }
        }
    }
}

void UTF8String::pushChars(CCharPtr& it, size_t& index, int num) {
    if (num == 1) {
        chars_[index] = UTF8Char(*it);
        index ++;
        it += 1;
    } else if (num == 2) {
        chars_[index] = UTF8Char(*it, *(it + 1));
        index ++;
        it += 2;
    } else if (num == 3) {
        chars_[index] = UTF8Char(*it, *(it + 1), *(it + 2));
        index ++;
        it += 3;
    } else if (num == 4) {
        chars_[index] = UTF8Char(*it, *(it + 1), *(it + 2), *(it + 3));
        index ++;
        it += 3;
    } else {
            LOG_ERROR("invalid number in pushChars");
    }
}

std::string UTF8String::ToString() const {
    size_t count = 0;
    for (size_t i = 0; i < size_; i++) {
        count += (chars_ + i)->CharNum();
    }

    std::string result;
    result.resize(count);
    char* ptr = result.data();
    const UTF8Char* u8ptr = chars_;
    while (u8ptr - chars_ < (int)size_) {
        size_t count = u8ptr->CharNum();
        size_t idx = 0;
        while (idx < count) {
            *ptr = u8ptr->data_[idx++];
            ptr++;
        }
        u8ptr++;
    }
    return result;
}

UTF8String::~UTF8String() {
    if (chars_)
        delete[] chars_;
}

std::ostream& operator<<(std::ostream& o, const UTF8String& s) {
    for (size_t i = 0; i < s.Size(); i++) {
        o << s[i];
    }
    return o;
}

}
