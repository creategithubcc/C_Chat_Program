#include <encode.h>
const int hangzifushu = 75;//e//fanhui//
void chushihua2(W64bianmazhuangtai* input_sta)
{
	input_sta->step = step_a;
	input_sta->res_a = 0;
	input_sta->ste_a = 0;
}
char bianmazhi(char input_val)
{
	static const char* encoding = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	if (input_val >= 64)
	{
		return '=';
	} 
	return encoding[(int)input_val];
}
int bianmakuai(const char* input_pla2, int input_len, char* output_cod, W64bianmazhuangtai* input_sta)
{
	const char* p_char = input_pla2;
	const char* const mingwen_end = input_pla2 + input_len;
	char fenduan;
	char res_a;
	char* cod_char = output_cod;
	res_a = input_sta->res_a;
	switch (input_sta->step)
	{
		while (1)
		{
		case step_a:
			if (mingwen_end==p_char)
			{
				input_sta->res_a = res_a;
				input_sta->step = step_a;
				return cod_char - output_cod;
			}
			fenduan = *p_char++;
			res_a = (fenduan & 0x0fc) >> 2;
			*cod_char++ = bianmazhi(res_a);
			res_a = (fenduan & 0x003) << 4;
		case step_b:
			if (mingwen_end==p_char)
			{
				input_sta->res_a = res_a;
				input_sta->step = step_b;
				return cod_char - output_cod;
			}
			fenduan = *p_char++;
			res_a |= (fenduan & 0x0f0) >> 4;
			*cod_char++ = bianmazhi(res_a);
			res_a = (fenduan & 0x00f) << 2;
		case step_c:
			if (mingwen_end==p_char)
			{
				input_sta->res_a = res_a;
				input_sta->step = step_c;
				return cod_char - output_cod;
			}
			fenduan = *p_char++;
			res_a |= (fenduan & 0x0c0) >> 6;
			*cod_char++ = bianmazhi(res_a);
			res_a  = (fenduan & 0x03f) >> 0;
			*cod_char++ = bianmazhi(res_a);
			++(input_sta->ste_a);
			if (hangzifushu/4==input_sta->ste_a)
			{
				*cod_char++ = '\n';
				input_sta->ste_a = 0;
			}
		}
	}
	return cod_char - output_cod;
}
int bianmakuai2(char* output_cod, W64bianmazhuangtai* input_sta)
{
	char* cod_char = output_cod;
	switch (input_sta->step)
	{
	case step_b:
		*cod_char++ = bianmazhi(input_sta->res_a);
		*cod_char++ = '=';
		break;
	case step_c:
		*cod_char++ = bianmazhi(input_sta->res_a);
		*cod_char++ = '=';
		break;
	case step_a:
		break;
	}
	*cod_char++ = '\n';
	return cod_char - output_cod;
}