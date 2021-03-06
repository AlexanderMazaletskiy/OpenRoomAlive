// This file is part of the DerpVision Project.
// Licensing information can be found in the LICENSE file.
// (C) 2015 Group 13. All rights reserved.

#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>

#include <thrift/server/TServer.h>
#include <thrift/transport/TTransportUtils.h>

#include "core/GrayCode.h"
#include "core/ProCam.h"
#include "procam/GLDisplay.h"


namespace dv { namespace procam {

class BaselineCapture;
class BGRDCamera;
class Display;
class LaserDetector;


/**
 * Encapsulates most of the functionality of the application.
 */
class ProCamApplication : public ProCamIf {
 public:
  ProCamApplication(
      const std::string &masterIP,
      uint16_t port,
      bool enableDisplay,
      bool enableKinect,
      bool enableMaster,
      uint16_t logLevel,
      const std::string &logFile,
      const Size &effectiveSize,
      uint32_t latency);
  ~ProCamApplication();

  /**
   * Main loop of the application.
   */
  int run();

  /**
   * Retrieves the procam parameters.
   */
  void getParam(ProCamParam& cameraParams) override;
  /**
   * Retrieves the FullHD grayscale image (1920x1080).
   */
  void getGrayscaleImage(Frame &frame) override;

  /**
   * Retrieves the undistorted Depth image (512x424).
   */
  void getDepthImage(Frame &frame) override;

  /**
   * Retrieves the color image for depth data (512x424).
   */
  void getUndistortedColorImage(Frame &frame) override;

  /**
   * Retrieves the color baseline.
   */
  void getColorBaseline(Frame &frame) override;

  /**
   * Retrieves the depth baseline.
   */
  void getDepthBaseline(Frame& frame) override;

  /**
   * Retrieves the depth variance.
   */
  void getDepthVariance(Frame& frame) override;

  /**
   * Displays the specified gray code pattern.
   */
  void displayGrayCode(
      const Orientation::type orientation,
      const int16_t level,
      bool invertedGrayCode) override;

  /**
   * Displays white image on projector.
   */
  void displayWhite() override;

  /**
   * Clears the display (sets it to a black image).
   */
  void clearDisplay() override;

  /**
   * Closes the client.
   */
  void close() override;

  /**
   * Undistorts provided HD image using Kinect's parameters and depth baseline.
   */
  void undistort(
      Frame& undistortedImageThrift,
      const Frame& HDImageThrift);

  /**
   * Starts detecting the laser.
   */
  void startLaserDetection() override;

  /**
   * Update in image the laser path of color 'color' with provided 'segments'.
   */
  void updateLaser(
      const std::vector<Segment> &segments,
      const Color &color) override;

 private:
  /**
   * Pings the master server.
   */
  void pingMaster();

  /**
   * Continuously detects laser positions and sends updates to the master node.
   */
  void detectLaser();

 private:
  /// IP of the master node.
  const std::string masterIP_;
  /// Port used for conenctions.
  const uint16_t port_;
  /// OpenGL window.
  const std::shared_ptr<Display> display_;
  /// Kinect camera implementation.
  const std::shared_ptr<BGRDCamera> camera_;
  /// Gray code generator
  const GrayCode grayCode_;
  /// Server instance.
  const std::shared_ptr<apache::thrift::server::TServer> server_;
  /// Transport layer of connection to master.
  const boost::shared_ptr<apache::thrift::transport::TBufferedTransport> transport_;
  /// Master client handler.
  const std::shared_ptr<MasterClient> master_;
  /// Baseline capture.
  const std::shared_ptr<BaselineCapture> baseline_;
  /// Laser pointer tracker.
  const std::shared_ptr<LaserDetector> laser_;
  /// True if the master is pinged.
  const bool enableMaster_;
  /// Projector-Camera latency.
  const uint32_t latency_;
  /// True if thread detecting laser is still trying to send updates to master.
  std::atomic<bool> updatesStreamOn_;
  /// Flag indicating if laser detection has been started.
  bool detectingLaser_;
  /// Mutex guarding the detectingLaser_ flag.
  std::mutex detectionLock_;
  /// Condition variable waiting on laser detectiong to start.
  std::condition_variable detectionCond_;
  /// Canvas displayed on the screen.
  cv::Mat canvas_;
};

}}
