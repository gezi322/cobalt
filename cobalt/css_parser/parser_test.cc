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

#include "cobalt/css_parser/parser.h"

#include <cmath>

#include "base/bind.h"
#include "cobalt/cssom/const_string_list_value.h"
#include "cobalt/cssom/css_rule_list.h"
#include "cobalt/cssom/css_style_declaration.h"
#include "cobalt/cssom/css_style_rule.h"
#include "cobalt/cssom/css_style_sheet.h"
#include "cobalt/cssom/font_style_value.h"
#include "cobalt/cssom/font_weight_value.h"
#include "cobalt/cssom/initial_style.h"
#include "cobalt/cssom/integer_value.h"
#include "cobalt/cssom/keyword_value.h"
#include "cobalt/cssom/length_value.h"
#include "cobalt/cssom/linear_gradient_value.h"
#include "cobalt/cssom/matrix_function.h"
#include "cobalt/cssom/number_value.h"
#include "cobalt/cssom/percentage_value.h"
#include "cobalt/cssom/property_list_value.h"
#include "cobalt/cssom/property_names.h"
#include "cobalt/cssom/property_value_visitor.h"
#include "cobalt/cssom/rgba_color_value.h"
#include "cobalt/cssom/rotate_function.h"
#include "cobalt/cssom/scale_function.h"
#include "cobalt/cssom/string_value.h"
#include "cobalt/cssom/time_list_value.h"
#include "cobalt/cssom/timing_function.h"
#include "cobalt/cssom/timing_function_list_value.h"
#include "cobalt/cssom/transform_function_list_value.h"
#include "cobalt/cssom/translate_function.h"
#include "cobalt/cssom/url_value.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace cobalt {
namespace css_parser {

using ::testing::_;
using ::testing::AtLeast;

class MockParserObserver {
 public:
  MOCK_METHOD1(OnWarning, void(const std::string& message));
  MOCK_METHOD1(OnError, void(const std::string& message));
};

class ParserTest : public ::testing::Test {
 public:
  ParserTest();
  ~ParserTest() OVERRIDE {}

 protected:
  ::testing::StrictMock<MockParserObserver> parser_observer_;
  Parser parser_;
  const base::SourceLocation source_location_;
};

ParserTest::ParserTest()
    : parser_(base::Bind(&MockParserObserver::OnWarning,
                         base::Unretained(&parser_observer_)),
              base::Bind(&MockParserObserver::OnError,
                         base::Unretained(&parser_observer_)),
              Parser::kShort),
      source_location_("[object ParserTest]", 1, 1) {}

// TODO(***REMOVED***): Test every reduction that has semantic action.

TEST_F(ParserTest, ParsesEmptyInput) {
  scoped_refptr<cssom::CSSStyleSheet> style_sheet =
      parser_.ParseStyleSheet("", source_location_);
  ASSERT_NE(scoped_refptr<cssom::CSSStyleSheet>(), style_sheet);
  EXPECT_EQ(0, style_sheet->css_rules()->length());
}

TEST_F(ParserTest, HandlesUnrecoverableSyntaxError) {
  EXPECT_CALL(
      parser_observer_,
      OnError("[object ParserTest]:1:1: error: unrecoverable syntax error"));

  scoped_refptr<cssom::CSSStyleSheet> style_sheet =
      parser_.ParseStyleSheet("@casino-royale", source_location_);
  ASSERT_NE(scoped_refptr<cssom::CSSStyleSheet>(), style_sheet);
  EXPECT_EQ(0, style_sheet->css_rules()->length());
}

TEST_F(ParserTest, ComputesErrorLocationOnFirstLine) {
  EXPECT_CALL(
      parser_observer_,
      OnError("[object ParserTest]:10:18: error: unrecoverable syntax error"));

  scoped_refptr<cssom::CSSStyleSheet> style_sheet = parser_.ParseStyleSheet(
      "cucumber", base::SourceLocation("[object ParserTest]", 10, 10));
  ASSERT_NE(scoped_refptr<cssom::CSSStyleSheet>(), style_sheet);
  EXPECT_EQ(0, style_sheet->css_rules()->length());
}

TEST_F(ParserTest, ComputesErrorLocationOnSecondLine) {
  EXPECT_CALL(
      parser_observer_,
      OnError("[object ParserTest]:11:9: error: unrecoverable syntax error"));

  scoped_refptr<cssom::CSSStyleSheet> style_sheet = parser_.ParseStyleSheet(
      "\n"
      "cucumber",
      base::SourceLocation("[object ParserTest]", 10, 10));
  ASSERT_NE(scoped_refptr<cssom::CSSStyleSheet>(), style_sheet);
  EXPECT_EQ(0, style_sheet->css_rules()->length());
}

TEST_F(ParserTest, IgnoresSgmlCommentDelimiters) {
  scoped_refptr<cssom::CSSStyleSheet> style_sheet =
      parser_.ParseStyleSheet("<!-- body {} -->", source_location_);
  ASSERT_NE(scoped_refptr<cssom::CSSStyleSheet>(), style_sheet);
  EXPECT_EQ(1, style_sheet->css_rules()->length());
}

TEST_F(ParserTest, RecoversFromInvalidAtToken) {
  EXPECT_CALL(parser_observer_,
              OnWarning("[object ParserTest]:1:9: warning: invalid rule"));

  scoped_refptr<cssom::CSSStyleSheet> style_sheet = parser_.ParseStyleSheet(
      "body {} @cobalt-magic; div {}", source_location_);
  ASSERT_NE(scoped_refptr<cssom::CSSStyleSheet>(), style_sheet);
  EXPECT_EQ(2, style_sheet->css_rules()->length());
}

TEST_F(ParserTest, RecoversFromInvalidRuleWhichEndsWithSemicolon) {
  EXPECT_CALL(parser_observer_,
              OnWarning("[object ParserTest]:1:9: warning: invalid rule"));

  scoped_refptr<cssom::CSSStyleSheet> style_sheet = parser_.ParseStyleSheet(
      "body {} @charset 'utf-8'; div {}", source_location_);
  ASSERT_NE(scoped_refptr<cssom::CSSStyleSheet>(), style_sheet);
  EXPECT_EQ(2, style_sheet->css_rules()->length());
}

TEST_F(ParserTest, RecoversFromInvalidRuleWhichEndsWithBlock) {
  EXPECT_CALL(parser_observer_,
              OnWarning("[object ParserTest]:1:9: warning: invalid rule"));

  scoped_refptr<cssom::CSSStyleSheet> style_sheet =
      parser_.ParseStyleSheet("body {} !important {} div {}", source_location_);
  ASSERT_NE(scoped_refptr<cssom::CSSStyleSheet>(), style_sheet);
  EXPECT_EQ(2, style_sheet->css_rules()->length());
}

TEST_F(ParserTest, ParsesDeclarationListWithTrailingSemicolon) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "color: #0047ab;  /* Cobalt blue */\n"
          "font-size: 100px;\n",
          source_location_);

  EXPECT_NE(scoped_refptr<cssom::RGBAColorValue>(), style->color());
  EXPECT_NE(scoped_refptr<cssom::LengthValue>(), style->font_size());
}

TEST_F(ParserTest, ParsesDeclarationListWithoutTrailingSemicolon) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "color: #0047ab;\n"
          "font-size: 100px\n",
          source_location_);

  EXPECT_NE(scoped_refptr<cssom::RGBAColorValue>(), style->color());
  EXPECT_NE(scoped_refptr<cssom::LengthValue>(), style->font_size());
}

TEST_F(ParserTest, ParsesDeclarationListWithEmptyDeclaration) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "color: #0047ab;\n"
          ";\n"
          "font-size: 100px;\n",
          source_location_);

  EXPECT_NE(scoped_refptr<cssom::RGBAColorValue>(), style->color());
  EXPECT_NE(scoped_refptr<cssom::LengthValue>(), style->font_size());
}

TEST_F(ParserTest, RecoversFromInvalidPropertyDeclaration) {
  EXPECT_CALL(
      parser_observer_,
      OnWarning("[object ParserTest]:2:1: warning: invalid declaration"));

  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "color: #0047ab;\n"
          "1px;\n"
          "font-size: 100px;\n",
          source_location_);

  EXPECT_NE(scoped_refptr<cssom::RGBAColorValue>(), style->color());
  EXPECT_NE(scoped_refptr<cssom::LengthValue>(), style->font_size());
}

TEST_F(ParserTest, SilentlyIgnoresNonWebKitProperties) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "-moz-transform: scale(2);\n"
          "-ms-transform: scale(2);\n"
          "-o-transform: scale(2);\n"
          "transform: scale(2);\n",
          source_location_);

  EXPECT_NE(scoped_refptr<cssom::PropertyValue>(), style->transform());
}

TEST_F(ParserTest, WarnsAboutInvalidStandardAndWebKitProperties) {
  EXPECT_CALL(parser_observer_, OnWarning(
                                    "[object ParserTest]:1:1: warning: "
                                    "unsupported property -webkit-pony"));
  EXPECT_CALL(
      parser_observer_,
      OnWarning("[object ParserTest]:2:1: warning: unsupported property pony"));

  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "-webkit-pony: rainbowdash;\n"
          "pony: rainbowdash;\n"
          "color: #9edbf9;\n",
          source_location_);

  EXPECT_NE(scoped_refptr<cssom::PropertyValue>(), style->color());
}

TEST_F(ParserTest, WarnsAboutInvalidPropertyValues) {
  EXPECT_CALL(
      parser_observer_,
      OnWarning("[object ParserTest]:1:19: warning: unsupported value"));

  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "background-color: morning haze over Paris;\n"
          "color: #fff;\n",
          source_location_);

  EXPECT_EQ(scoped_refptr<cssom::PropertyValue>(), style->background_color());
  EXPECT_NE(scoped_refptr<cssom::PropertyValue>(), style->color());
}

// TODO(***REMOVED***): Test selectors.

TEST_F(ParserTest, ParsesInherit) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-color: inherit;",
                                   source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetInherit(), style->background_color());
}

TEST_F(ParserTest, ParsesInitial) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-color: initial;",
                                   source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetInitial(), style->background_color());
}

TEST_F(ParserTest, ParsesBackgroundWithOnlyColor) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background: rgba(0, 0, 0, .8);",
                                   source_location_);

  scoped_refptr<cssom::RGBAColorValue> background_color =
      dynamic_cast<cssom::RGBAColorValue*>(style->background_color().get());
  ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), background_color);
  EXPECT_EQ(0x000000cc, background_color->value());
}

TEST_F(ParserTest, ParsesBackgroundWithColorAndURL) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "background: url(foo.png) rgba(0, 0, 0, .8);", source_location_);

  scoped_refptr<cssom::RGBAColorValue> background_color =
      dynamic_cast<cssom::RGBAColorValue*>(style->background_color().get());
  ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), background_color);
  EXPECT_EQ(0x000000cc, background_color->value());

  scoped_refptr<cssom::URLValue> background_image =
      dynamic_cast<cssom::URLValue*>(style->background_image().get());
  ASSERT_NE(scoped_refptr<cssom::URLValue>(), background_image);
  EXPECT_EQ("foo.png", background_image->value());
}

