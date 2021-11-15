#ifndef BASE64_CENCODE_H
#define BASE64_CENCODE_H
typedef enum
{
	step_a, step_b, step_c
} W64bianmabuzhou;

typedef struct
{
	int ste_a;
	char res_a;
	W64bianmabuzhou step;	
} W64bianmazhuangtai;
int bianmakuai(const char* input_pla2, int input_len, char* output_cod, W64bianmazhuangtai* input_sta);
int bianmakuai2(char* output_cod, W64bianmazhuangtai* input_sta);
char bianmazhi(char input_val);
void chushihua2(W64bianmazhuangtai* input_sta);
#endif