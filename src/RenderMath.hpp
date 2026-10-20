#pragma once
#include <algorithm>
#include <cmath>
#include <optional>
namespace lsf {
inline bool positive(float v) { return std::isfinite(v) && v > 0.f; }
struct PixelAxis { float scale; float nativeRequest; };
// Fit an integer number of cells into the existing physical render target.
// Native prepareTargetContainer multiplies its request by zoom * scaleFactor.
inline std::optional<PixelAxis> pixelAxis(float pixels, float factor, float request, float zoom) {
    if (!positive(pixels) || !positive(factor) || !positive(request) || !positive(zoom))
        return std::nullopt;
    float logical = pixels / factor;
    float cells = std::max(1.f, std::round(logical / std::max(1.f, request)));
    float scale = std::max(1.f, zoom * (pixels / cells));
    float nativeRequest = scale / factor / zoom;
    if (!positive(scale) || !positive(nativeRequest)) return std::nullopt;
    return PixelAxis{scale, nativeRequest};
}
template<class T> class RestoreValue {
    T& target; T saved;
public:
    RestoreValue(T& t, T v) : target(t), saved(t) { target = v; }
    ~RestoreValue() { target = saved; }
    RestoreValue(RestoreValue const&) = delete;
    RestoreValue& operator=(RestoreValue const&) = delete;
};
}
