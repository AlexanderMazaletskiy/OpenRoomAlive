// This file is part of the DerpVision Project.
// Licensing information can be found in the LICENSE file.
// (C) 2015 Group 13. All rights reserved.

#pragma once


namespace dv {

/**
 * All connections to procams are identified by ID equal to the ID of the
 * associated ProCam.
 */
using ConnectionID = uint64_t;

/**
 * Screen resolution.
 */
struct Resolution {
  size_t width;
  size_t height;
};

}
