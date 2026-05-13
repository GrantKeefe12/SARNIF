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

#ifndef POINTSTRUCTURE_HPP
#define POINTSTRUCTURE_HPP

#include <cstdint>

#ifdef _MSC_VER
#define ATTRIBUTE_PACKED
#else
#define ATTRIBUTE_PACKED __attribute__((__packed__))
#endif

namespace aeye {

  /// @enum PointReturnFlags
  /// @brief bit masks corresponding to what kind of data to get from the sensor
  /// @sa
  enum PointReturnFlags : uint16_t {
      SPHERICAL_COORDINATES = 0x0002,
      INTENSITY             = 0x0008,
      TIME_STAMP_OFFSET     = 0x0010,
  };

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

  /// @typedef Cartesian Point
  typedef struct {
    float  x; ///< [meters] Distance along the axis looking out the barrel of the sensor. (Front)
    float  y; ///< [meters] Distance along the axis which is neither x nor z. (Left)
    float  z; ///< [meters] Distance along the vertical axis. (Up)
  } ATTRIBUTE_PACKED PointReturnCartesianCoords;

  /// @cond IGNORE
  // Point SCALED Spherical Data. This is the data sent via UDP, scaled to reduce number of transmitted bytes.
  // It is not an output of AEyePoint, just something internal used to interpret the received scaled data
  // and generating the resulting real coordinates provided by the SDK.
  typedef struct {
    int16_t azimuth_scaled;      // [SCALED radians (10430)] Right (-y) is positive (left-handed).
    int16_t elevation_scaled;    // [SCALED radians (10430)] Up (+z) is positive
    uint16_t radius_scaled;      // [SCALED meters (64)] Distance from sensor to point.
  } ATTRIBUTE_PACKED PointReturnSphericalCoordsScaled;
  /// @endcond

  /// @typedef Point Spherical Data
  // With no scale!!
  typedef struct {
    float azimuth;       ///< [rad] Right (-y) is positive (left-handed).
    float elevation;     ///<  [rad] Up (+z) is positive.
    float radius;        ///< [meters] Distance from sensor to point.
  } ATTRIBUTE_PACKED PointReturnSphericalCoords;

  /// @typedef Point Reflectivity
  typedef struct {
    uint16_t intensity_type_data;       ///< [0, 10000] Reflectivity (scaled light energy received from a specific reflectivity target)
  } ATTRIBUTE_PACKED PointReturnIntensity;

  /// @typedef Point Time Stamp Offset Data in 100 microsecond units
  typedef struct {
    uint16_t time_offset; ///< [100us] Offset since current FRAME time stamp.
  } ATTRIBUTE_PACKED PointReturnTimeStampOffset;

  /// @typedef Point Return Mask
  typedef struct {
    uint8_t  point_return_mask; ///< Selected Point Cloud Data Menu Items
  } ATTRIBUTE_PACKED PointReturnType;

#ifdef _MSC_VER
#pragma pack(pop)
#endif

  /// @typedef Combined Point Return Structure
  typedef struct {
    PointReturnCartesianCoords pointReturnCartesianCoords;
    PointReturnSphericalCoords pointReturnSphericalCoords;
    PointReturnIntensity       pointReturnIntensity;
    PointReturnTimeStampOffset pointReturnTimeStampOffset;
    PointReturnType            pointReturnType;
  } AEyePoint;
}

#endif //POINTSTRUCTURE_HPP