void stradd(char*s, char*s2){
	uint8_t n=0, m=0;
	while(s[n]) n++;
	while(s2[m]) {
		s[n]=s2[m];
		n++; m++;
		}
	s[n]=0;
	}

double expon10(int p){
	uint8_t i;
	double v=1;
	if(p>0){
		for(i=0;i<p;i++) v*=10;
	}else if(p<0){
		for(i=0;i<-p;i++) v*=.1;
		}
	return v;
	}

long strtoint(char*s){
	long v=0;
	uint8_t start=0, end=0, i;
	while((s[start]<48 || s[start]>57) && s[start]>0) start++;
	end=start;
	while(s[end]>47 && s[end]<58) end++;
	end--;
	i=end;
	while(i>=start){
		v += (s[i]-48) * expon10(end-i);
		i--;
		}
	return v;
	}

uint8_t startwith(char* s, char* s2){
	uint8_t n=0;
	while(s[n]==s2[n]) n++;
	if(s2[n]==0){
		return 1;
	}else{
		return 0;
		}
	}

