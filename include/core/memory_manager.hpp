#pragma once

#include <vector>

template <typename T>
class MemoryRecycler final {
public:
	template <typename... Args>
	T* New(Args&&... args) {
		if (cache_.empty()) {
			instances_.emplace_back(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
			return instances_.back().get();
		} else {
			instances_.push_back(std::move(cache_.back()));
			cache_.pop_back();
			return new(instances_.back().get())T(std::forward<Args>(args)...);
		}
	}

	void Recycle() {
		for (int i = 0; i < instances_.size(); i++) {
			cache_.push_back(std::move(instances_[i]));
		}
		instances_.clear();
	}

    size_t CacheNum() const { return cache_.size(); }
    size_t UsingInstanceNum() const { return instances_.size(); }

	static MemoryRecycler& Instance() {
		static MemoryRecycler instance;
		return instance;
	}

private:
	std::vector<std::unique_ptr<T>> instances_;
	std::vector<std::unique_ptr<T>> cache_;
};

template <typename T>
class SingleFrameRecycler final {
public:
    static SingleFrameRecycler& Instance() {
        static SingleFrameRecycler instance;
        return instance;
    }

    template <typename... Args>
    void EmplaceBack(Args&&... args) {
        if (curIndex_ < instances_.size()) {
            new (instances_[curIndex_++].get())T(std::forward<Args>(args)...);
        } else {
            instances_.emplace_back(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
            curIndex_++;
        }
    }

    std::vector<std::unique_ptr<T>>& Datas() { return instances_; }
    auto begin() { return instances_.begin(); }
    auto end() { return instances_.begin() + curIndex_; }

    void InvalidAll() {
        curIndex_ = 0;
    }

    T& operator[](size_t index) {
        return *instances_[index];
    }

    T& At(size_t index) {
        if (index >= curIndex_) {
            throw std::out_of_range("SingleFrameRecycler out of range");
        }
        return *instances_[index];
    }

    size_t Capacity() const {
        return instances_.size();
    }

    size_t UsingNum() const {
        return curIndex_;
    }

private:
    std::vector<std::unique_ptr<T>> instances_;
    int curIndex_ = 0;

    SingleFrameRecycler() {}
};