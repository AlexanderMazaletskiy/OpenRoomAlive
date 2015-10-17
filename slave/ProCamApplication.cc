// This file is part of the DerpVision Project.
// Licensing information can be found in the LICENSE file.
// (C) 2015 Group 13. All rights reserved.

#include <iostream>
#include <thread>
#include <iostream>

#include <boost/make_shared.hpp>

#include <libfreenect2/libfreenect2.hpp>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "core/Master.h"
#include "core/Exception.h"
#include "slave/GLDisplay.h"
#include "slave/ProCamApplication.h"
#include "slave/ProCamServer.h"

using namespace dv::slave;


ProCamApplication::ProCamApplication(
    const std::string &masterIP,
    uint16_t masterPort)
  : masterIP_(masterIP),
    masterPort_(masterPort),
    display_(new GLDisplay()),
    runProcam_(true)
{
  (void) masterIP_;
  (void) masterPort_;
  (void) runProcam_;
}

int ProCamApplication::run() {
  // Responding to master node requests
  std::thread networking([this]() {
   //respondToMaster();
  });

  // Send Procam's IP to master
  namespace at  = apache::thrift;
  namespace atp = apache::thrift::protocol;
  namespace att = apache::thrift::transport;

  auto socket    = boost::make_shared<att::TSocket>(masterIP_, masterPort_);
  auto transport = boost::make_shared<att::TBufferedTransport>(socket);
  auto protocol  = boost::make_shared<atp::TBinaryProtocol>(transport);
  MasterClient masterClient(protocol);

  try {
   transport->open();

   printf("Sending Procam's IP to master node...\n");
   if (!masterClient.ping()) {
     printf("Master node rejected IP of the Procam.\n");
     return EXIT_FAILURE;
   }

   printf("Master node accepted IP of the Procam.\n");

   transport->close();
  } catch (apache::thrift::TException& tx) {
   std::cout << "ERROR: " << tx.what() << std::endl;
  }

  // TODO(nandor): This is just a test.
  uint8_t data[] = {255, 0, 0, 128, 128, 128, 128, 128, 128, 128, 255, 0, 0};
  cv::Mat mat(2, 2, CV_8UC3, data);
  display_->displayImage(mat);
  display_->run();

  // Kinect.
  // TODO(ilijar): set up the kinect.
  libfreenect2::Freenect2 freenect2;

  if (freenect2.enumerateDevices() == 0) {
    std::cout << "No Kinect device connected!" << std::endl;
    // TODO(ilijar): T15
    return EXIT_FAILURE;
  }

  std::string serial = freenect2.getDefaultDeviceSerialNumber();

  libfreenect2::Freenect2Device* device = freenect2.openDevice(serial);
  std::shared_ptr<libfreenect2::Freenect2Device> kinect(device);

  kinect->start();

  slave::ProCamServer proCamServer(kinect);

  kinect->stop();
  kinect->close();

  // Projector.
  display_->run();

  return EXIT_SUCCESS;
}

void ProCamApplication::respondToMaster(
    const std::shared_ptr<libfreenect2::Freenect2Device>& kinect)
{
  (void) kinect;
}