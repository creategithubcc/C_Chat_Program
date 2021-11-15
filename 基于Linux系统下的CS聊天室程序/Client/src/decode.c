#include <decode.h>
int jiemazhi(char input_val)
{
	static const char decoding[] = {62,-1,-1,-1,63,52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-2,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51};
	static const char decoding_size = sizeof(decoding);
	input_val -= 43;
	if (input_val < 0 || input_val > decoding_size)
	{
		return -1;
	}
	return decoding[(int)input_val];
}
void chushihua(W64jiema* input_sta)
{
	input_sta->p_char = 0;
	input_sta->step = step_1;
}
int jiemakuai(const char* input_code, const int input_len, char* output_pla, W64jiema* input_sta)
{
	char fenduan;
	char* p_char = output_pla;
	const char* cod_char = input_code;
	*p_char = input_sta->p_char;
	switch (input_sta->step)
	{
		while (1)
		{
		case step_1:
			do {
				if (input_code+input_len==cod_char)
				{
					input_sta->p_char = *p_char;
					input_sta->step = step_1;
					return p_char - output_pla;
				}
				fenduan = (char)jiemazhi(*cod_char++);
			} while (fenduan < 0);
			*p_char = (fenduan & 0x03f) << 2;
		case step_2:
			do {
				if (input_code+input_len==cod_char)
				{
					input_sta->p_char = *p_char;
					input_sta->step = step_2;
					return p_char - output_pla;
				}
				fenduan = (char)jiemazhi(*cod_char++);
			} while (fenduan < 0);
			*p_char++ |= (fenduan & 0x030) >> 4;
			*p_char = (fenduan & 0x00f) << 4;
		case step_3:
			do {
				if (input_code+input_len==cod_char)
				{
					input_sta->p_char = *p_char;
					input_sta->step = step_3;
					return p_char - output_pla;
				}
				fenduan = (char)jiemazhi(*cod_char++);
			} while (fenduan < 0);
			*p_char++ |= (fenduan & 0x03c) >> 2;
			*p_char = (fenduan & 0x003) << 6;
		case step_4:
			do {
				if (input_code+input_len==cod_char)
				{
					input_sta->p_char = *p_char;
					input_sta->step = step_4;
					return p_char - output_pla;
				}
				fenduan = (char)jiemazhi(*cod_char++);
			} while (fenduan < 0);
			*p_char++ |= (fenduan & 0x03f);
		}
	}
	return p_char - output_pla;
}