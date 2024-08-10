#include <argos3/plugins/robots/kheperaiv/real_robot/real_kheperaiv_wifi_actuator.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <iostream>

void CRealKheperaIVWiFiActuator::Init(TConfigurationNode &t_node)
{
    try
    {
        // Initialize ZeroMQ context and publisher socket
        m_ctx = std::make_unique<zmq::context_t>(1);
        m_publisher = std::make_unique<zmq::socket_t>(*m_ctx, zmq::socket_type::pub);

        // Example: setting multicast address and port from the configuration
        std::string multicast_address;
        GetNodeAttribute(t_node, "multicast_address", multicast_address);
        GetNodeAttribute(t_node, "port", m_nPort);
        m_strAddress = multicast_address + ":" + std::to_string(m_nPort);

        // Bind the publisher to the multicast address
        m_publisher->bind("udp://" + m_strAddress);
    }
    catch (const zmq::error_t &ex)
    {
        THROW_ARGOSEXCEPTION("ZeroMQ initialization error: " << ex.what());
    }
}

void CRealKheperaIVWiFiActuator::Destroy()
{
    m_publisher->close();
    m_ctx->shutdown();
}

void CRealKheperaIVWiFiActuator::Do(Real f_elapsed_time)
{
    for (auto &msg : m_vecMsgQueue)
    {
        CByteArray outMsg = kheperaiv::WiFi::PackMessage(msg);
        SendDataMultiCast(outMsg);
    }
    m_vecMsgQueue.clear();
}

void CRealKheperaIVWiFiActuator::SendToOne(const std::string &str_addr,
                                           const CByteArray &c_message)
{
    // TODO: Implementation of ToOne
    SendDataMultiCast(c_message);
}

void CRealKheperaIVWiFiActuator::SendToMany(const CByteArray &c_message)
{
    SendDataMultiCast(c_message);
}

void CRealKheperaIVWiFiActuator::SendToAll(const CByteArray &c_payload)
{
    SendDataMultiCast(c_payload);
}

void CRealKheperaIVWiFiActuator::SendDataMultiCast(const CByteArray &c_message)
{
    uint32_t sizeToSend = c_message.Size();
    zmq::message_t zmqSize(sizeof(sizeToSend));
    memcpy(zmqSize.data(), &sizeToSend, sizeof(sizeToSend));
    m_publisher.send(zmqSize, zmq::send_flags::sndmore);

    // Send the actual message payload
    zmq::message_t zmqPayload(sizeToSend);
    memcpy(zmqPayload.data(), c_message.ToCArray(), c_message.Size());
    m_publisher.send(zmqPayload, zmq::send_flags::dontwait);
}
