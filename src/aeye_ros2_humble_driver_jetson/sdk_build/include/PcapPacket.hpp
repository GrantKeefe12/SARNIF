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

#ifndef AEYE_SDK_PCAP_PACKET_HPP
#define AEYE_SDK_PCAP_PACKET_HPP

#include "AEyeAPIConfig.hpp"

#include <cstdint>
#include <string>
#include <vector>

/// @file PcapPacket.hpp
/// @details Interface to interact with recreated PCAP data.

namespace aeye {

/// \brief A recreation of the PCAP packets that originally were sent to an AEye sensor to replay the data stream.
class PcapPacket {
 public:
  /// \brief Default constructor for an empty \a PcapPacket.
  PcapPacket() noexcept = default;
  /// \brief Parameterized constructor for a \a PcapPacket. Note that data is copied inside of the constructor.
  ///
  /// \param srcAddr Unsigned 32-bit integer representing the original PCAP packet's source address.
  /// \param srcIpAddress Human-readable string (format "127.0.0.1") representing the original PCAP packet's source address.
  /// \param srcPort The port the sensor's network traffic arrived over.
  /// \param data A pointer to the beginning of the data originally contained in the PCAP. If a null pointer is passed
  ///        as an argument, no data will be copied.
  /// \param length The length of the data making up the PCAP packet.
  PcapPacket(uint32_t srcAddr, std::string srcIpAddress, uint16_t srcPort, const uint8_t *data, size_t length);

  /// \brief Access the data source's IP address.
  /// \return A string representing the source IP address.
  AEYE_NODISCARD
  const std::string &sourceIpAddress() const noexcept;
  /// \brief Access the data source's port.
  /// \return An integer representation of the source port.
  AEYE_NODISCARD
  uint16_t sourcePort() const noexcept;
  /// \brief Access the raw underlying data of the PCAP packet.
  /// \return A pointer to the first element of the data. Not null terminated.
  AEYE_NODISCARD
  const uint8_t* data() const noexcept;
  /// \brief Access the size of the raw underlying data.
  /// \return The number of elements (bytes) in the underlying PCAP packet.
  AEYE_NODISCARD
  size_t size() const noexcept;
  /// \brief Directly access the underlying data as a \a std::vector.
  /// \return The underlying vector storing PCAP data.
  AEYE_NODISCARD
  std::vector<uint8_t>& getData() noexcept;

 private:
  const std::string     source_ip_address_;
  uint16_t              source_port_{};
  std::vector<uint8_t>  data_;
};

}

#endif // AEYE_SDK_PCAP_PACKET_HPP
