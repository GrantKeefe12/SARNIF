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

/// @file AEyeSDKStructures.hpp

#ifndef AEYESDKSTRUCTURES_HPP
#define AEYESDKSTRUCTURES_HPP

#include <cstdint>
#include <string>

#include "PointStructure.hpp"

//==============================================================================
//=                            Point Return Packets                            =
//==============================================================================

namespace aeye {

/// @cond IGNORE
static constexpr uint16_t DEFAULT_COMMAND_PORT        = 7000;
static constexpr uint16_t DEFAULT_CLIENT_POINTS_PORT  = 7001;
/// @endcond

/// @brief Port of the sensor used to stream point cloud data
static constexpr uint16_t DEFAULT_SERVER_POINTS_PORT  = 7001;

  /// @enum AEye LiDAR TCP Response Statuses
  enum class ReturnStatus : int16_t {  // uses in commands responses as returning value.
    NO_ERRORS                   =  0,
    GENERIC_ERROR               = -1,
    /// @cond UNSUPPORTED
    ALREADY_RUNNING             = -2,
    NOT_RUNNING                 = -3,
    COULD_NOT_CREATE_THREAD     = -4,
    ZERO_POINTS                 = -5,
    BAD_SHOTLIST                = -6,
    /// @endcond
    NETWORK_ERROR               = -8,
    UNKNOWN_COMMAND             = -9,
    INCORRECT_PACKET_SIZE       = -10,
    BAD_PARAMETER               = -11,
    EYESAFETY_SHUTDOWN_OCCURRED = -12,
    UNSUPPORTED_VERSION         = -13,
    AUTOMATIC_STATE_CHANGE      = -14,
    DISALLOWED_COMMAND          = -15,
    BAD_FILE                    = -16,
    LASER_ERROR                 = -17,
    CONFIG_ERROR                = -18,
    NOT_IMPLEMENTED_YET         = -20,
  };

  /// @cond UNSUPPORTED

#ifdef _MSC_VER
#pragma pack(push,1)
#endif
  /// @typedef AEye Sensor Active Zone helper struct
  struct Quadrilateral {
    uint16_t      topLeftX;
    uint16_t      topLeftY;
    uint16_t      topRightX;
    uint16_t      topRightY;
    uint16_t      btmLeftX;
    uint16_t      btmLeftY;
    uint16_t      btmRightX;
    uint16_t      btmRightY;
  } ATTRIBUTE_PACKED;

  /// @endcond
#ifdef _MSC_VER
#pragma pack(pop)
#endif

  /// @enum Passive Mode Parameters
  enum class PassiveModeParam : uint8_t {
    // Embedded shotlist
    SHOTLIST_FILE_NUMBER = 23 //!< Shotlist files stored in the device. 0-based index.
  };

  /// @enum PointFlags
  /// @details Fields ECHO_{0,1,2,3} are boolean, set to 1 if that is the corresponding echo, else 0. <br>
  enum class PointFlags : uint8_t {
    ECHO_LSB = (1U << 0U),                    //!< LSB for Echo Return 1-4
    ECHO_MSB = (1U << 1U) //!< MSB for Echo Return 1-4
  };

  /// @brief Specifies position and orientation.
  typedef struct {
      double x; ///< The axis looking out of the barrel of the sensor. (Front)
      double y; ///< The axis which is neither x nor z. (Left)
      double z; ///< The vertical axis. (Up)
      double yaw; ///< Rotation about the z axis.
      double pitch; ///< Rotation about the y axis.
      double roll; ///< Rotation about the x axis.
  } ATTRIBUTE_PACKED Pose;
}

#endif //AEYESDKSTRUCTURES_HPP
