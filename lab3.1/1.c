#include "bit.h"

int main() {
	int n; // The accuracy.
	printf("Please enter the accuracy that you want:");
	int symbol = 1;
	// Create the 2 variables. 
	De De_num;
	De_num.int_part = 0;
	De Bi_num;
	memset(De_num.inte, 0, sizeof(De_num.inte));
	memset(Bi_num.inte, 0, sizeof(Bi_num.inte));
	scanf("%d", &n);
	De_num.frac = (ElemType *)malloc(sizeof(ElemType) * n);
	Bi_num.frac = (ElemType *)malloc(sizeof(ElemType) * n);
	
	printf("Please enter the demical numebr:");
	char ch;
	char * c =  (char *)malloc(sizeof(char) * (DE_BIT));
	int * p = De_num.inte + DE_BIT - 1;
	int count = 0;

	// Get the integer part.
	while((ch=getchar()) != '.') {
		if(ch == '-') {
			symbol = -1;
		}
		if((int)ch >= 48 && (int)ch <= 57) {
			*c++ = ch;
			count++;
		}
	}
	De_num.len_int = count;
	while(count-- > 0) {
		*p-- = (int)*--c;
	}
	count++; // Let count be '0'.

	// Get the fraction part.
	char * s = (char *)malloc(n);
	scanf("%s", s);
	De_num.len_frac = strlen(s);
	while(count < De_num.len_frac) {
		De_num.frac[count] = s[count];
		count++;
	}
	
	// present the number you've just entered.
	printf("The num you've just entered is:");
	De_Num_Display(De_num);

	return 0;
}