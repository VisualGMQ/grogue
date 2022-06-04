#include "core/scence.hpp"
#include "core/layer.hpp"
#include "core/engine.hpp"
#include "core/log.hpp"
#include "physical/rigidbody.hpp"
#include "physical/collider.hpp"

constexpr int Body1Width = 32;
constexpr int Body1Height = 32;

constexpr int Body2Width = 32;
constexpr int Body2Height = 32;

class TestPhyLayer: public grogue::core::Layer {
public:
    TestPhyLayer(std::string_view name): grogue::core::Layer(name) {}

    void OnInit() override {
        body1_ = grogue::physical::RigidBody::Create(1);
        body1_->pos.Set(100, 100);
        body1_->fixture = grogue::physical::Fixture::Create(body1_.get(), {grogue::physical::BoxCollider({0, 0}, {Body1Width, Body1Width})});

        body2_ = grogue::physical::RigidBody::Create(1);
        body2_->pos.Set(400, 200);
        body2_->fixture = grogue::physical::Fixture::Create(body2_.get(), {grogue::physical::BoxCollider({0, 0}, {Body2Width, Body2Height})});
    }

    void Update() override {
        constexpr int Speed = 400;
        if (grogue::core::Keyboard::IsPress(SDL_SCANCODE_D)) {
            body1_->AddForce(grogue::core::Vec2(Speed, 0));
        }
        if (grogue::core::Keyboard::IsPress(SDL_SCANCODE_A)) {
            body1_->AddForce(grogue::core::Vec2(-Speed, 0));
        }
        if (grogue::core::Keyboard::IsPress(SDL_SCANCODE_W)) {
            body1_->AddForce(grogue::core::Vec2(0, -Speed));
        }
        if (grogue::core::Keyboard::IsPress(SDL_SCANCODE_S)) {
            body1_->AddForce(grogue::core::Vec2(0, Speed));
        }

        body1_->Update(grogue::core::Timer::SteadyTimer.GetSecondsBetweenFrames());
        body2_->Update(grogue::core::Timer::SteadyTimer.GetSecondsBetweenFrames());
    }

    void Render() override {
        grogue::core::GFX::DrawRect(grogue::core::Rect(body2_->pos, grogue::core::Size(Body2Width, Body2Height)),
                                    grogue::core::Color(0, 255, 0));
        grogue::core::GFX::DrawRect(grogue::core::Rect(body1_->pos, grogue::core::Size(Body1Width, Body1Height)),
                                    grogue::core::Color(255, 0, 0));

        auto collider1 = *body1_->fixture->begin();
        auto collider2 = *body2_->fixture->begin();

        grogue::core::GFX::DrawRect(grogue::core::Rect(collider1.pos, collider1.size),
                                    std::nullopt, grogue::core::Color(0, 0, 255));
        grogue::core::GFX::DrawRect(grogue::core::Rect(collider2.pos, collider2.size),
                                    std::nullopt, grogue::core::Color(0, 0, 255));

        if (grogue::physical::IsCollide(collider1, collider2)) {
            auto result = grogue::physical::Collide(collider1, collider2);
            if (result.msv.x > 0) {
                body1_->vel.x = 0;
            }
            if (result.msv.y > 0) {
                body1_->vel.y = 0;
            }
            auto dir = body1_->pos - body1_->oldPos;
            result.msv.x *= grogue::core::Sign(dir.x);
            result.msv.y *= grogue::core::Sign(dir.y);
            body1_->pos -= result.msv;
        }
    }

private:
    grogue::physical::RigidBodyPtr body1_;
    grogue::physical::RigidBodyPtr body2_;
};

class TestPhyScence: public grogue::core::Scence {
public:
    TestPhyScence(std::string_view name): grogue::core::Scence(name) {
        PushBackLayer<TestPhyLayer>("test_physical_layer");
    }
};

int main(int , char**) {
    grogue::core::Engine::Init("grogue", 1024, 720, true);
    grogue::core::Engine::RunScence<TestPhyScence>("TestPhyScence");
}
