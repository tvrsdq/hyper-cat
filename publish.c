#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"
#define ADDRESS     "172.16.2.17:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC1       "Topic1"
#define TOPIC2       "Topic2"
#define TIMEOUT     10000L
#define OUTFILE_PATH_IMAGE "/home/thanveer/smartbin/mqtt_study/mulitple_subscribe/topic1/image.jpeg"	
#define OUTFILE_PATH_FILE "/home/thanveer/smartbin/mqtt_study/mulitple_subscribe/topic1/topic1.txt"	
#define SAVEFILE_PATH_FILE "/home/thanveer/smartbin/mqtt_study/mulitple_subscribe/saved_file/received_file"
#define PATH_TO_NFILE "/home/thanveer/smartbin/mqtt_study/mulitple_subscribe/saved_file/details/nos.txt"

/*void initialise()
{
	FILE *fp
	int ch=0;
	fp=fopen("nos.txt","w");
	if(fp==NULL)
	{printf("error in initialising nos.txt\n");}
	fwrite(&ch,2,1,fp);
	fclose(fp);
}
*/
volatile MQTTClient_deliveryToken deliveredtoken;
MQTTClient client;
MQTTClient_deliveryToken token;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("PUBLISH Message with token value %d delivery confirmed\n", dt);
        deliveredtoken = dt;
}
MQTTClient_message pubmsg = MQTTClient_message_initializer;
topic1()
{
	int rc;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
   	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    	{
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    	}
    	FILE *fptr;
    	fptr=fopen(OUTFILE_PATH_IMAGE,"rb");
    	if(fptr==NULL)
   	{
	printf("error reading \n ");
   	}
    	fseek(fptr, 0, SEEK_END);
    	long fsize = ftell(fptr);
    	fseek(fptr, 0, SEEK_SET);  
    	char *fstring = malloc(fsize + 1);
    	fread(fstring, fsize, 1, fptr);
    	fclose(fptr);
    	pubmsg.payload = fstring;
    	pubmsg.payloadlen = fsize+1;
    	pubmsg.qos = 1;
    	pubmsg.retained = 0;
    	deliveredtoken = 0;
    	MQTTClient_publishMessage(client, TOPIC1, &pubmsg, &token);
}
topic2()
{    
	int rc;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
        	printf("Failed to connect, return code %d\n", rc);
        	exit(EXIT_FAILURE);
        }
        FILE *fptr;
    	fptr=fopen(OUTFILE_PATH_FILE,"rb");
    	if(fptr==NULL)
    	{
	printf("error reading \n ");
        }
    	fseek(fptr, 0, SEEK_END);
    	long fsize = ftell(fptr);
    	fseek(fptr, 0, SEEK_SET);  
  	char *fstring = malloc(fsize + 1);
  	fread(fstring, fsize, 1, fptr);
   	fclose(fptr);
   	pubmsg.payload = fstring;
   	pubmsg.payloadlen = strlen(fstring);
    	pubmsg.qos = 1;
    	pubmsg.retained = 0;
   	deliveredtoken = 0;
   	MQTTClient_publishMessage(client, TOPIC1, &pubmsg, &token);
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	int i;

	char* payloadptr;
   	printf("Message arrived\n");
    	printf("     topic: %s\n", topicName);
    	printf("   message: ");
    	payloadptr = message->payload;
    	for(i=0; i<message->payloadlen; i++)
    	{
       	 putchar(*payloadptr++);
    	}
    	putchar('\n');
    	MQTTClient_freeMessage(&message);
    	MQTTClient_free(topicName);
    	return 1;
}
void connlost(void *context, char *cause)
{
    	printf("\nConnection lost\n");
    	printf("     cause: %s\n", cause);
}
int main(int argc, char* argv[])
{
    	int rc;
    	//initialise();						// initialise for the first time.
  	while(1)
   	{
	int cret,callret;
	cret=MQTTClient_create(&client, ADDRESS, CLIENTID,MQTTCLIENT_PERSISTENCE_NONE, NULL);
	printf("cret::%d\n",cret);
	callret=MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    	printf("callret::%d\n",callret);
   	printf("enter 1 for image and 2 for file to be sent\n");
   	int ch;
   	scanf("%d",&ch);
	printf("you selected %d\n",ch);
    	if(ch==1)
    	{topic1();}
    	if(ch==2)
    	{topic2();}
    	while(deliveredtoken != token);
    	MQTTClient_disconnect(client, 10000);
    	MQTTClient_destroy(&client);
        }
    
        return rc;
}










