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

/// @file Sensor.hpp
/// @brief High-level interface to AEye's 4SightM sensor.

#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include "AEyeAPIConfig.hpp"
#include "AEyeSDKStructures.hpp"
#include "PcapPacket.hpp"

#ifdef _MSC_VER
#include <initializer_list>
#endif
class SensorReader;

namespace aeye {
  class PointCloudFrame;
  class SensorCommander;
}

namespace aeye {

  enum class UdpBroadcastMode : bool {
    DISABLED,
    ENABlED,
  };
  struct SensorConfig {
    UdpBroadcastMode broadcast_mode = UdpBroadcastMode::DISABLED;
  };
  struct DataPortConfig {
    uint16_t points = DEFAULT_CLIENT_POINTS_PORT;
  };


  /// @class Sensor
  /// @brief High-level interface to AEye's 4SightM sensor
#ifdef AEYE_SDK_DEV
  class Sensor {
#else
  class Sensor final {
#endif
  public:

    /**
   * @brief Data type to describe size, with fields width and height
   */
    struct Size {
      size_t width;
      size_t height;
    };

    /// @brief Constructor for Sensor object. Configuration objects can be constructed with default values and
    /// overwritten with desired configuration changes.
    ///
    /// @param sensor_config Configuration object for miscellaneous sensor options.
    /// @param data_ports Configuration object for the client server and image ports.
    /// @param net_interface The network interface the sensor is on.
    /// @param sensor_ip The IP address of the sensor to connect to.
    /// @param sensor_name A convenient name to use to refer to the sensor.
    Sensor(SensorConfig sensor_config, DataPortConfig data_ports,
           std::string net_interface, std::string sensor_ip, std::string sensor_name);

    /// @brief Destructor for Sensor object. Tells the real-world sensor to stop().
    virtual ~Sensor();

    /// @brief Set handler for incoming point cloud data.
    /// @param processPointCloudCallback Functor designated for point cloud processing.
    void setOnPointCloudFrameCallback(const std::function<void(PointCloudFrame&&)>& processPointCloudCallback);

    /// @brief Connect to sensor and update Sensor object's member variables to match sensor state.
    /// @details This function also has the option to configure the network interface to ensure
    /// reliable sensor data reception.
    /// @details If it is enabled (autoNetworkConfiguration), Admin privileges are required.
    void connect(bool autoNetworkConfiguration = true);

    /// @brief After connect() has been called, this tells the sensor to start scanning.
    virtual void start();

    /// @brief Disconnect from sensor. Tells the sensor to stop() beforehand.
    void disconnect();

    /// @brief Tells the sensor to stop and the SDK to stop processing data.
    ///        The sensor will remain available for client connection.
    void stop();

    /// @cond UNSUPPORTED
    /// @brief Reboot the connected sensor.
    ///        Note that this will first stop the sensor service, then shut down the sensor, disconnecting it.
    void reboot();
    /// @endcond

    /// @brief Reset the internal state of data-collecting processes.
    ///        This is necessary if looping recorded PCAP data, to restart from the beginning of a stream.
    void resetPlaybackState();

    /// @brief Get the index of the currently firing shotlist from the sensor.
    /// @return Index of the shot list. -1 if error or not in static shot list mode.
    int8_t getCurrentShotlistIndex();

    /// @brief Refresh local cache of sensor internal state.
    /// @return True if the sensor's internal state was successfully updated, false if any one portion failed.
    bool collectSensorState();

    /// @brief Synchronizes sensor time with client time.
    void sendTime();

    /// @brief Set what data will be returned in the point cloud data stream.
    /// @param pointReturnMask Bitmask of available data types.
    /// @sa enum PointReturnFlags
    void sendPointReturnMask(uint16_t pointReturnMask);

    /// @brief Set the parameter whose data will be returned as PointReturnIntensity.intensity_type_data.
    /// @param intensityType Identifier of requested intensity type.
    /// @sa enum IntensityTypes
    void sendIntensityType(uint8_t intensityType);

    /// @brief Set which embedded shotlist file to use.
    ///        NOTE: N is the number of shotlists on your sensor, which can be obtained by calling the
    ///              Sensor::getScanPatternFileNames(), and getting the length of the returned vector.
    /// @param value index of your desired shot list {from 0 to N-1}
    void sendShotlistFileNumber(int value);

