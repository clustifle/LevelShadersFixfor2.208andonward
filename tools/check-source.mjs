// Structural regression guard; runtime behavior still requires gameplay testing.
import fs from 'node:fs';
import path from 'node:path';
import assert from 'node:assert/strict';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const source=fs.readFileSync(path.join(root,'src/main.cpp'),'utf8');
const m=JSON.parse(fs.readFileSync(path.join(root,'mod.json'),'utf8'));
assert.equal(m.version,'v0.1.0-alpha.3');
assert.equal(m.geode,'5.11.2');
assert.deepEqual(m.gd,{win:'2.2081',android:'2.2081'});
assert(!m.resources);
for(const forbidden of ['void setupShader','void visit','CCRenderTexture::create',
'm_targetTextureSize =','m_screenSize =','m_fContentScaleFactor',
'setAnimationInterval','setSwapInterval','glGetIntegerv'])
 assert(!source.includes(forbidden),'Forbidden renderer replacement: '+forbidden);
assert(source.includes('ShaderLayer::prePixelateShader()'));
assert(source.includes('ShaderLayer::prepareTargetContainer()'));
console.log('PASS: 2.2081 pixelation port; no target replacement, GLSL override or FPS controls');
