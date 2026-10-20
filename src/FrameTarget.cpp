#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "RuntimeSettings.hpp"
#include <cmath>
using namespace geode::prelude;
namespace {
bool targetApplied = false;
double previousInterval = 1. / 60.;
}
void lsf::applyFrameTarget() {
    auto* director = CCDirector::get();
    if (!director || !director->getOpenGLView()) return;
    if (target60) {
        if (!targetApplied) previousInterval = director->getAnimationInterval();
        targetApplied = true;
        director->setAnimationInterval(1. / 60.);
    } else if (targetApplied) {
        director->setAnimationInterval(previousInterval);
        targetApplied = false;
    }
}
class $modify(LSFNativeFrameTarget, GameManager) {
    void updateCustomFPS() {
        GameManager::updateCustomFPS();
        auto* director = CCDirector::get();
        if (lsf::target60 && targetApplied && director) {
            auto requested = director->getAnimationInterval();
            if (std::isfinite(requested) && requested > 0. &&
                std::abs(requested - 1. / 60.) > 0.000001)
                previousInterval = requested;
        }
        lsf::applyFrameTarget();
    }
};
class $modify(LSFFrameTargetStartup, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        lsf::applyFrameTarget();
        return true;
    }
};
