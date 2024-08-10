#include <argos3/plugins/robots/kheperaiv/real_robot/real_kheperaiv_wifi_sensor.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <iostream>

void CRealKheperaIVWiFiSensor::Init(TConfigurationNode &t_node)
{
    try
    {
        // Initialize ZeroMQ context and subscriber socket
        m_ctx = std::make_unique<zmq::context_t>(1);
        m_subscriber = std::make_unique<zmq::socket_t>(*m_ctx, zmq::socket_type::sub);

        // Example: setting multicast address and port from the configuration
        std::string multicast_address;
        GetNodeAttribute(t_node, "multicast_address", multicast_address);
        GetNodeAttribute(t_node, "port", m_nPort);
        m_strAddress = multicast_address + ":" + std::to_string(m_nPort);

        // Connect to the publisher socket
        m_subscriber->connect("udp://" + m_strAddress);
        m_subscriber->setsockopt(ZMQ_SUBSCRIBE, "", 0); // Subscribe to all messages

        // Create and start the listening thread
        pthread_create(&m_tListeningThread, NULL, StartListeningThread, this);
        pthread_mutex_init(&m_tListeningMutex, NULL);
    }
    catch (const zmq::error_t &ex)
    {
        THROW_ARGOSEXCEPTION("ZeroMQ initialization error: " << ex.what());
    }
}

void CRealKheperaIVWiFiSensor::Destroy()
{
    // Stop the listening thread and clean up resources
    pthread_cancel(m_tListeningThread);
    pthread_join(m_tListeningThread, NULL);
    pthread_mutex_destroy(&m_tListeningMutex);
    m_subscriber->close();
    m_ctx->shutdown();
}

void CRealKheperaIVWiFiSensor::GetMessages(std::vector<SMessage> &vec_messages)
{
    pthread_mutex_lock(&m_tListeningMutex);
    vec_messages = m_vecMsgQueue;
    m_vecMsgQueue.clear();
    pthread_mutex_unlock(&m_tListeningMutex);
}

void CRealKheperaIVWiFiSensor::FlushMessages()
{
    pthread_mutex_lock(&m_tListeningMutex);
    m_vecMsgQueue.clear();
    pthread_mutex_unlock(&m_tListeningMutex);
}

void *CRealKheperaIVWiFiSensor::ListeningThread()
{
    zmq::message_t message;
    while (true)
    {
        // Receive the size of the message
        if (m_subscriber->recv(message))
        {
            // TODO: Better handling of missing data
            uint32_t sizeToReceive;
            memcpy(&sizeToReceive, message.data(), sizeof(sizeToReceive));

            // Receive the actual message payload
            zmq::message_t payloadMessage(sizeToReceive);
            if (m_subscriber->recv(payloadMessage))
            {
                pthread_mutex_lock(&m_tListeningMutex);
                kheperaiv::WiFi::SMessage msg = kheperaiv::WiFi::UnpackMessage(CByteArray(payloadMessage.data(), payloadMessage.size()));
                m_vecMsgQueue.push_back(msg);
                pthread_mutex_unlock(&m_tListeningMutex);
            }
        }
    }
    return nullptr;
}

void *CRealKheperaIVWiFiSensor::StartListeningThread(void *pt_context)
{
    return reinterpret_cast<CRealKheperaIVWiFiSensor *>(pt_context)->ListeningThread();
}