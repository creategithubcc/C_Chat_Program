#include <string.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include "JSON.h"
static const char *ep;
const char *JSON_cuowuzhizhen(void) {return ep;}
static int JSON_bijiao_str(const char *p1,const char *p2)
{
	if (!p1) 
	{
		return (p1==p2)?0:1;
	}
	if (!p2) 
	{
		return 1;
	}
	for(; tolower(*p1) == tolower(*p2); ++p1, ++p2)	
	{
		if(*p1 == 0)
		{
			return 0;
		}
	}	
	return tolower(*(const unsigned char *)p1) - tolower(*(const unsigned char *)p2);
}
static void (*cJSON_free)(void *ptr) = free;
static void *(*cJSON_malloc)(size_t sz) = malloc;
void JSON_csh_gouzi(JSON_gouzi * hooks)////
{
    if (!hooks) 
	{
		cJSON_free = free;
        cJSON_malloc = malloc;
        return;
    }
	cJSON_free	 = (hooks->free_fn)?hooks->free_fn:free;
	cJSON_malloc = (hooks->malloc_fn)?hooks->malloc_fn:malloc;
}
static char* JSON_chuanlian(const char* str)////
{
	char* copy;
    size_t len;
    len = strlen(str) + 1;
    if (!(copy = (char*)cJSON_malloc(len)))
	{
		return 0;
	} 
    memcpy(copy,str,len);
    return copy;
}
static cJSON *JSON_xwp(void)
{
	cJSON* node = (cJSON*)cJSON_malloc(sizeof(cJSON));
	if (node) 
	{
		memset(node,0,sizeof(cJSON));
	}
	return node;
}
void JSON_shanchu(cJSON *c)
{
	cJSON *next;
	while(c)
	{
		next=c->next;
		if (c->child&&!(c->type&cJSON_IsReference)) 
		{
			JSON_shanchu(c->child);
		}
		if (c->valuestring&&!(c->type&cJSON_IsReference))
		{
			cJSON_free(c->valuestring);
		} 
		if (c->string&&!(c->type&cJSON_StringIsConst))
		{
			cJSON_free(c->string);
		} 
		cJSON_free(c);
		c=next;
	}
}
static const char *par_num(cJSON *item,const char *num)
{
	int sub=0,signsub=1;
	double n=0,sca=0,sig=1;
	if (*num=='-')
	{
		sig=-1,num++;
	}
	if (*num=='0') 
	{
		num++;
	}
	if (*num<='9'&&*num>='1')	
	{
		do	n=(n*10.0)+(*num++ -'0');	
		while (*num<='9'&&*num>='0');
	}
	if (*num=='.' && num[1]>='0' && num[1]<='9') 
	{
		num++;		
		do	n=(n*10.0)+(*num++ -'0'),sca--; 
		while (*num<='9'&&*num>='0');
	}
	if (*num=='E'||*num=='e')
	{	
		num++;
		if (*num=='+') 
		{
			num++;
		}
		else if (*num=='-') 
		{
			signsub=-1,num++;
		}
		while (*num<='9'&&*num>='0') sub=(sub*10)+(*num++ - '0');
	}
	n=sig*n*pow(10.0,(sca+sub*signsub));
	item->valuedouble=n;
	item->valueint=(int)n;
	item->type=JSON_NUM;
	return num;
}
static int po2 (int x)	
{	
	--x;
	x|=x>>1;	
	x|=x>>2;	
	x|=x>>4;	
	x|=x>>8;	
	x|=x>>16;	
	return x+1;	
}
typedef struct {char *buffer; int length; int offset; } printbuffer;
static char* queren(printbuffer *p,int nedd)
{
	int new_siz;
	char *new_buf;
	if (!p || !p->buffer) 
	{
		return 0;
	}
	nedd+=p->offset;
	if (nedd<=p->length)
	{
		return p->buffer+p->offset;
	} 
	new_siz=po2(nedd);
	new_buf=(char*)cJSON_malloc(new_siz);
	if (!new_buf) 
	{
		cJSON_free(p->buffer);
		p->length=0,p->buffer=0;
		return 0;
	}
	if (new_buf) 
	{
		memcpy(new_buf,p->buffer,p->length);
	}
	cJSON_free(p->buffer);
	p->buffer=new_buf;
	p->length=new_siz;
	return new_buf+p->offset;
}
static int gengxin(printbuffer *p)
{
	char *str;
	if (!p || !p->buffer) 
	{
		return 0;
	}
	str=p->buffer+p->offset;
	return p->offset+strlen(str);
}
static char *print_number(cJSON *item,printbuffer *p)
{
	double d=item->valuedouble;
	char *str=0;
	if (d==0)
	{
		if (p)
		{
			str=queren(p,2);
		}	
		else
		{
			str=(char*)cJSON_malloc(2);
		}
		if (str) 
		{
			strcpy(str,"0");
		}
	}
	else if (d<=INT_MAX && d>=INT_MIN && fabs(((double)item->valueint)-d)<=DBL_EPSILON)
	{
		if (p)	
		{
			str=queren(p,21);
		}
		else	
		{
			str=(char*)cJSON_malloc(21);
		}
		if (str)	
		{
			sprintf(str,"%d",item->valueint);
		}
	}
	else
	{
		if (p)
		{
			str=queren(p,64);
		}	
		else
		{
			str=(char*)cJSON_malloc(64);
		}
		if (str)
		{
			if (fabs(d)<1.0e60&&fabs(floor(d)-d)<=DBL_EPSILON)
			{
				sprintf(str,"%.0f",d);
			}
			else if (fabs(d)<1.0e-6 || fabs(d)>1.0e9)
			{
				sprintf(str,"%e",d);
			}			
			else
			{
				sprintf(str,"%f",d);
			}												
		}
	}
	return str;
}
static unsigned par_h4(const char *str)
{
	unsigned h=0;
	if (*str<='9'&&*str>='0') 
	{
		h+=(*str)-'0'; 
	}
	else if (*str<='F'&&*str>='A') 
	{
		h+=10+(*str)-'A'; 
	}
	else if (*str<='f'&&*str>='a') 
	{
		h+=10+(*str)-'a'; 
	}
	else 
	{
		return 0;
	}
	h=h<<4;
	str+=1;
	if (*str<='9'&&*str>='0') 
	{
		h+=(*str)-'0'; 
	}
	else if (*str<='F'&&*str>='A')
	{
		h+=10+(*str)-'A'; 
	} 
	else if (*str<='f'&&*str>='a') 
	{
		h+=10+(*str)-'a'; 
	}
	else 
	{
		return 0;
	}
	h=h<<4;
	str+=1;
	if (*str<='9'&&*str>='0') 
	{
		h+=(*str)-'0'; 
	}
	else if (*str<='F'&&*str>='A')
	{
		h+=10+(*str)-'A'; 
	} 
	else if (*str<='f'&&*str>='a') 
	{
		h+=10+(*str)-'a'; 
	}
	else 
	{
		return 0;
	}
	h=h<<4;
	str+=1;
	if (*str<='9'&&*str>='0') 
	{
		h+=(*str)-'0'; 
	}
	else if (*str<='F'&&*str>='A')
	{
		h+=10+(*str)-'A'; 
	} 
	else if (*str<='f'&&*str>='a') 
	{
		h+=10+(*str)-'a'; 
	}
	else 
	{
		return 0;
	}
	return h;
}
static const unsigned char firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
static const char *par_str(cJSON *item,const char *str)
{
	int len=0;
	char *out;
	const char *ptr=str+1;
	char *ptr2;
	unsigned ac_a,ac_b;
	if (*str!='\"') 
	{
		ep=str;
		return 0;
	}
	while (*ptr && ++len && *ptr!='\"') 
	{
		if (*ptr++ == '\\')
		{
			ptr+=1;
		}
	}
	out=(char*)cJSON_malloc(len+1);
	if (!out)
	{
		return 0;
	} 
	ptr=str+1;
	ptr2=out;
	while (*ptr && *ptr!='\"')
	{
		if (*ptr!='\\') 
		{
			*ptr2++=*ptr++;
		}
		else
		{
			ptr+=1;
			switch (*ptr)
			{
				case 'b': *ptr2++='\b';	break;
				case 'f': *ptr2++='\f';	break;
				case 'n': *ptr2++='\n';	break;
				case 'r': *ptr2++='\r';	break;
				case 't': *ptr2++='\t';	break;
				case 'u':
					ac_a=par_h4(ptr+1);
					ptr+=4;
					if ((ac_a>=0xDC00 && ac_a<=0xDFFF) || ac_a==0)	
					{
						break;
					}
					if (ac_a>=0xD800 && ac_a<=0xDBFF)
					{
						if (ptr[1]!='\\' || ptr[2]!='u')
						{
							break;
						}
						ac_b=par_h4(ptr+3);
						ptr+=6;
						if (ac_b<0xDC00 || ac_b>0xDFFF)		
						{
							break;
						}
						ac_a=0x10000 + (((ac_a&0x3FF)<<10) | (ac_b&0x3FF));
					}
					len=4;
					if (ac_a<0x80) 
					{
						len=1;
					}
					else if (ac_a<0x800) 
					{
						len=2;
					}
					else if (ac_a<0x10000) 
					{
						len=3;
					} 
					ptr2+=len;
					switch (len) 
					{
						case 1: *--ptr2 =(ac_a | firstByteMark[len]);
						case 2: *--ptr2 =((ac_a | 0x80) & 0xBF); ac_a >>= 6;
						case 3: *--ptr2 =((ac_a | 0x80) & 0xBF); ac_a >>= 6;
						case 4: *--ptr2 =((ac_a | 0x80) & 0xBF); ac_a >>= 6;
					}
					ptr2+=len;
					break;
				default:  *ptr2++=*ptr; 
				break;
			}
			ptr+=1;
		}
	}
	*ptr2=0;
	if (*ptr=='\"') 
	{
		ptr+=1;
	}
	item->type=JSON_STRING;
	item->valuestring=out;
	return ptr;
}
static char *prt_str_ptr(const char *str,printbuffer *p)
{
	int len=0,flag=0;
	const char *ptr;
	char *ptr2,*out;
	unsigned char tok;
	for (ptr=str;*ptr;ptr++) 
	{
		flag|=((*ptr>0 && *ptr<32)||(*ptr=='\"')||(*ptr=='\\'))?1:0;
	}
	if (!flag)
	{
		len=ptr-str;
		if (p) 
		{
			out=queren(p,len+3);
		}
		else		
		{
			out=(char*)cJSON_malloc(len+3);
		}
		if (!out) 
		{
			return 0;
		}
		ptr2=out;
		*ptr2++='\"';
		strcpy(ptr2,str);
		ptr2[len]='\"';
		ptr2[len+1]=0;
		return out;
	}
	if (!str)
	{
		if (p)	
		{
			out=queren(p,3);
		}
		else	
		{
			out=(char*)cJSON_malloc(3);
		}
		if (!out) 
		{
			return 0;
		}
		strcpy(out,"\"\"");
		return out;
	}
	ptr=str;
	while ((tok=*ptr) && ++len) 
	{
		if (strchr("\"\\\b\f\n\r\t",tok)) 
		{
			len+=1; 
		}
		else if (tok<=31) 
		{
			len+=5;
		}
		ptr+=1;
	}
	if (p)	
	{
		out=queren(p,len+3);
	}
	else	
	{
		out=(char*)cJSON_malloc(len+3);
	}
	if (!out) 
	{
		return 0;
	}
	ptr=str;
	ptr2=out;
	*ptr2++='\"';
	while (*ptr)
	{
		if ((unsigned char)*ptr>31 && *ptr!='\"' && *ptr!='\\') 
		{
			*ptr2++=*ptr++;
		}
		else
		{
			*ptr2++='\\';
			switch (tok=*ptr++)
			{
				case '\\':	*ptr2++='\\';	break;
				case '\"':	*ptr2++='\"';	break;
				case '\b':	*ptr2++='b';	break;
				case '\f':	*ptr2++='f';	break;
				case '\n':	*ptr2++='n';	break;
				case '\r':	*ptr2++='r';	break;
				case '\t':	*ptr2++='t';	break;
				default: sprintf(ptr2,"u%04x",tok);ptr2+=5;	
				break;
			}
		}
	}
	*ptr2++='\"';
	*ptr2++=0;
	return out;
}
static char *prt_str(cJSON *item,printbuffer *p)	
{
	return prt_str_ptr(item->valuestring,p);
}
static const char *par_obj(cJSON *item,const char *value);
static const char *par_val(cJSON *item,const char *value);
static const char *par_arr(cJSON *item,const char *value);
static char *prt_obj(cJSON *item,int dep,int fmt,printbuffer *p);
static char *prt_val(cJSON *item,int dep,int fmt,printbuffer *p);
static char *prt_arr(cJSON *item,int dep,int fmt,printbuffer *p);
static const char *skip(const char *in) 
{
	while (*in && (unsigned char)*in<=32 && in) 
	{
		in+=1; 
	}
	return in;
}
cJSON *JSON_Par_wit(const char *value,const char **return_parse_end,int req_null_ter)
{
	cJSON *c=JSON_xwp();
	const char *end=0;
	ep=0;
	if (!c) 
	{
		return 0;
	}
	end=par_val(c,skip(value));
	if (!end)	
	{
		JSON_shanchu(c);
		return 0;
	}
	if (req_null_ter) 
	{
		end=skip(end);
		if (*end) 
		{
			JSON_shanchu(c);
			ep=end;
			return 0;
		}
	}
	if (return_parse_end) 
	{
		*return_parse_end=end;
	}
	return c;
}
cJSON *JSON_Par(const char *value) 
{
	return JSON_Par_wit(value,0,0);
}
char *JSON_prt(cJSON *item)				
{
	return prt_val(item,0,1,0);
}
char *JSON_puf(cJSON *item)	
{
	return prt_val(item,0,0,0);
}

