///
/// AEye, Inc. ("COMPANY") CONFIDENTIAL
/// Unpublished Copyright (c) 2013-2026 AEye, Inc., All Rights Reserved.
///
/// NOTICE:  All information contained herein is, and remains the property of COMPANY.
/// The intellectual and technical concepts contained herein are proprietary to COMPANY and
/// may be covered by U.S. and Foreign Patents, patents in process, and are protected by trade secret or copyright law.
/// Dissemination of this information or reproduction of this material is strictly forbidden unless
/// prior written permission is obtained from COMPANY.
/// Access to the source code contained herein is hereby forbidden to anyone except current COMPANY employees, managers,
/// or contractors who have executed Confidentiality and Non-disclosure agreements explicitly covering such access.
///
/// The copyright notice above does not evidence any actual or intended publication or disclosure of this source code,
/// which includes information that is confidential and/or proprietary, and is a trade secret, of  COMPANY.
/// ANY REPRODUCTION, MODIFICATION, DISTRIBUTION, PUBLIC PERFORMANCE, OR PUBLIC DISPLAY OF OR THROUGH USE OF THIS
/// SOURCE CODE WITHOUT THE EXPRESS WRITTEN CONSENT OF COMPANY IS STRICTLY PROHIBITED, AND IN VIOLATION OF APPLICABLE
/// LAWS AND INTERNATIONAL TREATIES. THE RECEIPT OR POSSESSION OF THIS SOURCE CODE AND/OR RELATED INFORMATION DOES NOT
/// CONVEY OR IMPLY ANY RIGHTS TO REPRODUCE, DISCLOSE, OR DISTRIBUTE ITS CONTENTS, OR TO MANUFACTURE, USE, OR SELL
/// ANYTHING THAT IT  MAY DESCRIBE, IN WHOLE OR IN PART.
///

#ifndef SENSOR_FRAME_HPP
#define SENSOR_FRAME_HPP

/// @file PointCloudFrame.hpp

#include <vector>
#include <string>
#include <utility>

#include "AEyeAPIConfig.hpp"
#include "AEyeSDKStructures.hpp"

namespace aeye {

/// @class PointCloudFrame
/// @brief All the information pertaining to a single point cloud frame.
class PointCloudFrame {
 public:
  PointCloudFrame() noexcept;
  explicit PointCloudFrame(std::string sensorIp, int frameId, uint16_t pointReturnMask,
                           std::vector<AEyePoint> &&inPoints, size_t expectedNumPoints, uint32_t sec, uint32_t usec,
                           Pose pose, std::string scan_pattern) noexcept;

  /// @brief get frame ID of point cloud frame
  AEYE_NODISCARD
  int getFrameId() const;

  /// @brief get IP address of the sensor that provided this point cloud frame
  AEYE_NODISCARD
  const std::string &getIpAddress() const;
  AEYE_NODISCARD
  std::string &getIpAddress();

  /// @brief Get the point return flags that correspond to this frame
  /// @sa enum PointStructures.hpp PointReturnFlags
  AEYE_NODISCARD
  uint16_t getPointReturnFlags() const;

  /// @brief Get the point cloud data of this frame
  /// @sa enum PointStructures.hpp PointReturnFlags and getPointReturnFlags()
  AEYE_NODISCARD
  const std::vector<AEyePoint> &getPoints() const;

  /// @brief Get the point cloud data of this frame
  /// @sa enum PointStructures.hpp PointReturnFlags and getPointReturnFlags()
  AEYE_NODISCARD
  std::vector<AEyePoint> &getPoints();

  /// @brief get seconds component of FRAME timestamp (frameTs = frameTs_s + frameTs_us * 1e-6)
  AEYE_NODISCARD
  uint32_t getTimeSec() const;

  /// @brief get microseconds component of FRAME timestamp (frameTs = frameTs_s + frameTs_us * 1e-6)
  AEYE_NODISCARD
  uint32_t getTimeuSec() const;

  /// @brief get how many points have been received by the SDK
  AEYE_NODISCARD
  size_t getNumReceivedPoints() const;

  /// @brief get how many points have been streamed from the sensor
  AEYE_NODISCARD
  size_t getNumExpectedPoints() const;

  /// @brief Returns the pose of the sensor used to collect this PointCloudFrame
  AEYE_NODISCARD
  Pose getSensorPose() const;

  /// @brief Returns the name of the shotlist used to collect this PointCloudFrame
  AEYE_NODISCARD
  const std::string &getScanPatternUsed() const;

 private:
  int frameId;             ///< Frame Id
  std::string sensorIp;            ///< Sensor Ip Address
  uint32_t tsSec;               ///< Frame timestamp, seconds component (frameTs = frameTs_s + frameTs_us * 1e-6)
  uint32_t tsuSec;              ///< Frame timestamp, microseconds component (frameTs = frameTs_s + frameTs_us * 1e-6)
  uint16_t pointReturnFlags{0}; ///< Frame point returns flag
  std::vector<AEyePoint> points;              ///< Vector of AEyePoint that constitute a single point cloud frame.
  size_t numPointsReportedBySensor; ///< Expected number of points to be received in this frame
  Pose sensor_pose;         ///< Sensor's pose when this frame was received
  std::string scan_pattern_name;   ///< Name of the scan pattern that was being shot for this frame
};

}
#endif // SENSOR_FRAME_HPP
