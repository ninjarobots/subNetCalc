/*
	
	I want a function that accepts two char * parameters. The first is an IP address, the second is a subnet.

	Without using the string library, determine the IP range that the IP address identified can communicate without
		the use of a gateway.

	use BitWise operands, pointer arithmetic, and return a struct containing two in_addr_t variables by utilizing 
		the netinet/in.h library. 

	Ensure to do proper input validation and return -1 on invalid inputs.

*/

#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<math.h>

struct Network {
	char* first;
	char* last;
};

struct Network *netAddr(char* addr, char* subNet);
int binMaker(char* string);
char* makeString(uint32_t binValue);

int main() {
	char* addr = "122.15.11.18";
	char* subNet = "255.255.192.0";
	struct Network *networks = netAddr(addr, subNet);
	printf("The first IP address in this subnet is %s.\n" , networks->first);
	printf("The second IP address in this subnet is %s.", networks->last);
	getchar();
}

struct Network *netAddr(char* addr, char* subNet){
	struct Network *returnStruct = malloc(sizeof(struct Network));
	uint32_t binAddr = binMaker(addr);
	int subValue = 1;
	if (binAddr == 0) {
		printf("INVALID NETWORK ADDRESS");
		return -1;
	}
	uint32_t binSubNet = binMaker(subNet);
	if (binSubNet == 0) {
		printf("INVALID SUBNET MASK");
		return -1;
	}
	if (binSubNet == 0xFFFFFFFF) {
		subValue = 0;
	}
	uint32_t wildCardAddr = (binSubNet ^ 0xFFFFFFFF);
	uint32_t binFirst = ((binSubNet & binAddr) + subValue);
	uint32_t binLast = (binFirst + (wildCardAddr)-(subValue + subValue));
	returnStruct->first = makeString(binFirst);
	returnStruct->last = makeString(binLast);
	return returnStruct;
}

int binMaker(char* string) {
	uint32_t buffer = { 0 };
	int tmp = 0;
	double tensCount = 0;
	int dotPosition = 0;
	int lastDotPosition = 0;
	int count = 0;
	for (int i = 0; dotPosition < 4; i++) {
		if (*(string + i) != '.' && *(string + i) != NULL) {
			count++;
		}
		else {
			tensCount = pow(10, (count - 1));
			for (int o = (i - count); o < i; o++) {
				tmp += ((*(string + o) - 48) * tensCount);
				tensCount /= 10;
			}
			if (tmp < 0 | tmp > 255) {
				return 0;
			}
			if (dotPosition < 3) {
				buffer += tmp;
				buffer = buffer << 8;
				dotPosition++;
				tmp = 0;
			}
			else if (dotPosition == 3) {
				buffer += tmp;
				dotPosition++;
			}
			tensCount = 0;
			lastDotPosition = count + 2;
			count = 0;
		}

	}
	if (dotPosition == 4) {
		return buffer;
	}
	return 0;
}
char* makeString(uint32_t binValue) {
	char* returnString = malloc(sizeof(char) * 21);
	uint32_t checkString = binValue;
	uint32_t andBit = 255;
	int first = (checkString & andBit);
	checkString = checkString >> 8;
	int second = (checkString & andBit);
	checkString = checkString >> 8;
	int third = (checkString & andBit);
	checkString = checkString >> 8;
	int fourth = (checkString & andBit);
	snprintf(returnString, 20, "(%d.%d.%d.%d)", fourth, third, second, first);
	return returnString;
}
