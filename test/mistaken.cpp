#include <functional>
#include <memory>

using DelaySpawnFunc = std::function<void(void *)>;

class Test final {
public:
    template <typename T>
    void SaveComponentInfo(T &&component) {
        DelaySpawnFunc func = [=, c = std::move(component)](void* elem) {
            (*((T *)elem)) = std::move(c);
        };
        spawnFuncs_.push_back(func);
    }

    void DoSpawn() {
        for (auto& info : spawnFuncs_) {
            // void* elem = pool.Alloc();
            void* elem = nullptr;
            info(elem);
        }
    }

private:
    std::vector<DelaySpawnFunc> spawnFuncs_;
};

struct NoUniquePtr final {
    int a;
};

struct WithUniquePtr final {
    std::unique_ptr<int> a;
};

int main() {
    Test test;
    test.SaveComponentInfo<NoUniquePtr>(NoUniquePtr{});
    test.SaveComponentInfo<WithUniquePtr>(WithUniquePtr{});
    return 0;
}