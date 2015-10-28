// This file is part of the DerpVision Project.
// Licensing information can be found in the LICENSE file.
// (C) 2015 Group 13. All rights reserved.

#pragma once

#include <atomic>
#include <memory>

#include <thrift/server/TServer.h>

#include "core/ProCam.h"
#include "slave/GLDisplay.h"


namespace dv { namespace slave {

class Display;
class GrayCode;
class RGBDCamera;


/**
 * Encapsulates most of the functionality of the application.
 */
class ProCamApplication : public ProCamIf {
 public:
  ProCamApplication(
      const std::string &masterIP,
      uint16_t port,
      bool enableDisplay,
      bool enableKinect);
  ~ProCamApplication();

  /**
   * Main loop of the application.
   */
  int run();

  /**
   * Retrieves the camera parameters.
   */
  void getCameraParams(CameraParams& cameraParams) override;

  /**
   * Retrieves the parameters of the display.
   */
  void getDisplayParams(DisplayParams& displayParams) override;

  /**
   * Displays the specified gray code pattern.
   */
  void displayGrayCode(
      const Orientation::type orientation,
      const int16_t level) override;

  /**
   * Closes the client.
   */
  void close() override;

 private:
  /// IP of the master node.
  const std::string masterIP_;
  /// Port used for conenctions.
  const uint16_t port_;
  /// Gray code generator
  const std::shared_ptr<GrayCode> grayCode_;
  /// Server instance.
  const std::shared_ptr<apache::thrift::server::TServer> server_;
  /// OpenGL window.
  const std::shared_ptr<Display> display_;
  /// Kinect camera implementation.
  const std::shared_ptr<RGBDCamera> camera_;
};

}}
