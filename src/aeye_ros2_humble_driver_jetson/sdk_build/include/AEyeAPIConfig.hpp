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
#ifndef AEYECLIENTS_AEYESENSORSDK_INCLUDE_AEYEAPICONFIG_HPP_
#define AEYECLIENTS_AEYESENSORSDK_INCLUDE_AEYEAPICONFIG_HPP_

/// @file AEyeAPIConfig.hpp
/// @brief This file contains macros used to control the AEye API.


/// @def AEYE_DEPRECATED
/// @brief Expands to [[deprecated]] or a pre-standardization compiler specific attribute to warn that a specific
///        function is deprecated.
/// @details Define the macro AEYE_NO_DEPRECATED to disable this attribute.


/// @def AEYE_NODISCARD
/// @brief Expands to [[nodiscard]] or a pre-standardization compiler specific attribute to warn that the return value
///        of a function should not be discarded without inspection.
/// @details Define the macro AEYE_NO_NODISCARD to disable this attribute.


#ifndef AEYE_NO_DEPRECATED

#if __cplusplus >= 201402L
#define AEYE_DEPRECATED [[deprecated]]
#elif (defined(__GNUC__) || defined(__clang__))
#define AEYE_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define AEYE_DEPRECATED __declspec(deprecated)
#else
#define AEYE_DEPRECATED
#endif

#else // AEYE_NO_DEPRECATED
#define AEYE_DEPRECATED
#endif // AEYE_NO_DEPRECATED

#ifndef AEYE_NO_NODISCARD

#if __cplusplus >= 201703L
#define AEYE_NODISCARD [[nodiscard]]
#elif (defined(__GNUC__) || defined(__clang__))
#define AEYE_NODISCARD __attribute__((warn_unused_result))
#else // MSVC does not support anything like WUR without the /analyze switch, so do not offer pre-C++17 compiler support
#define AEYE_NODISCARD
#endif

#else // AEYE_NO_NODISCARD
#define AEYE_NODISCARD
#endif // AEYE_NO_NODISCARD

#endif //AEYECLIENTS_AEYESENSORSDK_INCLUDE_AEYEAPICONFIG_HPP_