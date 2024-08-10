#ifndef REAL_KHEPERAIV_WIFI_SENSOR_H
#define REAL_KHEPERAIV_WIFI_SENSOR_H

#include <argos3/plugins/robots/kheperaiv/control_interface/ci_kheperaiv_wifi_sensor.h>
#include <argos3/plugins/robots/kheperaiv/real_robot/real_kheperaiv_device.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <vector>
#include <string>
#include <zmq.hpp>

namespace argos {

   class CRealKheperaIVWiFiSensor : public CCI_KheperaIVWiFiSensor, CRealKheperaIVDevice {

   public:

      virtual ~CRealKheperaIVWiFiSensor() {}

      /**
       * Initializes the sensor.
       * @param t_node Configuration node
       */
      virtual void Init(TConfigurationNode& t_node) override;

      /**
       * Destroys the sensor and releases resources.
       */
      virtual void Destroy() override;

      /**
       * Retrieves the messages from the internal queue.
       * @param vec_messages Vector to be filled with messages
       */
      virtual void GetMessages(std::vector<kheperaiv::WiFi::SMessage>& vec_messages);

      /**
       * Discards all messages in the queue.
       */
      virtual void FlushMessages();

   private:

      /**
       * Listening thread for receiving messages.
       */
      void* ListeningThread();

      /**
       * Static method to start the listening thread.
       * @param pt_context Pointer to the sensor object
       */
      static void* StartListeningThread(void* pt_context);

   private:

      std::unique_ptr<zmq::context_t> m_ctx;
      std::unique_ptr<zmq::socket_t> m_subscriber;
      pthread_t m_tListeningThread;
      pthread_mutex_t m_tListeningMutex;
      std::vector<kheperaiv::WiFi::SMessage> m_vecMsgQueue;
      std::string m_strAddress;
      int m_nPort;
   };

}

#endif // REAL_KHEPERAIV_WIFI_SENSOR_H
