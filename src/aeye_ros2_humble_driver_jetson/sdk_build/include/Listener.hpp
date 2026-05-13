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
#ifndef AEYECLIENTS_LISTENER_H
#define AEYECLIENTS_LISTENER_H

#include <functional>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include "AEyeSDKStructures.hpp"
#include "Sensor.hpp"

#ifdef _MSC_VER
#include <initializer_list>
#endif
class SensorReader;

namespace aeye {
  class PointCloudFrame;
  class PcapPacket;
  class SensorCommander;
}

namespace aeye {
  /// @class Listener
  /// @brief High-level interface to AEye's Apollo sensor
  class Listener final {
  public:

    /// @brief Constructor for Listener object. Configuration objects can be constructed with default values and
    /// overwritten with desired configuration changes.
    ///
    /// @param sensor_config Configuration object for miscellaneous sensor options. Note that the UdpBroadcastMode
    ///        argument will be ignored and UdpBroadcastMode::ENABLED will be used instead.
    /// @param data_ports Configuration object for the client server and image ports.
    /// @param net_interface The network interface the sensor is on.
    /// @param sensor_ip The IP address of the sensor to connect to.
    /// @param sensor_name A convenient name to use to refer to the sensor.
    Listener(SensorConfig sensor_config, DataPortConfig data_ports,
             std::string net_interface, std::string sensor_ip, std::string sensor_name);

    /// @brief Destructor for Listener object. Tells the real-world sensor to stop().
    ~Listener();

    /// @brief Set handler for incoming point cloud data.
    /// @param processPointCloudCallback Functor designated for point cloud processing.
    void setOnPointCloudFrameCallback(const std::function<void(PointCloudFrame&&)>& processPointCloudCallback);

    /// @brief This tells the sensor to start scanning.
    /// @details This function also has the option to configure the network interface to ensure
    /// reliable sensor data reception.
    /// @details If it is enabled (autoNetworkConfiguration), Admin privileges are required.
    void start(bool autoNetworkConfiguration = true);

    /// @brief Disconnect from sensor. Tells the sensor to stop() beforehand.
    void disconnect();

    /// @brief Tells the sensor to stop and the SDK to stop processing data.
    ///        The sensor will remain available for client connection.
    void stop();

    // Getters
    const std::string& getClientIp()                      const;
    const std::string& getInterface()                     const;
    const std::string& getSensorIp()                      const;
    const std::string& getName()                          const;
    bool               isSensing()                        const;
    bool               isPcapMode()                       const;
    uint16_t           getRangePort()                     const;

    /**
     * @brief Takes in PcapPacket, processes it, and feeds it to the sensorReader.
     * @param packet PcapPacket to be processed.
     */
    void processPcapPacket(PcapPacket& packet);

  private:
    std::string                            clientIp;
    std::string                            clientInterface;
    std::string                            sensorIp;
    std::string                            name;
    uint16_t                               rangePort;
    std::unique_ptr<SensorReader>          sensorReader;
    bool                                   sensingOn;
    bool                                   pcapModeOn;
  };
}
#endif //AEYECLIENTS_LISTENER_H
