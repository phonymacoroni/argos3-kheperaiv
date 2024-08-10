#ifndef CI_KHEPERAIV_WIFI_ACTUATOR_H
#define CI_KHEPERAIV_WIFI_ACTUATOR_H

#include <argos3/core/control_interface/ci_actuator.h>
#include <argos3/core/utility/datatypes/byte_array.h>

namespace argos {
   
   class CCI_KheperaIVWiFiActuator : public CCI_Actuator {

   public:

      virtual ~CCI_KheperaIVWiFiActuator() {}

      /**
       * Sends a UDP message to the specified address.
       * The address is expected to be either in this format:
       * "ip://xxx.yyy.zzz.www" -> for raw ips
       * or just a generic string of letters and numbers.
       */
      virtual void SendToOne(const std::string& str_addr,
                             const CByteArray& c_message) = 0;

      /**
       * Sends a UDP message to the robots connected to the configured
       * multicast address.
       */
      virtual void SendToMany(const CByteArray& c_message) = 0;

      /**
       * Sends a UDP message to every device connected to the network (robot or
       * not).
       */
      virtual void SendToAll(const CByteArray& c_payload) = 0;

   };

}

#endif // CI_KHEPERAIV_WIFI_ACTUATOR_H