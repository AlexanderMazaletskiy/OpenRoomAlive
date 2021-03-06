// This file is part of the DerpVision Project.
// Licensing information can be found in the LICENSE file.
// (C) 2015 Group 13. All rights reserved.

#pragma once

#include <mutex>

#include "core/GL.h"

#include "procam/Display.h"


namespace dv { namespace procam {


/**
 * Class encapsulating all the OpenGL rendering logic.
 */
class GLDisplay : public Display {
 public:
  GLDisplay();
  ~GLDisplay();

  bool isRunning() override;
  void update() override;
  void stop() override;
  void displayImage(const cv::Mat &image) override;
  cv::Size getResolution() override;
  virtual size_t getWidth() override;
  virtual size_t getHeight() override;
  cv::Mat getImage() override;

 private:
  /**
   * GLFW cleanup.
   */
  void destroy();

  /**
   * Called when a key was pressed.
   */
  void onKeyPressed(int key);

 private:
  /// Lock protecting the data.
  std::mutex lock_;
  /// Image (gray code) to display. Initialized to a single pixel RGBA image.
  cv::Mat image_;
  /// Image being displayed (copy to make multithreading easiery).
  cv::Mat displayedImage_;
  /// OpenGL texture acting as an intermediary between OpenCV and OpenGL.
  GLuint texture_;
  /// GLFW window handle.
  GLFWwindow *window_;
  /// Resolution.
  cv::Size resolution_;

  // GLFW callback functions.
  static void onKeyCallback(GLFWwindow*, int, int, int, int);
};

}}
