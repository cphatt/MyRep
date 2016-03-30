#include<stdio.h>
#include<string.h>
#include "C:/curllib/include/curl/curl.h"

char filename[1024];//save download.list
size_t write_data(char *buffer,size_t size, size_t nitems,void *outstream)
{
	int written = fwrite(buffer, size, nitems, (FILE *)outstream);
	return written;
}

/*
	this is function can download a file  from url
*/
int getFile(char *url)
{
	int i;
	if(!strrchr(url,'/')){				
		printf("No file on the url\n");
		return 0;
	}
	stpcpy(filename,strrchr(url,'/')+1);
	FILE *file = fopen(filename,"r");
	if (file){
		printf("%s is exist,put 1 to cover or put 0 to leave \n");
		char c = fgetc(stdin);
		if (c != '1'){
			return;
		}
	}
	
	file = fopen(filename,"wb");
	
	CURL *curl = curl_easy_init();
	if(curl){
		printf("%s is downloading ..\n",filename);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void *)file);
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		if(curl_easy_perform(curl)){
			printf("fail,No sach file on the url\n");
			i = 0;
		}else{
			printf("success\n");
			i = 1;
		}
	}
	curl_easy_cleanup(curl);
	fclose(file);
	return i;	
}

int main(int argc,char **argv)
{
	FILE * file;
	char url[1024] = {0};
	
	printf("Please input a url:");
	fgets(url,1024,stdin);				//rcv user url
	if(strlen(url) == 1)
		stpcpy(url,"http://cph/download.list");	//default url
		
	if(!getFile(url)){//get download.list
		remove(filename);
		printf("please Enter\n");
		char c = fgetc(stdin);
		return 0;
	}
	
	file = fopen(filename,"r");
	char buf[1024] = {0};
	int num = 0;
	fseek(file,0,SEEK_SET);
	while(fgets(buf,1024,file)){
		num += getFile(buf);
	}
	printf("%d files has been downloaded\n",num);
	fclose(file);
	
	printf("Enter key to exit \n");
	char c = fgetc(stdin);
	return 0;
}
