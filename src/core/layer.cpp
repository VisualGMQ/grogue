#include "core/layer.hpp"
#include "core/scence.hpp"

namespace grogue::core {

Layer::Layer(std::string_view name): name_(name) {}

AnimationMgr& Layer::GetAnimMgr() {
    return scence_->GetAnimMgr();
}

}
