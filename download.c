/*
For downloading something from webs
*/
#include<stdio.h>
#include "C:/curllib/include/curl/curl.h"

size_t write_data(char *buffer,size_t size, size_t nitems,void *outstream)
{
	int written = fwrite(buffer, size, nitems, (FILE *)outstream);
	return written;
}

int main(int argc,char **argv)
{
	CURL *curl;
	CURLcode res;
	FILE * file;
	curl = curl_easy_init();
	char url[1024] = {0};
	char *path = "http://lft.gzhc365.com";
	char *path1 = "http://r.heartwith.me";
	//where are you want to download
	char filename[4][1024] ={"install-ie-no-ad.bat","install-ie-no-ad.es6","node.exe","ChromeStandaloneSetup.exe"};//the file name

	if(curl) {
		int i;
		for(i = 0;i < 4;i++){
			if(!(file = fopen(filename[i],"wb"))){//open a file on local
				perror("fopen");
				return -1;
			}
			if(i < 2)
				sprintf(url,"%s/%s",path1,filename[i]);
			else
				sprintf(url,"%s/%s",path,filename[i]);

			curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void *)file);
			curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
			curl_easy_setopt(curl, CURLOPT_URL, url);
			printf("%s is downloading...\n ",filename[i]);
			res = curl_easy_perform(curl);//begin download file
			fclose(file);	//close File
			printf("success!\n ");
		}

		curl_easy_cleanup(curl);
	}

	return 0;
} 
