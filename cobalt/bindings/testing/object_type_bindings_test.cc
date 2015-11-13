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

#include "cobalt/bindings/testing/base_interface.h"
#include "cobalt/bindings/testing/bindings_test_base.h"
#include "cobalt/bindings/testing/derived_interface.h"
#include "cobalt/bindings/testing/object_type_bindings_interface.h"
#include "cobalt/bindings/testing/script_object_owner.h"
#include "testing/gtest/include/gtest/gtest.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;

namespace cobalt {
namespace bindings {
namespace testing {

namespace {
class ObjectTypeBindingsTest
    : public InterfaceBindingsTest<ObjectTypeBindingsInterface> {
 public:
  ObjectTypeBindingsTest()
      : arbitrary_interface_(new StrictMock<ArbitraryInterface>()),
        base_interface_(new StrictMock<BaseInterface>()),
        derived_interface_(new StrictMock<DerivedInterface>()) {
    ON_CALL(test_mock(), arbitrary_object())
        .WillByDefault(Return(arbitrary_interface_));
    ON_CALL(test_mock(), base_interface())
        .WillByDefault(Return(base_interface_));
    ON_CALL(test_mock(), derived_interface())
        .WillByDefault(Return(derived_interface_));
  }

 protected:
  const scoped_refptr<ArbitraryInterface> arbitrary_interface_;
  const scoped_refptr<BaseInterface> base_interface_;
  const scoped_refptr<DerivedInterface> derived_interface_;
};
}  // namespace

TEST_F(ObjectTypeBindingsTest, InstanceOfObject) {
  EXPECT_CALL(test_mock(), arbitrary_object());

  std::string result;
  EXPECT_TRUE(
      EvaluateScript("test.arbitraryObject instanceof Object;", &result));
  EXPECT_STREQ("true", result.c_str());
}

TEST_F(ObjectTypeBindingsTest, Prototype) {
  EXPECT_CALL(test_mock(), arbitrary_object());

  std::string result;
  EXPECT_TRUE(
      EvaluateScript("Object.getPrototypeOf(test.arbitraryObject);", &result));
  EXPECT_STREQ("[object ArbitraryInterfacePrototype]", result.c_str());
}

TEST_F(ObjectTypeBindingsTest, PropertyIsOwnProperty) {
  EXPECT_CALL(test_mock(), arbitrary_object());

  std::string result;
  EXPECT_TRUE(EvaluateScript(
      "test.arbitraryObject.hasOwnProperty(\"arbitraryProperty\");", &result));
  EXPECT_STREQ("true", result.c_str());
}

TEST_F(ObjectTypeBindingsTest, MemberFunctionIsPrototypeProperty) {
  EXPECT_CALL(test_mock(), arbitrary_object()).Times(3);

  std::string result;
  EXPECT_TRUE(EvaluateScript(
      "test.arbitraryObject.hasOwnProperty(\"arbitraryFunction\");", &result));
  EXPECT_STREQ("false", result.c_str());

  EXPECT_TRUE(EvaluateScript(
      "Object.getPrototypeOf(test.arbitraryObject).hasOwnProperty("
      "\"arbitraryFunction\");",
      &result));
  EXPECT_STREQ("true", result.c_str());

  EXPECT_TRUE(EvaluateScript("\"arbitraryFunction\" in test.arbitraryObject;",
                             &result));
  EXPECT_STREQ("true", result.c_str());
}

TEST_F(ObjectTypeBindingsTest, DerivedProto) {
  EXPECT_CALL(test_mock(), derived_interface());

  std::string result;
  EXPECT_TRUE(EvaluateScript(
      "Object.getPrototypeOf(test.derivedInterface) === "
      "DerivedInterface.prototype;",
      &result));
  EXPECT_STREQ("true", result.c_str());
}

TEST_F(ObjectTypeBindingsTest, PropertyOnBaseClass) {
  EXPECT_CALL(test_mock(), derived_interface());
  EXPECT_CALL(*derived_interface_, base_attribute())
      .WillOnce(Return("mock_value"));

  std::string result;
  EXPECT_TRUE(EvaluateScript("test.derivedInterface.baseAttribute;", &result));
  EXPECT_STREQ("mock_value", result.c_str());
}

TEST_F(ObjectTypeBindingsTest, OperationOnBaseClass) {
  EXPECT_CALL(test_mock(), derived_interface());
  EXPECT_CALL(*derived_interface_, BaseOperation());

  EXPECT_TRUE(EvaluateScript("test.derivedInterface.baseOperation();", NULL));
}

TEST_F(ObjectTypeBindingsTest, PropertyOnDerivedClass) {
  EXPECT_CALL(test_mock(), derived_interface());
  EXPECT_CALL(*derived_interface_, derived_attribute())
      .WillOnce(Return("mock_value"));

  std::string result;
  EXPECT_TRUE(
      EvaluateScript("test.derivedInterface.derivedAttribute;", &result));
  EXPECT_STREQ("mock_value", result.c_str());
}

TEST_F(ObjectTypeBindingsTest, OperationOnDerivedClass) {
  EXPECT_CALL(test_mock(), derived_interface());
  EXPECT_CALL(*derived_interface_, DerivedOperation());

  EXPECT_TRUE(
      EvaluateScript("test.derivedInterface.derivedOperation();", NULL));
}

TEST_F(ObjectTypeBindingsTest, ReturnDerivedClassWrapper) {
  ON_CALL(test_mock(), base_interface())
      .WillByDefault(Return(derived_interface_));

  std::string result;
  EXPECT_CALL(test_mock(), base_interface());
  EXPECT_TRUE(
      EvaluateScript("Object.getPrototypeOf(test.baseInterface);", &result));
  EXPECT_STREQ("[object DerivedInterfacePrototype]", result.c_str());

  EXPECT_CALL(test_mock(), base_interface());
  EXPECT_CALL(*derived_interface_, DerivedOperation());
  EXPECT_TRUE(EvaluateScript("test.baseInterface.derivedOperation();", NULL));
}

TEST_F(ObjectTypeBindingsTest, ReturnCorrectWrapperForObjectType) {
  typedef ScriptObjectOwner<script::OpaqueHandleHolder> ObjectOwner;
  ObjectOwner object_owner(&test_mock());

  // Store the Wrappable object as an opaque handle.
  EXPECT_CALL(test_mock(), set_object_property(_))
      .WillOnce(Invoke(&object_owner, &ObjectOwner::TakeOwnership));
  EXPECT_TRUE(
      EvaluateScript("test.objectProperty = new ArbitraryInterface();", NULL));
  ASSERT_TRUE(object_owner.IsSet());

  // Return it back to JS as the original type.
  EXPECT_CALL(test_mock(), object_property())
      .WillOnce(Return(&object_owner.reference().referenced_object()));
  EXPECT_TRUE(
      EvaluateScript("Object.getPrototypeOf(test.objectProperty) === "
                     "ArbitraryInterface.prototype;",
                     NULL));
}

TEST_F(ObjectTypeBindingsTest, PassUserObjectforObjectType) {
  typedef ScriptObjectOwner<script::OpaqueHandleHolder> ObjectOwner;
  ObjectOwner object_owner(&test_mock());

  EXPECT_TRUE(EvaluateScript("var obj = new Object();", NULL));
  EXPECT_CALL(test_mock(), set_object_property(_))
      .WillOnce(Invoke(&object_owner, &ObjectOwner::TakeOwnership));
  EXPECT_TRUE(EvaluateScript("test.objectProperty = obj;", NULL));
  ASSERT_TRUE(object_owner.IsSet());

  EXPECT_CALL(test_mock(), object_property())
      .WillOnce(Return(&object_owner.reference().referenced_object()));
  EXPECT_TRUE(EvaluateScript("test.objectProperty === obj;", NULL));
}

TEST_F(ObjectTypeBindingsTest, NullObject) {
  typedef ScriptObjectOwner<script::OpaqueHandleHolder> ObjectOwner;
  ObjectOwner object_owner(&test_mock());

  EXPECT_CALL(test_mock(), set_object_property(_))
      .WillOnce(Invoke(&object_owner, &ObjectOwner::TakeOwnership));
  EXPECT_TRUE(EvaluateScript("test.objectProperty = null;", NULL));
  EXPECT_FALSE(object_owner.IsSet());

  EXPECT_CALL(test_mock(), object_property());
  EXPECT_TRUE(EvaluateScript("test.objectProperty === null;", NULL));
}

TEST_F(ObjectTypeBindingsTest, NonObjectType) {
  EXPECT_FALSE(EvaluateScript("test.objectProperty = 5;", NULL));
  EXPECT_FALSE(EvaluateScript("test.objectProperty = false;", NULL));
  EXPECT_FALSE(EvaluateScript("test.objectProperty = \"string\";", NULL));
  EXPECT_FALSE(EvaluateScript("test.objectProperty = undefined;", NULL));
}

}  // namespace testing
}  // namespace bindings
}  // namespace cobalt
