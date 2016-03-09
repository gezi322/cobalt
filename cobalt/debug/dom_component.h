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
#ifndef COBALT_DEBUG_DOM_COMPONENT_H_
#define COBALT_DEBUG_DOM_COMPONENT_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "cobalt/debug/debug_server.h"
#include "cobalt/debug/json_object.h"
#include "cobalt/debug/render_overlay.h"

namespace cobalt {
namespace debug {

class DOMComponent : public DebugServer::Component {
 public:
  DOMComponent(const base::WeakPtr<DebugServer>& server,
               RenderOverlay* debug_overlay);

 private:
  JSONObject Enable(const JSONObject& params);
  JSONObject Disable(const JSONObject& params);

  // Gets a JSON representation of the document object, including its children
  // to a few levels deep (subsequent levels will be returned via an event
  // when the client calls |RequestChildNodes|).
  JSONObject GetDocument(const JSONObject& params);

  // Requests that the children of a specified node should be returned via
  // an event.
  JSONObject RequestChildNodes(const JSONObject& params);

  // Gets the nodeId corresponding to a remote objectId. Also sends all nodes
  // on the path between the requested node object and the root (document) as
  // a series of DOM.setChildNodes events.
  JSONObject RequestNode(const JSONObject& params);

  // Creates a Runtime.RemoteObject corresponding to a node.
  JSONObject ResolveNode(const JSONObject& params);

  // Highlights a specified node according to highlight parameters.
  JSONObject HighlightNode(const JSONObject& params);

  // Hides the node highlighting.
  JSONObject HideHighlight(const JSONObject& params);

  // Renders a highlight to the overlay.
  void RenderHighlight(const scoped_refptr<dom::DOMRect>& bounding_rect,
                       const base::DictionaryValue* highlight_config_value);

  // No ownership.
  RenderOverlay* debug_overlay_;
};

}  // namespace debug
}  // namespace cobalt

#endif  // COBALT_DEBUG_DOM_COMPONENT_H_
