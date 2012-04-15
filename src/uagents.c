#include "uagents.h"
#include <strings.h>
#include <stdlib.h>

const char* uagents[] = {

 //web browsers
 "Mozilla/5.0 (Windows NT 6.1; rv:12.0) Gecko/20120403211507 Firefox/12.0",
 "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.6 (KHTML, like Gecko) Chrome/20.0.1092.0 Safari/536.6",	
 "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_6; en-us) AppleWebKit/533.20.25 (KHTML, like Gecko) Version/5.0.4 Safari/533.20.27",
 "Mozilla/5.0 (BlackBerry; U; BlackBerry 9900; en) AppleWebKit/534.11+ (KHTML, like Gecko) Version/7.1.0.346 Mobile Safari/534.11+",
 "Opera/9.80 (Windows NT 6.1; U; es-ES) Presto/2.9.181 Version/12.00",
 "Opera/9.80 (Android 2.3.3; Linux; Opera Mobi/ADR-1111101157; U; es-ES) Presto/2.9.201 Version/11.50",
 "Mozilla/5.0 (Android; Linux armv7l; rv:9.0) Gecko/20111216 Firefox/9.0 Fennec/9.0",
 "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; WOW64; Trident/6.0)",

 //crawlers
 "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
 "Mozilla/5.0 (compatible; bingbot/2.0; +http://www.bing.com/bingbot.htm)",
 "Baiduspider+(+http://www.baidu.com/search/spider_jp.html)"
};


const char* resolve_ua(char* ua_short) {

	extern const char* uagents[];
	
    if (strcmp(ua_short,"ff") == 0) return uagents[0];
	if (strcmp(ua_short,"chr") == 0) return uagents[1];
	if (strcmp(ua_short,"saf") == 0) return uagents[2];
	if (strcmp(ua_short,"bb") == 0) return uagents[3];
	if (strcmp(ua_short,"ope") == 0) return uagents[4];
	if (strcmp(ua_short,"opem") == 0) return uagents[5];
	if (strcmp(ua_short,"fen") == 0) return uagents[6];
	if (strcmp(ua_short,"ie") == 0) return uagents[7];
	if (strcmp(ua_short,"gbot") == 0) return uagents[8];
	if (strcmp(ua_short,"bing") == 0) return uagents[9];
	if (strcmp(ua_short,"bspid") == 0) return uagents[10];
	return NULL;
    
}