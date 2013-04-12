// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.ui.gfx;

import android.content.Context;
import android.graphics.PixelFormat;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.WindowManager;

import org.chromium.base.CalledByNative;
import org.chromium.base.JNINamespace;

/**
 * This class facilitates access to android information typically only
 * available using the Java SDK, including {@link Display} properties.
 *
 * Currently the information consists of very raw display information (height, width, DPI scale)
 * regarding the main display.
 */
@JNINamespace("gfx")
public class DeviceDisplayInfo {

  private WindowManager mWinManager;

  private DeviceDisplayInfo(Context context) {
      Context appContext = context.getApplicationContext();
      mWinManager = (WindowManager) appContext.getSystemService(Context.WINDOW_SERVICE);
  }

  /**
   * @return Display height in physical pixels.
   */
  @CalledByNative
  public int getDisplayHeight() {
      return getMetrics().heightPixels;
  }

  /**
   * @return Display width in physical pixels.
   */
  @CalledByNative
  public int getDisplayWidth() {
      return getMetrics().widthPixels;
  }

  /**
   * @return Bits per pixel.
   */
  @CalledByNative
  public int getBitsPerPixel() {
      PixelFormat info = new PixelFormat();
      PixelFormat.getPixelFormatInfo(getDisplay().getPixelFormat(), info);
      return info.bitsPerPixel;
  }

  /**
   * @return Bits per component.
   */
  @CalledByNative
  public int getBitsPerComponent() {
      int format = getDisplay().getPixelFormat();
      switch (format) {
      case PixelFormat.RGBA_4444:
          return 4;

      case PixelFormat.RGBA_5551:
          return 5;

      case PixelFormat.RGBA_8888:
      case PixelFormat.RGBX_8888:
      case PixelFormat.RGB_888:
          return 8;

      case PixelFormat.RGB_332:
          return 2;

      case PixelFormat.RGB_565:
          return 5;

      // Non-RGB formats.
      case PixelFormat.A_8:
      case PixelFormat.LA_88:
      case PixelFormat.L_8:
          return 0;

      // Unknown format. Use 8 as a sensible default.
      default:
          return 8;
      }
  }

  /**
   * @return A scaling factor for the Density Independent Pixel unit.
   *         1.0 is 160dpi, 0.75 is 120dpi, 2.0 is 320dpi.
   */
  @CalledByNative
  public double getDIPScale() {
      return getMetrics().density;
  }

  /**
   * @return Display refresh rate in frames per second.
   */
  @CalledByNative
  public double getRefreshRate() {
      double result = getDisplay().getRefreshRate();
      // Sanity check.
      return (result >= 61 || result < 30) ? 0 : result;
  }

  private Display getDisplay() {
      return mWinManager.getDefaultDisplay();
  }

  private DisplayMetrics getMetrics() {
      DisplayMetrics metrics = new DisplayMetrics();
      getDisplay().getMetrics(metrics);
      return metrics;
  }

  /**
   * Creates DeviceDisplayInfo for a given Context.
   * @param context A context to use.
   * @return DeviceDisplayInfo associated with a given Context.
   */
  @CalledByNative
  public static DeviceDisplayInfo create(Context context) {
      return new DeviceDisplayInfo(context);
  }
}
