void stradd(char*s, char*s2){
	uint8_t n=0, m=0;
	while(s[n]) n++;
	while(s2[m]) {
		s[n]=s2[m];
		n++; m++;
		}
	s[n]=0;
	}