TEST_F(ParserTest, ParsesBackgroundWithColorAndURLInDifferentOrder) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "background: rgba(0, 0, 0, .8) url(foo.png);", source_location_);

  scoped_refptr<cssom::RGBAColorValue> background_color =
      dynamic_cast<cssom::RGBAColorValue*>(style->background_color().get());
  ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), background_color);
  EXPECT_EQ(0x000000cc, background_color->value());

  scoped_refptr<cssom::URLValue> background_image =
      dynamic_cast<cssom::URLValue*>(style->background_image().get());
  ASSERT_NE(scoped_refptr<cssom::URLValue>(), background_image);
  EXPECT_EQ("foo.png", background_image->value());
}

TEST_F(ParserTest, ParsesBackgroundWithColorAndPosition) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background: rgba(0, 0, 0, .8) 70px 45%;",
                                   source_location_);

  scoped_refptr<cssom::RGBAColorValue> background_color =
      dynamic_cast<cssom::RGBAColorValue*>(style->background_color().get());
  ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), background_color);
  EXPECT_EQ(0x000000cc, background_color->value());

  scoped_refptr<cssom::PropertyListValue> background_position_list =
      dynamic_cast<cssom::PropertyListValue*>(
          style->background_position().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(),
            background_position_list);

  scoped_refptr<cssom::LengthValue> length_value_left =
      dynamic_cast<cssom::LengthValue*>(
          background_position_list->value()[0].get());
  EXPECT_FLOAT_EQ(70, length_value_left->value());
  EXPECT_EQ(cssom::kPixelsUnit, length_value_left->unit());

  scoped_refptr<cssom::PercentageValue> percentage_value_right =
      dynamic_cast<cssom::PercentageValue*>(
          background_position_list->value()[1].get());
  EXPECT_FLOAT_EQ(0.45f, percentage_value_right->value());
}


TEST_F(ParserTest, ParsesBackgroundWithColorAndPositionInDifferentOrder) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background: 70px 45% rgba(0, 0, 0, .8);",
                                   source_location_);

  scoped_refptr<cssom::PropertyListValue> background_position_list =
      dynamic_cast<cssom::PropertyListValue*>(
          style->background_position().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(),
            background_position_list);

  scoped_refptr<cssom::LengthValue> length_value_left =
      dynamic_cast<cssom::LengthValue*>(
          background_position_list->value()[0].get());
  EXPECT_FLOAT_EQ(70, length_value_left->value());
  EXPECT_EQ(cssom::kPixelsUnit, length_value_left->unit());

  scoped_refptr<cssom::PercentageValue> percentage_value_right =
      dynamic_cast<cssom::PercentageValue*>(
          background_position_list->value()[1].get());
  EXPECT_FLOAT_EQ(0.45f, percentage_value_right->value());

  scoped_refptr<cssom::RGBAColorValue> background_color =
      dynamic_cast<cssom::RGBAColorValue*>(style->background_color().get());
  ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), background_color);
  EXPECT_EQ(0x000000cc, background_color->value());
}

TEST_F(ParserTest, ParsesBackgroundWithURLPositionAndSize) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "background: url(foo.png) center / 50px 80px;", source_location_);

  scoped_refptr<cssom::URLValue> background_image =
      dynamic_cast<cssom::URLValue*>(style->background_image().get());

  ASSERT_NE(scoped_refptr<cssom::URLValue>(), background_image);
  EXPECT_EQ("foo.png", background_image->value());

  scoped_refptr<cssom::PropertyListValue> background_position_list =
      dynamic_cast<cssom::PropertyListValue*>(
          style->background_position().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(),
            background_position_list);

  scoped_refptr<cssom::PercentageValue> position_value_left =
      dynamic_cast<cssom::PercentageValue*>(
          background_position_list->value()[0].get());
  EXPECT_FLOAT_EQ(0.5f, position_value_left->value());

  scoped_refptr<cssom::PercentageValue> position_value_right =
      dynamic_cast<cssom::PercentageValue*>(
          background_position_list->value()[1].get());
  EXPECT_FLOAT_EQ(0.5f, position_value_right->value());

  scoped_refptr<cssom::PropertyListValue> background_size_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_size().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_size_list);

  scoped_refptr<cssom::LengthValue> size_value_left =
      dynamic_cast<cssom::LengthValue*>(background_size_list->value()[0].get());
  EXPECT_FLOAT_EQ(50, size_value_left->value());
  EXPECT_EQ(cssom::kPixelsUnit, size_value_left->unit());

  scoped_refptr<cssom::LengthValue> size_value_right =
      dynamic_cast<cssom::LengthValue*>(background_size_list->value()[1].get());
  EXPECT_FLOAT_EQ(80, size_value_right->value());
  EXPECT_EQ(cssom::kPixelsUnit, size_value_right->unit());
}

TEST_F(ParserTest, ParsesBackgroundWithURLPositionAndSizeInDifferentOrder) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background: center/50px 80px url(foo.png);",
                                   source_location_);

  scoped_refptr<cssom::URLValue> background_image =
      dynamic_cast<cssom::URLValue*>(style->background_image().get());

  ASSERT_NE(scoped_refptr<cssom::URLValue>(), background_image);
  EXPECT_EQ("foo.png", background_image->value());

  scoped_refptr<cssom::PropertyListValue> background_position_list =
      dynamic_cast<cssom::PropertyListValue*>(
          style->background_position().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(),
            background_position_list);

  scoped_refptr<cssom::PercentageValue> position_value_left =
      dynamic_cast<cssom::PercentageValue*>(
          background_position_list->value()[0].get());
  EXPECT_FLOAT_EQ(0.5f, position_value_left->value());

  scoped_refptr<cssom::PercentageValue> position_value_right =
      dynamic_cast<cssom::PercentageValue*>(
          background_position_list->value()[1].get());
  EXPECT_FLOAT_EQ(0.5f, position_value_right->value());

  scoped_refptr<cssom::PropertyListValue> background_size_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_size().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_size_list);

  scoped_refptr<cssom::LengthValue> size_value_left =
      dynamic_cast<cssom::LengthValue*>(background_size_list->value()[0].get());
  EXPECT_FLOAT_EQ(50, size_value_left->value());
  EXPECT_EQ(cssom::kPixelsUnit, size_value_left->unit());

  scoped_refptr<cssom::LengthValue> size_value_right =
      dynamic_cast<cssom::LengthValue*>(background_size_list->value()[1].get());
  EXPECT_FLOAT_EQ(80, size_value_right->value());
  EXPECT_EQ(cssom::kPixelsUnit, size_value_right->unit());
}


TEST_F(ParserTest, ParsesBackgroundColor) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-color: #fff;", source_location_);

  scoped_refptr<cssom::RGBAColorValue> background_color =
      dynamic_cast<cssom::RGBAColorValue*>(style->background_color().get());
  ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), background_color);
  EXPECT_EQ(0xffffffff, background_color->value());
}

TEST_F(ParserTest, ParsesBackgroundImageNone) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-image: none;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetNone(), style->background_image());
}

TEST_F(ParserTest, ParsesBackgroundImageInherit) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-image: inherit;",
                                   source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetInherit(), style->background_image());
}

TEST_F(ParserTest, ParsesSingleBackgroundImageURL) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-image: url(foo.png);",
                                   source_location_);

  scoped_refptr<cssom::PropertyListValue> background_image_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_image().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_image_list);
  EXPECT_EQ(1, background_image_list->value().size());

  scoped_refptr<cssom::URLValue> url_value_1 =
      dynamic_cast<cssom::URLValue*>(background_image_list->value()[0].get());
  EXPECT_EQ("foo.png", url_value_1->value());
}

TEST_F(ParserTest, ParsesMultipleBackgroundImageURLs) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "background-image: url(foo.png), url(bar.jpg), url(baz.png);",
          source_location_);

  scoped_refptr<cssom::PropertyListValue> background_image_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_image().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_image_list);
  EXPECT_EQ(3, background_image_list->value().size());

  scoped_refptr<cssom::URLValue> url_value_1 =
      dynamic_cast<cssom::URLValue*>(background_image_list->value()[0].get());
  EXPECT_EQ("foo.png", url_value_1->value());

  scoped_refptr<cssom::URLValue> url_value_2 =
      dynamic_cast<cssom::URLValue*>(background_image_list->value()[1].get());
  EXPECT_EQ("bar.jpg", url_value_2->value());

  scoped_refptr<cssom::URLValue> url_value_3 =
      dynamic_cast<cssom::URLValue*>(background_image_list->value()[2].get());
  EXPECT_EQ("baz.png", url_value_3->value());
}

TEST_F(ParserTest, ParsesBackgroundImageLinearGradientWithDirection) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "background-image: linear-gradient(180deg, rgba(34, 34, 34, 0) 50px,"
          "rgba(34, 34, 34, 0) 100px);",
          source_location_);

  scoped_refptr<cssom::PropertyListValue> background_image_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_image().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_image_list);
  EXPECT_EQ(1, background_image_list->value().size());

  scoped_refptr<cssom::LinearGradientValue> linear_gradient_value =
      dynamic_cast<cssom::LinearGradientValue*>(
          background_image_list->value()[0].get());
  EXPECT_FLOAT_EQ(static_cast<float>(M_PI),
                  *linear_gradient_value->angle_in_radians());
  EXPECT_TRUE(!linear_gradient_value->side_or_corner());

  const cssom::LinearGradientValue::ColorStopList* color_stop_list_value =
      linear_gradient_value->color_stop_list();
  EXPECT_EQ(2, color_stop_list_value->size());

  float percentage_list[2] = {50.0f, 100.0f};

  for (size_t i = 0; i < color_stop_list_value->size(); ++i) {
    const cssom::ColorStop* color_stop_value = (*color_stop_list_value)[i];
    scoped_refptr<cssom::RGBAColorValue> color = color_stop_value->rgba();
    ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), color);
    EXPECT_EQ(0x22222200, color->value());

    scoped_refptr<cssom::LengthValue> position =
        dynamic_cast<cssom::LengthValue*>(color_stop_value->position().get());
    EXPECT_FLOAT_EQ(percentage_list[i], position->value());
    EXPECT_EQ(cssom::kPixelsUnit, position->unit());
  }
}

TEST_F(ParserTest, ParsesBackgroundImageLinearGradientWithoutDirection) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "background-image: linear-gradient(rgba(34, 34, 34, 0),"
          "rgba(34, 34, 34, 0) 60%);",
          source_location_);

  scoped_refptr<cssom::PropertyListValue> background_image_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_image().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_image_list);
  EXPECT_EQ(1, background_image_list->value().size());

  scoped_refptr<cssom::LinearGradientValue> linear_gradient_value =
      dynamic_cast<cssom::LinearGradientValue*>(
          background_image_list->value()[0].get());
  EXPECT_TRUE(!linear_gradient_value->angle_in_radians());
  EXPECT_EQ(cssom::LinearGradientValue::kBottom,
            *linear_gradient_value->side_or_corner());

  const cssom::LinearGradientValue::ColorStopList* color_stop_list_value =
      linear_gradient_value->color_stop_list();
  EXPECT_EQ(2, color_stop_list_value->size());

  for (size_t i = 0; i < color_stop_list_value->size(); ++i) {
    const cssom::ColorStop* color_stop_value = (*color_stop_list_value)[i];
    scoped_refptr<cssom::RGBAColorValue> color = color_stop_value->rgba();
    ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), color);
    EXPECT_EQ(0x22222200, color->value());

    if (i == 0) {
      EXPECT_EQ(scoped_refptr<cssom::PropertyListValue>(),
                color_stop_value->position());
    } else {
      scoped_refptr<cssom::PercentageValue> position =
          dynamic_cast<cssom::PercentageValue*>(
              color_stop_value->position().get());
      EXPECT_FLOAT_EQ(0.6f, position->value());
    }
  }
}

