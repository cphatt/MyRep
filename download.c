#include<stdio.h>
#include <string.h>
#include "C:/curllib/include/curl/curl.h"
//定义宏
#define DOWNLOADURL "http://r.heartwith.me/download.list"

char urls[10][1024];//保存.list文件的内容

/**
*消除url后面的换行符
**/
char *toURL(char *str){
	char s[1024] = {0};
	char *url = s;
	
	while(*str != '\n' && *str){
		*url++ = *str++;
	}

	return s;
}

size_t write_data(char *buffer,size_t size, size_t nitems,void *outstream)
{
	int written = fwrite(buffer, size, nitems, (FILE *)outstream);
	return written;
}

/**以指定分隔符切割字符串，
*str--文件地址
*return -- 文件名
* */
char *split(char *str){
	return (strrchr(str,'/')+1);
}

/**根据url获取该url里面的文本内容。
 * @params  url  需要读取内容的url
 * return  url的内容
 * */
void getTextContentFromUrl(char *url){
	downlaodFileFromUrl(url);
	
	FILE *file = fopen(split(url),"r+");
	fseek(file,0,SEEK_SET);
	
	char buf[1024] = {0};
	int i = 0;
	while(fgets(urls[i++],1024,file));
	fclose(file);
	remove(split(url));
}

/**
 * 从单个url下载文件
 * @params  url  需要下载的url
 * @params isOverride  当文件存在时是否覆盖文件。 1为覆盖， 0为不覆盖
 * return
 *    下载完成 返回 1 并打印成功信息和文件名
 *    下载失败 返回 0 并打印出错信息和文件名
 *    不覆盖 返回 －1 并打印不覆盖信息和文件名
 */
int downlaodFileFromUrl(char *url){
	
	FILE *file = fopen(split(url),"wb");
	puts(url);
	CURL *curl = curl_easy_init();
	int i;
	
	printf("%s is downloading \n",split(url));
	if(curl){
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void *)file);
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		
		if(curl_easy_perform(curl)){
			printf("fail\n");
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

/**
 * 根据url数组下载文件
 * @params  urls   字符串数组，表示要下载的文件*/
void downloadFileFormArray(){
	//循环遍历 urls ， 获取元素url
	//解析出文件名，判断是否存在，存在则给用户选择 是否覆盖 isOverride？
	//除非输入 N   其它任何选项都是覆盖
	//然后调用downlaodFileFromUrl(url， isOverride);
	int i = 0;
	FILE *file;
	
	while(strlen(urls[i]) != 0){
		if(file = fopen(split(toURL(urls[i])),"r")){
			printf("file %s is exist,put 1 to cover or put 0 to leave \n",split(toURL(urls[i])));
			char c = fgetc(stdin);
			fclose(file);
			if (c != '1'){
				while(getchar() != '\n');
				i++;
				continue;
			}
		}
		downlaodFileFromUrl(toURL(urls[i++]));
	}
	return;
}


/**
 * 下载
 * */
void download(char *downloadUrl){
	getTextContentFromUrl(downloadUrl);//下载list并且获取文件内容
	downloadFileFormArray();//根据文件内容下载文件
}


int main(int argc,char **argv)
{
	//用户输入下载列表的网址 默认该网址是正确的， 哪怕是错的也不用处理， 让程序自己发生错误退出就好。
   // 如果输入了 则使用用户输入的网址，如果用户直接回车，则使用默认的宏定义 DOWNLOADURL
   // char ＊url = 用户输入 or DOWNLOADURL
	char url[1024] = {0};

	printf("Please input a url:");
	fgets(url,1024,stdin);
	
	if(strlen(url) == 1)
		stpcpy(url,DOWNLOADURL);//默认的地址
	
	download(url);
	return 0;
}
