#include <blib.h>

size_t strlen(const char *s) {
	char * p=(char*)s;
	unsigned int length=0;
	while((*p)++!='\0'){ length++;}
	return length;
}

char *strcpy(char *dst, const char *src) {
	assert(dst != NULL && src != NULL); 
	char *ret = dst;
	while ((*dst++=*src++)!='\0');
	return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
	char *res = dst;
	while (*src && n--) {
		*dst++ = *src++;
	}
	*dst = '\0';
	return res;
}

char *strcat(char *dst, const char *src) {
        char *tmp = dst;
        while (*dst)
                dst++;
        while ((*dst++ = *src++) != '\0');
        return tmp;
}

int strcmp(const char *s1, const char *s2) {
	const unsigned char *p1 = (const unsigned char *) s1;
	const unsigned char *p2 = (const unsigned char *) s2;
	unsigned char c1, c2;
	do {
		c1 = (unsigned char) *p1++;
        	c2 = (unsigned char) *p2++;
        	if(c1 == '\0')
		{
			return c1 - c2;
			break;
		}
		else if(c2 == '\0')
		{
			return c1 - c2;
			break;
		}
   	 } while (c1 == c2);
    return c1 - c2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
	while (n--) {
		if (*s1 != *s2) {
			return *s1 - *s2;
		}
		if (*s1 == 0) {
			break;
		}
		s1++;
		s2++;
	}
	return 0;
}

void *memset(void *s, int c, size_t n) {
	unsigned char* p=s;
	while(n--)
        *p++ = (unsigned char)c;
	return s;
}

void *memcpy(void *out, const void *in, size_t n) {
	char *csrc = (char *)in;
	char *cdest = (char *)out;
	int i = 0;
	for (i = 0; i < n; i++) {
		cdest[i] = csrc[i];
	}
	return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {	
	if(!n)
	{
		return 0;
	}
	while(n-- && *(char*)s1 == *(char*)s2)
	{
        	s1 = (char*)s1 + 1;
		s2 = (char*)s2 + 1;
	}
	return(*((unsigned char *)s1) - *((unsigned char *)s2));
}
