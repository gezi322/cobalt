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

#include "cobalt/dom/html_element.h"

#include "cobalt/cssom/cascaded_style.h"
#include "cobalt/cssom/computed_style.h"
#include "cobalt/cssom/css_parser.h"
#include "cobalt/cssom/css_style_sheet.h"
#include "cobalt/cssom/property_names.h"
#include "cobalt/cssom/specified_style.h"
#include "cobalt/dom/document.h"
#include "cobalt/dom/html_anchor_element.h"
#include "cobalt/dom/html_body_element.h"
#include "cobalt/dom/htmlbr_element.h"
#include "cobalt/dom/html_div_element.h"
#include "cobalt/dom/html_element_context.h"
#include "cobalt/dom/html_element_factory.h"
#include "cobalt/dom/html_head_element.h"
#include "cobalt/dom/html_heading_element.h"
#include "cobalt/dom/html_html_element.h"
#include "cobalt/dom/html_link_element.h"
#include "cobalt/dom/html_media_element.h"
#include "cobalt/dom/html_paragraph_element.h"
#include "cobalt/dom/html_script_element.h"
#include "cobalt/dom/html_span_element.h"
#include "cobalt/dom/html_style_element.h"
#include "cobalt/dom/html_unknown_element.h"
#include "cobalt/dom/rule_matching.h"

