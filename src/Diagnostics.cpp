#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "Diagnostics.hpp"
using namespace geode::prelude;
namespace d = lsf::diagnostics;
class $modify(LSFUpdateTiming, GJBaseGameLayer) {
    void update(float dt) {
        if (!d::enabled) { GJBaseGameLayer::update(dt); return; }
        if (d::owner != this) { d::reset(); d::owner = this; }
        auto start = d::Clock::now();
        GJBaseGameLayer::update(dt);
        d::add(d::Update, start);
        if (d::Clock::now()-d::windowStart < std::chrono::seconds(5)) return;
        log::info("LSF CPU timings: objects {}; update avg/max {:.3f}/{:.3f} ms ({} calls); visibility {:.3f}/{:.3f} ms ({}); particles {:.3f}/{:.3f} ms ({}); shader calculations {:.3f}/{:.3f} ms ({}). Inclusive CPU timings; not GPU time or FPS.",
            m_objects ? m_objects->count() : 0,
            d::metrics[d::Update].average(), d::metrics[d::Update].maxMs, d::metrics[d::Update].calls,
            d::metrics[d::Visibility].average(), d::metrics[d::Visibility].maxMs, d::metrics[d::Visibility].calls,
            d::metrics[d::Particles].average(), d::metrics[d::Particles].maxMs, d::metrics[d::Particles].calls,
            d::metrics[d::ShaderCPU].average(), d::metrics[d::ShaderCPU].maxMs, d::metrics[d::ShaderCPU].calls);
        d::metrics = {}; d::windowStart = d::Clock::now();
    }
    void updateParticles(float dt) {
        if (!d::enabled) { GJBaseGameLayer::updateParticles(dt); return; }
        auto start = d::Clock::now();
        GJBaseGameLayer::updateParticles(dt);
        d::add(d::Particles,start);
    }
};
class $modify(LSFVisibilityTiming, PlayLayer) {
    void updateVisibility(float dt) {
        if (!d::enabled) { PlayLayer::updateVisibility(dt); return; }
        auto start = d::Clock::now();
        PlayLayer::updateVisibility(dt);
        d::add(d::Visibility,start);
    }
};
