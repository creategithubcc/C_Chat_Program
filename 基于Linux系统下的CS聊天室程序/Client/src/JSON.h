#ifndef cJSON__h
#define cJSON__h
#ifdef __cplusplus
extern "C"
{
#endif
#define cJSON_StringIsConst 512
#define cJSON_IsReference 256
#define JSON_OBJECT 6
#define JSON_ARRAY 5
#define JSON_STRING 4
#define JSON_NUM 3
#define JSON_NULL 2
#define JSON_TRUE 1
#define JSON_FALSE 0
typedef struct cJSON 
{
	struct cJSON *next,*prev;
	struct cJSON *child;		
	int type;
	int valueint;				
	char *string;
	char *valuestring;				
	double valuedouble;			
} cJSON;
typedef struct JSON_gouzi  
{
  void (*free_fn)(void *ptr);
  void *(*malloc_fn)(size_t sz);
} JSON_gouzi ;
extern void JSON_csh_gouzi(JSON_gouzi * hooks);
extern cJSON *JSON_Par(const char *value);
extern char  *JSON_prt(cJSON *item);
extern char  *JSON_puf(cJSON *item);
extern char *JSON_pbuf(cJSON *item,int prebuffer,int fmt);
extern void   JSON_shanchu(cJSON *c);
extern int	  JSON_Get_Arr_Siz(cJSON *array);
extern cJSON *JSON_Get_Arr_Item(cJSON *array,int item);
extern cJSON *JSON_Get_Obj_Item(cJSON *object,const char *string);
extern const char *JSON_cuowuzhizhen(void);
extern cJSON *JSON_Cre_Null(void);
extern cJSON *JSON_Cre_True(void);
extern cJSON *JSON_Cre_False(void);
extern cJSON *JSON_Cre_Bool(int b);
extern cJSON *JSON_Cre_Num(double num);
extern cJSON *JSON_Cre_Obj(void);
extern cJSON *JSON_Cre_Str(const char *string);
extern cJSON *JSON_Cre_Arr(void);
extern cJSON *JSON_Cre_Int_Arr(const int *numbers,int count);
extern cJSON *JSON_Cre_Double_Arr(const double *numbers,int count);
extern void	JSON_Add_Item_To_Obj(cJSON *object,const char *string,cJSON *item);
extern cJSON *JSON_Cre_Str_Arr(const char **strings,int count);
extern void JSON_Add_Item_To_Arr(cJSON *array, cJSON *item);
extern cJSON *JSON_Cre_Float_Arr(const float *numbers,int count);
extern void	JSON_Add_Item_To_Obj_CS(cJSON *object,const char *string,cJSON *item);
extern void JSON_Add_Item_Ref_To_Arr(cJSON *array, cJSON *item);
extern void	JSON_Add_Item_Ref_To_Obj(cJSON *object,const char *string,cJSON *item);
extern void   JSON_Del_Item_From_Arr(cJSON *array,int which);
extern cJSON *JSON_Det_Item_From_Arr(cJSON *array,int which);
extern cJSON *JSON_Det_Item_From_Obj(cJSON *object,const char *string);
extern void JSON_Ins_Item_In_Arr(cJSON *array,int which,cJSON *newitem);
extern void JSON_Rep_Item_In_Arr(cJSON *array,int which,cJSON *newitem);
extern void   JSON_Del_Item_From_Obj(cJSON *object,const char *string);
extern void JSON_Rep_Item_In_Obj(cJSON *object,const char *string,cJSON *newitem);
extern cJSON *JSON_Dup(cJSON *item,int recurse);
extern cJSON *JSON_Par_wit(const char *value,const char **return_parse_end,int req_null_ter);
extern void JSON_Minify(char *json);
#define cJSON_AddNullToObject(object,name)		JSON_Add_Item_To_Obj(object, name, JSON_Cre_Null())
#define cJSON_AddTrueToObject(object,name)		JSON_Add_Item_To_Obj(object, name, JSON_Cre_True())
#define cJSON_AddFalseToObject(object,name)		JSON_Add_Item_To_Obj(object, name, JSON_Cre_False())
#define cJSON_AddBoolToObject(object,name,b)	JSON_Add_Item_To_Obj(object, name, JSON_Cre_Bool(b))
#define cJSON_AddNumberToObject(object,name,n)	JSON_Add_Item_To_Obj(object, name, JSON_Cre_Num(n))
#define cJSON_AddStringToObject(object,name,s)	JSON_Add_Item_To_Obj(object, name, JSON_Cre_Str(s))
#define cJSON_SetIntValue(object,val)			((object)?(object)->valueint=(object)->valuedouble=(val):(val))
#define cJSON_SetNumberValue(object,val)		((object)?(object)->valueint=(object)->valuedouble=(val):(val))
#ifdef __cplusplus
}
#endif
#endif