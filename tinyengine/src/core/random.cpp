#include "engine/core/random.hpp"

namespace engine {

Lcg::Lcg(int k, int c): n_(1), c_(c) {
    lambda_ = 2 << (k + 1);
    m_ = 2 << n_;
}

void Lcg::SetSeed(uint32_t seed) {
    x_ = seed;
}

double Lcg::Random() {
    n_ ++;
    if (n_ >= 34) {
        n_ = 2;
    }

    x_ = (lambda_ * x_ + c_) % m_;
    return x_ / (double)m_;
}

void CppRandomer::SetSeed(uint32_t seed) {
}

double CppRandomer::Random() {
    return dist_(device_);
}


std::unique_ptr<Randomer> Random::randomer_ = nullptr;

void Random::Init(Type type, uint64_t seed) {
    switch (type) {
    case Type::LCG:
        randomer_.reset(new Lcg(34, 13));
        break;
    case Type::CPP:
        randomer_.reset(new CppRandomer());
        break;
    default:
        randomer_.reset(new Lcg(34, 13));
        break;
    }
    randomer_->SetSeed(seed);
}

void Random::Quit() {
    randomer_.reset();
}

int Random::Int(int low, int high) {
    return randomer_->Random() * (high - low) + low;
}

}