TEST_F(ParserTest, ParsesBackgroundImageLinearGradientAndURL) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "background-image: linear-gradient(to right top, "
          "rgba(34, 34, 34, 0) 0%, rgba(34, 34, 34, 0) 80px), url(foo.png), "
          "url(bar.jpg);",
          source_location_);

  scoped_refptr<cssom::PropertyListValue> background_image_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_image().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_image_list);
  EXPECT_EQ(3, background_image_list->value().size());

  scoped_refptr<cssom::LinearGradientValue> linear_gradient_value =
      dynamic_cast<cssom::LinearGradientValue*>(
          background_image_list->value()[0].get());
  EXPECT_TRUE(!linear_gradient_value->angle_in_radians());
  EXPECT_EQ(cssom::LinearGradientValue::kTopRight,
            *linear_gradient_value->side_or_corner());

  const cssom::LinearGradientValue::ColorStopList* color_stop_list_value =
      linear_gradient_value->color_stop_list();
  EXPECT_EQ(2, color_stop_list_value->size());

  for (size_t i = 0; i < color_stop_list_value->size(); ++i) {
    const cssom::ColorStop* color_stop_value = (*color_stop_list_value)[i];
    scoped_refptr<cssom::RGBAColorValue> color = color_stop_value->rgba();
    ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), color);
    EXPECT_EQ(0x22222200, color->value());

    if (i == 0) {
      scoped_refptr<cssom::PercentageValue> position =
          dynamic_cast<cssom::PercentageValue*>(
              color_stop_value->position().get());
      EXPECT_FLOAT_EQ(0.0f, position->value());
    } else {
      scoped_refptr<cssom::LengthValue> position =
          dynamic_cast<cssom::LengthValue*>(color_stop_value->position().get());
      EXPECT_FLOAT_EQ(80.0f, position->value());
      EXPECT_EQ(cssom::kPixelsUnit, position->unit());
    }
  }

  scoped_refptr<cssom::URLValue> background_image_1 =
      dynamic_cast<cssom::URLValue*>(background_image_list->value()[1].get());
  ASSERT_NE(scoped_refptr<cssom::URLValue>(), background_image_1);
  EXPECT_EQ("foo.png", background_image_1->value());

  scoped_refptr<cssom::URLValue> background_image_2 =
      dynamic_cast<cssom::URLValue*>(background_image_list->value()[2].get());
  ASSERT_NE(scoped_refptr<cssom::URLValue>(), background_image_2);
  EXPECT_EQ("bar.jpg", background_image_2->value());
}

TEST_F(ParserTest, ParsesBackgroundPositionCenter) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-position: center;",
                                   source_location_);

  scoped_refptr<cssom::PropertyListValue> background_position_list =
      dynamic_cast<cssom::PropertyListValue*>(
          style->background_position().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(),
            background_position_list);
  EXPECT_EQ(2, background_position_list->value().size());

  scoped_refptr<cssom::PercentageValue> percentage_value_left =
      dynamic_cast<cssom::PercentageValue*>(
          background_position_list->value()[0].get());
  EXPECT_FLOAT_EQ(0.5f, percentage_value_left->value());

  scoped_refptr<cssom::PercentageValue> percentage_value_right =
      dynamic_cast<cssom::PercentageValue*>(
          background_position_list->value()[1].get());
  EXPECT_FLOAT_EQ(0.5f, percentage_value_right->value());
}

TEST_F(ParserTest, ParsesBackgroundPositionDoublePercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-position: 60% 90%;",
                                   source_location_);

  scoped_refptr<cssom::PropertyListValue> background_position_list =
      dynamic_cast<cssom::PropertyListValue*>(
          style->background_position().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(),
            background_position_list);
  EXPECT_EQ(2, background_position_list->value().size());

  scoped_refptr<cssom::PercentageValue> percentage_value_left =
      dynamic_cast<cssom::PercentageValue*>(
          background_position_list->value()[0].get());
  EXPECT_FLOAT_EQ(0.6f, percentage_value_left->value());

  scoped_refptr<cssom::PercentageValue> percentage_value_right =
      dynamic_cast<cssom::PercentageValue*>(
          background_position_list->value()[1].get());
  EXPECT_FLOAT_EQ(0.9f, percentage_value_right->value());
}

TEST_F(ParserTest, ParsesBackgroundPositionDoubleLength) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-position: 60px 90px;",
                                   source_location_);

  scoped_refptr<cssom::PropertyListValue> background_position_list =
      dynamic_cast<cssom::PropertyListValue*>(
          style->background_position().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(),
            background_position_list);
  EXPECT_EQ(2, background_position_list->value().size());

  scoped_refptr<cssom::LengthValue> length_value_left =
      dynamic_cast<cssom::LengthValue*>(
          background_position_list->value()[0].get());
  EXPECT_FLOAT_EQ(60, length_value_left->value());
  EXPECT_EQ(cssom::kPixelsUnit, length_value_left->unit());

  scoped_refptr<cssom::LengthValue> length_value_right =
      dynamic_cast<cssom::LengthValue*>(
          background_position_list->value()[1].get());
  EXPECT_FLOAT_EQ(90, length_value_right->value());
  EXPECT_EQ(cssom::kPixelsUnit, length_value_right->unit());
}

TEST_F(ParserTest, ParsesBackgroundPositionLengthAndPercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-position: 60px 70%;",
                                   source_location_);

  scoped_refptr<cssom::PropertyListValue> background_position_list =
      dynamic_cast<cssom::PropertyListValue*>(
          style->background_position().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(),
            background_position_list);
  EXPECT_EQ(2, background_position_list->value().size());

  scoped_refptr<cssom::LengthValue> length_value_left =
      dynamic_cast<cssom::LengthValue*>(
          background_position_list->value()[0].get());
  EXPECT_FLOAT_EQ(60, length_value_left->value());
  EXPECT_EQ(cssom::kPixelsUnit, length_value_left->unit());

  scoped_refptr<cssom::PercentageValue> percentage_value_right =
      dynamic_cast<cssom::PercentageValue*>(
          background_position_list->value()[1].get());
  EXPECT_FLOAT_EQ(0.7f, percentage_value_right->value());
}

TEST_F(ParserTest, ParsesBackgroundSizeContain) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-size: contain;",
                                   source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetContain(), style->background_size());
}

TEST_F(ParserTest, ParsesBackgroundSizeCover) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-size: cover;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetCover(), style->background_size());
}

TEST_F(ParserTest, ParsesBackgroundSizeSingleAuto) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-size: auto;", source_location_);

  scoped_refptr<cssom::PropertyListValue> background_size_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_size().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_size_list);
  EXPECT_EQ(2, background_size_list->value().size());
  EXPECT_EQ(cssom::KeywordValue::GetAuto(), background_size_list->value()[0]);
  EXPECT_EQ(cssom::KeywordValue::GetAuto(), background_size_list->value()[1]);
}

TEST_F(ParserTest, ParsesBackgroundSizeDoubleAuto) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-size: auto auto;",
                                   source_location_);

  scoped_refptr<cssom::PropertyListValue> background_size_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_size().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_size_list);
  EXPECT_EQ(2, background_size_list->value().size());
  EXPECT_EQ(cssom::KeywordValue::GetAuto(), background_size_list->value()[0]);
  EXPECT_EQ(cssom::KeywordValue::GetAuto(), background_size_list->value()[1]);
}

TEST_F(ParserTest, ParsesBackgroundSizeSinglePercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-size: 20%;", source_location_);

  scoped_refptr<cssom::PropertyListValue> background_size_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_size().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_size_list);
  EXPECT_EQ(2, background_size_list->value().size());

  scoped_refptr<cssom::PercentageValue> percentage_value =
      dynamic_cast<cssom::PercentageValue*>(
          background_size_list->value()[0].get());
  EXPECT_FLOAT_EQ(0.2f, percentage_value->value());
  EXPECT_EQ(cssom::KeywordValue::GetAuto(), background_size_list->value()[1]);
}

TEST_F(ParserTest, ParsesBackgroundSizeDoublePercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-size: 40% 60%;",
                                   source_location_);

  scoped_refptr<cssom::PropertyListValue> background_size_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_size().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_size_list);
  EXPECT_EQ(2, background_size_list->value().size());

  scoped_refptr<cssom::PercentageValue> percentage_value_left =
      dynamic_cast<cssom::PercentageValue*>(
          background_size_list->value()[0].get());
  scoped_refptr<cssom::PercentageValue> percentage_value_right =
      dynamic_cast<cssom::PercentageValue*>(
          background_size_list->value()[1].get());

  EXPECT_FLOAT_EQ(0.4f, percentage_value_left->value());
  EXPECT_FLOAT_EQ(0.6f, percentage_value_right->value());
}

TEST_F(ParserTest, ParsesBackgroundSizeOneAutoOnePercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-size: auto 20%;",
                                   source_location_);

  scoped_refptr<cssom::PropertyListValue> background_size_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_size().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_size_list);
  EXPECT_EQ(2, background_size_list->value().size());

  EXPECT_EQ(cssom::KeywordValue::GetAuto(),
            background_size_list->value()[0].get());

  scoped_refptr<cssom::PercentageValue> percentage_value =
      dynamic_cast<cssom::PercentageValue*>(
          background_size_list->value()[1].get());
  EXPECT_FLOAT_EQ(0.2f, percentage_value->value());
}

TEST_F(ParserTest, ParsesBackgroundSizeOnePercentageOneAuto) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("background-size: 20% auto;",
                                   source_location_);

  scoped_refptr<cssom::PropertyListValue> background_size_list =
      dynamic_cast<cssom::PropertyListValue*>(style->background_size().get());
  ASSERT_NE(scoped_refptr<cssom::PropertyListValue>(), background_size_list);
  EXPECT_EQ(2, background_size_list->value().size());

  scoped_refptr<cssom::PercentageValue> percentage_value =
      dynamic_cast<cssom::PercentageValue*>(
          background_size_list->value()[0].get());
  EXPECT_FLOAT_EQ(0.2f, percentage_value->value());

  EXPECT_EQ(cssom::KeywordValue::GetAuto(),
            background_size_list->value()[1].get());
}

TEST_F(ParserTest, ParsesBorderRadius) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("border-radius: 0.2em;", source_location_);

  scoped_refptr<cssom::LengthValue> border_radius =
      dynamic_cast<cssom::LengthValue*>(style->border_radius().get());
  ASSERT_NE(scoped_refptr<cssom::LengthValue>(), border_radius);
  EXPECT_FLOAT_EQ(0.2f, border_radius->value());
  EXPECT_EQ(cssom::kFontSizesAkaEmUnit, border_radius->unit());
}

