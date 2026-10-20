#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/modify/ShaderLayer.hpp>
#include "RenderMath.hpp"
using namespace geode::prelude;
#if GEODE_COMP_GD_VERSION != 22081
#error "Pixelation port reviewed for GD 2.2081 only."
#endif

namespace {
float s_renderRatio = 1.f;
bool s_smoothUpscale = true;
bool s_fixPixelation = true;
}
class $modify(LSFPixelation, ShaderLayer) {
    struct Fields { bool reported = false; bool performanceReported = false; };
    struct Grid { lsf::PixelAxis x; lsf::PixelAxis y; };
    std::optional<Grid> correctedGrid() {
        if (!s_fixPixelation || !m_state.m_pixelatePixelating || !m_renderTexture || !m_sprite ||
            !lsf::positive(m_scaleFactor)) return std::nullopt;
        auto* sprite = m_renderTexture->getSprite();
        if (!sprite || !sprite->getTexture()) return std::nullopt;
        auto size = sprite->getTexture()->getContentSizeInPixels();
        float zoom = 1.f;
        if (m_state.m_pixelateRelative && m_gameLayer) {
            if (!m_gameLayer->m_objectLayer) return std::nullopt;
            zoom = std::abs(m_gameLayer->m_objectLayer->getScale());
        }
        auto x = lsf::pixelAxis(size.width, m_scaleFactor, m_state.m_pixelateTargetX, zoom);
        auto y = lsf::pixelAxis(size.height, m_scaleFactor, m_state.m_pixelateTargetY, zoom);
        if (!x || !y) return std::nullopt;
        return Grid{*x, *y};
    }
    void prePixelateShader() {
        // Preserve native activation, animation tags, AA and disabled-state reset.
        ShaderLayer::prePixelateShader();
        // Use native internal-scale downsampling; do not allocate or resize an FBO.
        // Pixelate triggers (including their animations) always take precedence.
        if (lsf::performanceAllowed(s_renderRatio, m_state.m_pixelatePixelating) &&
            m_renderTexture && m_sprite) {
            m_state.m_textureScaleX = s_renderRatio;
            m_state.m_textureScaleY = s_renderRatio;
            m_renderTexture->updateInternalScale(s_renderRatio, s_renderRatio);
            float upscale = 1.f / s_renderRatio;
            m_sprite->setScaleX(upscale);
            m_sprite->setScaleY(upscale);
            if (s_smoothUpscale) this->toggleAntiAlias(true);
            if (!m_fields->performanceReported) {
                log::info("LSF alpha.5: scene render scale {}%; native FBO dimensions unchanged.",
                    s_renderRatio * 100.f);
                m_fields->performanceReported = true;
            }
            return;
        }
        auto grid = correctedGrid();
        if (!grid) return;
        m_state.m_textureScaleX = 1.f / grid->x.scale;
        m_state.m_textureScaleY = 1.f / grid->y.scale;
        m_renderTexture->updateInternalScale(m_state.m_textureScaleX, m_state.m_textureScaleY);
        m_sprite->setScaleX(grid->x.scale);
        m_sprite->setScaleY(grid->y.scale);
        if (!m_fields->reported) {
            log::info("LSF alpha.5: pixel grid correction active; native framebuffer retained.");
            m_fields->reported = true;
        }
    }
    CCPoint prepareTargetContainer() {
        auto grid = correctedGrid();
        if (!grid) return ShaderLayer::prepareTargetContainer();
        // Native snap-grid math must see the same cell size as the downsample.
        // Restore animated trigger values immediately after the native call.
        lsf::RestoreValue x(m_state.m_pixelateTargetX, grid->x.nativeRequest);
        lsf::RestoreValue y(m_state.m_pixelateTargetY, grid->y.nativeRequest);
        return ShaderLayer::prepareTargetContainer();
    }
};
$execute {
    auto* mod = Mod::get();
    s_renderRatio = lsf::renderRatio(mod->getSettingValue<int64_t>("render-percent"));
    s_smoothUpscale = mod->getSettingValue<bool>("smooth-upscale");
    s_fixPixelation = mod->getSettingValue<bool>("fix-pixelation");
    listenForSettingChanges<int64_t>("render-percent", [](int64_t value) {
        s_renderRatio = lsf::renderRatio(value);
        log::info("LSF: scene render scale changed to {}%", s_renderRatio * 100.f);
    });
    listenForSettingChanges<bool>("smooth-upscale", [](bool value) {
        s_smoothUpscale = value;
    });
    listenForSettingChanges<bool>("fix-pixelation", [](bool value) {
        s_fixPixelation = value;
    });
    log::info("LSF alpha.5 settings: render scale {}%, smooth upscale {}, pixelation fix {}. Changes apply on the next shader update.",
        s_renderRatio * 100.f, s_smoothUpscale, s_fixPixelation);
    log::info("LSF alpha.5: GD 2.2081 pixelation port; native shader and framebuffer setup retained.");
}
