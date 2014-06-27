// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo/common/common_type_converters.h"

#include "base/strings/utf_string_conversions.h"
#include "mojo/public/bindings/allocation_scope.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace mojo {
namespace common {
namespace test {
namespace {

void ExpectEqualsStringPiece(const std::string& expected,
                             const base::StringPiece& str) {
  EXPECT_EQ(expected, str.as_string());
}

void ExpectEqualsMojoString(const std::string& expected,
                            const String& str) {
  EXPECT_EQ(expected, str.To<std::string>());
}

void ExpectEqualsString16(const base::string16& expected,
                          const base::string16& actual) {
  EXPECT_EQ(expected, actual);
}

void ExpectEqualsMojoString(const base::string16& expected,
                            const String& str) {
  EXPECT_EQ(expected, str.To<base::string16>());
}

}  // namespace

TEST(CommonTypeConvertersTest, StringPiece) {
  AllocationScope scope;

  std::string kText("hello world");

  base::StringPiece string_piece(kText);
  String mojo_string(string_piece);

  ExpectEqualsMojoString(kText, mojo_string);
  ExpectEqualsStringPiece(kText, mojo_string.To<base::StringPiece>());

  // Test implicit construction and conversion:
  ExpectEqualsMojoString(kText, string_piece);
  ExpectEqualsStringPiece(kText, mojo_string);

  // Test null String:
  base::StringPiece empty_string_piece = String();
  EXPECT_TRUE(empty_string_piece.empty());
}

TEST(CommonTypeConvertersTest, String16) {
  AllocationScope scope;

  const base::string16 string16(base::ASCIIToUTF16("hello world"));
  const String mojo_string(string16);

  ExpectEqualsMojoString(string16, mojo_string);
  EXPECT_EQ(string16, mojo_string.To<base::string16>());

  // Test implicit construction and conversion:
  ExpectEqualsMojoString(string16, string16);
  ExpectEqualsString16(string16, mojo_string);

  // Test empty string conversion.
  ExpectEqualsMojoString(base::string16(), base::string16());
}

}  // namespace test
}  // namespace common
}  // namespace mojo
