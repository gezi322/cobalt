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

#include "cobalt/dom/html_style_element.h"

#include "cobalt/cssom/css_style_sheet.h"
#include "cobalt/dom/document.h"

namespace cobalt {
namespace dom {

// static
const char* HTMLStyleElement::kTagName = "style";

HTMLStyleElement::HTMLStyleElement(HTMLElementFactory* html_element_factory,
                                   cssom::CSSParser* css_parser)
    : HTMLElement(html_element_factory, css_parser),
      content_location_("[object HTMLStyleElement]", 1, 1) {}

const std::string& HTMLStyleElement::tag_name() const {
  static const std::string kStyleTagString(kTagName);
  return kStyleTagString;
}

void HTMLStyleElement::SetOpeningTagLocation(
    const base::SourceLocation& opening_tag_location) {
  content_location_ = opening_tag_location;
  ++content_location_.column_number;  // CSS code starts after ">".
}

void HTMLStyleElement::AttachToDocument(Document* document) {
  HTMLElement::AttachToDocument(document);
  scoped_refptr<cssom::CSSStyleSheet> style_sheet =
      css_parser_->ParseStyleSheet(text_content().value(), content_location_);
  style_sheet->SetLocationUrl(GURL(content_location_.file_path));
  owner_document()->style_sheets()->Append(style_sheet);
}

HTMLStyleElement::~HTMLStyleElement() {}

}  // namespace dom
}  // namespace cobalt
