
#define READ_all_hex 1
#define READ_all_dat 2

#define I7017_max_inp_err 10

void f_7017C(int ii);
void f_7017_rd_all_hex(int ii);
void f_7017_Flt(int ii);

extern float ADC[32];
extern float ADC_c[32];
extern float ADC_c4[4];

extern struct s_icp_dev I7017C[];
extern struct COM_rqst I7017_Rqst[];

extern long int dat_i[];
extern long int Lvl_7017[12];

extern unsigned int INP_7017;
extern float P_inp;

//15.02.2021 YN
extern float P_before;
extern float P_after;


