#pragma once

#include <random>

namespace engine {

class Randomer {
public:
    virtual ~Randomer() = default;

    virtual void SetSeed(uint32_t) = 0;

    /* you must return float number in [0, 1] */
    virtual double Random() = 0;
};

class Lcg: public Randomer {
public:
    Lcg(int k, int c);
    void SetSeed(uint32_t) override;
    double Random() override;

private:
    long lambda_;
    long c_;
    long m_;
    long n_;
    long x_;
};

class CppRandomer: public Randomer {
public:
    CppRandomer() = default;
    void SetSeed(uint32_t) override;
    double Random() override;

private:
    std::random_device device_;
    std::uniform_real_distribution<double> dist_;
};


class Random final {
public:
    enum Type {
        LCG = 1,
        CPP = 2,
    };

    static void Init(Type type, uint64_t seed);
    static void Quit();

    static int Int(int low, int high);

    static Randomer* Instance() { return randomer_.get(); }

private:
    static std::unique_ptr<Randomer> randomer_;
};

}