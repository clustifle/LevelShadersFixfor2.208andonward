#include <Geode/Geode.hpp>
#include <Geode/modify/ShaderLayer.hpp>
#include "RenderMath.hpp"
using namespace geode::prelude;
#if GEODE_COMP_GD_VERSION != 22081
#error "Pixelation port reviewed for GD 2.2081 only."
#endif

class $modify(LSFPixelation, ShaderLayer) {
    struct Fields { bool reported = false; };
    struct Grid { lsf::PixelAxis x; lsf::PixelAxis y; };
    std::optional<Grid> correctedGrid() {
        if (!m_state.m_pixelatePixelating || !m_renderTexture || !m_sprite ||
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
        auto grid = correctedGrid();
        if (!grid) return;
        m_state.m_textureScaleX = 1.f / grid->x.scale;
        m_state.m_textureScaleY = 1.f / grid->y.scale;
        m_renderTexture->updateInternalScale(m_state.m_textureScaleX, m_state.m_textureScaleY);
        m_sprite->setScaleX(grid->x.scale);
        m_sprite->setScaleY(grid->y.scale);
        if (!m_fields->reported) {
            log::info("LSF alpha.3: pixel grid correction active; native framebuffer retained.");
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
    log::info("LSF alpha.3: GD 2.2081 pixelation port; native shader and framebuffer setup retained.");
}