TEST_F(ParserTest, Parses3DigitColor) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("color: #123;", source_location_);

  scoped_refptr<cssom::RGBAColorValue> color =
      dynamic_cast<cssom::RGBAColorValue*>(style->color().get());
  ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), color);
  EXPECT_EQ(0x112233ff, color->value());
}

TEST_F(ParserTest, Parses6DigitColor) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("color: #0047ab;  /* Cobalt blue */\n",
                                   source_location_);

  scoped_refptr<cssom::RGBAColorValue> color =
      dynamic_cast<cssom::RGBAColorValue*>(style->color().get());
  ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), color);
  EXPECT_EQ(0x0047abff, color->value());
}

TEST_F(ParserTest, ParsesRGBColorWithOutOfRangeIntegers) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("color: rgb(300, 0, -300);",
                                   source_location_);

  scoped_refptr<cssom::RGBAColorValue> color =
      dynamic_cast<cssom::RGBAColorValue*>(style->color().get());
  ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), color);
  EXPECT_EQ(0xff0000ff, color->value());
}

TEST_F(ParserTest, ParsesRGBAColorWithIntegers) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("color: rgba(255, 128, 1, 0.5);",
                                   source_location_);

  scoped_refptr<cssom::RGBAColorValue> color =
      dynamic_cast<cssom::RGBAColorValue*>(style->color().get());
  ASSERT_NE(scoped_refptr<cssom::RGBAColorValue>(), color);
  EXPECT_EQ(0xff80017f, color->value());
}

TEST_F(ParserTest, ParsesBlockDisplay) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("display: block;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetBlock(), style->display());
}

TEST_F(ParserTest, ParsesInlineDisplay) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("display: inline;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetInline(), style->display());
}

TEST_F(ParserTest, ParsesInlineBlockDisplay) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("display: inline-block;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetInlineBlock(), style->display());
}

TEST_F(ParserTest, ParsesNoneDisplay) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("display: none;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetNone(), style->display());
}

TEST_F(ParserTest, ParsesFontFamily) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("font-family: \"Droid Sans\";",
                                   source_location_);

  scoped_refptr<cssom::StringValue> font_family =
      dynamic_cast<cssom::StringValue*>(style->font_family().get());
  ASSERT_NE(scoped_refptr<cssom::StringValue>(), font_family);
  EXPECT_EQ("Droid Sans", font_family->value());
}

// TODO(***REMOVED***): Test other length units, including negative ones.

TEST_F(ParserTest, ParsesFontSize) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("font-size: 100px;", source_location_);

  scoped_refptr<cssom::LengthValue> font_size =
      dynamic_cast<cssom::LengthValue*>(style->font_size().get());
  ASSERT_NE(scoped_refptr<cssom::LengthValue>(), font_size);
  EXPECT_FLOAT_EQ(100, font_size->value());
  EXPECT_EQ(cssom::kPixelsUnit, font_size->unit());
}

TEST_F(ParserTest, ParsesFontStyle) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("font-style: italic;", source_location_);

  EXPECT_EQ(cssom::FontStyleValue::GetItalic(), style->font_style());
}

TEST_F(ParserTest, ParsesNormalFontWeight) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("font-weight: normal;", source_location_);

  EXPECT_EQ(cssom::FontWeightValue::GetNormalAka400(), style->font_weight());
}

TEST_F(ParserTest, ParsesBoldFontWeight) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("font-weight: bold;", source_location_);

  EXPECT_EQ(cssom::FontWeightValue::GetBoldAka700(), style->font_weight());
}

TEST_F(ParserTest, ParsesHeight) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("height: 100px;", source_location_);

  scoped_refptr<cssom::LengthValue> height =
      dynamic_cast<cssom::LengthValue*>(style->height().get());
  ASSERT_NE(scoped_refptr<cssom::LengthValue>(), height);
  EXPECT_FLOAT_EQ(100, height->value());
  EXPECT_EQ(cssom::kPixelsUnit, height->unit());
}

TEST_F(ParserTest, ParsesNormalLineHeight) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("line-height: normal;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetNormal(), style->line_height());
}

TEST_F(ParserTest, ParsesLineHeightInEm) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("line-height: 1.2em;", source_location_);

  scoped_refptr<cssom::LengthValue> line_height =
      dynamic_cast<cssom::LengthValue*>(style->line_height().get());
  ASSERT_NE(scoped_refptr<cssom::LengthValue>(), line_height);
  EXPECT_FLOAT_EQ(1.2f, line_height->value());
  EXPECT_EQ(cssom::kFontSizesAkaEmUnit, line_height->unit());
}

TEST_F(ParserTest, ParsesOpacity) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("opacity: 0.5;", source_location_);

  scoped_refptr<cssom::NumberValue> translucent =
      dynamic_cast<cssom::NumberValue*>(style->opacity().get());
  ASSERT_NE(scoped_refptr<cssom::NumberValue>(), translucent);
  EXPECT_FLOAT_EQ(0.5f, translucent->value());
}

TEST_F(ParserTest, ClampsOpacityToZero) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("opacity: -3.14;", source_location_);

  scoped_refptr<cssom::NumberValue> transparent =
      dynamic_cast<cssom::NumberValue*>(style->opacity().get());
  ASSERT_NE(scoped_refptr<cssom::NumberValue>(), transparent);
  EXPECT_FLOAT_EQ(0, transparent->value());
}

TEST_F(ParserTest, ClampsOpacityToOne) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("opacity: 2.72;", source_location_);

  scoped_refptr<cssom::NumberValue> opaque =
      dynamic_cast<cssom::NumberValue*>(style->opacity().get());
  ASSERT_NE(scoped_refptr<cssom::NumberValue>(), opaque);
  EXPECT_FLOAT_EQ(1, opaque->value());
}

TEST_F(ParserTest, ParsesHiddenOverflow) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("overflow: hidden;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetHidden(), style->overflow());
}

TEST_F(ParserTest, ParsesVisibleOverflow) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("overflow: visible;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetVisible(), style->overflow());
}

TEST_F(ParserTest, ParsesStaticPosition) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("position: static;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetStatic(), style->position());
}

TEST_F(ParserTest, ParsesRelativePosition) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("position: relative;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetRelative(), style->position());
}

TEST_F(ParserTest, ParsesAbsolutePosition) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("position: absolute;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetAbsolute(), style->position());
}

TEST_F(ParserTest, ParsesNoneTransform) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: none;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetNone(), style->transform());
}

TEST_F(ParserTest, ParsesRotateTransformInDegrees) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: rotate(180deg);",
                                   source_location_);

  scoped_refptr<cssom::TransformFunctionListValue> transform_list =
      dynamic_cast<cssom::TransformFunctionListValue*>(
          style->transform().get());
  ASSERT_NE(scoped_refptr<cssom::TransformFunctionListValue>(), transform_list);
  ASSERT_EQ(1, transform_list->value().size());

  const cssom::RotateFunction* rotate_function =
      dynamic_cast<const cssom::RotateFunction*>(transform_list->value()[0]);
  ASSERT_NE(static_cast<cssom::RotateFunction*>(NULL), rotate_function);
  EXPECT_FLOAT_EQ(static_cast<float>(M_PI),
                  rotate_function->angle_in_radians());
}

TEST_F(ParserTest, ParsesRotateTransformInGradians) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: rotate(200grad);",
                                   source_location_);

  scoped_refptr<cssom::TransformFunctionListValue> transform_list =
      dynamic_cast<cssom::TransformFunctionListValue*>(
          style->transform().get());
  ASSERT_NE(scoped_refptr<cssom::TransformFunctionListValue>(), transform_list);
  ASSERT_EQ(1, transform_list->value().size());

  const cssom::RotateFunction* rotate_function =
      dynamic_cast<const cssom::RotateFunction*>(transform_list->value()[0]);
  ASSERT_NE(static_cast<cssom::RotateFunction*>(NULL), rotate_function);
  EXPECT_FLOAT_EQ(static_cast<float>(M_PI),
                  rotate_function->angle_in_radians());
}

TEST_F(ParserTest, ParsesRotateTransformInRadians) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: rotate(3.141592653589793rad);",
                                   source_location_);

  scoped_refptr<cssom::TransformFunctionListValue> transform_list =
      dynamic_cast<cssom::TransformFunctionListValue*>(
          style->transform().get());
  ASSERT_NE(scoped_refptr<cssom::TransformFunctionListValue>(), transform_list);
  ASSERT_EQ(1, transform_list->value().size());

  const cssom::RotateFunction* rotate_function =
      dynamic_cast<const cssom::RotateFunction*>(transform_list->value()[0]);
  ASSERT_NE(static_cast<cssom::RotateFunction*>(NULL), rotate_function);
  EXPECT_FLOAT_EQ(static_cast<float>(M_PI),
                  rotate_function->angle_in_radians());
}

TEST_F(ParserTest, ParsesRotateTransformInTurns) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: rotate(0.5turn);",
                                   source_location_);

  scoped_refptr<cssom::TransformFunctionListValue> transform_list =
      dynamic_cast<cssom::TransformFunctionListValue*>(
          style->transform().get());
  ASSERT_NE(scoped_refptr<cssom::TransformFunctionListValue>(), transform_list);
  ASSERT_EQ(1, transform_list->value().size());

  const cssom::RotateFunction* rotate_function =
      dynamic_cast<const cssom::RotateFunction*>(transform_list->value()[0]);
  ASSERT_NE(static_cast<cssom::RotateFunction*>(NULL), rotate_function);
  EXPECT_FLOAT_EQ(static_cast<float>(M_PI),
                  rotate_function->angle_in_radians());
}

TEST_F(ParserTest, ParsesIsotropicScaleTransform) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: scale(1.5);", source_location_);

  scoped_refptr<cssom::TransformFunctionListValue> transform_list =
      dynamic_cast<cssom::TransformFunctionListValue*>(
          style->transform().get());
  ASSERT_NE(scoped_refptr<cssom::TransformFunctionListValue>(), transform_list);
  ASSERT_EQ(1, transform_list->value().size());

  const cssom::ScaleFunction* scale_function =
      dynamic_cast<const cssom::ScaleFunction*>(transform_list->value()[0]);
  ASSERT_NE(static_cast<cssom::ScaleFunction*>(NULL), scale_function);
  EXPECT_FLOAT_EQ(1.5, scale_function->x_factor());
  EXPECT_FLOAT_EQ(1.5, scale_function->y_factor());
}

// TODO(***REMOVED***): Test integers, including negative ones.
// TODO(***REMOVED***): Test reals, including negative ones.
// TODO(***REMOVED***): Test non-zero lengths without units.

TEST_F(ParserTest, ParsesAnisotropicScaleTransform) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: scale(16, 9);",
                                   source_location_);

  scoped_refptr<cssom::TransformFunctionListValue> transform_list =
      dynamic_cast<cssom::TransformFunctionListValue*>(
          style->transform().get());
  ASSERT_NE(scoped_refptr<cssom::TransformFunctionListValue>(), transform_list);
  ASSERT_EQ(1, transform_list->value().size());

  const cssom::ScaleFunction* scale_function =
      dynamic_cast<const cssom::ScaleFunction*>(transform_list->value()[0]);
  ASSERT_NE(static_cast<cssom::ScaleFunction*>(NULL), scale_function);
  EXPECT_FLOAT_EQ(16, scale_function->x_factor());
  EXPECT_FLOAT_EQ(9, scale_function->y_factor());
}

