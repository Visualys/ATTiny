uint8_t startwith(char* s, char* s2){
	uint8_t n=0;
	while(s[n]==s2[n]) n++;
	if(s2[n]==0){
		return 1;
	}else{
		return 0;
		}
	}
