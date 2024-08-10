#ifndef CI_KHEPERAIV_WIFI_STRUCT_H
#define CI_KHEPERAIV_WIFI_STRUCT_H

#include <argos3/core/utility/datatypes/byte_array.h>
#include <string>

namespace argos
{
    namespace kheperaiv
    {
        namespace WiFi
        {
            struct SMessage
            {
                std::string Address;
                CByteArray Payload;
            };

            CByteArray PackMessage(const SMessage &message)
            {
                std::vector<uint8_t> buffer;

                // Serialize Address
                uint32_t addressSize = static_cast<uint32_t>(message.Address.size());
                buffer.insert(buffer.end(), reinterpret_cast<uint8_t *>(&addressSize), reinterpret_cast<uint8_t *>(&addressSize) + sizeof(addressSize));
                buffer.insert(buffer.end(), message.Address.begin(), message.Address.end());

                // Serialize Payload
                uint32_t payloadSize = static_cast<uint32_t>(message.Payload.Size());
                buffer.insert(buffer.end(), reinterpret_cast<uint8_t *>(&payloadSize), reinterpret_cast<uint8_t *>(&payloadSize) + sizeof(payloadSize));
                buffer.insert(buffer.end(), message.Payload.ToCArray(), message.Payload.ToCArray() + payloadSize);

                // Create and return CByteArray from the serialized buffer
                return CByteArray(buffer.data(), buffer.size());
            }

            SMessage UnpackMessage(const CByteArray &byteArray)
            {
                SMessage message;
                const uint8_t *data = byteArray.ToCArray();
                size_t offset = 0;

                // Deserialize Address
                uint32_t addressSize;
                std::memcpy(&addressSize, data + offset, sizeof(addressSize));
                offset += sizeof(addressSize);

                message.Address.assign(reinterpret_cast<const char *>(data + offset), addressSize);
                offset += addressSize;

                // Deserialize Payload
                uint32_t payloadSize;
                std::memcpy(&payloadSize, data + offset, sizeof(payloadSize));
                offset += sizeof(payloadSize);

                message.Payload = CByteArray(data + offset, payloadSize);

                return message;
            }
        }
    }
}

#endif // CI_KHEPERAIV_WIFI_STRUCT_H