TEST_F(ParserTest, ParsesTranslateXTransform) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: translateX(0);",
                                   source_location_);

  scoped_refptr<cssom::TransformFunctionListValue> transform_list =
      dynamic_cast<cssom::TransformFunctionListValue*>(
          style->transform().get());
  ASSERT_NE(scoped_refptr<cssom::TransformFunctionListValue>(), transform_list);
  ASSERT_EQ(1, transform_list->value().size());

  const cssom::TranslateFunction* translate_function =
      dynamic_cast<const cssom::TranslateFunction*>(transform_list->value()[0]);
  ASSERT_NE(static_cast<cssom::TranslateFunction*>(NULL), translate_function);

  scoped_refptr<cssom::LengthValue> offset = translate_function->offset();
  EXPECT_FLOAT_EQ(0, offset->value());
  EXPECT_EQ(cssom::kPixelsUnit, offset->unit());
  EXPECT_EQ(cssom::TranslateFunction::kXAxis, translate_function->axis());
}

TEST_F(ParserTest, ParsesTranslateYTransform) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: translateY(30em);",
                                   source_location_);

  scoped_refptr<cssom::TransformFunctionListValue> transform_list =
      dynamic_cast<cssom::TransformFunctionListValue*>(
          style->transform().get());
  ASSERT_NE(scoped_refptr<cssom::TransformFunctionListValue>(), transform_list);
  ASSERT_EQ(1, transform_list->value().size());

  const cssom::TranslateFunction* translate_function =
      dynamic_cast<const cssom::TranslateFunction*>(transform_list->value()[0]);
  ASSERT_NE(static_cast<cssom::TranslateFunction*>(NULL), translate_function);

  scoped_refptr<cssom::LengthValue> offset = translate_function->offset();
  EXPECT_FLOAT_EQ(30, offset->value());
  EXPECT_EQ(cssom::kFontSizesAkaEmUnit, offset->unit());
  EXPECT_EQ(cssom::TranslateFunction::kYAxis, translate_function->axis());
}

TEST_F(ParserTest, ParsesTranslateZTransform) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: translateZ(-22px);",
                                   source_location_);

  scoped_refptr<cssom::TransformFunctionListValue> transform_list =
      dynamic_cast<cssom::TransformFunctionListValue*>(
          style->transform().get());
  ASSERT_NE(scoped_refptr<cssom::TransformFunctionListValue>(), transform_list);
  ASSERT_EQ(1, transform_list->value().size());

  const cssom::TranslateFunction* translate_function =
      dynamic_cast<const cssom::TranslateFunction*>(transform_list->value()[0]);
  ASSERT_NE(static_cast<cssom::TranslateFunction*>(NULL), translate_function);

  scoped_refptr<cssom::LengthValue> offset = translate_function->offset();
  EXPECT_FLOAT_EQ(-22, offset->value());
  EXPECT_EQ(cssom::kPixelsUnit, offset->unit());
  EXPECT_EQ(cssom::TranslateFunction::kZAxis, translate_function->axis());
}

TEST_F(ParserTest, ParsesMatrixTransform) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: matrix(1, 2, 3, 4, 5, 6);",
                                   source_location_);

  scoped_refptr<cssom::TransformFunctionListValue> transform_list =
      dynamic_cast<cssom::TransformFunctionListValue*>(
          style->transform().get());
  ASSERT_NE(scoped_refptr<cssom::TransformFunctionListValue>(), transform_list);
  ASSERT_EQ(1, transform_list->value().size());

  const cssom::MatrixFunction* matrix_function =
      dynamic_cast<const cssom::MatrixFunction*>(transform_list->value()[0]);
  ASSERT_NE(static_cast<cssom::MatrixFunction*>(NULL), matrix_function);

  EXPECT_FLOAT_EQ(1.0f, matrix_function->value().Get(0, 0));
  EXPECT_FLOAT_EQ(3.0f, matrix_function->value().Get(0, 1));
  EXPECT_FLOAT_EQ(5.0f, matrix_function->value().Get(0, 2));
  EXPECT_FLOAT_EQ(2.0f, matrix_function->value().Get(1, 0));
  EXPECT_FLOAT_EQ(4.0f, matrix_function->value().Get(1, 1));
  EXPECT_FLOAT_EQ(6.0f, matrix_function->value().Get(1, 2));
  EXPECT_FLOAT_EQ(0.0f, matrix_function->value().Get(2, 0));
  EXPECT_FLOAT_EQ(0.0f, matrix_function->value().Get(2, 1));
  EXPECT_FLOAT_EQ(1.0f, matrix_function->value().Get(2, 2));
}

TEST_F(ParserTest, ParsesMultipleTransforms) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transform: scale(2) translateZ(10px);",
                                   source_location_);

  scoped_refptr<cssom::TransformFunctionListValue> transform_list =
      dynamic_cast<cssom::TransformFunctionListValue*>(
          style->transform().get());
  ASSERT_NE(scoped_refptr<cssom::TransformFunctionListValue>(), transform_list);
  ASSERT_EQ(2, transform_list->value().size());
  EXPECT_NE(static_cast<cssom::TransformFunction*>(NULL),
            transform_list->value()[0]);
  EXPECT_EQ(base::GetTypeId<cssom::ScaleFunction>(),
            transform_list->value()[0]->GetTypeId());
  EXPECT_NE(static_cast<cssom::TransformFunction*>(NULL),
            transform_list->value()[1]);
  EXPECT_EQ(base::GetTypeId<cssom::TranslateFunction>(),
            transform_list->value()[1]->GetTypeId());
}

TEST_F(ParserTest, RecoversFromInvalidTransformList) {
  EXPECT_CALL(
      parser_observer_,
      OnWarning(
          "[object ParserTest]:1:25: warning: invalid transform function"));

  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "transform: scale(0.001) warp(spacetime);\n"
          "color: #000;\n",
          source_location_);

  EXPECT_NE(scoped_refptr<cssom::PropertyValue>(), style->color());
}

TEST_F(ParserTest, ParsesBaselineVerticalAlign) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("vertical-align: baseline;",
                                   source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetBaseline(), style->vertical_align());
}

TEST_F(ParserTest, ParsesMiddleVerticalAlign) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("vertical-align: middle;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetMiddle(), style->vertical_align());
}

TEST_F(ParserTest, ParsesTopVerticalAlign) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("vertical-align: top;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetTop(), style->vertical_align());
}

TEST_F(ParserTest, ParsesWidth) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("width: 100px;", source_location_);

  scoped_refptr<cssom::LengthValue> width =
      dynamic_cast<cssom::LengthValue*>(style->width().get());
  ASSERT_NE(scoped_refptr<cssom::LengthValue>(), width);
  EXPECT_FLOAT_EQ(100, width->value());
  EXPECT_EQ(cssom::kPixelsUnit, width->unit());
}

TEST_F(ParserTest, ParsesValidPropertyValue) {
  scoped_refptr<cssom::RGBAColorValue> color =
      dynamic_cast<cssom::RGBAColorValue*>(
          parser_.ParsePropertyValue("color", "#c0ffee", source_location_)
              .get());
  EXPECT_NE(scoped_refptr<cssom::RGBAColorValue>(), color);
}

TEST_F(ParserTest, WarnsAboutImportantInPropertyValue) {
  EXPECT_CALL(parser_observer_,
              OnWarning(
                  "[object ParserTest]:1:1: warning: !important is not allowed "
                  "in property value"));

  scoped_refptr<cssom::PropertyValue> null_value = parser_.ParsePropertyValue(
      "color", "#f0000d !important", source_location_);
  EXPECT_EQ(scoped_refptr<cssom::PropertyValue>(), null_value);
}

TEST_F(ParserTest, FailsIfPropertyValueHasTrailingSemicolon) {
  EXPECT_CALL(
      parser_observer_,
      OnError("[object ParserTest]:1:8: error: unrecoverable syntax error"));

  scoped_refptr<cssom::PropertyValue> null_value =
      parser_.ParsePropertyValue("color", "#baaaad;", source_location_);
  EXPECT_EQ(scoped_refptr<cssom::PropertyValue>(), null_value);
}

TEST_F(ParserTest, WarnsAboutInvalidPropertyValue) {
  EXPECT_CALL(parser_observer_,
              OnWarning("[object ParserTest]:1:1: warning: unsupported value"));

  scoped_refptr<cssom::PropertyValue> null_value =
      parser_.ParsePropertyValue("color", "spring grass", source_location_);
  EXPECT_EQ(scoped_refptr<cssom::PropertyValue>(), null_value);
}

TEST_F(ParserTest, ParsesAnimatablePropertyNameListWithSingleValue) {
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          parser_.ParsePropertyValue("transition-property", "color",
                                     source_location_).get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());

  ASSERT_EQ(1, property_name_list->value().size());
  EXPECT_EQ(cssom::kColorPropertyName, property_name_list->value()[0]);
}

TEST_F(ParserTest, ParsesAnimatablePropertyNameListWithMultipleValues) {
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          parser_.ParsePropertyValue("transition-property",
                                     "color, transform , background-color",
                                     source_location_).get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());

  ASSERT_EQ(3, property_name_list->value().size());
  EXPECT_EQ(cssom::kColorPropertyName, property_name_list->value()[0]);
  EXPECT_EQ(cssom::kTransformPropertyName, property_name_list->value()[1]);
  EXPECT_EQ(cssom::kBackgroundColorPropertyName,
            property_name_list->value()[2]);
}

TEST_F(ParserTest, ParsesTransitionPropertyWithAllValue) {
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          parser_.ParsePropertyValue("transition-property", "all",
                                     source_location_).get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());

  ASSERT_EQ(1, property_name_list->value().size());
  EXPECT_EQ(cssom::kAllPropertyName, property_name_list->value()[0]);
}

TEST_F(ParserTest, ParsesTransitionPropertyWithNoneValue) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transition-property: none;",
                                   source_location_);

  scoped_refptr<cssom::KeywordValue> transition_property =
      dynamic_cast<cssom::KeywordValue*>(style->transition_property().get());
  EXPECT_EQ(cssom::KeywordValue::GetNone().get(), transition_property.get());
}

TEST_F(ParserTest, ParsesTimeListWithSingleValue) {
  scoped_refptr<cssom::TimeListValue> time_list_value =
      dynamic_cast<cssom::TimeListValue*>(
          parser_.ParsePropertyValue("transition-duration", "1s",
                                     source_location_).get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), time_list_value.get());

  ASSERT_EQ(1, time_list_value->value().size());
  EXPECT_DOUBLE_EQ(1.0, time_list_value->value()[0].InSecondsF());
}

TEST_F(ParserTest, ParsesTimeListWithMultipleValues) {
  scoped_refptr<cssom::TimeListValue> time_list_value =
      dynamic_cast<cssom::TimeListValue*>(
          parser_.ParsePropertyValue("transition-duration",
                                     "2s, 1ms, 0, 2ms, 3s, 3ms",
                                     source_location_).get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), time_list_value.get());

  ASSERT_EQ(6, time_list_value->value().size());
  EXPECT_DOUBLE_EQ(2, time_list_value->value()[0].InSecondsF());
  EXPECT_DOUBLE_EQ(0.001, time_list_value->value()[1].InSecondsF());
  EXPECT_DOUBLE_EQ(0, time_list_value->value()[2].InSecondsF());
  EXPECT_DOUBLE_EQ(0.002, time_list_value->value()[3].InSecondsF());
  EXPECT_DOUBLE_EQ(3, time_list_value->value()[4].InSecondsF());
  EXPECT_DOUBLE_EQ(0.003, time_list_value->value()[5].InSecondsF());
}