char *JSON_pbuf(cJSON *item,int prebuffer,int fmt)
{
	printbuffer p;
	p.buffer=(char*)cJSON_malloc(prebuffer);
	p.length=prebuffer;
	p.offset=0;
	return prt_val(item,0,fmt,&p);
	return p.buffer;
}
static const char *par_val(cJSON *item,const char *value)
{
	if (!value)						
	{
		return 0;
	}
	if (!strncmp(value,"null",4))	
	{ 
		item->type=JSON_NULL;  
		return value+4; 
	}
	if (!strncmp(value,"false",5))	
	{ 
		item->type=JSON_FALSE; 
		return value+5; 
	}
	if (!strncmp(value,"true",4))	
	{ 
		item->type=JSON_TRUE; 
		item->valueint=1;	
		return value+4; 
	}
	if (*value=='\"')				
	{ 
		return par_str(item,value); 
	}
	if (*value=='-' || (*value<='9' && *value>='0'))	
	{ 
		return par_num(item,value); 
	}
	if (*value=='[')				
	{ 
		return par_arr(item,value); 
	}
	if (*value=='{')				
	{ 
		return par_obj(item,value); 
	}
	ep=value;
	return 0;
}
static char *prt_val(cJSON *item,int dep,int fmt,printbuffer *p)
{
	char *out=0;
	if (!item) 
	{
		return 0;
	}
	if (p)
	{
		switch ((item->type)&255)
		{
			case JSON_NULL:	{
								out=queren(p,5);	
								if (out) 
								{
									strcpy(out,"null");
								}
								break;
							}
			case JSON_FALSE:{
								out=queren(p,6);	
								if (out) 
								{
									strcpy(out,"false");
								}	
								break;
							}
			case JSON_TRUE:	{
								out=queren(p,5);	
								if (out) 
								{
									strcpy(out,"true");	
								}
								break;
							}
			case JSON_NUM:	
							out=print_number(item,p);
							break;
			case JSON_STRING:	
							out=prt_str(item,p);
							break;
			case JSON_ARRAY:	
							out=prt_arr(item,dep,fmt,p);
							break;
			case JSON_OBJECT:	
							out=prt_obj(item,dep,fmt,p);
							break;
		}
	}
	else
	{
		switch ((item->type)&255)
		{
			case JSON_NULL:	
							out=JSON_chuanlian("null");	
							break;
			case JSON_FALSE:	
							out=JSON_chuanlian("false");
							break;
			case JSON_TRUE:	
							out=JSON_chuanlian("true"); 
							break;
			case JSON_NUM:	
							out=print_number(item,0);
							break;
			case JSON_STRING:	
							out=prt_str(item,0);
							break;
			case JSON_ARRAY:	
							out=prt_arr(item,dep,fmt,0);
							break;
			case JSON_OBJECT:	
							out=prt_obj(item,dep,fmt,0);
							break;
		}
	}
	return out;
}
static const char *par_arr(cJSON *item,const char *value)
{
	cJSON *child;
	if (*value!='[')	
	{
		ep=value;
		return 0;
	}
	value=skip(value+1);
	item->type=JSON_ARRAY;
	if (*value==']') 
	{
		return value+1;
	}
	item->child=child=JSON_xwp();
	if (!item->child) 
	{
		return 0;	
	}
	value=skip(par_val(child,skip(value)));
	if (!value) 
	{
		return 0;
	}
	while (*value==',')
	{
		cJSON *new_item;
		if (!(new_item=JSON_xwp())) 
		{
			return 0;
		}
		child->next=new_item;
		new_item->prev=child;
		child=new_item;
		value=skip(par_val(child,skip(value+1)));
		if (!value) 
		{
			return 0;
		}
	}
	if (*value==']') 
	{
		return value+1;
	}
	ep=value;return 0;
}
static char *prt_arr(cJSON *item,int dep,int fmt,printbuffer *p)
{
	char *out=0,*ptr,*ret;
	char **entri;
	cJSON *child=item->child;
	int len=5,nert=0,i=0,fail=0;
	size_t tmplen=0;
	while (child) 
	{
		nert++,child=child->next;
	}
	if (!nert)
	{
		if (p)	
		{
			out=queren(p,3);
		}
		else	
		{
			out=(char*)cJSON_malloc(3);
		}
		if (out) 
		{
			strcpy(out,"[]");
		}
		return out;
	}
	if (p)
	{
		i=p->offset;
		ptr=queren(p,1);
		if (!ptr) 
		{
			return 0;
		}
		*ptr='[';	
		p->offset++;
		child=item->child;
		while (child && !fail)
		{
			prt_val(child,dep+1,fmt,p);
			p->offset=gengxin(p);
			if (child->next) 
			{
				len=fmt?2:1;
				ptr=queren(p,len+1);
				if (!ptr) 
				{
					return 0;
				}
				*ptr++=',';
				if(fmt)
				{
					*ptr++=' ';
				}
				*ptr=0;
				p->offset+=len;
			}
			child=child->next;
		}
		ptr=queren(p,2);
		if (!ptr) 
		{
			return 0;
		}
		*ptr++=']';
		*ptr=0;
		out=(p->buffer)+i;
	}
	else
	{
		entri=(char**)cJSON_malloc(nert*sizeof(char*));
		if (!entri) 
		{
			return 0;
		}
		memset(entri,0,nert*sizeof(char*));
		child=item->child;
		while (child && !fail)
		{
			ret=prt_val(child,dep+1,fmt,0);
			entri[i++]=ret;
			if (ret) 
			{
				len+=strlen(ret)+2+(fmt?1:0); 
			}
			else 
			{
				fail=1;
			}
			child=child->next;
		}
		if (!fail)	
		{
			out=(char*)cJSON_malloc(len);
		}
		if (!out) 
		{
			fail=1;
		}
		if (fail)
		{
			for (i=0;i<nert;i++) 
			{
				if (entri[i]) 
				{
					cJSON_free(entri[i]);
				}
			}
			cJSON_free(entri);
			return 0;
		}
		*out='[';
		ptr=out+1;
		*ptr=0;
		for (i=0;i<nert;i++)
		{
			tmplen=strlen(entri[i]);
			memcpy(ptr,entri[i],tmplen);
			ptr+=tmplen;
			if (i!=nert-1) 
			{
				*ptr++=',';
				if(fmt)
				{
					*ptr++=' ';
				}
				*ptr=0;
			}
			cJSON_free(entri[i]);
		}
		cJSON_free(entri);
		*ptr++=']';
		*ptr++=0;
	}
	return out;	
}
static const char *par_obj(cJSON *item,const char *value)
{
	cJSON *child;
	if (*value!='{')	
	{
		ep=value;
		return 0;
	}
	item->type=JSON_OBJECT;
	value=skip(value+1);
	if (*value=='}') 
	{
		return value+1;
	}
	item->child=child=JSON_xwp();
	if (!item->child)
	{
		return 0;
	} 
	value=skip(par_str(child,skip(value)));
	if (!value)
	{
		return 0;
	} 
	child->string=child->valuestring;
	child->valuestring=0;
	if (*value!=':') 
	{
		ep=value;
		return 0;
	}
	value=skip(par_val(child,skip(value+1)));
	if (!value)
	{
		return 0;
	} 
	while (*value==',')
	{
		cJSON *new_item;
		if (!(new_item=JSON_xwp()))	
		{
			return 0;
		}
		child->next=new_item;
		new_item->prev=child;
		child=new_item;
		value=skip(par_str(child,skip(value+1)));
		if (!value) 
		{
			return 0;
		}
		child->string=child->valuestring;
		child->valuestring=0;
		if (*value!=':') 
		{
			ep=value;
			return 0;
		}
		value=skip(par_val(child,skip(value+1)));
		if (!value)
		{
			return 0;
		}
	}
	if (*value=='}') 
	{
		return value+1;
	}
	ep=value;
	return 0;
}
static char *prt_obj(cJSON *item,int dep,int fmt,printbuffer *p)
{
	char **entri=0,**names=0,*out=0,*ptr,*ret,*str;
	int len=7,i=0,j,nert=0,fail=0;
	cJSON *child=item->child;
	size_t tmplen=0;
	while (child)
	{
		nert++,child=child->next;
	} 
	if (!nert)
	{
		if (p) 
		{
			out=queren(p,fmt?dep+4:3);
		}
		else	
		{
			out=(char*)cJSON_malloc(fmt?dep+4:3);
		}
		if (!out)	
		{
			return 0;
		}
		ptr=out;
		*ptr++='{';
		if (fmt) 
		{
			*ptr++='\n';
			for (i=0;i<dep-1;i++)
			{
				*ptr++='\t';
			} 
		}
		*ptr++='}';
		*ptr++=0;
		return out;
	}
	if (p)
	{
		i=p->offset;
		len=fmt?2:1;	
		ptr=queren(p,len+1);	
		if (!ptr) 
		{
			return 0;
		}
		*ptr++='{';	
		if (fmt) 
		{
			*ptr++='\n';
		}	
		*ptr=0;	
		p->offset+=len;
		child=item->child;
		dep+=1;
		while (child)
		{
			if (fmt)
			{
				ptr=queren(p,dep);	
				if (!ptr) 
				{
					return 0;
				}
				for (j=0;j<dep;j++) 
				{
					*ptr++='\t';
				}
				p->offset+=dep;
			}
			prt_str_ptr(child->string,p);
			p->offset=gengxin(p);
			len=fmt?2:1;
			ptr=queren(p,len);	
			if (!ptr) 
			{
				return 0;
			}
			*ptr++=':';
			if (fmt) 
			{
				*ptr++='\t';
			}
			p->offset+=len;
			prt_val(child,dep,fmt,p);
			p->offset=gengxin(p);
			len=(fmt?1:0)+(child->next?1:0);
			ptr=queren(p,len+1); 
			if (!ptr) //////////////////////////////////////
			{
				return 0;
			}
			if (child->next) 
			{
				*ptr++=',';
			}
			
			if (fmt)
			{
				*ptr++='\n';
			} 
			*ptr=0;
			p->offset+=len;
			child=child->next;
		}
		ptr=queren(p,fmt?(dep+1):2);	 
		if (!ptr) ////////////////////////////////////////////
		{
			return 0;
		}
		if (fmt)
		{
			for (i=0;i<dep-1;i++) 
			{
				*ptr++='\t';
			}
		}	
		*ptr++='}';
		*ptr=0;
		out=(p->buffer)+i;
	}
	else
	{
		entri=(char**)cJSON_malloc(nert*sizeof(char*));
		if (!entri) 
		{
			return 0;
		}
		names=(char**)cJSON_malloc(nert*sizeof(char*));
		if (!names) 
		{
			cJSON_free(entri);
			return 0;
		}
		memset(entri,0,sizeof(char*)*nert);
		memset(names,0,sizeof(char*)*nert);
		child=item->child;
		dep++;
		if (fmt) 
		{
			len+=dep;
		}
		while (child)
		{
			names[i]=str=prt_str_ptr(child->string,0);
			entri[i++]=ret=prt_val(child,dep,fmt,0);
			if (str && ret) 
			{
				len+=strlen(ret)+strlen(str)+2+(fmt?2+dep:0);
			} 
			else 
			{
				fail=1;
			}
			child=child->next;
		}
		if (!fail)	
		{
			out=(char*)cJSON_malloc(len);
		}
		if (!out) 
		{
			fail=1;
		}
		if (fail)
		{/////////////////////////////////////////////
			for (i=0;i<nert;i++) 
			{
				if (names[i]) 
				{
					cJSON_free(names[i]);
				}
				if (entri[i]) 
				{
					cJSON_free(entri[i]);
				}
			}
			cJSON_free(names);
			cJSON_free(entri);
			return 0;
		}
		*out='{';
		ptr=out+1;////////////////////////////
		if (fmt)
		{
			*ptr++='\n';
		}
		*ptr=0;
		for (i=0;i<nert;i++)
		{
			if (fmt) 
			{
				for (j=0;j<dep;j++) 
				{
					*ptr++='\t';
				}
			}
			tmplen=strlen(names[i]);
			memcpy(ptr,names[i],tmplen);
			ptr+=tmplen;
			*ptr++=':';
			if (fmt) 
			{
				*ptr++='\t';
			}
			strcpy(ptr,entri[i]);
			ptr+=strlen(entri[i]);
			if (i!=nert-1) 
			{
				*ptr++=',';
			}
			if (fmt) 
			{
				*ptr++='\n';
			}
			*ptr=0;
			cJSON_free(names[i]);
			cJSON_free(entri[i]);
		}
		cJSON_free(names);
		cJSON_free(entri);
		if (fmt)
		{
			for (i=0;i<dep-1;i++) 
			{
				*ptr++='\t';
			}
		} 
		*ptr++='}';
		*ptr++=0;
	}///////////////////////////////*/******//////
	return out;	
}
int JSON_Get_Arr_Siz(cJSON *array)							
{
	cJSON *c=array->child;
	int i=0;
	while(c)i++,c=c->next;
	return i;
}
cJSON *JSON_Get_Arr_Item(cJSON *array,int item)				
{
	cJSON *c=array->child;  
	while (c && item>0) item--,c=c->next; 
	return c;
}
cJSON *JSON_Get_Obj_Item(cJSON *object,const char *string)	
{
	cJSON *c=object->child; 
	while (c && JSON_bijiao_str(c->string,string)) c=c->next; 
	return c;
}
static void suffix_object(cJSON *prev,cJSON *item) 
{
	prev->next=item;
	item->prev=prev;
}
static cJSON *create_reference(cJSON *item) 
{
	cJSON *ref=JSON_xwp();
	if (!ref) 
	{
		return 0;
	}
	memcpy(ref,item,sizeof(cJSON));
	ref->string=0;
	ref->type|=cJSON_IsReference;
	ref->next=ref->prev=0;
	return ref;
}
void JSON_Add_Item_To_Arr(cJSON *array, cJSON *item)						
{
	cJSON *c=array->child;
	if (!item) 
	{
		return;
	}
	if (!c) 
	{
		array->child=item;
	} 
	else 
	{
		while (c && c->next) c=c->next;
		suffix_object(c,item);
	}
}
void JSON_Add_Item_To_Obj(cJSON *object,const char *string,cJSON *item)	
{
	if (!item) return; 
	if (item->string) cJSON_free(item->string);
	item->string=JSON_chuanlian(string);
	JSON_Add_Item_To_Arr(object,item);
}
void JSON_Add_Item_To_Obj_CS(cJSON *object,const char *string,cJSON *item)	
{
	if (!item) return; 
	if (!(item->type&cJSON_StringIsConst) && item->string) 
	cJSON_free(item->string);
	item->string=(char*)string;item->type|=cJSON_StringIsConst;
	JSON_Add_Item_To_Arr(object,item);
}
void JSON_Add_Item_Ref_To_Arr(cJSON *array, cJSON *item)						
{
	JSON_Add_Item_To_Arr(array,create_reference(item));
}
void JSON_Add_Item_Ref_To_Obj(cJSON *object,const char *string,cJSON *item)	
{
	JSON_Add_Item_To_Obj(object,string,create_reference(item));
}

