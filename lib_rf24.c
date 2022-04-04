volatile uint8_t rf24_ce_pin;
volatile uint8_t rf24_cs_pin;
volatile uint8_t rf24_mosi_pin;
volatile uint8_t rf24_miso_pin;
volatile uint8_t rf24_clk_pin;
volatile uint8_t rf24_status;

void rf24_ce(uint8_t level){if(level){PORTA |= rf24_ce_pin;}else{PORTA &= ~rf24_ce_pin;}}
void rf24_cs(uint8_t level){if(level){PORTA |= rf24_cs_pin;}else{PORTA &= ~rf24_cs_pin;}}
void rf24_mosi(uint8_t level){if(level){PORTA |= rf24_mosi_pin;}else{PORTA &= ~rf24_mosi_pin;}}
void rf24_clk(uint8_t level){if(level){PORTA |= rf24_clk_pin;}else{PORTA &= ~rf24_clk_pin;}}
uint8_t rf24_miso(void){return (PINA & rf24_miso_pin)?0xFF:0x00;}

uint8_t rf24_init(uint8_t ce_pin, uint8_t cs_pin, uint8_t mosi_pin, uint8_t miso_pin, uint8_t clk_pin) {
	rf24_ce_pin = (1 << ce_pin);
	rf24_cs_pin = (1 << cs_pin);
	rf24_mosi_pin = (1 << mosi_pin);
	rf24_miso_pin = (1 << miso_pin);
	rf24_clk_pin = (1 << clk_pin);
	DDRA |= rf24_ce_pin | rf24_cs_pin | rf24_mosi_pin | rf24_clk_pin;
	rf24_ce(0);
	rf24_cs(1);
	}

uint8_t rf24_command(uint8_t cmd) {
	uint8_t n=128;
	rf24_status = 0;
	while(n){
		rf24_mosi(cmd & n);
		rf24_clk(1);
		rf24_status |= (rf24_miso() & n);
		rf24_clk(0);
		n >>= 1;
		}
	}

uint8_t rf24_readbyte(void){
	uint8_t n=128, rx = 0;
	while(n){
		rf24_clk(1);
		rx |= (rf24_miso() & n);
		rf24_clk(0);
		n >>= 1;
		}
	return rx;
	}

uint8_t rf24_writebyte(uint8_t value){
	uint8_t n=128;
	while(n){
		rf24_mosi(value & n);
		rf24_clk(1);
		rf24_clk(0);
		n >>= 1;
		}
	}
	
uint8_t rf24_reg_read(uint8_t reg) {
	uint8_t rx = 0;
	rf24_cs(0);
	rf24_command(reg);
	rx = rf24_readbyte();
	rf24_cs(1);
	return rx;
	}

uint8_t rf24_reg_write(uint8_t reg, uint8_t value) {
	rf24_cs(0);
	rf24_command(0x20 | reg);
	rf24_writebyte(value);
	rf24_cs(1);
	return 0;
	}

void rf24_setfrequency(uint8_t f){rf24_reg_write(5,f);}
void rf24_setspeedlevel(uint8_t speed, uint8_t level){
	// speed 0=1Mbps 1=2Mbps 2=250kbps 
	// level 0=-18dbm 1=-12dbm 2=-6dbm 3=0dbm
	uint8_t buf=0;
	buf = ((speed << 3) & 0b00001000);
	if(speed == 2) buf |= 0b00100000;
	buf |= ((level < 1) & 0b00000110);
	rf24_reg_write(6,buf);
	}

void rf24_setautoretransmit(uint8_t delay, uint8_t count){
	uint8_t buf=0;
	buf = (delay << 4) | (count & 0b00001111);
	rf24_reg_write(4,buf);
	}