TEST_F(ParserTest, ParsesNegativeTimeList) {
  scoped_refptr<cssom::TimeListValue> time_list_value =
      dynamic_cast<cssom::TimeListValue*>(
          parser_.ParsePropertyValue("transition-duration", "-4s",
                                     source_location_).get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), time_list_value.get());

  ASSERT_EQ(1, time_list_value->value().size());
  EXPECT_DOUBLE_EQ(-4, time_list_value->value()[0].InSecondsF());
}

TEST_F(ParserTest, ParsesTransitionDelayWithSingleValue) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transition-delay: 1s;", source_location_);

  scoped_refptr<cssom::ListValue<base::TimeDelta> > transition_delay =
      dynamic_cast<cssom::ListValue<base::TimeDelta>*>(
          style->transition_delay().get());
  EXPECT_NE(static_cast<cssom::ListValue<base::TimeDelta>*>(NULL),
            transition_delay.get());
  ASSERT_EQ(1, transition_delay->value().size());
  EXPECT_DOUBLE_EQ(1, transition_delay->value()[0].InSecondsF());
}

TEST_F(ParserTest, ParsesTransitionDurationWithSingleValue) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transition-duration: 1s;",
                                   source_location_);

  scoped_refptr<cssom::TimeListValue> transition_duration =
      dynamic_cast<cssom::TimeListValue*>(style->transition_duration().get());
  EXPECT_NE(static_cast<cssom::TimeListValue*>(NULL),
            transition_duration.get());
  ASSERT_EQ(1, transition_duration->value().size());
  EXPECT_DOUBLE_EQ(1, transition_duration->value()[0].InSecondsF());
}

namespace {

scoped_refptr<cssom::TimingFunctionListValue> CreateSingleTimingFunctionValue(
    const scoped_refptr<cssom::TimingFunction>& timing_function) {
  scoped_ptr<cssom::TimingFunctionListValue::Builder>
      expected_result_list_builder(
          new cssom::TimingFunctionListValue::Builder());
  expected_result_list_builder->push_back(timing_function);
  return new cssom::TimingFunctionListValue(
      expected_result_list_builder.Pass());
}

// Returns true if the timing function obtained from parsing the passed in
// property declaration string is equal to the passed in expected timing
// function.
bool TestTransitionTimingFunctionKeyword(
    Parser* parser, const base::SourceLocation& source_location,
    const std::string& property_declaration_string,
    const scoped_refptr<cssom::TimingFunction>& expected_result) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser->ParseDeclarationList(
          "transition-timing-function: " + property_declaration_string + ";",
          source_location);

  scoped_refptr<cssom::TimingFunctionListValue> transition_timing_function =
      dynamic_cast<cssom::TimingFunctionListValue*>(
          style->transition_timing_function().get());
  CHECK_NE(static_cast<cssom::TimingFunctionListValue*>(NULL),
           transition_timing_function.get());

  scoped_refptr<cssom::TimingFunctionListValue> expected_result_list(
      CreateSingleTimingFunctionValue(expected_result));

  return expected_result_list->Equals(*transition_timing_function);
}

}  // namespace

TEST_F(ParserTest, ParsesTransitionTimingFunctionEaseKeyword) {
  EXPECT_TRUE(TestTransitionTimingFunctionKeyword(
      &parser_, source_location_, "ease",
      new cssom::CubicBezierTimingFunction(0.25f, 0.1f, 0.25f, 1.0f)));
}
TEST_F(ParserTest, ParsesTransitionTimingFunctionEaseInKeyword) {
  EXPECT_TRUE(TestTransitionTimingFunctionKeyword(
      &parser_, source_location_, "ease-in",
      new cssom::CubicBezierTimingFunction(0.42f, 0.0f, 1.0f, 1.0f)));
}
TEST_F(ParserTest, ParsesTransitionTimingFunctionEaseInOutKeyword) {
  EXPECT_TRUE(TestTransitionTimingFunctionKeyword(
      &parser_, source_location_, "ease-in-out",
      new cssom::CubicBezierTimingFunction(0.42f, 0.0f, 0.58f, 1.0f)));
}
TEST_F(ParserTest, ParsesTransitionTimingFunctionEaseOutKeyword) {
  EXPECT_TRUE(TestTransitionTimingFunctionKeyword(
      &parser_, source_location_, "ease-out",
      new cssom::CubicBezierTimingFunction(0.0f, 0.0f, 0.58f, 1.0f)));
}
TEST_F(ParserTest, ParsesTransitionTimingFunctionLinearKeyword) {
  EXPECT_TRUE(TestTransitionTimingFunctionKeyword(
      &parser_, source_location_, "linear",
      new cssom::CubicBezierTimingFunction(0.0f, 0.0f, 1.0f, 1.0f)));
}
TEST_F(ParserTest, ParsesTransitionTimingFunctionStepEndKeyword) {
  EXPECT_TRUE(TestTransitionTimingFunctionKeyword(
      &parser_, source_location_, "step-end",
      new cssom::SteppingTimingFunction(1,
                                        cssom::SteppingTimingFunction::kEnd)));
}
TEST_F(ParserTest, ParsesTransitionTimingFunctionStepStartKeyword) {
  EXPECT_TRUE(TestTransitionTimingFunctionKeyword(
      &parser_, source_location_, "step-start",
      new cssom::SteppingTimingFunction(
          1, cssom::SteppingTimingFunction::kStart)));
}
TEST_F(ParserTest, ParsesTransitionTimingFunctionArbitraryCubicBezierKeyword) {
  EXPECT_TRUE(TestTransitionTimingFunctionKeyword(
      &parser_, source_location_, "cubic-bezier(0.1, -2.0, 0.3, 2.0)",
      new cssom::CubicBezierTimingFunction(0.1f, -2.0f, 0.3f, 2.0f)));
}
TEST_F(ParserTest,
       ParsesTransitionTimingFunctionArbitrarySteppingStartKeyword) {
  EXPECT_TRUE(TestTransitionTimingFunctionKeyword(
      &parser_, source_location_, "steps(2, start)",
      new cssom::SteppingTimingFunction(
          2, cssom::SteppingTimingFunction::kStart)));
}
TEST_F(ParserTest, ParsesTransitionTimingFunctionArbitrarySteppingEndKeyword) {
  EXPECT_TRUE(TestTransitionTimingFunctionKeyword(
      &parser_, source_location_, "steps(2, end)",
      new cssom::SteppingTimingFunction(2,
                                        cssom::SteppingTimingFunction::kEnd)));
}
TEST_F(ParserTest,
       ParsesTransitionTimingFunctionArbitrarySteppingNoStartOrEndKeyword) {
  EXPECT_TRUE(TestTransitionTimingFunctionKeyword(
      &parser_, source_location_, "steps(2)",
      new cssom::SteppingTimingFunction(2,
                                        cssom::SteppingTimingFunction::kEnd)));
}

TEST_F(ParserTest, ParsesMultipleTransitionTimingFunctions) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "transition-timing-function: ease, ease-in, step-start;",
          source_location_);

  scoped_refptr<cssom::TimingFunctionListValue> transition_timing_function =
      dynamic_cast<cssom::TimingFunctionListValue*>(
          style->transition_timing_function().get());
  ASSERT_NE(static_cast<cssom::TimingFunctionListValue*>(NULL),
            transition_timing_function.get());

  scoped_ptr<cssom::TimingFunctionListValue::Builder>
      expected_result_list_builder(
          new cssom::TimingFunctionListValue::Builder());
  expected_result_list_builder->push_back(
      cssom::TimingFunction::GetEase().get());
  expected_result_list_builder->push_back(
      cssom::TimingFunction::GetEaseIn().get());
  expected_result_list_builder->push_back(
      cssom::TimingFunction::GetStepStart().get());
  scoped_refptr<cssom::TimingFunctionListValue> expected_result_list(
      new cssom::TimingFunctionListValue(expected_result_list_builder.Pass()));

  EXPECT_TRUE(expected_result_list->Equals(*transition_timing_function));
}

TEST_F(ParserTest, AboveRangeCubicBezierP1XParameterProduceError) {
  EXPECT_CALL(parser_observer_,
              OnError(
                  "[object ParserTest]:1:1: error: cubic-bezier control point "
                  "x values must be in the range [0, 1]."));
  scoped_refptr<cssom::PropertyValue> error_value = parser_.ParsePropertyValue(
      "transition-timing-function", "cubic-bezier(2, 0, 0.5, 0)",
      source_location_);
  // Test that the ease function was returned in place of the error function.
  EXPECT_TRUE(error_value->Equals(*CreateSingleTimingFunctionValue(
                                      cssom::TimingFunction::GetEase().get())));
}
TEST_F(ParserTest, BelowRangeCubicBezierP1XParameterProduceError) {
  EXPECT_CALL(parser_observer_,
              OnError(
                  "[object ParserTest]:1:1: error: cubic-bezier control point "
                  "x values must be in the range [0, 1]."));
  scoped_refptr<cssom::PropertyValue> error_value = parser_.ParsePropertyValue(
      "transition-timing-function", "cubic-bezier(-1, 0, 0.5, 0)",
      source_location_);
  // Test that the ease function was returned in place of the error function.
  EXPECT_TRUE(error_value->Equals(*CreateSingleTimingFunctionValue(
                                      cssom::TimingFunction::GetEase().get())));
}
TEST_F(ParserTest, AboveRangeCubicBezierP2XParameterProduceError) {
  EXPECT_CALL(parser_observer_,
              OnError(
                  "[object ParserTest]:1:1: error: cubic-bezier control point "
                  "x values must be in the range [0, 1]."));
  scoped_refptr<cssom::PropertyValue> error_value = parser_.ParsePropertyValue(
      "transition-timing-function", "cubic-bezier(0.5, 0, 2, 0)",
      source_location_);
  // Test that the ease function was returned in place of the error function.
  EXPECT_TRUE(error_value->Equals(*CreateSingleTimingFunctionValue(
                                      cssom::TimingFunction::GetEase().get())));
}
TEST_F(ParserTest, BelowRangeCubicBezierP2XParameterProduceError) {
  EXPECT_CALL(parser_observer_,
              OnError(
                  "[object ParserTest]:1:1: error: cubic-bezier control point "
                  "x values must be in the range [0, 1]."));
  scoped_refptr<cssom::PropertyValue> error_value = parser_.ParsePropertyValue(
      "transition-timing-function", "cubic-bezier(0.5, 0, -1, 0)",
      source_location_);
  // Test that the ease function was returned in place of the error function.
  EXPECT_TRUE(error_value->Equals(*CreateSingleTimingFunctionValue(
                                      cssom::TimingFunction::GetEase().get())));
}

