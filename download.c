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
	char *path = "http://lft.gzhc365.com";//where the files are
	char filename[2][1024] ={"node.exe","ChromeStandaloneSetup.exe"};//this array contains the files you want to download

	if(curl) {
		int i;
		for(i = 0;i < 2;i++){
			if(!(file = fopen(filename[i],"wb"))){
				perror("fopen");
				return -1;
			}

			sprintf(url,"%s/%s",path,filename[i]);
			curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void *)file);
			curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
			curl_easy_setopt(curl, CURLOPT_URL, url);
			printf("%s is downloading...\n ",filename[i]);
			res = curl_easy_perform(curl);
			printf("success!\n ");
		}

		curl_easy_cleanup(curl);
	}

	return 0;
}
