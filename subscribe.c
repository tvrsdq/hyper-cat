#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "MQTTClient.h"
#include <pthread.h>
#define ADDRESS     		"172.16.2.17:1883"
#define CLIENTID    		"ExampleClientSub1"
#define TIMEOUT    		 10000L
#define TOPIC1      		"Topic1"
#define TOPIC2       		"Topic2"
#define SAVEFILE_PATH_FILE 	"/home/thanveer/smartbin/mqtt_study/mulitple_subscribe/saved_file/received_file"
#define PATH_TO_NFILE 		"/home/thanveer/smartbin/mqtt_study/mulitple_subscribe/saved_file/details/nos.txt"
volatile MQTTClient_deliveryToken deliveredtoken;
int no_of_files;				// to store the nubmer of files arrived
void delivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;	
}
void get_no_of_files()				// function to get the number of files already saved 
{
	FILE *fp;
	int ch;
	fp = fopen(PATH_TO_NFILE,"r");
	if(fp==NULL)
	{
		printf("error opening the details nos file\n");
	}
	printf("fetching the number of files arrived\n");
	fread(&ch,2,1,fp);
	no_of_files=ch;
	fclose(fp);
}
void save_no_of_files()				// function to save the number of files that have arrived
{
	int chs;
	chs=no_of_files;
	FILE *fp;
	fp=fopen(PATH_TO_NFILE,"w+");
	if(fp==NULL)
	{
		printf("error opening the file the save the number of files\n");	
	}
	printf("saving the number of files arrived..\n");
	fwrite(&chs,2,1,fp);
	fclose(fp);
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	int i;
	char* payloadptr;
    	printf("Message arrived\n");
    	payloadptr = message->payload;
	get_no_of_files();
	++no_of_files;
	char filename[100];
    	FILE *file_reached;
	sprintf(filename,"/home/thanveer/smartbin/mqtt_study/mulitple_subscribe/saved_file/file%d",no_of_files);
	printf("filename:%s\n",filename);
	printf("opening the file for saving the file.\n");
	file_reached=fopen(filename,"ab+");
    	if(file_reached==NULL)
    	{
		printf("unable to open a file for saving the file\n");
     	}
    	for(i=0; i<message->payloadlen; i++)
     	{
 		fputc(*payloadptr++, file_reached);
     	}
	printf("writing the save file completed\n");
    	fclose(file_reached);
	save_no_of_files();
		printf("saving number of files completed\n");
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

    	char* topics[2] =
	{
		"Topic1","Topic2",
	};
	int qoss[2] = {1, 1};
    	MQTTClient client;
    	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    	int rc;
    	int ch;
    	MQTTClient_create(&client, ADDRESS, CLIENTID,MQTTCLIENT_PERSISTENCE_DEFAULT, NULL);
    	conn_opts.keepAliveInterval = 20;
    	conn_opts.cleansession = 1;
    	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    	{
        	printf("Failed to connect client, return code %d\n", rc);
        	exit(EXIT_FAILURE);
    	}
    	int check;
    	check=MQTTClient_subscribeMany(client,2,topics,qoss); 	
	printf("Subscribing to %s and %s \n",topics[0],topics[1]);
    	printf("MQTTClient_subscribeMany() return :%d\n",check);
    	do
    	{
 		ch=getchar();
 	} while(ch!='Q' && ch != 'q');
    	MQTTClient_disconnect(client, 10000);
    	MQTTClient_destroy(&client);
    	return rc;
}
