#ifndef CI_KHEPERAIV_WIFI_SENSOR_H
#define CI_KHEPERAIV_WIFI_SENSOR_H

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/plugins/robots/kheperaiv/control_interface/ci_kheperaiv_wifi_struct.h>

namespace argos {
   
   class CCI_KheperaIVWiFiSensor : public CCI_Sensor {


   public:

      virtual ~CCI_KheperaIVWiFiSensor() {}

      /**
       * Fills the given message vector and flushes the internal message queue
       */
      virtual void GetMessages(std::vector<kheperaiv::WiFi::SMessage>& vec_messages) = 0;

      /**
       * Discards all the messages in the queue.
       */
      virtual void FlushMessages() = 0;

   };

}

#endif // CI_KHEPERAIV_WIFI_SENSOR_H