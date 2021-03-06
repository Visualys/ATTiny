void stradd(char*s, char*s2){
    uint8_t n=0, m=0;
    while(s[n]) n++;
    while(s2[m]) {
        s[n]=s2[m];
        n++; m++;
        }
    s[n]=0;
    }

void strset(char*s, char*s2){
    uint8_t n=0;
    while(s2[n]) {
        s[n]=s2[n];
        n++;
        }
    s[n]=0;
    }

void stxt(char*s, char*s2, uint8_t position, uint8_t length){
    uint8_t n = position, nmax = n+length, m = 0;
    while(n<nmax){
        s[m]=s2[n];
        n++;m++;
        }
    s[m]=0;
    }

double puissance10(int p){
    uint8_t i;
    double v=1;
    if(p>0){
        for(i=0;i<p;i++) v*=10;
    }else if(p<0){
        for(i=0;i<-p;i++) v*=.1;
        }
    return v;
    }

uint32_t strtolong(char*s){
    uint32_t v=0;
    uint8_t start=0, end=0;
    int i;
    while((s[start]<48 || s[start]>57) && s[start]>0) start++;  
    end=start;
    while(s[end]>47 && s[end]<58) end++;
    end--;
    i=end;
    while(i>=start){
        v += (s[i]-48) * puissance10(end-i);
        i--;
        }
    return v;
    }

void longtostr(uint32_t value, char* s){
    uint8_t n=0, m=0, c;
    long temp=value;
    if(temp<0) {
        temp *=-1;
        s[n]=45;
        n++;m=1;
        }
    while(temp) {
        s[n]=48 + (temp % 10);
        temp/=10;
        n++;
        }
    s[n]=0; n--;
    while(m<n) {
        c=s[m];
        s[m]=s[n];
        s[n]=c;
        m++;
        n--;
        }
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
