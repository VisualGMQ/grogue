#pragma once

#include <string>
#include <iostream>
#include <string_view>
#include <exception>
#include <stdexcept>
#include <tuple>
#include <variant>

namespace engine {

class ResultError: public std::runtime_error {
public:
    ResultError(const char* err): std::runtime_error(err) {}
};

class Err {};

template <typename T>
class Ok {
public:
    Ok(T&& value): value(std::forward<T>(value)) {}
    T value;
};

template <>
class Ok<void> {
};

template <typename T>
class Result {
public:
    Result(Ok<T> ok): data_(ok), type_(Type::Ok) {}
    Result(Err err): data_(err), type_(Type::Err) {}

    bool IsOk() const { return type_ == Type::Ok; }
    bool IsErr() const { return type_ == Type::Err; }

    T& Unwrap() {
        return Except("result is Err");
    }

    T& Except(const char* err) {
        if (IsOk()) {
            return std::get<Ok<T>>(data_).value;
        } else {
            throw ResultError(err);
        }
    }

private:
    std::variant<Ok<T>, Err> data_;
    enum class Type: unsigned char {
        Ok,
        Err,
    } type_;
};

template <>
class Result<void> {
public:
    Result(const Ok<void>& ok): type_(Type::Ok) {}
    Result(const Err& err): type_(Type::Err) {}

    bool IsOk() const { return type_ == Type::Ok; }
    bool IsErr() const { return type_ == Type::Err; }

    void Unwrap() {
        return Except("result is Err");
    }

    void Except(const char* err) {
        if (IsOk()) {
            return;
        } else {
            throw ResultError(err);
        }
    }

private:
    enum class Type: unsigned char {
        Ok,
        Err,
    } type_;   
};

#define MATCH(result, ok_expr, err_expr) do { \
    if (result.IsOk()) { \
        { ok_expr } \
    } else { \
        { err_expr } \
    } \
} while(0)

#define PROPAGATING(result, value) do {if (result.IsErr()) return engine::Err{}; else value = result.Unwrap(); } while(0)

}