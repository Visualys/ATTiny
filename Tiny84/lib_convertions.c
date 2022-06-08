void stradd(char*s, char*s2){
	uint8_t n=0, m=0;
	while(s[n]) n++;
	while(s2[m]) {
		s[n]=s2[m];
		n++; m++;
		}
	s[n]=0;
	}

int strtoint(char*s){
	int v=0, i;
	uint8_t start=0, end=0;
	while((s[start]<48 || s[start]>57) && s[start]>0) start++;
	end=start;
	while(s[end]>47 && s[end]<58) end++;
	end--;
	i=end;
	while(i>=start){
		v += (s[i]-48) * pow(10,(end-i));
		i--;
		}
	return v;
	}