namespace cobalt {
namespace dom {

scoped_refptr<Node> HTMLElement::Duplicate() const {
  DCHECK(owner_document()->html_element_context()->html_element_factory());
  scoped_refptr<HTMLElement> new_html_element =
      owner_document()
          ->html_element_context()
          ->html_element_factory()
          ->CreateHTMLElement(owner_document(), tag_name());
  new_html_element->attribute_map_ = attribute_map_;
  return new_html_element;
}

void HTMLElement::OnCSSMutation() {
  // Invalidate the computed style of this node.
  computed_style_valid_ = false;

  owner_document()->OnElementInlineStyleMutation();
}

scoped_refptr<HTMLAnchorElement> HTMLElement::AsHTMLAnchorElement() {
  return NULL;
}

scoped_refptr<HTMLBodyElement> HTMLElement::AsHTMLBodyElement() { return NULL; }

scoped_refptr<HTMLBRElement> HTMLElement::AsHTMLBRElement() { return NULL; }

scoped_refptr<HTMLDivElement> HTMLElement::AsHTMLDivElement() { return NULL; }

scoped_refptr<HTMLHeadElement> HTMLElement::AsHTMLHeadElement() { return NULL; }

scoped_refptr<HTMLHeadingElement> HTMLElement::AsHTMLHeadingElement() {
  return NULL;
}

scoped_refptr<HTMLHtmlElement> HTMLElement::AsHTMLHtmlElement() { return NULL; }

scoped_refptr<HTMLLinkElement> HTMLElement::AsHTMLLinkElement() { return NULL; }

scoped_refptr<HTMLMediaElement> HTMLElement::AsHTMLMediaElement() {
  return NULL;
}

scoped_refptr<HTMLParagraphElement> HTMLElement::AsHTMLParagraphElement() {
  return NULL;
}

scoped_refptr<HTMLScriptElement> HTMLElement::AsHTMLScriptElement() {
  return NULL;
}

scoped_refptr<HTMLSpanElement> HTMLElement::AsHTMLSpanElement() { return NULL; }

scoped_refptr<HTMLStyleElement> HTMLElement::AsHTMLStyleElement() {
  return NULL;
}

scoped_refptr<HTMLUnknownElement> HTMLElement::AsHTMLUnknownElement() {
  return NULL;
}

namespace {

scoped_refptr<cssom::CSSStyleDeclarationData>
PromoteMatchingRulesToComputedStyle(
    cssom::RulesWithCascadePriority* matching_rules,
    cssom::GURLMap* property_name_to_base_url_map,
    const scoped_refptr<const cssom::CSSStyleDeclarationData>& inline_style,
    const scoped_refptr<const cssom::CSSStyleDeclarationData>&
        parent_computed_style,
    const base::Time& style_change_event_time,
    cssom::TransitionSet* transitions,
    const scoped_refptr<const cssom::CSSStyleDeclarationData>&
        previous_computed_style) {
  scoped_refptr<cssom::CSSStyleDeclarationData> computed_style =
      new cssom::CSSStyleDeclarationData();

  // Get the element's inline styles.
  computed_style->AssignFrom(*inline_style);

  // Select the winning value for each property by performing the cascade,
  // that is, apply values from matching rules on top of inline style, taking
  // into account rule specificity and location in the source file, as well as
  // property declaration importance.
  cssom::PromoteToCascadedStyle(computed_style, matching_rules,
                                property_name_to_base_url_map);

  // Up to this point many properties may lack a value. Perform defaulting
  // in order to ensure that every property has a value. Resolve "initial" and
  // "inherit" keywords, initialize properties with missing or semantically
  // invalid values with default values.
  cssom::PromoteToSpecifiedStyle(computed_style, parent_computed_style);

  // Lastly, absolutize the values, if possible. Convert length units and
  // percentages into pixels, convert color keywords into RGB triplets,
  // and so on. For certain properties, like "font-family", computed value is
  // the same as specified value. Declarations that cannot be absolutized
  // easily, like "width: auto;", will be resolved during layout.
  cssom::PromoteToComputedStyle(computed_style, parent_computed_style,
                                property_name_to_base_url_map);

  if (previous_computed_style) {
    // Now that we have updated our computed style, compare it to the previous
    // style and see if we need to adjust our animations.
    transitions->UpdateTransitions(style_change_event_time,
                                   *previous_computed_style, *computed_style);
  }

  return computed_style;
  // Cache the results of the computed style calculation.
}

}  // namespace

void HTMLElement::UpdateComputedStyle(
    const scoped_refptr<const cssom::CSSStyleDeclarationData>&
        parent_computed_style,
    const base::Time& style_change_event_time) {
  scoped_refptr<Document> document = owner_document();
  DCHECK(document) << "Element should be attached to document in order to "
                      "participate in layout.";

  // TODO(***REMOVED***): It maybe helpful to generalize this mapping framework in the
  // future to allow more data and context about where a cssom::PropertyValue
  // came from.
  cssom::GURLMap property_name_to_base_url_map;
  property_name_to_base_url_map[cssom::kBackgroundImagePropertyName] =
      document->url_as_gurl();

  computed_style_ = PromoteMatchingRulesToComputedStyle(
      matching_rules(), &property_name_to_base_url_map, style_->data(),
      parent_computed_style, style_change_event_time, transitions(),
      computed_style());

  // Promote the matching rules for all known pseudo elements.
  for (int pseudo_element_type = 0; pseudo_element_type < kMaxPseudoElementType;
       ++pseudo_element_type) {
    if (pseudo_elements_[pseudo_element_type]) {
      scoped_refptr<cssom::CSSStyleDeclarationData>
          pseudo_elemnent_computed_style = PromoteMatchingRulesToComputedStyle(
              pseudo_elements_[pseudo_element_type]->matching_rules(),
              &property_name_to_base_url_map, style_->data(), computed_style_,
              style_change_event_time,
              pseudo_elements_[pseudo_element_type]->transitions(),
              pseudo_elements_[pseudo_element_type]->computed_style());

      pseudo_elements_[pseudo_element_type]->set_computed_style(
          pseudo_elemnent_computed_style);
    }
  }
  computed_style_valid_ = true;
}

void HTMLElement::UpdateComputedStyleRecursively(
    const scoped_refptr<const cssom::CSSStyleDeclarationData>&
        parent_computed_style,
    const base::Time& style_change_event_time, bool ancestors_were_valid) {
  bool is_valid = ancestors_were_valid && computed_style_valid_;
  if (!is_valid) {
    UpdateComputedStyle(parent_computed_style, style_change_event_time);
    DCHECK(computed_style_valid_);
  }

  // Update computed style for this element's descendants.
  for (Element* element = first_element_child(); element;
       element = element->next_element_sibling()) {
    HTMLElement* html_element = element->AsHTMLElement();
    DCHECK(html_element);
    html_element->UpdateComputedStyleRecursively(
        computed_style(), style_change_event_time, is_valid);
  }
}

void HTMLElement::ClearMatchingRules() {
  matching_rules_.reset(new cssom::RulesWithCascadePriority());
  for (int pseudo_element_type = 0; pseudo_element_type < kMaxPseudoElementType;
       ++pseudo_element_type) {
    if (pseudo_elements_[pseudo_element_type]) {
      pseudo_elements_[pseudo_element_type]->clear_matching_rules();
    }
  }
}

void HTMLElement::UpdateMatchingRules(
    const scoped_refptr<cssom::CSSStyleSheet>& user_agent_style_sheet,
    const scoped_refptr<cssom::StyleSheetList>& author_style_sheets) {
  // Update matching rules for this element.
  //
  ClearMatchingRules();

  // Match with user agent style sheet.
  if (user_agent_style_sheet) {
    GetMatchingRulesFromStyleSheet(user_agent_style_sheet, this,
                                   cssom::kNormalUserAgent);
  }
  // Match with all author style sheets.
  DCHECK(author_style_sheets);
  for (unsigned int style_sheet_index = 0;
       style_sheet_index < author_style_sheets->length(); ++style_sheet_index) {
    scoped_refptr<cssom::CSSStyleSheet> style_sheet =
        author_style_sheets->Item(style_sheet_index);
    GetMatchingRulesFromStyleSheet(style_sheet, this, cssom::kNormalAuthor);
  }

  computed_style_valid_ = false;
}

void HTMLElement::UpdateMatchingRulesRecursively(
    const scoped_refptr<cssom::CSSStyleSheet>& user_agent_style_sheet,
    const scoped_refptr<cssom::StyleSheetList>& author_style_sheets) {
  UpdateMatchingRules(user_agent_style_sheet, author_style_sheets);
  // Update matching rules for this element's descendants.
  //
  for (Element* element = first_element_child(); element;
       element = element->next_element_sibling()) {
    HTMLElement* html_element = element->AsHTMLElement();
    DCHECK(html_element);
    html_element->UpdateMatchingRulesRecursively(user_agent_style_sheet,
                                                 author_style_sheets);
  }
}

HTMLElement::HTMLElement(Document* document)
    : Element(document),
      style_(new cssom::CSSStyleDeclaration(
          document->html_element_context()->css_parser())),
      computed_style_valid_(false) {
  style_->set_mutation_observer(this);
}

HTMLElement::HTMLElement(Document* document, const std::string& tag_name)
    : Element(document, tag_name),
      style_(new cssom::CSSStyleDeclaration(
          document->html_element_context()->css_parser())),
      computed_style_valid_(false) {
  style_->set_mutation_observer(this);
}

HTMLElement::~HTMLElement() {}

}  // namespace dom
}  // namespace cobalt
