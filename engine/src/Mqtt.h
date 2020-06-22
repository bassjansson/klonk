#ifndef __MQTT_H__
#define __MQTT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTAsync.h"

#if defined(_WRS_KERNEL)
# include <OsWrapper.h>
#endif

#include "Defines.h"
#include "Track.h"

using namespace std;

class Mqtt
{
public:
    Mqtt(Track ** tracks)
        : tracks(tracks)
    {
        MQTTAsync_create(&client, MQTT_BROKER_ADDRESS, MQTT_CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
        MQTTAsync_setCallbacks(client, client, onConnectionLost, onMessage, NULL);
    }

    ~Mqtt()
    {
        MQTTAsync_destroy(&client);
    }

    void connect()
    {
        printf("[Mqtt] Connecting client '%s'.", MQTT_CLIENT_ID);

        MQTTAsync_connectOptions connectOptions = MQTTAsync_connectOptions_initializer;

        connectOptions.keepAliveInterval = MQTT_KEEP_ALIVE_INT;
        connectOptions.cleansession      = 1;

        connectOptions.onSuccess = onConnect;
        connectOptions.onFailure = onConnectFailure;
        connectOptions.context   = client;

        int errorCode = MQTTAsync_connect(client, &connectOptions);

        if (errorCode != MQTTASYNC_SUCCESS)
        {
            printf("[Mqtt] Failed to start connect, error code: %d\n", errorCode);
            exit(EXIT_FAILURE); // TODO: To harsh?
        }
    }

    void subscribe()
    {
        printf("[Mqtt] Subscribing to topic '%s' using QoS %d.\n", MQTT_TOPIC, MQTT_QOS);

        MQTTAsync_responseOptions subscribeOptions = MQTTAsync_responseOptions_initializer;

        subscribeOptions.onSuccess = onSubscribe;
        subscribeOptions.onFailure = onSubscribeFailure;
        subscribeOptions.context   = client;

        int errorCode = MQTTAsync_subscribe(client, MQTT_TOPIC, MQTT_QOS, &subscribeOptions);

        if (errorCode != MQTTASYNC_SUCCESS)
        {
            printf("[Mqtt] Failed to start subscribe, error code: %d\n", errorCode);
            exit(EXIT_FAILURE); // TODO: To harsh?
        }
    }

    void disconnect()
    {
        printf("[Mqtt] Disconnecting client '%s'.", MQTT_CLIENT_ID);

        MQTTAsync_disconnectOptions disconnectOptions = MQTTAsync_disconnectOptions_initializer;

        disconnectOptions.onSuccess = onDisconnect;
        disconnectOptions.onFailure = onDisconnectFailure;
        disconnectOptions.context   = client;

        int errorCode = MQTTAsync_disconnect(client, &disconnectOptions);

        if (errorCode != MQTTASYNC_SUCCESS)
        {
            printf("[Mqtt] Failed to start disconnect, error code: %d\n", errorCode);
            exit(EXIT_FAILURE); // TODO: To harsh?
        }
    }

private:
    void onConnect(void * context, MQTTAsync_successData * response)
    {
        // MQTTAsync client = (MQTTAsync) context;
        // deliveredtoken = 0; // TODO: Is this needed?

        printf("[Mqtt] Connect succeeded.\n");

        subscribe();
    }

    void onConnectFailure(void * context, MQTTAsync_failureData * response)
    {
        printf("[Mqtt] Connect failed, error code: %d\n", response ? response->code : 0);

        usleep(MQTT_RETRY_INTERVAL);

        connect();
    }

    void onSubscribe(void * context, MQTTAsync_successData * response)
    {
        printf("[Mqtt] Subscribe succeeded.\n");
    }

    void onSubscribeFailure(void * context, MQTTAsync_failureData * response)
    {
        printf("[Mqtt] Subscribe failed, error code: %d\n", response ? response->code : 0);

        usleep(MQTT_RETRY_INTERVAL);

        subscribe();
    }

    void onDisconnect(void * context, MQTTAsync_successData * response)
    {
        printf("[Mqtt] Disconnect succeeded.\n");
    }

    void onDisconnectFailure(void * context, MQTTAsync_failureData * response)
    {
        printf("[Mqtt] Disconnect failed, error code: %d\n", response ? response->code : 0);

        usleep(MQTT_RETRY_INTERVAL);

        disconnect();
    }

    void onConnectionLost(void * context, char * cause)
    {
        printf("[Mqtt] Lost connection to broker, cause: %s\n", cause ? cause : "Unknown");

        connect();
    }

    int onMessage(void * context, char * topicName, int topicLen, MQTTAsync_message * message)
    {
        // TODO: send data to tracks here

        printf("[Mqtt] Message received on topic '%s':\n", topicName);

        char * payloadptr = (char *) message->payload;

        for (int i = 0; i < message->payloadlen; i++)
            putchar(*payloadptr++);

        putchar('\n');

        MQTTAsync_freeMessage(&message);
        MQTTAsync_free(topicName);

        return 1;
    }

    Track ** tracks;

    MQTTAsync client;
    // volatile MQTTAsync_token deliveredtoken; // TODO: Needed? volatile?
};

#endif // __MQTT_H__
