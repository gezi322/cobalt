/*
 * Copyright 2016 Google Inc. All Rights Reserved.
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

#include "cobalt/renderer/rasterizer/blitter/render_tree_node_visitor.h"

#include "cobalt/math/matrix3_f.h"
#include "cobalt/math/rect.h"
#include "cobalt/math/rect_f.h"
#include "cobalt/math/size.h"
#include "cobalt/math/vector2d_f.h"
#include "cobalt/renderer/rasterizer/blitter/image.h"
#include "cobalt/renderer/rasterizer/blitter/skia_blitter_conversions.h"
#include "starboard/blitter.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkImageInfo.h"

#if SB_HAS(BLITTER)

namespace cobalt {
namespace renderer {
namespace rasterizer {
namespace blitter {

using math::Matrix3F;
using math::Rect;
using math::RectF;
using math::Size;
using math::Vector2dF;
using render_tree::Border;
using render_tree::Brush;
using render_tree::ColorRGBA;
using render_tree::SolidColorBrush;

namespace {
SbBlitterRect ToBlitterRect(const RectF& rect) {
  // We convert from floating point to integer in such a way that two boxes
  // joined at the seams in float-space continue to be joined at the seams in
  // integer-space.
  int x = static_cast<int>(std::floor(rect.x()));
  int y = static_cast<int>(std::floor(rect.y()));

  return SbBlitterMakeRect(x, y, static_cast<int>(std::floor(rect.right())) - x,
                           static_cast<int>(std::floor(rect.bottom())) - y);
}
}  // namespace

RenderTreeNodeVisitor::RenderTreeNodeVisitor(
    SbBlitterDevice device, SbBlitterContext context,
    SbBlitterRenderTarget render_target, const Size& bounds,
    skia::SkiaSoftwareRasterizer* software_rasterizer)
    : device_(device),
      context_(context),
      render_target_(render_target),
      bounds_(bounds),
      software_rasterizer_(software_rasterizer) {}

void RenderTreeNodeVisitor::Visit(
    render_tree::CompositionNode* composition_node) {
  const render_tree::CompositionNode::Children& children =
      composition_node->data().children();

  if (children.empty()) {
    return;
  }

  transform_.ApplyOffset(composition_node->data().offset());
  for (render_tree::CompositionNode::Children::const_iterator iter =
           children.begin();
       iter != children.end(); ++iter) {
    (*iter)->Accept(this);
  }
  transform_.ApplyOffset(-composition_node->data().offset());
}

void RenderTreeNodeVisitor::Visit(render_tree::FilterNode* filter_node) {
  if (!filter_node->data().opacity_filter ||
      filter_node->data().opacity_filter->opacity() > 0.0f) {
    filter_node->data().source->Accept(this);
  }
}

void RenderTreeNodeVisitor::Visit(render_tree::ImageNode* image_node) {
  Image* blitter_image =
      base::polymorphic_downcast<Image*>(image_node->data().source.get());
  const Size& image_size = blitter_image->GetSize();

  const Matrix3F& local_matrix = image_node->data().local_transform;
  if (local_matrix.Get(1, 0) != 0 || local_matrix.Get(0, 1) != 0) {
    NOTIMPLEMENTED()
        << "We don't currently handle non-scale/non-translate matrices.";
    return;
  }

  // Apply the local image coordinate transform to the source rectangle.  Note
  // that the render tree local transform matrix is normalized, but the Blitter
  // API source rectangle is specified in pixel units, so we must multiply the
  // offset by |image_size| in order to get the correct values.
  Transform local_transform;
  local_transform.ApplyScale(
      Vector2dF(1.0f / local_matrix.Get(0, 0), 1.0f / local_matrix.Get(1, 1)));
  local_transform.ApplyOffset(
      Vector2dF(-local_matrix.Get(0, 2) * image_size.width(),
                -local_matrix.Get(1, 2) * image_size.height()));

  // Render the image.
  SbBlitterSetBlending(context_, true);
  SbBlitterSetModulateBlitsWithColor(context_, false);
  SbBlitterBlitRectToRectTiled(
      context_, blitter_image->surface(),
      ToBlitterRect(local_transform.TransformRect(RectF(image_size))),
      ToBlitterRect(
          transform_.TransformRect(image_node->data().destination_rect)));
}

void RenderTreeNodeVisitor::Visit(
    render_tree::MatrixTransformNode* matrix_transform_node) {
  const Matrix3F& transform = matrix_transform_node->data().transform;

  if (transform.Get(1, 0) != 0 || transform.Get(0, 1) != 0) {
    NOTIMPLEMENTED()
        << "We don't currently handle non-scale/non-translate matrices.";
    return;
  }

  Transform old_transform(transform_);

  transform_.ApplyOffset(Vector2dF(transform.Get(0, 2), transform.Get(1, 2)));
  transform_.ApplyScale(Vector2dF(transform.Get(0, 0), transform.Get(1, 1)));

  matrix_transform_node->data().source->Accept(this);

  transform_ = old_transform;
}

void RenderTreeNodeVisitor::Visit(
    render_tree::PunchThroughVideoNode* punch_through_video_node) {
  SbBlitterSetColor(context_, SbBlitterColorFromRGBA(0, 0, 0, 0));
  SbBlitterSetBlending(context_, false);
  SbBlitterFillRect(context_, ToBlitterRect(transform_.TransformRect(
                                  punch_through_video_node->data().rect)));
}

namespace {
bool AllBorderSidesHaveSameColorAndStyle(const Border& border) {
  return border.left.style == border.right.style &&
         border.left.style == border.top.style &&
         border.left.style == border.bottom.style &&
         border.left.color == border.right.color &&
         border.left.color == border.top.color &&
         border.left.color == border.bottom.color;
}

SbBlitterColor RenderTreeToBlitterColor(const ColorRGBA& color) {
  return SbBlitterColorFromRGBA(color.r() * 255, color.g() * 255,
                                color.b() * 255, color.a() * 255);
}

void RenderRectNodeBorder(SbBlitterContext context, ColorRGBA color, float left,
                          float right, float top, float bottom,
                          const RectF& rect) {
  SbBlitterSetColor(context, RenderTreeToBlitterColor(color));
  SbBlitterSetBlending(context, true);

  // We draw four rectangles, one for each border edge.  They have the following
  // layout:
  //
  //  -------------
  //  | |   T     |
  //  | |---------|
  //  | |       | |
  //  |L|       |R|
  //  | |       | |
  //  |---------| |
  //  |     B   | |
  //  -------------

  // Left
  SbBlitterFillRect(context, ToBlitterRect(RectF(rect.x(), rect.y(), left,
                                                 rect.height() - bottom)));
  // Bottom
  SbBlitterFillRect(context,
                    ToBlitterRect(RectF(rect.x(), rect.bottom() - bottom,
                                        rect.width() - right, bottom)));
  // Right
  SbBlitterFillRect(
      context, ToBlitterRect(RectF(rect.right() - right, rect.y() + top, right,
                                   rect.height() - top)));
  // Top
  SbBlitterFillRect(context, ToBlitterRect(RectF(rect.x() + left, rect.y(),
                                                 rect.width() - left, top)));
}

}  // namespace

void RenderTreeNodeVisitor::Visit(render_tree::RectNode* rect_node) {
  if (rect_node->data().rounded_corners) {
    // We can't render rounded corners through the Blitter API.
    RenderWithSoftwareRenderer(rect_node);
    return;
  }
  if (rect_node->data().border) {
    if (!AllBorderSidesHaveSameColorAndStyle(*rect_node->data().border)) {
      // If the borders don't all have the same color and style, we can't
      // render them with the Blitter API (because we can't just render 4
      // rectangles), so fallback to software.
      RenderWithSoftwareRenderer(rect_node);
      return;
    }
  }
  if (rect_node->data().background_brush) {
    if (rect_node->data().background_brush->GetTypeId() !=
        base::GetTypeId<SolidColorBrush>()) {
      // We can only render solid color rectangles, if we have a more
      // complicated brush (like gradients), fallback to software.
      RenderWithSoftwareRenderer(rect_node);
      return;
    }
  }

  const RectF& transformed_rect =
      transform_.TransformRect(rect_node->data().rect);

  // Render the solid color fill, if a brush exists.
  if (rect_node->data().background_brush) {
    SbBlitterSetBlending(context_, true);

    SolidColorBrush* solid_color_brush =
        base::polymorphic_downcast<SolidColorBrush*>(
            rect_node->data().background_brush.get());
    ColorRGBA color = solid_color_brush->color();

    SbBlitterSetColor(context_, RenderTreeToBlitterColor(color));

    SbBlitterFillRect(context_, ToBlitterRect(transformed_rect));
  }

  // Render the border, if it exists.
  if (rect_node->data().border) {
    const Border& border = *rect_node->data().border;
    DCHECK(AllBorderSidesHaveSameColorAndStyle(border));

    if (border.left.style != render_tree::kBorderStyleNone) {
      DCHECK_EQ(render_tree::kBorderStyleSolid, border.left.style);

      float left_width = border.left.width * transform_.scale.x();
      float right_width = border.right.width * transform_.scale.x();
      float top_width = border.top.width * transform_.scale.y();
      float bottom_width = border.bottom.width * transform_.scale.y();

      RenderRectNodeBorder(context_, border.left.color, left_width, right_width,
                           top_width, bottom_width, transformed_rect);
    }
  }
}

void RenderTreeNodeVisitor::Visit(
    render_tree::RectShadowNode* rect_shadow_node) {
  RenderWithSoftwareRenderer(rect_shadow_node);
}

void RenderTreeNodeVisitor::Visit(render_tree::TextNode* text_node) {
  RenderWithSoftwareRenderer(text_node);
}

RectF RenderTreeNodeVisitor::Transform::TransformRect(const RectF& rect) const {
  return RectF(rect.x() * scale.x() + translate.x(),
               rect.y() * scale.y() + translate.y(), rect.width() * scale.x(),
               rect.height() * scale.y());
}

void RenderTreeNodeVisitor::RenderWithSoftwareRenderer(
    render_tree::Node* node) {
  // Start by retrieving the bounding box of our output, intersected with the
  // viewport bounds.
  RectF bounds = math::IntersectRects(
      transform_.TransformRect(node->GetBounds()), math::RectF(bounds_));
  // Round it out to integer bounds so that we can fit it within a surface with
  // integer dimensions.
  Rect integer_bounds = math::RoundOut(bounds);
  if (integer_bounds.IsEmpty()) {
    // There's nothing to render if the bounds are 0.
    return;
  }

  SkImageInfo output_image_info = SkImageInfo::MakeN32(
      integer_bounds.width(), integer_bounds.height(), kPremul_SkAlphaType);

  // Allocate the pixels for the output image.
  SbBlitterPixelDataFormat blitter_pixel_data_format =
      SkiaToBlitterPixelFormat(output_image_info.colorType());
  DCHECK(SbBlitterIsPixelFormatSupportedByPixelData(
      device_, blitter_pixel_data_format, kSbBlitterAlphaFormatPremultiplied));
  SbBlitterPixelData pixel_data = SbBlitterCreatePixelData(
      device_, integer_bounds.width(), integer_bounds.height(),
      blitter_pixel_data_format, kSbBlitterAlphaFormatPremultiplied);
  CHECK(SbBlitterIsPixelDataValid(pixel_data));

  SkBitmap bitmap;
  bitmap.installPixels(output_image_info,
                       SbBlitterGetPixelDataPointer(pixel_data),
                       SbBlitterGetPixelDataPitchInBytes(pixel_data));

  // Setup our Skia canvas that we will be using as the target for all CPU Skia
  // output.
  SkCanvas canvas(bitmap);
  canvas.clear(SkColorSetARGB(0, 0, 0, 0));

  // Now setup our canvas so that the render tree will be rendered to the top
  // left corner instead of at node->GetBounds().origin().
  canvas.translate(transform_.translate.x() - integer_bounds.x(),
                   transform_.translate.y() - integer_bounds.y());
  // And finally set the scale on our target canvas to match that of the current
  // |transform_|.
  canvas.scale(transform_.scale.x(), transform_.scale.y());

  // Use the Skia software rasterizer to render our subtree.
  software_rasterizer_->Submit(node, &canvas);

  // Create a surface out of the now populated pixel data.
  SbBlitterSurface surface =
      SbBlitterCreateSurfaceFromPixelData(device_, pixel_data);

  // Finally blit the resulting surface to our actual render target.
  SbBlitterSetBlending(context_, true);
  SbBlitterSetModulateBlitsWithColor(context_, false);
  SbBlitterBlitRectToRect(
      context_, surface,
      SbBlitterMakeRect(0, 0, integer_bounds.width(), integer_bounds.height()),
      ToBlitterRect(integer_bounds));

  // Clean up our temporary surface.
  SbBlitterDestroySurface(surface);
}

}  // namespace blitter
}  // namespace rasterizer
}  // namespace renderer
}  // namespace cobalt

#endif  // #if SB_HAS(BLITTER)
