/*
 * Copyright 2014 Google Inc. All Rights Reserved.
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

#include "cobalt/browser/browser_module.h"

#include "base/bind.h"
#include "base/logging.h"

namespace cobalt {
namespace browser {

BrowserModule::BrowserModule(const Options& options)
    : renderer_module_(options.renderer_module_options),
      css_parser_(css_parser::Parser::Create()),
      javascript_engine_(script::JavaScriptEngine::CreateEngine()),
      global_object_proxy_(javascript_engine_->CreateGlobalObject()),
      resource_loader_factory_(FakeResourceLoaderFactory::Create(
          options.fake_resource_loader_factory_options)),
      dom_module_(css_parser_.get(), resource_loader_factory_.get(),
                  global_object_proxy_, options.url),
      // TODO(***REMOVED***): Request viewport size from graphics pipeline and
      //               subscribe to viewport size changes.
      layout_manager_(dom_module_.document().get(), math::SizeF(1920, 1080),
                      renderer_module_.pipeline()->GetResourceProvider(),
                      base::Bind(&BrowserModule::OnRenderTreeProduced,
                                 base::Unretained(this))) {
  // TODO(***REMOVED***): Temporarily bind the document here for Cobalt Oxide.
  global_object_proxy_->Bind("document", dom_module_.document());
}

BrowserModule::~BrowserModule() {}

void BrowserModule::OnRenderTreeProduced(
    const scoped_refptr<render_tree::Node>& render_tree) {
  renderer_module_.pipeline()->Submit(render_tree);
}

}  // namespace browser
}  // namespace cobalt