void rf24_setaddress(uint8_t pipe, uint8_t a5, uint8_t a4, uint8_t a3, uint8_t a2, uint8_t a1){
	rf24_cs(0);
	rf24_command(0x20 | 0x0A | pipe);
	rf24_writebyte(a1);
	if(pipe>1){
		rf24_writebyte(a2);
		rf24_writebyte(a3);
		rf24_writebyte(a4);
		rf24_writebyte(a5);
		}
	rf24_cs(1);
	if(pipe==0){
		rf24_cs(0);
		rf24_command(0x20 | 0x10);
		rf24_writebyte(a1);
		rf24_writebyte(a2);
		rf24_writebyte(a3);
		rf24_writebyte(a4);
		rf24_writebyte(a5);
		rf24_cs(1);
		}

void rf24_set_payload_length(uint8_t length){
	uint8__t n;
	for(n=0;n<6;n++){
		rf24_reg_write(0x11 + n, length);
		}
	}

void rf24_clear_status(){
	rf24_reg_write(0x07, 0b01110000); // STATUS
	}
	
void rf24_powerup_rx(){
	uint8_t CONFIG;
	rf24_cs(0);
	rf24_command(0b11100010); //flush RX
	rf24_cs(1);
	rf24_reg_write(0x07, 0); // Clear Status
	CONFIG = rf24_reg_read(0x00);
	rf24_reg_write(0x00, (CONFIG & 0b11111100) | 0b00000011);
	wait_ms(5);
	rf24_ce(1);
	}
	
void rf24_powerup_tx(){
	uint8_t CONFIG;
	rf24_reg_write(0x07, 0); // Clear Status
	CONFIG = rf24_reg_read(0x00);
	rf24_reg_write(0x00, (CONFIG & 0b11111100) | 0b00000010);
	wait_ms(5);
	rf24_ce(1);
	}

void rf24_powerdown(){
	uint8_t CONFIG;
	CONFIG = rf24_reg_read(0x00);
	rf24_reg_write(0x00, (CONFIG & 0b11111100));
	rf24_ce(0);
	}
		
uint8_t rf24_datasent(){
	rf24_cs(0);
	rf24_command(0xFF); // NOP
	rf24_cs(0);
	return (rf24_status & 0b00100000);
	}

uint8_t rf24_maxretry(){
	rf24_cs(0);
	rf24_command(0xFF); // NOP
	rf24_cs(0);
	return (rf24_status & 0b00010000);
	}

uint8_t nrf24_dataready() {
	uint8_t buf = 0;
	rf24_cs(0);
	rf24_command(0xFF); // NOP
	rf24_cs(0);
    if(rf24_status & 0b01000000){  // RX_DR 
		return 1;
		}
	buf = rf24_reg_read(0x17);     // FIFO_STATUS
	if(buf & 0b00000001){          // RX_EMPTY
		return 0;
	}else{
		return 1;
		}
	}

uint8_t rf24_get_payloadlength() {
    uint8_t buf;
    rf24_cs(0);
    rf24_command(0b01100000);           // R_RX_PL_WID
    buf = rf24_readbyte();
    rf24_cs(1);
    return buf;
	}

void rf24_send(char* msg){
	uint8_t n = 0;
	rf24_cs(0);
	rf24_command(0b10100000);           // W_TX_PAYLOAD
	while(msg[n]){
		rf24_writebyte(msg[n]);
		n++;
		}
	rf24_cs(1);
	}	

void rf24_get_message(char* msg, uint8_t length){
	uint8_t n = 0;
	rf24_cs(0);
	rf24_command(0b01100001);           // R_TX_PAYLOAD
	for(n=0;n<length;n++){
		msg[n]=rf24_readbyte();
		}
	rf24_cs(1);
	}	

void rf24_send_noack(char* msg){
	//EN_DYN_ACK ?...
	uint8_t n = 0;
	rf24_cs(0);
	rf24_command(0b10110000);           // W_TX_PAYLOAD_NO_ACK
	while(msg[n]){
		rf24_writebyte(msg[n]);
		n++;
		}
	rf24_cs(1);
	}	

void rf24_printdetails(){
	uint8_t regid=0, B;
	char s[50];
	for(regid=0;regid<0x1e;regid++){
		B = rf24_reg_read(regid);
		sprintf(s,"REG(%x%X) = 0b%i%i%i%i%i%i%i%i\r\n", regid >> 4, regid & 0x0f, B&0x80?1:0, B&0x40?1:0, B&0x20?1:0, B&0x10?1:0, B&0x08?1:0, B&0x04?1:0, B&0x02?1:0, B&0x01?1:0);
		serialcom_send(PB0, s);
		}
	sprintf(s,"DataSent = %x\r\n", rf24_datasent());
	serialcom_send(PB0, s);
	sprintf(s,"MaxRetry = %x\r\n", rf24_maxretry());
	serialcom_send(PB0, s);
	sprintf(s,"DataReady = %x\r\n", nrf24_dataready());
	serialcom_send(PB0, s);
	}