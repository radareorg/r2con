#include <string.h>
#include <stdio.h>

void test1(){

	int ri;
	void *rp;
	char *rm;
	int rs;

	int _val;
	char str1[] = "1234567890";
	char str2[] = "ABCDEFGHIJ";
	char str[] = "abcdefghil";
	char *s1 = (char *)str;
	char *_s = s1;
	char *_str = s1;
	char _fmt[] = "%d\n";
	char *buf = str;
	char *_buf = str;

	void *arr1 = str1;
	void *arr2 = str2;
	void *arr = str;
	void *_ptr = arr;
	void *__ptr = arr;
	

	char _c;

	size_t __size = 5;
	size_t _size = __size;
	size_t __nmemb = 6;

	int s = 10;
	int i = 5;
	int _accept = 3;
	int _reject = 4;

	FILE *__stream;

	void *funptr;
	
	int __n = 3;

	ri =	putc(_c, __stream);
	ri =	putchar(_c);
	ri =	printf(_fmt, i);
	ri =	sprintf(_s, _fmt, i);
	ri =	snprintf(_s, __n, _fmt, i);
	ri =	fprintf(__stream, _fmt, i);
	ri =	fputs(_str, __stream);
	ri =	puts(_str);
	rs =	fwrite(_ptr, __size, __nmemb, __stream);
	ri =	fgetc(__stream);
	ri =	getc(__stream);
	ri =	getchar();
	rp = memccpy(arr2, arr1, i, _size);
	rp = memchr(arr1, i, _size);
	ri = memcmp(arr1, arr2, _size);
	rp = memcpy(arr2, arr1, _size);
	rp = memmem(arr1, _size, arr1, _size);
	rp = memmove(arr2, arr1, _size);
	rp = memrchr(arr1, i, _size);
	rp = memset(arr2, i, _size);
	rm = strcat(str1, str2);
	rm = strchr(str2, i);
	rm = strchrnul(str2, i);
	ri = strcmp(str1, str2);
	rm = strcpy(str1, str2);
	ri = strcasecmp(str1, str2);
	rm = strcasestr(str1, str2);
	rm = strdup(s1);
	rs =strlcat(str1, str2, _size);
	rs =strlcpy(str1, str2, _size);
	rs =strlen(str2);
	rm = strlwr(str1);
	rm = strncat(str1, str2, _size);
	ri = strncmp(str1, str2, _size);
	rm = strncpy(str1, str2, _size);
	ri = strncasecmp(str1, str2, _size);
	rs =strnlen(str2, _size);
	rm = strrchr(str2, i);
	rm = strrev(str1);
	rm = strstr(str1, str2);
	rm = strtok(str1, str2);
	rm = strupr(str1);
}

void setup(){
	String str = "123";
	char c1 = 1;
	char arr[128];
	test1();
	Serial.begin(9600);
	Serial.println("Test!");
	Serial.println(arr);
	Serial.println(str.length());
	Serial.println(str.c_str());
	str = Serial.readString();
	str = Serial.readStringUntil('\n');
	Serial.readBytesUntil('\n', arr, 128);
	Serial.find("tst");
	Serial.setTimeout(1000);
	
	double d = 0.0;
	d += 1.0;
	d /= 2;
	d *= 3;
	d--;

	digitalWrite(13, c1);
	c1 = digitalRead(10);
	pinMode(13, OUTPUT);
	pinMode(10, OUTPUT);

	c1 = Serial.available();
	delay(10);
	Serial.print("123");
}

void loop(){ }