TEST_F(ParserTest, ParsesTransitionShorthandOfMultipleItemsWithNoDefaults) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "transition: background-color 1s ease-in 0.5s, "
          "transform 2s ease-out 0.5s;",
          source_location_);

  // Test transition-property was set properly.
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          style->transition_property().get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());
  ASSERT_EQ(2, property_name_list->value().size());
  EXPECT_EQ(cssom::kBackgroundColorPropertyName,
            property_name_list->value()[0]);
  EXPECT_EQ(cssom::kTransformPropertyName, property_name_list->value()[1]);

  // Test transition-duration was set properly.
  scoped_refptr<cssom::TimeListValue> duration_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_duration().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), duration_list.get());
  ASSERT_EQ(2, duration_list->value().size());
  EXPECT_DOUBLE_EQ(1, duration_list->value()[0].InSecondsF());
  EXPECT_DOUBLE_EQ(2, duration_list->value()[1].InSecondsF());

  // Test transition-timing-function was set properly.
  scoped_refptr<cssom::TimingFunctionListValue> timing_function_list =
      dynamic_cast<cssom::TimingFunctionListValue*>(
          style->transition_timing_function().get());
  ASSERT_NE(static_cast<cssom::TimingFunctionListValue*>(NULL),
            timing_function_list.get());
  ASSERT_EQ(2, timing_function_list->value().size());
  EXPECT_TRUE(timing_function_list->value()[0]->Equals(
      *cssom::TimingFunction::GetEaseIn()));
  EXPECT_TRUE(timing_function_list->value()[1]->Equals(
      *cssom::TimingFunction::GetEaseOut()));

  // Test transition-delay was set properly.
  scoped_refptr<cssom::TimeListValue> delay_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_delay().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), delay_list.get());
  ASSERT_EQ(2, delay_list->value().size());
  EXPECT_DOUBLE_EQ(0.5, delay_list->value()[0].InSecondsF());
  EXPECT_DOUBLE_EQ(0.5, delay_list->value()[1].InSecondsF());
}

TEST_F(ParserTest,
       ParsesTransitionShorthandOfSingleItemWith1PropertySpecified) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transition: background-color;",
                                   source_location_);

  // Test transition-property was set properly.
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          style->transition_property().get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());
  ASSERT_EQ(1, property_name_list->value().size());
  EXPECT_EQ(cssom::kBackgroundColorPropertyName,
            property_name_list->value()[0]);

  // Test transition-duration was set properly.
  scoped_refptr<cssom::TimeListValue> duration_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_duration().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), duration_list.get());
  ASSERT_EQ(1, duration_list->value().size());
  EXPECT_EQ(
      base::polymorphic_downcast<const cssom::TimeListValue*>(
          cssom::GetInitialStyle()->transition_duration().get())->value()[0],
      duration_list->value()[0]);

  // Test transition-timing-function was set properly.
  scoped_refptr<cssom::TimingFunctionListValue> timing_function_list =
      dynamic_cast<cssom::TimingFunctionListValue*>(
          style->transition_timing_function().get());
  ASSERT_NE(static_cast<cssom::TimingFunctionListValue*>(NULL),
            timing_function_list.get());
  ASSERT_EQ(1, timing_function_list->value().size());
  EXPECT_TRUE(base::polymorphic_downcast<const cssom::TimingFunctionListValue*>(
                  cssom::GetInitialStyle()->transition_timing_function().get())
                  ->value()[0]
                  ->Equals(*timing_function_list->value()[0]));

  // Test transition-delay was set properly.
  scoped_refptr<cssom::TimeListValue> delay_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_delay().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), delay_list.get());
  ASSERT_EQ(1, delay_list->value().size());
  EXPECT_EQ(base::polymorphic_downcast<const cssom::TimeListValue*>(
                cssom::GetInitialStyle()->transition_delay().get())->value()[0],
            delay_list->value()[0]);
}

TEST_F(ParserTest,
       ParsesTransitionShorthandOfSingleItemWith2PropertiesSpecified) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transition: background-color 1s;",
                                   source_location_);

  // Test transition-property was set properly.
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          style->transition_property().get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());
  ASSERT_EQ(1, property_name_list->value().size());
  EXPECT_EQ(cssom::kBackgroundColorPropertyName,
            property_name_list->value()[0]);

  // Test transition-duration was set properly.
  scoped_refptr<cssom::TimeListValue> duration_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_duration().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), duration_list.get());
  ASSERT_EQ(1, duration_list->value().size());
  EXPECT_DOUBLE_EQ(1, duration_list->value()[0].InSecondsF());

  // Test transition-timing-function was set properly.
  scoped_refptr<cssom::TimingFunctionListValue> timing_function_list =
      dynamic_cast<cssom::TimingFunctionListValue*>(
          style->transition_timing_function().get());
  ASSERT_NE(static_cast<cssom::TimingFunctionListValue*>(NULL),
            timing_function_list.get());
  ASSERT_EQ(1, timing_function_list->value().size());
  EXPECT_TRUE(base::polymorphic_downcast<const cssom::TimingFunctionListValue*>(
                  cssom::GetInitialStyle()->transition_timing_function().get())
                  ->value()[0]
                  ->Equals(*timing_function_list->value()[0]));

  // Test transition-delay was set properly.
  scoped_refptr<cssom::TimeListValue> delay_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_delay().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), delay_list.get());
  ASSERT_EQ(1, delay_list->value().size());
  EXPECT_EQ(base::polymorphic_downcast<const cssom::TimeListValue*>(
                cssom::GetInitialStyle()->transition_delay().get())->value()[0],
            delay_list->value()[0]);
}

TEST_F(ParserTest,
       ParsesTransitionShorthandOfSingleItemWith3PropertiesSpecified) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transition: background-color 1s ease-in;",
                                   source_location_);

  // Test transition-property was set properly.
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          style->transition_property().get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());
  ASSERT_EQ(1, property_name_list->value().size());
  EXPECT_EQ(cssom::kBackgroundColorPropertyName,
            property_name_list->value()[0]);

  // Test transition-duration was set properly.
  scoped_refptr<cssom::TimeListValue> duration_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_duration().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), duration_list.get());
  ASSERT_EQ(1, duration_list->value().size());
  EXPECT_DOUBLE_EQ(1, duration_list->value()[0].InSecondsF());

  // Test transition-timing-function was set properly.
  scoped_refptr<cssom::TimingFunctionListValue> timing_function_list =
      dynamic_cast<cssom::TimingFunctionListValue*>(
          style->transition_timing_function().get());
  ASSERT_NE(static_cast<cssom::TimingFunctionListValue*>(NULL),
            timing_function_list.get());
  ASSERT_EQ(1, timing_function_list->value().size());
  EXPECT_TRUE(timing_function_list->value()[0]->Equals(
      *cssom::TimingFunction::GetEaseIn()));

  // Test transition-delay was set properly.
  scoped_refptr<cssom::TimeListValue> delay_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_delay().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), delay_list.get());
  ASSERT_EQ(1, delay_list->value().size());
  EXPECT_EQ(base::polymorphic_downcast<const cssom::TimeListValue*>(
                cssom::GetInitialStyle()->transition_delay().get())->value()[0],
            delay_list->value()[0]);
}

TEST_F(
    ParserTest,
    ParsesTransitionShorthandOfSingleItemWith3PropertiesSpecifiedNotInOrder) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transition: ease-in background-color 1s;",
                                   source_location_);

  // Test transition-property was set properly.
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          style->transition_property().get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());
  ASSERT_EQ(1, property_name_list->value().size());
  EXPECT_EQ(cssom::kBackgroundColorPropertyName,
            property_name_list->value()[0]);

  // Test transition-duration was set properly.
  scoped_refptr<cssom::TimeListValue> duration_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_duration().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), duration_list.get());
  ASSERT_EQ(1, duration_list->value().size());
  EXPECT_DOUBLE_EQ(1, duration_list->value()[0].InSecondsF());

  // Test transition-timing-function was set properly.
  scoped_refptr<cssom::TimingFunctionListValue> timing_function_list =
      dynamic_cast<cssom::TimingFunctionListValue*>(
          style->transition_timing_function().get());
  ASSERT_NE(static_cast<cssom::TimingFunctionListValue*>(NULL),
            timing_function_list.get());
  ASSERT_EQ(1, timing_function_list->value().size());
  EXPECT_TRUE(timing_function_list->value()[0]->Equals(
      *cssom::TimingFunction::GetEaseIn()));

  // Test transition-delay was set properly.
  scoped_refptr<cssom::TimeListValue> delay_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_delay().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), delay_list.get());
  ASSERT_EQ(1, delay_list->value().size());
  EXPECT_EQ(base::polymorphic_downcast<const cssom::TimeListValue*>(
                cssom::GetInitialStyle()->transition_delay().get())->value()[0],
            delay_list->value()[0]);
}

TEST_F(ParserTest,
       ParsesTransitionShorthandOfMultipleItemsWith2PropertiesSpecified) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "transition: background-color 1s, transform 2s;", source_location_);

  // Test transition-property was set properly.
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          style->transition_property().get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());
  ASSERT_EQ(2, property_name_list->value().size());
  EXPECT_EQ(cssom::kBackgroundColorPropertyName,
            property_name_list->value()[0]);
  EXPECT_EQ(cssom::kTransformPropertyName, property_name_list->value()[1]);

  // Test transition-duration was set properly.
  scoped_refptr<cssom::TimeListValue> duration_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_duration().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), duration_list.get());
  ASSERT_EQ(2, duration_list->value().size());
  EXPECT_DOUBLE_EQ(1, duration_list->value()[0].InSecondsF());
  EXPECT_DOUBLE_EQ(2, duration_list->value()[1].InSecondsF());

  // Test transition-timing-function was set properly.
  scoped_refptr<cssom::TimingFunctionListValue> timing_function_list =
      dynamic_cast<cssom::TimingFunctionListValue*>(
          style->transition_timing_function().get());
  ASSERT_NE(static_cast<cssom::TimingFunctionListValue*>(NULL),
            timing_function_list.get());
  ASSERT_EQ(2, timing_function_list->value().size());
  EXPECT_TRUE(base::polymorphic_downcast<const cssom::TimingFunctionListValue*>(
                  cssom::GetInitialStyle()->transition_timing_function().get())
                  ->value()[0]
                  ->Equals(*timing_function_list->value()[0]));
  EXPECT_TRUE(base::polymorphic_downcast<const cssom::TimingFunctionListValue*>(
                  cssom::GetInitialStyle()->transition_timing_function().get())
                  ->value()[0]
                  ->Equals(*timing_function_list->value()[1]));

  // Test transition-delay was set properly.
  scoped_refptr<cssom::TimeListValue> delay_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_delay().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), delay_list.get());
  ASSERT_EQ(2, delay_list->value().size());
  EXPECT_EQ(base::polymorphic_downcast<const cssom::TimeListValue*>(
                cssom::GetInitialStyle()->transition_delay().get())->value()[0],
            delay_list->value()[0]);
  EXPECT_EQ(base::polymorphic_downcast<const cssom::TimeListValue*>(
                cssom::GetInitialStyle()->transition_delay().get())->value()[0],
            delay_list->value()[1]);
}

