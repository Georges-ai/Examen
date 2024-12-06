#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "Config.h"

//sturct meet data 
struct meter_data{
    char datumTijdStroom[DATA_TIME_LEN];
    int tariefIndicator;
    float actueelStroomverbruik;
    float actueelSpanning;
    float totaalDagVerbruik;
    float totaalNachtVerbruik;
    float totaalDagOpbrengst;
    float totaalNachtOpbrengst;
    char datumTijdGas[DATA_TIME_LEN];
    float totaalGasVerbruik;
};

//format CSV-record
void format_csv_record(struct meter_data meter_data, char *csv_record) {
    
    char *token1, *token2, *token3, *token4;
    // char *datumTijdStroom = NULL;
    // int *tariefIndicator = NULL;
    // char *actueelStroomverbruik = NULL;
    // char *actueelSpanning = NULL;
    // char *totaalDagVerbruik = NULL;
    // char *totaalNachtVerbruik = NULL;
    // char *totaalDagOpbrengst = NULL;
    // char *totaalNachtOpbrengst = NULL;
    // int *datumTijdGas = NULL;
    // char *totaalGasVerbruik = NULL;   
    // print incoming message
    //printf( "Received message: <%s>\n", msg ); 

    // Get the first token
    token1 = strtok(msg, ";");
    if (token1 != NULL) {
        // Get the second token
        datumTijdStroom = token1;
        datumTijdGas = token1;
        token2 = strtok(NULL, ";");
        if (token2 != NULL) {
            // Get the third token
            tariefIndicator = token2;
            token3 = strtok(NULL, ";");
            if (token3 != NULL) {
                actueelStroomverbruik = token3;
                // Get the fourth token
                token4 = strtok(NULL, ";");
                if (token4 != NULL) {
                    actueelSpanning = token4;
                }
            }
        }
    }

    // Now you can use Error_Code and Text
    if (Error_Code != NULL) {
        printf("Error Code: %s\n", Error_Code);
    }
    if (Text != NULL) {
        printf("Text: %s\n", Text);
    }
}

//print values
// void print_Start_values(char *tijd, char *DTotVerbruik, char *DTotOpbrengst, char *NTotVerbruik, char *NTotOpbrengst, char *TotGas) {

//     printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//     printf("Elekriciteit- en gas verbruik - totalen per dag\n");
//     printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
//     printf("STARTWAARDE\n\n");
//     printf("DATUM - TIJD: %s\n", tijd);
//     printf("DAG     Totaal verbruik     = %s KWh\n", DTotVerbruik);
//     printf("DAG     Totaal opbrengst    = %s KWh\n", DTotOpbrengst);
//     printf("DAG     Totaal verbruik     = %s KWh\n", NTotVerbruik);
//     printf("DAG     Totaal opbrengst    = %s KWh\n", NTotOpbrengst);
//     printf("DAG     Totaal verbuik      = %s m3\n", TotGas);
//     printf("__________________________________________________________\n");
//     printf("TOTALEN:\n");
//     printf("__________________________________________________________\n");
// }

// This function is called upon when an incoming message from mqtt is arrived
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char *msg = message->payload;

    char *token1, *token2, *token3, *token4;
    char *datumTijdStroom = NULL;
    int *tariefIndicator = NULL;
    char *actueelStroomverbruik = NULL;
    char *actueelSpanning = NULL;
    char *totaalDagVerbruik = NULL;
    char *totaalNachtVerbruik = NULL;
    char *totaalDagOpbrengst = NULL;
    char *totaalNachtOpbrengst = NULL;
    int *datumTijdGas = NULL;
    char *totaalGasVerbruik = NULL;   
    // print incoming message
    //printf( "Received message: <%s>\n", msg ); 

    // Get the first token
    token1 = strtok(msg, ";");
    if (token1 != NULL) {
        // Get the second token
        datumTijdStroom = token1;
        datumTijdGas = token1;
        token2 = strtok(NULL, ";");
        if (token2 != NULL) {
            // Get the third token
            tariefIndicator = token2;
            token3 = strtok(NULL, ";");
            if (token3 != NULL) {
                actueelStroomverbruik = token3;
                // Get the fourth token
                token4 = strtok(NULL, ";");
                if (token4 != NULL) {
                    actueelSpanning = token4;
                }
            }
        }
    }

    // Now you can use Error_Code and Text
    if (Error_Code != NULL) {
        printf("Error Code: %s\n", Error_Code);
    }
    if (Text != NULL) {
        printf("Text: %s\n", Text);
    }
    
    
    // Free the message and topic
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    
    return 1;
}

// This function is called upon when the connection to the mqtt-broker is lost
void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main() {
   // Open MQTT client for listening
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Define the correct call back functions when messages arrive
    MQTTClient_setCallbacks(client, client, connlost, msgarrvd, NULL);

    if ((rc = MQTTClient_connect(client, &conn_opts))!= MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("Subscribing to topic %s for client %s using QoS%d\n\n", TOPIC, CLIENTID, QOS);
    MQTTClient_subscribe(client, TOPIC, QOS);

    // Keep the program running to continue receiving messages
    for(;;) {
        ;
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}