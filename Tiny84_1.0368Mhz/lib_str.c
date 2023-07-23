void strset(char *s, char *s2, int8_t pos){                                    // add s2 to s at position pos (pos is -1 means at end)
    if(pos==-1){
      while(*s) s++;
      }else{
      s+=pos;
      }
    while(*s2) *(s++)=*(s2++);
    *s=0;
    }

int32_t strtolong(char *s){
    int32_t v=0;
    int8_t sign=1;
    while(*s && (*s<48 || *s>57)) {
        if (*(s++) == 45){  
            sign = -1;
            break;
            }
        }
    while (*s>47 && *s<58){
        v = (v << 1) + (v << 3) + (*(s++) - 48);
        }
    return sign * v;
    }

void longtostr(int32_t value, char *s){
    int32_t temp=value;
    uint8_t neg=0, m=0, n=0, c;
    if(temp<0) {
        temp *=-1; neg=1;
        }
    while(temp) {
        *(s++)=48 + (temp % 10); n++;
        temp/=10;
        }
    if(neg){*(s++)=45; n++;}
    *s=0;
    s-=n; n--;
    while(m<n) {
        c=s[m];
        s[m]=s[n];
        s[n]=c;
        m++;
        n--;
        }
    }

uint8_t startwith(char *s, char *s2){
    while(*(s++)==*(s2++));
    if(*s2=='\0') return 1;
    return 0;
    }

