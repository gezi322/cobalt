// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_CONTENT_EXPORT_H_
#define CONTENT_COMMON_CONTENT_EXPORT_H_
#pragma once

#if 0 // TODO(dpranke): Uncomment: defined(COMPONENT_BUILD).
#if defined(WIN32)

#if defined(CONTENT_IMPLEMENTATION)
#define CONTENT_EXPORT __declspec(dllexport)
#else
#define CONTENT_EXPORT __declspec(dllimport)
#endif  // defined(CONTENT_IMPLEMENTATION)

#else // defined(WIN32)
#define CONTENT_EXPORT __attribute__((visibility("default")))
#endif

#else // defined(COMPONENT_BUILD)
#define CONTENT_EXPORT
#endif

#endif  // CONTENT_COMMON_CONTENT_EXPORT_H_
