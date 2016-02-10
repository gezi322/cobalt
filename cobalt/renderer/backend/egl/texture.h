/*
 * Copyright 2015 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef COBALT_RENDERER_BACKEND_EGL_TEXTURE_H_
#define COBALT_RENDERER_BACKEND_EGL_TEXTURE_H_

#include <GLES2/gl2.h>

#include "base/memory/scoped_ptr.h"
#include "cobalt/renderer/backend/egl/pbuffer_render_target.h"
#include "cobalt/renderer/backend/egl/texture_data.h"
#include "cobalt/renderer/backend/surface_info.h"
#include "cobalt/renderer/backend/texture.h"

namespace cobalt {
namespace renderer {
namespace backend {

class GraphicsContextEGL;
class ResourceContext;

class TextureEGL : public Texture {
 public:
  // Create a texture from source pixel data possibly filled in by the CPU.
  TextureEGL(GraphicsContextEGL* graphics_context,
             scoped_ptr<TextureDataEGL> texture_source_data,
             bool bgra_supported);
  // Create a texture from a pre-existing offscreen PBuffer render target.
  TextureEGL(GraphicsContextEGL* graphics_context,
             const RawTextureMemoryEGL* data, intptr_t offset,
             const SurfaceInfo& surface_info, int pitch_in_bytes,
             bool bgra_supported);

  // Create a texture from a pre-existing offscreen PBuffer render target.
  explicit TextureEGL(
      GraphicsContextEGL* graphics_context,
      const scoped_refptr<PBufferRenderTargetEGL>& render_target);
  virtual ~TextureEGL();

  const SurfaceInfo& GetSurfaceInfo() const OVERRIDE;

  Origin GetOrigin() const OVERRIDE { return kTopLeft; }

  intptr_t GetPlatformHandle() OVERRIDE {
    return static_cast<intptr_t>(gl_handle());
  }

  // Returns an index to the texture that can be passed to OpenGL functions.
  GLuint gl_handle() const { return gl_handle_; }

 private:
  // A reference to the graphics context that this texture is associated with.
  GraphicsContextEGL* graphics_context_;

  // Metadata about the texture.
  SurfaceInfo surface_info_;

  // The OpenGL handle to the texture that can be passed into OpenGL functions.
  GLuint gl_handle_;

  // If the texture was constructed from a render target, we keep a reference
  // to the render target.
  scoped_refptr<PBufferRenderTargetEGL> source_render_target_;
};

}  // namespace backend
}  // namespace renderer
}  // namespace cobalt

#endif  // COBALT_RENDERER_BACKEND_EGL_TEXTURE_H_