    /// @brief Selects a subset of 4 multi echo returns to be streamed.
    /// @param whichEchoes Bit mask whose lower 4 bits determine which echo to enable (0th bit is 0th echo, etc.).
    ///                    Upper 4 bits are unused.
    /// @sa enum PointRxType
    void sendNumMultiEchoes(uint8_t whichEchoes);

    /// @brief get number of echoes that are being streamed from sensor
    /// @return num of echoes
    AEYE_NODISCARD
    uint8_t getNumMultiEchoes();

    /// @brief get shot list name that corresponds to index i
    AEYE_NODISCARD
    std::string getShotlistName(uint16_t i);

    /// @cond UNSUPPORTED
    struct Quadrilateral {
      uint16_t      topLeftX;
      uint16_t      topLeftY;
      uint16_t      topRightX;
      uint16_t      topRightY;
      uint16_t      btmLeftX;
      uint16_t      btmLeftY;
      uint16_t      btmRightX;
      uint16_t      btmRightY;

      Quadrilateral() = default;

      explicit Quadrilateral(const aeye::Quadrilateral& other) {
        topLeftX = other.topLeftX;
        topLeftY = other.topLeftY;
        topRightX = other.topRightX;
        topRightY = other.topRightY;
        btmLeftX = other.btmLeftX;
        btmLeftY = other.btmLeftY;
        btmRightX = other.btmRightX;
        btmRightY = other.btmRightY;
      };
      Quadrilateral& operator=(const aeye::Quadrilateral& other);
    };
    /// @endcond

    // Getters
    AEYE_NODISCARD
    const std::string& getClientIp()                      const;
    AEYE_NODISCARD
    const std::string& getInterface()                     const;
    AEYE_NODISCARD
    const std::string& getSensorIp()                      const;
    AEYE_NODISCARD
    const std::string& getName()                          const;
    AEYE_NODISCARD
    bool               isConnected()                      const;
    AEYE_NODISCARD
    bool               isSensing()                        const;
    AEYE_NODISCARD
    bool               isPcapMode()                       const;
    AEYE_NODISCARD
    uint16_t           getCommandPort()                   const;
    AEYE_NODISCARD
    uint16_t           getRangePort()                     const;
    AEYE_NODISCARD
    uint16_t           getServerRangePort()               const;
    /// @endcond
    AEYE_NODISCARD
    uint8_t            getIntensityType()                 const;
    AEYE_NODISCARD
    uint16_t           getPointReturnMask()               const;
    AEYE_NODISCARD
    const Pose&        getPose()                          const;

    /// @brief Switch between Pcap playback and live sensing.
    /// @param value Enable/disable.
    void setPcapMode(bool value);

    /// @brief set sensor name in Sensor object (does not change name of physical sensor)
    void setName(const std::string& name);

    /**
     * @brief Takes in PcapPacket, processes it, and feeds it to the sensorReader.
     * @param packet PcapPacket to be processed.
     */
    void processPcapPacket(PcapPacket& packet);

    /// @brief Print cached sensor state to \p out.
    /// @param out Stream to which to print sensor state.
    void printState(std::ostream& out);

    AEYE_NODISCARD
    const std::vector<std::string>& getScanPatternFileNames();

  protected:
    std::string                            clientIp;
    std::string                            clientInterface;
    std::string                            sensorIp;
    std::string                            name;
    uint16_t                               commandPort;
    uint16_t                               rangePort;
    uint16_t                               serverRangePort;
    std::unique_ptr<SensorCommander>       sensorCommander;
    std::unique_ptr<SensorReader>          sensorReader;
    bool                                   sensingOn;
    bool                                   pcapModeOn;

    // Multi echoes
    uint8_t numEchoes;

    uint8_t intensityType;
    uint16_t pointReturnMask;

    Pose pose;

    std::vector<std::string> scanPatternFileNames;

  private:
    /// @brief Applies predefined initial configuration to the sensor. Used during connect to establish consistent sensor state
    void applyInitialParams();

};
}
#endif // SENSOR_HPP
