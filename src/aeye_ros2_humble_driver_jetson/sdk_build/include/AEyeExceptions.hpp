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

#ifndef AEYE_EXCEPTIONS_HPP
#define AEYE_EXCEPTIONS_HPP

/// @file AEyeExceptions.hpp

#include <stdexcept>
#include <string>
#include "AEyeSDKStructures.hpp"

namespace aeye {
  /// @class error_base
  /// @brief AEye base exception class. A simple wrapper around std::runtime_error.
  class error_base : public std::runtime_error {
  public:
    explicit error_base(const char *const message) : std::runtime_error(message) {}
    explicit error_base(const std::string& message) : std::runtime_error(message) {}
  };

  /// @class socket_error
  /// @brief Thrown if creating, configuring, binding, or connecting to a socket fails.
  class socket_error : public aeye::error_base {
  public:
    socket_error() : error_base("Encountered socket error") {}
    explicit socket_error(const char *const message) : error_base(message) {}
    explicit socket_error(const std::string& message) : error_base(message) {}
  };

  /// @class sensor_comm_error
  /// @brief Thrown if sending a command to or receiving a response from a sensor fails.
  class sensor_comm_error : public aeye::error_base {
  public:
    /// @var int16_t error_code
    /// @brief Indicates which error (if any) sensor responded with
    int16_t error_code{static_cast<int16_t>(ReturnStatus::NO_ERRORS)};
    explicit sensor_comm_error(int16_t err = static_cast<int16_t>(ReturnStatus::NO_ERRORS))
      : error_base("Encountered sensor communication error"), error_code(err) {}
    explicit sensor_comm_error(const char *const message, int16_t err = static_cast<int16_t>(ReturnStatus::NO_ERRORS))
      : error_base(message), error_code(err) {}
    explicit sensor_comm_error(const std::string& message, int16_t err = static_cast<int16_t>(ReturnStatus::NO_ERRORS))
      : error_base(message), error_code(err) {}
  };

  /// @class return_status_error
  /// @brief Thrown when encountering a ReturnStatus indicating an error is found while parsing packets.
  class return_status_error : public sensor_comm_error {
   public:
    explicit return_status_error(ReturnStatus status) : sensor_comm_error("Encountered return status error", static_cast<int16_t>(status)) {}
    explicit return_status_error(const char *const message, ReturnStatus status) : sensor_comm_error(message, static_cast<int16_t>(status)) {}
    explicit return_status_error(const std::string& message, ReturnStatus status) : sensor_comm_error(message, static_cast<int16_t>(status)) {}
  };
}

#endif // AEYE_EXCEPTIONS_HPP
