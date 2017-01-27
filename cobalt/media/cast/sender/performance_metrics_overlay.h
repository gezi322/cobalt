// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COBALT_MEDIA_CAST_SENDER_PERFORMANCE_METRICS_OVERLAY_H_
#define COBALT_MEDIA_CAST_SENDER_PERFORMANCE_METRICS_OVERLAY_H_

#include "base/time/time.h"

// This module provides a display of frame-level performance metrics, rendered
// in the lower-right corner of a VideoFrame.  It looks like this:
//
// +---------------------------------------------------------------------+
// |                         @@@@@@@@@@@@@@@@@@@@@@@                     |
// |                 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                     |
// |              @@@@@@@@@@@@@@@@@@@@@@@ @@@@@@@@@@@@                   |
// |             @@@@@@@@@@@@@                    @@@@                   |
// |            @@@@@@@@@@                        @@@@                   |
// |           @@@@@  @@@               @@@       @@@@                   |
// |           @@@     @    @@@         @@@@      @@@@                   |
// |          @@@@          @@@@                  @@@@                   |
// |          @@@@                  @@@           @@@                    |
// |            @@@@                 @@           @@@                    |
// |             @@@@@      @@@            @@@   @@@                     |
// |              @@@@@     @@@@@        @@@@   @@@@                     |
// |               @@@@@      @@@@@@@@@@@@@    @@@@                      |
// |                @@@@@@                    @@@@           1  45%  75% |
// |                    @@@@@@@@         @@@@@@            22  400. 4000 |
// |                         @@@@@@@@@@@@@@@@      16.7 1280x720 0:15.12 |
// +---------------------------------------------------------------------+
//
// Line 1: Reads as, "1 frame ago, the encoder utilization for the frame was 45%
// and the lossy utilization was 75%."  For CPU-bound encoders, encoder
// utilization is usually measured as the amount of real-world time it took to
// encode the frame, divided by the maximum amount of time allowed.  Lossy
// utilization is the amount of "complexity" in the frame's content versus the
// target encoded byte size, where a value over 100% means the frame's content
// is too complex to encode within the target number of bytes.
//
// Line 2: Reads as, "Capture of this frame took 22 ms.  The current target
// playout delay is 400 ms and low-latency adjustment mode is not active.  The
// target bitrate for this frame is 4000 kbps."  If there were an exclamation
// mark (!) after the playout delay number instead of a period (.), it would
// indicate low-latency adjustment mode is active.  See VideoSender for more
// details.
//
// Line 3: Contains the frame's duration (16.7 milliseconds), resolution, and
// media timestamp in minutes:seconds.hundredths format.

namespace media {

class VideoFrame;

namespace cast {

// Renders an overlay of frame-level performance metrics in the lower-right
// corner of the |frame|, as described above.  The verbose logging level for
// video_frame_overlay.cc determines which lines, if any, are rendered: VLOG
// level 1 renders the bottom line only, level 2 renders the bottom and middle
// lines, and level 3 renders all three lines.  So, use the
// --vmodule=performance_metrics_overlay=3 command line argument to turn on
// rendering of the entire overlay.
void MaybeRenderPerformanceMetricsOverlay(base::TimeDelta target_playout_delay,
                                          bool in_low_latency_mode,
                                          int target_bitrate,
                                          int frames_ago,
                                          double encoder_utilization,
                                          double lossy_utilization,
                                          VideoFrame* frame);

}  // namespace cast
}  // namespace media

#endif  // COBALT_MEDIA_CAST_SENDER_PERFORMANCE_METRICS_OVERLAY_H_
