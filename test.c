#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#include "Config.h"


//sturct meet data 
// struct meter_data{
//     char datumTijdStroom[DATA_TIME_LEN];
//     int tariefIndicator;
//     float actueelStroomverbruik;
//     float actueelSpanning;
//     float totaalDagVerbruik;
//     float totaalNachtVerbruik;
//     float totaalDagOpbrengst;
//     float totaalNachtOpbrengst;
//     char datumTijdGas[DATA_TIME_LEN];
//     float totaalGasVerbruik;
// };


//print values
void print_Start_values(char *tijd, char *DTotVerbruik, char *DTotOpbrengst, char *NTotVerbruik, char *NTotOpbrengst, char *TotGas) {

    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("Elekriciteit- en gas verbruik - totalen per dag\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("STARTWAARDE\n\n");
    printf("DATUM - TIJD: %s\n", tijd);
    printf("DAG     Totaal verbruik     = %s KWh\n", DTotVerbruik);
    printf("DAG     Totaal opbrengst    = %s KWh\n", DTotOpbrengst);
    printf("NACHT     Totaal verbruik   = %s KWh\n", NTotVerbruik);
    printf("NACHT     Totaal opbrengst  = %s KWh\n", NTotOpbrengst);
    printf("GAS     Totaal verbuik      = %s m3\n", TotGas);
    printf("__________________________________________________________\n");
    printf("TOTALEN:\n");
    printf("__________________________________________________________\n");

    totaal(DTotVerbruik, NTotVerbruik, DTotOpbrengst, NTotOpbrengst, TotGas, tijd);
}

void split_str(char *msg){
    char *token;
    char datumTijdStroom[DATA_TIME_LEN], tariefIndicator[DATA_LEN], actueelStroomverbruik[DATA_LEN], actueelSpanning[DATA_LEN], totaalDagVerbruik[DATA_LEN],
    totaalNachtVerbruik[DATA_LEN], totaalDagOpbrengst[DATA_LEN], totaalNachtOpbrengst[DATA_LEN], datumTijdGas[DATA_TIME_LEN], totaalGasVerbruik[DATA_LEN];

    token = strtok(msg, ";");
    strcpy(datumTijdStroom, token);

    token = strtok(NULL, ";");
    strcpy(tariefIndicator, token);

    token = strtok(NULL, ";");
    strcpy(actueelStroomverbruik, token);

    token = strtok(NULL, ";");
    strcpy(actueelSpanning, token);

    token = strtok(NULL, ";");
    strcpy(totaalDagVerbruik, token);

    token = strtok(NULL, ";");
    strcpy(totaalNachtVerbruik, token);

    token = strtok(NULL, ";");
    strcpy(totaalDagOpbrengst, token);

    token = strtok(NULL, ";");
    strcpy(totaalNachtOpbrengst, token);

    token = strtok(NULL, ";");
    strcpy(datumTijdGas, token);

    token = strtok(NULL, ";");
    strcpy(totaalGasVerbruik, token);

    // printf("_________________________________________\n");
    // printf("%s%s%s%s%s%s%s%s%s%s", datumTijdStroom, tariefIndicator, actueelStroomverbruik, actueelSpanning, totaalDagVerbruik, totaalDagOpbrengst, totaalNachtVerbruik, totaalNachtOpbrengst, datumTijdGas, totaalGasVerbruik);
    // printf("__________________________________________\n");
    
    print_Start_values(datumTijdStroom, totaalDagVerbruik ,totaalDagOpbrengst, totaalNachtVerbruik, totaalNachtOpbrengst, totaalGasVerbruik);
   

    return;
}

void totaal(char dagVerbruik, char nachtVerbruik,char dagOpbrengst,char nachtOpbregst,char gasVerbruik,char datum){
    float totaalStroomVerbruik, totaalStroomOpbrengst, DV, NV, DO, NO, GV;

    DV = atof(dagVerbruik);
    NV = atof(nachtVerbruik);
    DO = atof(dagOpbrengst);
    NO = atof(nachtOpbregst);
    GV = atof(gasVerbruik);

    totaalStroomVerbruik=  DV + NV;
    totaalStroomOpbrengst = DO + NO;

    printf("Datum: %s\n", datum);
    printf("________________\n");
    printf("STROOM:\n");
    printf("        Totaal verbruk      = %f", totaalStroomVerbruik);
    printf("        Totaal opbrengst    = %f", totaalStroomOpbrengst);
    printf("GAS:");
    printf("        Totaal verbruik     = %f", GV);
    
    return;
}

// This function is called upon when an incoming message from mqtt is arrived
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {  
    char *msg = message->payload;

    // print incoming message
    // printf( "Received message: <%s>\n", msg ); 
    
    FILE *file = fopen("log.log", "ab+"); 
    if (file != NULL) { 
        fputs(msg, file); 
        fclose(file); 
    } 
    split_str(msg);

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