cJSON *JSON_Det_Item_From_Arr(cJSON *array,int which)			
{
	cJSON *c=array->child;
	while (c && which>0) c=c->next,which--;
	if (!c) return 0;
	if (c->prev) ////////////////////////////
	c->prev->next=c->next;
	if (c->next) 
	c->next->prev=c->prev;
	if (c==array->child) 
	array->child=c->next;
	c->prev=c->next=0;
	return c;
}
void   JSON_Del_Item_From_Arr(cJSON *array,int which)			
{
	JSON_shanchu(JSON_Det_Item_From_Arr(array,which));
}
cJSON *JSON_Det_Item_From_Obj(cJSON *object,const char *string) 
{
	int i=0;
	cJSON *c=object->child;
	while (c && JSON_bijiao_str(c->string,string)) i++,c=c->next;
	if (c) return JSON_Det_Item_From_Arr(object,i);return 0;
}
void   JSON_Del_Item_From_Obj(cJSON *object,const char *string) 
{
	JSON_shanchu(JSON_Det_Item_From_Obj(object,string));
}
void   JSON_Ins_Item_In_Arr(cJSON *array,int which,cJSON *newitem)		
{
	cJSON *c=array->child;
	while (c && which>0) c=c->next,which--;
	if (!c) 
	{
		JSON_Add_Item_To_Arr(array,newitem);
		return;
	}
	newitem->next=c;
	newitem->prev=c->prev;c->prev=newitem;
	if (c==array->child) array->child=newitem; 
	else newitem->prev->next=newitem;
}
void   JSON_Rep_Item_In_Arr(cJSON *array,int which,cJSON *newitem)		
{
	cJSON *c=array->child;
	while (c && which>0) c=c->next,which--;
	if (!c) return;
	newitem->next=c->next;newitem->prev=c->prev;
	if (newitem->next) newitem->next->prev=newitem;
	if (c==array->child) array->child=newitem; 
	else newitem->prev->next=newitem;
	c->next=c->prev=0;
	JSON_shanchu(c);
}
void   JSON_Rep_Item_In_Obj(cJSON *object,const char *string,cJSON *newitem)
{
	int i=0;
	cJSON *c=object->child;
	while(c && JSON_bijiao_str(c->string,string))i++,c=c->next;
	if(c)
	{
		newitem->string=JSON_chuanlian(string);
		JSON_Rep_Item_In_Arr(object,i,newitem);
	}
}
cJSON *JSON_Cre_Null(void)					
{
	cJSON *item=JSON_xwp();
	if(item)item->type=JSON_NULL;
	return item;
}
cJSON *JSON_Cre_True(void)					
{
	cJSON *item=JSON_xwp();
	if(item)item->type=JSON_TRUE;
	return item;
}
cJSON *JSON_Cre_False(void)					
{
	cJSON *item=JSON_xwp();
	if(item)item->type=JSON_FALSE;
	return item;
}
cJSON *JSON_Cre_Bool(int b)					
{
	cJSON *item=JSON_xwp();
	if(item)item->type=b?JSON_TRUE:JSON_FALSE;
	return item;
}
cJSON *JSON_Cre_Num(double num)			
{
	cJSON *item=JSON_xwp();
	if(item)
	{
		item->type=JSON_NUM;
		item->valuedouble=num;
		item->valueint=(int)num;
	}
	return item;
}
cJSON *JSON_Cre_Str(const char *string)	
{
	cJSON *item=JSON_xwp();
	if(item)
	{
		item->type=JSON_STRING;
		item->valuestring=JSON_chuanlian(string);
	}
	return item;
}
cJSON *JSON_Cre_Arr(void)					
{
	cJSON *item=JSON_xwp();
	if(item)item->type=JSON_ARRAY;
	return item;
}
cJSON *JSON_Cre_Obj(void)					
{
	cJSON *item=JSON_xwp();
	if(item)item->type=JSON_OBJECT;
	return item;
}
cJSON *JSON_Cre_Int_Arr(const int *numbers,int count)		
{
	int i;
	cJSON *n=0,*p=0,*a=JSON_Cre_Arr();
	for(i=0;a && i<count;i++)
	{
		n=JSON_Cre_Num(numbers[i]);
		if(!i)a->child=n;
		else suffix_object(p,n);
		p=n;
	}
	return a;
}
cJSON *JSON_Cre_Float_Arr(const float *numbers,int count)	
{
	int i;
	cJSON *n=0,*p=0,*a=JSON_Cre_Arr();
	for(i=0;a && i<count;i++)
	{
		n=JSON_Cre_Num(numbers[i]);
		if(!i)a->child=n;
		else suffix_object(p,n);
		p=n;
	}
	return a;
}
cJSON *JSON_Cre_Double_Arr(const double *numbers,int count)	
{
	int i;
	cJSON *n=0,*p=0,*a=JSON_Cre_Arr();
	for(i=0;a && i<count;i++)
	{
		n=JSON_Cre_Num(numbers[i]);
		if(!i)a->child=n;
		else suffix_object(p,n);
		p=n;
	}
	return a;
}
cJSON *JSON_Cre_Str_Arr(const char **strings,int count)	
{
	int i;
	cJSON *n=0,*p=0,*a=JSON_Cre_Arr();
	for(i=0;a && i<count;i++)
	{
		n=JSON_Cre_Str(strings[i]);
		if(!i)a->child=n;
		else suffix_object(p,n);
		p=n;
	}
	return a;
}
cJSON *JSON_Dup(cJSON *item,int recurse)
{
	cJSON *newitem,*cptr,*nptr=0,*newchild;
	if (!item) return 0;
	newitem=JSON_xwp();
	if (!newitem) return 0;
	newitem->type=item->type&(~cJSON_IsReference),newitem->valueint=item->valueint,newitem->valuedouble=item->valuedouble;
	if (item->valuestring)	
	{
		newitem->valuestring=JSON_chuanlian(item->valuestring);	
		if (!newitem->valuestring)	
		{
			JSON_shanchu(newitem);
			return 0;
		}
	}
	if (item->string)		
	{
		newitem->string=JSON_chuanlian(item->string);			
		if (!newitem->string)		
		{
			JSON_shanchu(newitem);
			return 0;
		}
	}
	if (!recurse) return newitem;
	cptr=item->child;
	while (cptr)
	{
		newchild=JSON_Dup(cptr,1);
		if (!newchild) 
		{
			JSON_shanchu(newitem);
			return 0;
		}
		if (nptr)	
		{
			nptr->next=newchild,newchild->prev=nptr;
			nptr=newchild;
		}	
		else		
		{
			newitem->child=newchild;
			nptr=newchild;
		}
		cptr=cptr->next;
	}
	return newitem;
}
void JSON_Minify(char *json)
{
	char *into=json;
	while (*json)
	{
		if (*json==' ') json++;
		else if (*json=='\t') json++;
		else if (*json=='\r') json++;
		else if (*json=='\n') json++;
		else if (*json=='/' && json[1]=='/')  while (*json && *json!='\n') json++;
		else if (*json=='/' && json[1]=='*') 
		{
			while (*json && !(*json=='*' && json[1]=='/')) json++;json+=2;
		}
		else if (*json=='\"')
		{
			*into++=*json++;
			while (*json && *json!='\"')
			{
				if (*json=='\\') *into++=*json++;
				*into++=*json++;
			}
			*into++=*json++;
		} 
		else *into++=*json++;	
	}
	*into=0;
}