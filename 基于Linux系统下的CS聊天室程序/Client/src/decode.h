#ifndef BASE64_CDECODE_H
#define BASE64_CDECODE_H
typedef enum
{
	step_1, step_2, step_3, step_4
} W64jiemabuzhou;

typedef struct
{
	char p_char;
	W64jiemabuzhou step;
} W64jiema;
int jiemakuai(const char* input_code, const int input_len, char* output_pla, W64jiema* input_sta);
int jiemazhi(char input_val);
void chushihua(W64jiema* input_sta);
#endif