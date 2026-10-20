#include "../src/RenderMath.hpp"
#include <limits>
#include <stdexcept>
static void check(bool ok) { if (!ok) throw std::runtime_error("Pixel grid check failed"); }
static bool near(float a,float b) { return std::abs(a-b)<0.0001f; }
int main() {
    // 1920 physical pixels at 2x scale: 960 logical units / 7 -> 137 cells.
    auto a=lsf::pixelAxis(1920.f,2.f,7.f,1.f);
    check(a && near(1920.f/a->scale,137.f));
    check(near(a->nativeRequest*2.f,a->scale));
    for(float zoom : {0.5f,1.f,2.f}) {
        auto x=lsf::pixelAxis(1366.f,2.4f,11.f,zoom);
        auto y=lsf::pixelAxis(768.f,2.4f,5.f,zoom);
        check(x && y && x->scale>=1 && y->scale>=1);
        check(near(x->nativeRequest*2.4f*zoom,x->scale));
        check(near(y->nativeRequest*2.4f*zoom,y->scale));
    }
    auto huge=lsf::pixelAxis(1920.f,2.f,1e30f,1.f);
    check(huge && huge->scale==1920.f);
    auto one=lsf::pixelAxis(1920.f,1.f,1.f,1.f);
    check(one && one->scale==1.f);
    for(float invalid : {0.f,-1.f,std::numeric_limits<float>::infinity(),
                          std::numeric_limits<float>::quiet_NaN()}) {
        check(!lsf::pixelAxis(invalid,2.f,7.f,1.f));
        check(!lsf::pixelAxis(1920.f,invalid,7.f,1.f));
        check(!lsf::pixelAxis(1920.f,2.f,invalid,1.f));
        check(!lsf::pixelAxis(1920.f,2.f,7.f,invalid));
    }
    float v=7.f;
    {lsf::RestoreValue outer(v,3.f);{lsf::RestoreValue inner(v,2.f);check(v==2.f);}check(v==3.f);}
    check(v==7.f);
}