TEST_F(ParserTest, ParsesTransitionShorthandWithErrorBeforeSemicolon) {
  EXPECT_CALL(parser_observer_, OnError(
                                    "[object ParserTest]:1:16: error: "
                                    "unsupported property value for animation"))
      .Times(AtLeast(1));

  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "transition: 1s voodoo-magic; display: inline;", source_location_);

  // Test transition-property was set properly.
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          style->transition_property().get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());
  ASSERT_EQ(0, property_name_list->value().size());

  // Test transition-duration was set properly.
  scoped_refptr<cssom::TimeListValue> duration_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_duration().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), duration_list.get());
  ASSERT_EQ(0, duration_list->value().size());

  // Test transition-timing-function was set properly.
  scoped_refptr<cssom::TimingFunctionListValue> timing_function_list =
      dynamic_cast<cssom::TimingFunctionListValue*>(
          style->transition_timing_function().get());
  ASSERT_NE(static_cast<cssom::TimingFunctionListValue*>(NULL),
            timing_function_list.get());
  ASSERT_EQ(0, timing_function_list->value().size());

  // Test transition-delay was set properly.
  scoped_refptr<cssom::TimeListValue> delay_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_delay().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), delay_list.get());
  ASSERT_EQ(0, delay_list->value().size());

  // Test that despite the error, display inline was still set correctly.
  EXPECT_EQ(cssom::KeywordValue::GetInline(), style->display());
}

TEST_F(ParserTest, ParsesTransitionShorthandWithErrorBeforeSpace) {
  EXPECT_CALL(parser_observer_, OnError(
                                    "[object ParserTest]:1:16: error: "
                                    "unsupported property value for animation"))
      .Times(AtLeast(1));

  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transition: 1s voodoo-magic 2s;",
                                   source_location_);

  // Test transition-property was set properly.
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          style->transition_property().get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());
  ASSERT_EQ(0, property_name_list->value().size());

  // Test transition-duration was set properly.
  scoped_refptr<cssom::TimeListValue> duration_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_duration().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), duration_list.get());
  ASSERT_EQ(0, duration_list->value().size());

  // Test transition-timing-function was set properly.
  scoped_refptr<cssom::TimingFunctionListValue> timing_function_list =
      dynamic_cast<cssom::TimingFunctionListValue*>(
          style->transition_timing_function().get());
  ASSERT_NE(static_cast<cssom::TimingFunctionListValue*>(NULL),
            timing_function_list.get());
  ASSERT_EQ(0, timing_function_list->value().size());

  // Test transition-delay was set properly.
  scoped_refptr<cssom::TimeListValue> delay_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_delay().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), delay_list.get());
  ASSERT_EQ(0, delay_list->value().size());
}

TEST_F(ParserTest,
       ParsesTransitionShorthandIgnoringErrorButProceedingWithNonError) {
  EXPECT_CALL(parser_observer_, OnError(
                                    "[object ParserTest]:1:16: error: "
                                    "unsupported property value for animation"))
      .Times(AtLeast(1));

  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("transition: 1s voodoo-magic, transform 2s;",
                                   source_location_);

  // Test transition-property was set properly.
  scoped_refptr<cssom::ConstStringListValue> property_name_list =
      dynamic_cast<cssom::ConstStringListValue*>(
          style->transition_property().get());
  ASSERT_NE(static_cast<cssom::ConstStringListValue*>(NULL),
            property_name_list.get());
  ASSERT_EQ(1, property_name_list->value().size());
  EXPECT_EQ(cssom::kTransformPropertyName, property_name_list->value()[0]);

  // Test transition-duration was set properly.
  scoped_refptr<cssom::TimeListValue> duration_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_duration().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), duration_list.get());
  ASSERT_EQ(1, duration_list->value().size());
  EXPECT_DOUBLE_EQ(2, duration_list->value()[0].InSecondsF());

  // Test transition-timing-function was set properly.
  scoped_refptr<cssom::TimingFunctionListValue> timing_function_list =
      dynamic_cast<cssom::TimingFunctionListValue*>(
          style->transition_timing_function().get());
  ASSERT_NE(static_cast<cssom::TimingFunctionListValue*>(NULL),
            timing_function_list.get());
  ASSERT_EQ(1, timing_function_list->value().size());
  EXPECT_TRUE(base::polymorphic_downcast<const cssom::TimingFunctionListValue*>(
                  cssom::GetInitialStyle()->transition_timing_function().get())
                  ->value()[0]
                  ->Equals(*timing_function_list->value()[0]));

  // Test transition-delay was set properly.
  scoped_refptr<cssom::TimeListValue> delay_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_delay().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), delay_list.get());
  ASSERT_EQ(1, delay_list->value().size());
  EXPECT_EQ(base::polymorphic_downcast<const cssom::TimeListValue*>(
                cssom::GetInitialStyle()->transition_delay().get())->value()[0],
            delay_list->value()[0]);
}

TEST_F(ParserTest, ParsesTransitionShorthandWithNoneIsValid) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList(
          "transition: none 0s;", source_location_);

  // Test transition-property was set properly.
  EXPECT_EQ(cssom::KeywordValue::GetNone(), style->transition_property());

  // Test transition-duration was set properly.
  scoped_refptr<cssom::TimeListValue> duration_list =
      dynamic_cast<cssom::TimeListValue*>(style->transition_duration().get());
  ASSERT_NE(static_cast<cssom::TimeListValue*>(NULL), duration_list.get());
  ASSERT_EQ(1, duration_list->value().size());
  EXPECT_DOUBLE_EQ(0, duration_list->value()[0].InSecondsF());
}

TEST_F(ParserTest, ParsesLeftWithLength) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("left: 10px;", source_location_);

  scoped_refptr<cssom::LengthValue> length =
      dynamic_cast<cssom::LengthValue*>(style->left().get());
  ASSERT_TRUE(length != NULL);
  EXPECT_EQ(10, length->value());
  EXPECT_EQ(cssom::kPixelsUnit, length->unit());
}

TEST_F(ParserTest, ParsesLeftWithPercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("left: 10%;", source_location_);

  scoped_refptr<cssom::PercentageValue> percentage =
      dynamic_cast<cssom::PercentageValue*>(style->left().get());
  ASSERT_TRUE(percentage != NULL);
  EXPECT_FLOAT_EQ(0.1f, percentage->value());
}

TEST_F(ParserTest, ParsesLeftWithNegativePercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("left: -10%;", source_location_);

  scoped_refptr<cssom::PercentageValue> percentage =
      dynamic_cast<cssom::PercentageValue*>(style->left().get());
  ASSERT_TRUE(percentage != NULL);
  EXPECT_FLOAT_EQ(-0.1f, percentage->value());
}

TEST_F(ParserTest, ParsesLeftWithAuto) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("left: auto;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetAuto(), style->left());
}

TEST_F(ParserTest, ParsesTopWithLength) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("top: 10px;", source_location_);

  scoped_refptr<cssom::LengthValue> length =
      dynamic_cast<cssom::LengthValue*>(style->top().get());
  ASSERT_TRUE(length != NULL);
  EXPECT_EQ(10, length->value());
  EXPECT_EQ(cssom::kPixelsUnit, length->unit());
}

TEST_F(ParserTest, ParsesTopWithPercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("top: 10%;", source_location_);

  scoped_refptr<cssom::PercentageValue> percentage =
      dynamic_cast<cssom::PercentageValue*>(style->top().get());
  ASSERT_TRUE(percentage != NULL);
  EXPECT_FLOAT_EQ(0.1f, percentage->value());
}

TEST_F(ParserTest, ParsesTopWithNegativePercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("top: -10%;", source_location_);

  scoped_refptr<cssom::PercentageValue> percentage =
      dynamic_cast<cssom::PercentageValue*>(style->top().get());
  ASSERT_TRUE(percentage != NULL);
  EXPECT_FLOAT_EQ(-0.1f, percentage->value());
}

TEST_F(ParserTest, ParsesTopWithAuto) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("top: auto;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetAuto(), style->top());
}

TEST_F(ParserTest, ParsesRightWithLength) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("right: 10px;", source_location_);

  scoped_refptr<cssom::LengthValue> length =
      dynamic_cast<cssom::LengthValue*>(style->right().get());
  ASSERT_TRUE(length != NULL);
  EXPECT_EQ(10, length->value());
  EXPECT_EQ(cssom::kPixelsUnit, length->unit());
}

TEST_F(ParserTest, ParsesRightWithPercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("right: 10%;", source_location_);

  scoped_refptr<cssom::PercentageValue> percentage =
      dynamic_cast<cssom::PercentageValue*>(style->right().get());
  ASSERT_TRUE(percentage != NULL);
  EXPECT_FLOAT_EQ(0.1f, percentage->value());
}

TEST_F(ParserTest, ParsesRightWithNegativePercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("right: -10%;", source_location_);

  scoped_refptr<cssom::PercentageValue> percentage =
      dynamic_cast<cssom::PercentageValue*>(style->right().get());
  ASSERT_TRUE(percentage != NULL);
  EXPECT_FLOAT_EQ(-0.1f, percentage->value());
}

TEST_F(ParserTest, ParsesRightWithAuto) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("right: auto;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetAuto(), style->right());
}

TEST_F(ParserTest, ParsesBottomWithLength) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("bottom: 10px;", source_location_);

  scoped_refptr<cssom::LengthValue> length =
      dynamic_cast<cssom::LengthValue*>(style->bottom().get());
  ASSERT_TRUE(length != NULL);
  EXPECT_EQ(10, length->value());
  EXPECT_EQ(cssom::kPixelsUnit, length->unit());
}

TEST_F(ParserTest, ParsesBottomWithPercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("bottom: 10%;", source_location_);

  scoped_refptr<cssom::PercentageValue> percentage =
      dynamic_cast<cssom::PercentageValue*>(style->bottom().get());
  ASSERT_TRUE(percentage != NULL);
  EXPECT_FLOAT_EQ(0.1f, percentage->value());
}

TEST_F(ParserTest, ParsesBottomWithNegativePercentage) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("bottom: -10%;", source_location_);

  scoped_refptr<cssom::PercentageValue> percentage =
      dynamic_cast<cssom::PercentageValue*>(style->bottom().get());
  ASSERT_TRUE(percentage != NULL);
  EXPECT_FLOAT_EQ(-0.1f, percentage->value());
}

TEST_F(ParserTest, ParsesBottomWithAuto) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("bottom: auto;", source_location_);

  EXPECT_EQ(cssom::KeywordValue::GetAuto(), style->bottom());
}

TEST_F(ParserTest, ParsesZIndex) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("z-index: 3;", source_location_);

  scoped_refptr<cssom::IntegerValue> z_index =
      dynamic_cast<cssom::IntegerValue*>(style->z_index().get());
  ASSERT_NE(scoped_refptr<cssom::IntegerValue>(), z_index);
  EXPECT_EQ(3, z_index->value());
}

TEST_F(ParserTest, ParsesNegativeZIndex) {
  scoped_refptr<cssom::CSSStyleDeclarationData> style =
      parser_.ParseDeclarationList("z-index: -2;", source_location_);

  scoped_refptr<cssom::IntegerValue> z_index =
      dynamic_cast<cssom::IntegerValue*>(style->z_index().get());
  ASSERT_NE(scoped_refptr<cssom::IntegerValue>(), z_index);
  EXPECT_EQ(-2, z_index->value());
}

}  // namespace css_parser
}  // namespace cobalt
