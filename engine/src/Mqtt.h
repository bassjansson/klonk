#ifndef __MQTT_H__
#define __MQTT_H__

#include <iostream>
#include <cmath>
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

#define ADDRESS  "tcp://localhost:1883"
#define CLIENTID "audio-engine"
#define TOPIC    "testtopic"
#define QOS      1
#define TIMEOUT  10000L

using namespace std;

class Mqtt
{
public:
    Mqtt(Track * *tracks)
        : tracks(tracks)
    {
        MQTTAsync_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
        MQTTAsync_setCallbacks(client, client, onConnectionLost, onMessage, NULL);
    }

    ~Mqtt()
    {
        MQTTAsync_destroy(&client);
    }

    bool connect()
    {
        MQTTAsync_connectOptions connectOptions = MQTTAsync_connectOptions_initializer;

        connectOptions.keepAliveInterval = 20;
        connectOptions.cleansession      = 1;
        connectOptions.onSuccess         = onConnect;
        connectOptions.onFailure         = onConnectFailure;
        connectOptions.context = client;

        int errorCode;

        if ((errorCode = MQTTAsync_connect(client, &connectOptions)) != MQTTASYNC_SUCCESS)
        {
            printf("Failed to start connect, return code %d\n", errorCode);
            exit(EXIT_FAILURE);
        }

        // TODO
        if (finished)
            return false;

        return true;
    }

    bool subscribe()
    {
        printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n", TOPIC, CLIENTID, QOS);

        MQTTAsync_responseOptions responseOptions = MQTTAsync_responseOptions_initializer;

        responseOptions.onSuccess = onSubscribe;
        responseOptions.onFailure = onSubscribeFailure;
        responseOptions.context   = client;

        int errorCode;

        if ((errorCode = MQTTAsync_subscribe(client, TOPIC, QOS, &responseOptions)) != MQTTASYNC_SUCCESS)
        {
            printf("Failed to start subscribe, return code %d\n", errorCode);
            exit(EXIT_FAILURE);
        }

        // TODO
        while (!subscribed)
            usleep(10000L);

        return true;
    }

    bool disconnect()
    {
        MQTTAsync_disconnectOptions disconnectOptions = MQTTAsync_disconnectOptions_initializer;

        disconnectOptions.onSuccess = onDisconnect;

        int errorCode;

        if ((errorCode = MQTTAsync_disconnect(client, &disconnectOptions)) != MQTTASYNC_SUCCESS)
        {
            printf("Failed to start disconnect, return code %d\n", errorCode);
            exit(EXIT_FAILURE);
        }

        // TODO
        while (!disc_finished)
            usleep(10000L);

        return true;
    }

private:
    void onConnect(void * context, MQTTAsync_successData * response)
    {
        // MQTTAsync client = (MQTTAsync)context;

        // deliveredtoken = 0; // Is this needed???

        printf("Successful connection\n");

        subscribe();
    }

    void onConnectFailure(void * context, MQTTAsync_failureData * response)
    {
        printf("Connect failed, errorCode %d\n", response ? response->code : 0);
        finished = 1;
    }

    void onSubscribe(void * context, MQTTAsync_successData * response)
    {
        printf("Subscribe succeeded\n");
        subscribed = 1;
    }

    void onSubscribeFailure(void * context, MQTTAsync_failureData * response)
    {
        printf("Subscribe failed, errorCode %d\n", response ? response->code : 0);
        finished = 1;
    }

    void onDisconnect(void * context, MQTTAsync_successData * response)
    {
        printf("Successful disconnection\n");
        disc_finished = 1;
    }

    void onConnectionLost(void * context, char * cause)
    {
        // MQTTAsync client = (MQTTAsync)context;

        printf("\nConnection lost\n");

        if (cause)
            printf("Cause: %s\n", cause);

        printf("Reconnecting\n");
        connect();
    }

    int onMessage(void * context, char * topicName, int topicLen, MQTTAsync_message * message)
    {
        // TODO: send data to tracks here

        printf("Message arrived\n");
        printf("     topic: %s\n", topicName);
        printf("   message: ");

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
    // MQTTAsync_token deliveredtoken; // Needed??? volatile???

    // TODO: Turn this into state
    int disc_finished = 0;
    int subscribed    = 0;
    int finished      = 0;
};

#endif // __MQTT_H__
