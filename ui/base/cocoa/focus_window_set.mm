// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "ui/base/cocoa/focus_window_set.h"

namespace ui {

// This attempts to match OS X's native behavior, namely that a window
// is only ever deminiaturized if ALL windows on ALL workspaces are
// miniaturized. (This callback runs before AppKit picks its own
// window to deminiaturize, so we get to pick one from the right set.)
//
// In addition, limit to the windows on the current
// workspace. Otherwise we jump spaces haphazardly.
//
// NOTE: If this is called in the
// applicationShouldHandleReopen:hasVisibleWindows: hook when clicking
// the dock icon, and that caused OS X to begin switch spaces,
// isOnActiveSpace gives the answer for the PREVIOUS space. This means
// that we actually raise and focus the wrong space's windows, leaving
// the new key window off-screen. To detect this, check if the key
// window prior to calling is on an active space.
//
// Also, if we decide to deminiaturize a window during a space switch,
// that can switch spaces and then switch back. Fortunately, this only
// happens if, say, space 1 contains an app, space 2 contains a
// miniaturized browser. We click the icon, OS X switches to space 1,
// we deminiaturize the browser, and that triggers switching back.
//
// TODO(davidben): To limit those cases, consider preferentially
// deminiaturizing a window on the current space.
void FocusWindowSet(const std::set<NSWindow*>& windows,
                    bool allow_workspace_switch) {
  NSArray* ordered_windows = [NSApp orderedWindows];
  NSWindow* frontmost_window = nil;
  NSWindow* frontmost_window_all_spaces = nil;
  NSWindow* frontmost_miniaturized_window = nil;
  bool all_miniaturized = true;
  for (int i = [ordered_windows count] - 1; i >= 0; i--) {
    NSWindow* win = [ordered_windows objectAtIndex:i];
    if (windows.find(win) != windows.end()) {
      if ([win isMiniaturized]) {
        frontmost_miniaturized_window = win;
      } else if ([win isVisible]) {
        all_miniaturized = false;
        frontmost_window_all_spaces = win;
        if ([win isOnActiveSpace]) {
          [win orderFront:nil];
          frontmost_window = win;
        }
      }
    }
  }
  if (all_miniaturized && frontmost_miniaturized_window) {
    [frontmost_miniaturized_window deminiaturize:nil];
    frontmost_window = frontmost_miniaturized_window;
  }
  // If we couldn't find one on this window, consider all spaces.
  if (allow_workspace_switch &&
      !frontmost_window && frontmost_window_all_spaces) {
    frontmost_window = frontmost_window_all_spaces;
    [frontmost_window orderFront:nil];
  }
  if (frontmost_window) {
    [NSApp activateIgnoringOtherApps:YES];
    [frontmost_window makeMainWindow];
    [frontmost_window makeKeyWindow];
  }
}

}  // namespace ui
