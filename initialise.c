#include <stdio.h>
#define PATH_TO_NFILE "/home/thanveer/smartbin/mqtt_study/mulitple_subscribe/saved_file/details/nos.txt"

void initialise()			// initialise the details file with 0
{
	FILE *fp;
	int ch=0;
	fp=fopen(PATH_TO_NFILE,"w");
	if(fp==NULL)
	{printf("error in initialising nos.txt\n");}
	fwrite(&ch,2,1,fp);
	fclose(fp);
}

int main()
{
initialise();
printf("initialisaton complete\n");
sleep(2);
system("clear");
}
