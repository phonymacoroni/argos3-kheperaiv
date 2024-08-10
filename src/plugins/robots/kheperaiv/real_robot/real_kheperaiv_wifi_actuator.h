#ifndef REAL_KHEPERAIV_WIFI_ACTUATOR_H
#define REAL_KHEPERAIV_WIFI_ACTUATOR_H

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/plugins/robots/kheperaiv/real_robot/real_kheperaiv_device.h>
#include <argos3/plugins/robots/kheperaiv/control_interface/ci_kheperaiv_wifi_actuator.h>
#include <argos3/plugins/robots/kheperaiv/control_interface/ci_kheperaiv_wifi_struct.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <string>
#include <zmq.hpp>

namespace argos {

   class CRealKheperaIVWiFiActuator : public CCI_KheperaIVWiFiActuator, CRealKheperaIVDevice  {

   public:

      virtual ~CRealKheperaIVWiFiActuator() {}

      /**
       * Initializes the actuator.
       * @param t_node Configuration node
       */
      virtual void Init(TConfigurationNode& t_node) override;

      /**
       * Destroys the actuator and releases resources.
       */
      virtual void Destroy() override;

      /**
       * Performs the actuator update.
       * @param f_elapsed_time Elapsed time
       */
      virtual void Do(Real f_elapsed_time) override;

      /**
       * Sends a TCP message to the specified address.
       * @param str_addr Address to send the message to
       * @param c_message Message payload
       */
      virtual void SendToOne(const std::string& str_addr, const CByteArray& c_message);

      /**
       * Sends a TCP message to the configured multicast address.
       * @param c_message Message payload
       */
      virtual void SendToMany(const CByteArray& c_message);

      /**
       * Sends a TCP message to all connected devices.
       * @param c_payload Message payload
       */
      virtual void SendToAll(const CByteArray& c_payload);

   private:

      /**
       * Helper method for sending multicast data.
       * @param c_message Message payload
       */
      void SendDataMultiCast(const CByteArray& c_message);

   private:

      std::unique_ptr<zmq::context_t> m_ctx;
      std::unique_ptr<zmq::socket_t> m_publisher;
      std::string m_strAddress;
      int m_nPort;
	  // Store payload size limit if needed 
	  size_t m_nMulticastMaxPayloadSize;
	  // Message queue to be processed in Do()
	  std::vector<kheperaiv::WiFi::SMessage> m_vecMsgQueue;
   };

}

#endif // REAL_KHEPERAIV_WIFI_ACTUATOR_H