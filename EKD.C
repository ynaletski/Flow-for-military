
// Emerson 800 Keypad Display  57600
int Ekd_fl=0;
int Ekd_inp_err=0;
int Ekd_out_err=0;

char ScrBufE[16][22];   /* �������㥬�� ���祭�� ���� ��࠭� */
char ScrBufE_c[16][22]; /* ����� ���ﭨ� ��࠭� */

/*
char AtrBufE[16][22];
char AtrBufE_c[16][22];
*/
int ekdkey[KEY_SIZE_e];

//--------------------
int EkdKbhit(void)
{
  return ((ekd_key_in-ekd_key_out) & KEY_SIZ1_e);
}
//--------------------
int EkdGetch(void)
{ int key;
  if(EkdKbhit())
  {
    key=ekdkey[ekd_key_out++];
    ekd_key_out &= KEY_SIZ1;
    return key;
  }
  else return -1;
}
//--------------------
int ekd_sw=0;
int fl_ekd_new=1;
int fl_ekd_new1=0;

int page_Ekd=3;      // �ਧ��� ���⪨ ��ᯫ�� ,
                     // 0x1 - ������ ��ᯫ��,
                     // 0x2-���孨� ��ᯫ��

int page_Ekd_c=-1;   /* ����� ���ﭨ� ��࠭��� Ekd  */

void f_init_ekd()
{

 int pnt;

 if(fn_Ekd != EKD_KBD) return;

 switch(Ekd_fl )
 {
  case 0:


    scom_tim[Ekd.port]=0;
    fn_Ekd=EKD_ST;
    Curs_stt=0;

    pnt=0;
//    pnt+= sprintf(&Ekd_str[pnt],"c[200B[200C[0Z[?25h#P");
    pnt+= sprintf(&Ekd_str[pnt],"c[200B[200C[0Z[?25h");
    f_clr_scr_c_Ekd(); // ���������� ���� ����� �஡�����
    Ekd_fl=1;
    break;

  case 1:
    fn_Ekd=EKD_ST;
    pnt=0;
//    pnt+= sprintf(&Ekd_str[pnt],"[1Z[?100l{0;0;8;21;0;0W[2J#P");  // ��頥� ���孨� ��ᯫ��
    pnt+= sprintf(&Ekd_str[pnt],"[1Z[?100l{0;0;8;21;0;0W[2J");  // ��頥� ���孨� ��ᯫ��
    Ekd_fl=2;
    break;

  case 2:
    fn_Ekd=EKD_ST;
     pnt=0;
//       pnt+= sprintf(&Ekd_str[pnt],"[0Z[?100l{0;0;8;21;0;0W[2J#P"); // ��頥� ������ ��ᯫ��
    pnt+= sprintf(&Ekd_str[pnt],"[0Z[?100l{0;0;8;21;0;0W[2J"); // ��頥� ������ ��ᯫ��
    Ekd_fl=-1;
    break;
   default:
    Ekd_fl=-1;
    break;
 }
}
/*----------------------------*/
// ����稭� ⠩���� � �� ��� ����祭�� �⢥� �� ������� �뢥������ � ����
unsigned int flag_fnE=0;
int cnt_flt_Ekd=0;
char P_cmd[4]="#P";
//char P_cmd[4]={0x1b,'#','P',0};
void SendToEkd()
/*
 �㭪�� � ���ࢠ��� PoolKbd_Ekd 諥� ������� �⥭��  ���ﭨ� ���������� (fn_Ekd==1).
 �� ����稨 �����, ��⠭�������� �ॡ㥬�� ��࠭��� ��ᯫ�� 'page' (fn_Ekd==2)
 ��� �뢮��� ᨬ���(�) �� ��ப�  Ekd_str � ������  ekdy, ekdx (fn_Ekd==3).
*/
{
  int itmp;
  unsigned char Cmd[n_buf_queue];
  int pnt=0;

  if( (fn_Ekd==EKD_PG ) ||
      (fn_Ekd==EKD_ST ) ||
      f_timer(Ekd.time_stamp_pool,Ekd.pool_time)  )
  {
      if(f_queue_chk(Ekd.port)==0) return;
 // ���� �� �뢮���� � ���� ���� � ��।� ����ᮢ
      if((Ekd_Rqst.status == Req_Flt)|| (Ekd_Rqst.status == Req_OK ))
       {
        cnt_flt_Ekd=0;
        goto m_new_send;
       }
  }
  return;

m_new_send:
 if(  f_timer(Ekd.time_stamp_pool,Ekd.pool_time)  )
  {
    if((fn_Ekd == EKD_PG) ||(fn_Ekd == EKD_ST))
    {
     flag_fnE++;
     if(flag_fnE & 1) goto m_sw;
    }
    else fn_Ekd=EKD_KBD;
    flag_fnE=0;
              /* ���� ����������*/
     strcpy(Cmd,P_cmd);
     Ekd_Rqst.answ_com=f_Ekd_KBD_ans;
     Ekd_Rqst.function=EKD_KBD;
     Ekd.time_stamp_pool=TimeStamp;
     LedToggle();  // ������� ᢥ⮤����� �� ������
     goto m11;
  }

m_sw:
 switch(fn_Ekd )
 {
  case EKD_PG:

        /* ��⠭���� ��࠭��� ��ᯫ��*/
           // ���⪠ ��࠭�
    //     Curs_stt=0;
           pnt=0;
        itmp=page_Ekd;
        if((page_Ekd & 3)==0) itmp=3;
        else itmp=page_Ekd;

        if(itmp & 1)
        {
           pnt+= sprintf(&Cmd[pnt],"[0Z");  //  ��⠭����� �뢮� �� ������ ��ᯫ��
           pnt+= sprintf(&Cmd[pnt],"{0;0;8;21;0;0W");  // ��६�頥� ���� �뢮�� ᨬ���� � ������ ���� �஫����
           pnt+= sprintf(&Cmd[pnt],"[m");   //�����頥� �⮡ࠦ���� � ��ଠ�쭮�� (�� �����᭮�� ����),
           pnt+= sprintf(&Cmd[pnt],"[?100l");   //�����頥� �⮡ࠦ���� � ��ଠ�쭮�� (�� �����᭮�� ����),
           pnt+= sprintf(&Cmd[pnt],"[2J");  // ��ࠥ� ���� �뢮�� (�஫����)
        }
        if(itmp & 2)
        {
           pnt+= sprintf(&Cmd[pnt],"[1Z");  //  ��⠭����� �뢮� �� ���孨� ��ᯫ��
           pnt+= sprintf(&Cmd[pnt],"{0;0;8;21;0;0W");  // ��६�頥� ���� �뢮�� ᨬ���� � ������ ���� �஫����
           pnt+= sprintf(&Cmd[pnt],"[m");   //�����頥� �⮡ࠦ���� � ��ଠ�쭮�� (�� �����᭮�� ����),
           pnt+= sprintf(&Cmd[pnt],"[?100l");   //�����頥� �⮡ࠦ���� � ��ଠ�쭮�� (�� �����᭮�� ����),
           pnt+= sprintf(&Cmd[pnt],"[2J");  // ��ࠥ� ���� �뢮�� (�஫����)
        }
//         pnt+= sprintf(&Cmd[pnt],"#P");  // ��ࠥ� ���� �뢮�� (�஫����)

           fn_Ekd=EKD_KBD;
  //       Ekd_Rqst.answ_com=f_Ekd_SetAns;
           Ekd_Rqst.answ_com=f_Ekd_KBD_ans;
           Ekd_Rqst.function=EKD_PG;
           break;
  case EKD_ST:

      /* �뢮� ��ப�  Ekd_str[] � ������  ekdy, ekdx */
           strcpy(Cmd,Ekd_str);
           fn_Ekd=EKD_KBD;
   //      Ekd_Rqst.answ_com=f_Ekd_SetAns;
           Ekd_Rqst.answ_com=f_Ekd_KBD_ans;
           Ekd_Rqst.function=EKD_ST;
           break;

   default:
        fn_Ekd=EKD_KBD;
    //  Ekd_Rqst.function=EKD_KBD;
        return;
 }
m11:
   strcpy(Ekd_Rqst.Cmd,Cmd);
   Ekd_Rqst.cmd_lgth=strlen(Cmd);
//printf("\r\nR3:Ekd[%d]=%s",Ekd_Rqst.cmd_lgth,Ekd_Rqst.Cmd);

   Ekd_Rqst.time_stamp=TimeStamp;
   Ekd.n_transaction++;

if( f_queue_put(Ekd.port, &Ekd_Rqst) != 0)  //printf("?");
   return;
}
/* ================================================== */
//-------------------------
void f_Ekd(void)
{
/*  �⮡ࠦ��� ��࠭�� ����
 � ������ � ���� ���������� ������ �� ��������� ������.
 ������ ������ �. EkdKbhit(),EkdGetch().
*/
 char *buf,*buf_c;
 char *bufa,*bufa_c;

 if(Ekd_fl>= 0)
 {
   f_init_ekd();
   goto m2;
 }
 if( fn_Ekd != EKD_KBD) goto m2;

 if((fl_ekd_new != 0) && (fl_ekd_new1==0) )
 {
 /* ���� � �⮡ࠧ��� ���������  � ᠬ��� ��砫� ���� */
   fl_ekd_new1=1;
   fl_ekd_new=0;
   ekdx=0;
   ekdy=0;
   ekd_sw=0;
 }
 if(fl_ekd_new1==0) goto m2;

 if(page_Ekd != page_Ekd_c)
  {
   f_clr_scr_c_Ekd(); // ���������� ���� ����� �஡�����
                      // �.�. 䠪��᪮� ���ﭨ� ��࠭� ��᫥ ᬥ�� ��࠭���
    f_clr_scr_c_Ekd_p(page_Ekd);
//printf("\r\n Ekd page %d",page_Ekd);
    fn_Ekd=EKD_PG;
    page_Ekd_c=page_Ekd;
    fl_ekd_new=1;
    fl_ekd_new1=0;
    goto m2;
  }

 switch(ekd_sw)
 {
 /*-------------------------*/
 // �ࠢ����� ���� ��࠭� � �뢮� � EKD �� ����稨 ��宦�����

  case 0:
mm1:
   ekdx=0;
   buf=&ScrBufE[ekdy][0];
   buf_c=&ScrBufE_c[ekdy][0];
// bufa=&AtrBufE[ekdy][0];
// bufa_c=&AtrBufE_c[ekdy][0];
   ekd_sw=2;
// if(( _fmemcmp( buf, buf_c , 21) != 0)|| ( _fmemcmp( bufa, bufa_c , 21) != 0))
   if( _fmemcmp( buf, buf_c , 21) != 0)
   { //  ������� ࠧ����
mm_out:
    f_chg_ekd(ekdy);

    _fmemcpy(buf_c,buf, 21);
    fn_Ekd=EKD_ST;
    break;
   }
  /*----------------------------*/
  case 2:
    if(++ekdy > 15)
    {
     fl_ekd_new1=0;
     ekdy=0;  ekdx=0;
     break;
    }
    goto  mm1;

  default:
       break;
 }
 m2:
 SendToEkd();
}

/*=====================*/

int fn_Ekd=0;
char Ekd_str[n_buf_queue];  // [50]

#define PoolKbd_Ekd    (250)
#define ToutAns_Ekd    50

void  f_chg_ekd(int nny)
{ // ��ॢ���� ��ப� nny  ��࠭���� ����
  // � ����� ������ EKD � ���� Ekd_str[]
  // ��ப� 0 - ������ ��ப� ���孥�� ��ᯫ��

  int pnt,Ny;

   pnt=0;
   if(nny < 8)
   {
     pnt+= sprintf(&Ekd_str[pnt],"[1Z");  //  ��⠭����� �뢮� �� ���孨� ��ᯫ��
     Ny=nny;
   }
   else
   {
     pnt+= sprintf(&Ekd_str[pnt],"[0Z");  //  ��⠭����� �뢮� �� ������ ��ᯫ��
     Ny=nny-8;
   }

//   pnt+= sprintf(&Ekd_str[pnt],"{0;%1d;1;21;0;0W",Ny);  // ��६�頥� ���� �뢮�� ᨬ���� � ������ ���� �஫����
// pnt+= sprintf(&Ekd_str[pnt],"{0;0;8;21;0;0W");  // ��६�頥� ���� �뢮�� ᨬ���� � ������ ���� �஫����
   if((Curs_y==nny)&& (Curs_stt!=0))
     pnt+= sprintf(&Ekd_str[pnt],"[?100l");  //  ������ �뢮�� � ����, 0 0 - ���孨� ���� 㣮�

   pnt+= sprintf(&Ekd_str[pnt],"[%1d;0H",Ny+1);  //  ������ �뢮�� � ����, 0 0 - ���孨� ���� 㣮�
//   pnt+= sprintf(&Ekd_str[pnt],"[1;1H");  //  ������ �뢮�� � ����, 0 0 - ���孨� ���� 㣮�
//   pnt+= sprintf(&Ekd_str[pnt],"[1;1Q");  //  ��⠭����� ������� ����
   if(Ny==7) ScrBufE[nny][20]=0;
   else ScrBufE[nny][21]=0;
// pnt+= sprintf(&Ekd_str[pnt],"%s#P",ScrBufE[nny]);
   pnt+= sprintf(&Ekd_str[pnt],"%s",ScrBufE[nny]);
   if((Curs_y==nny)&& (Curs_stt!=0))
     pnt+= sprintf(&Ekd_str[pnt],"[?100h#%d;%d;0;0H",Curs_x,Ny);
}
/* ================================================== */
void f_clr_scr_Ekd()
{  // ���⪠ ���� ��࠭�
  _fmemset( ScrBufE[0],' ',sizeof(ScrBufE)); // ���⪠ ���� ��࠭�
//_fmemset( ScrBufE[0],' ',248); // ���⪠ ���� ��࠭�
   fl_ekd_new=1;
}
/* ================================================== */
void f_clr_scr_c_Ekd()
{  // ���⪠ ����� ��࠭���� ����
   // � Ekd ���� ��।��� ��ப� �⫨�� �� �஡����
  _fmemset( ScrBufE_c[0],' ',sizeof(ScrBufE));
   fl_ekd_new=1;
}
/* ================================================== */
void f_clr_scr_Ekd_p(int page)
{  // ���⪠ ����� ��࠭���� ����
   // � Ekd ���� ��।��� ��ப� �⫨�� �� �஡����
  int itmp;
  itmp=page & 3;
  if(itmp == 0) itmp=3;
  if(itmp==3)
    _fmemset( ScrBufE[0],' ',sizeof(ScrBufE));
  else if(itmp==1)
    _fmemset( ScrBufE[8],' ',sizeof(ScrBufE)>>1 );
  else if(itmp==2)
    _fmemset( ScrBufE[0],' ',sizeof(ScrBufE)>>1 );

   fl_ekd_new=1;
}
/* ================================================== */
/* ================================================== */
void f_clr_scr_c_Ekd_p(int page)
{  // ���⪠ ����� ��࠭���� ����
   // � Ekd ���� ��।��� ��ப� �⫨�� �� �஡����
  int itmp;
  itmp=page & 3;
  if(itmp == 0) itmp=3;
  if(itmp==3)
    _fmemset( ScrBufE_c[0],' ',sizeof(ScrBufE));
  else if(itmp==1)
    _fmemset( ScrBufE_c[8],' ',sizeof(ScrBufE)>>1 );
  else if(itmp==2)
    _fmemset( ScrBufE_c[0],' ',sizeof(ScrBufE)>>1 );

   fl_ekd_new=1;
}
/* ================================================== */
void f_rev_scr_Ekd()
{  // ���������� ��࠭� (��᫥ ᬥ�� ��࠭���,���ਬ��)
   // ��뢠�� ��।��� �ᥣ� ��࠭���� ���� � Ekd
  _fmemset( ScrBufE_c[0],1,sizeof(ScrBufE)); // ���������� ���� ����� ���ᯮ��㥬� ���祭���
   fl_ekd_new=1;
}
/* ================================================== */
int x_ekd=0,y_ekd=0;
void EkdMoveToXY(int x,int y)
{
 x_ekd=x;y_ekd=y;
}
/* ================================================== */
/*
void EkdShowZone(int n_char)
{ // ��������� ᮤ�ন��� ���� �뢮��
  //  ��稭�� � ����樨 x_ekd,y_ekd ������ n_char

}
*/
/* ================================================== */
int ekdx=0,ekdy=0;

int Curs_x=0,Curs_y=0,Curs_stt=0;
void EkdShowCursor(void)
{ // ����砥� ����� � ����樨 x_ekd,y_ekd

 char Cmd[50];
  int pnt,Ny;

    Curs_x=x_ekd;
    Curs_y=y_ekd;
  pnt=0;
  if(Curs_y<8)
  {
     pnt+= sprintf(&Cmd[pnt],"[1Z");  //  ��⠭����� �뢮� �� ���孨� ��ᯫ��
     Ny=Curs_y;
  }
  else
  {
     pnt+= sprintf(&Cmd[pnt],"[0Z");  //  ��⠭����� �뢮� �� ������ ��ᯫ��
     Ny=Curs_y-8;
  }
   pnt+= sprintf(&Cmd[pnt],"[?100h#%d;%d;0;0H",Curs_x,Ny);

 //  [0Z[?100h#20;7;0;0H - ����砥� ����� �� ������ ��ᯫ�� � ����樮����� �����,
 //  �� ��᫥���� ᨬ���� ��᫥���� ��ப�,
   Curs_stt=1;
   strcpy(Ekd_Rqst_c.Cmd,Cmd);
   Ekd_Rqst_c.cmd_lgth=strlen(Cmd);
//printf("\r\nR3:Ekd[%d]=%s",Ekd_Rqst.cmd_lgth,Ekd_Rqst.Cmd);

   Ekd_Rqst_c.timeout=Ekd_Rqst.timeout;
   Ekd_Rqst_c.time_stamp=TimeStamp;
   Ekd.n_transaction++;

   f_queue_put(Ekd.port, &Ekd_Rqst_c);
   return;

}
/* ================================================== */
void EkdCursorOff(void)
{
 char Cmd[50];
  int pnt;
//  int Ny;

  pnt=0;
  if(Curs_y<8)
  {
     pnt+= sprintf(&Cmd[pnt],"[1Z");  //  ��⠭����� �뢮� �� ���孨� ��ᯫ��
//   Ny=Curs_y;
  }
  else
  {
     pnt+= sprintf(&Cmd[pnt],"[0Z");  //  ��⠭����� �뢮� �� ���孨� ��ᯫ��
//   Ny=Curs_y-8;
  }
   pnt+= sprintf(&Cmd[pnt],"[?100l");  //  ������ �뢮�� � ����, 0 0 - ���孨� ���� 㣮�


   Curs_stt=0;
   strcpy(Ekd_Rqst_c1.Cmd,Cmd);
   Ekd_Rqst_c1.cmd_lgth=strlen(Cmd);
//printf("\r\nR3:Ekd[%d]=%s",Ekd_Rqst.cmd_lgth,Ekd_Rqst.Cmd);

   Ekd_Rqst_c1.timeout=Ekd_Rqst.timeout;
   Ekd_Rqst_c1.time_stamp=TimeStamp;
   Ekd.n_transaction++;

   f_queue_put(Ekd.port, &Ekd_Rqst_c1);
   return;
}
/* ================================================== */
void EkdGotoxy(int x,int y)
{
  x_ekd=x;
  y_ekd=y;
//  EkdShowCursor();
}
/* ================================================== */
void EkdPutch(char ch)
{
   ScrBufE[y_ekd][x_ekd]=ch;
   if(x_ekd++ > 20) x_ekd=20;
   fl_ekd_new=1;
}
/* ================================================== */
void EkdPuts(char *str)
{
int i;
long int itmp;
   i=strlen(str);
   itmp=21 -x_ekd;
   if(i<itmp) itmp=i;
//printf("\n  EkdPuts( %s )",str);
  if(itmp != 0)
   _fmemcpy(&ScrBufE[y_ekd][x_ekd],str, itmp);
   x_ekd+=itmp;
   fl_ekd_new=1;
}
/* ================================================== */
char BufferEkdPrintf[128];
int  EkdPrintf(char *format , ...)
{
  va_list marker;
  va_start( marker, format);
  vsprintf(BufferEkdPrintf, format, marker);
  EkdPuts(BufferEkdPrintf);
  return (strlen(BufferEkdPrintf));
}
/* ================================================== */
void SetEkdPage(int page)
{
  page_Ekd=page;
  page_Ekd_c=-1;
}
/* ================================================== */
/* ================================================== */
//-------------------------------
struct s_icp_dev Ekd=
{
// device 1
0,                // status
3,                // port
00,               // addr
"EmKeyD",         // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0,                // channels
"B2.6",           // firmware
0,                // protocol
1,                // CRC_flag
PoolKbd_Ekd,      // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&Ekd_Rqst,        // *request
};
//-------------------------------
struct COM_rqst Ekd_Rqst={
0,                     //  ���ﭨ� �����
ffgets_com_Ekd,        //  gets_com(int ii);
f_Ekd_KBD_ans,         //  answ_com(int ii);
f_Ekd_Flt,             //  answ_flt(int ii);
"EmKeyDsp",            //  name[10]
0,                     //  n_dev
&Ekd,                  //  *ICP_dd
0,                     //  ⥪��� �㭪��
ToutAns_Ekd,           //  timeout �⢥�
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
struct COM_rqst Ekd_Rqst_c={
0,                     //  ���ﭨ� �����
ffgets_com_Ekd,        //  gets_com(int ii);
f_Ekd_KBD_ans,         //  answ_com(int ii);
f_Ekd_Flt,             //  answ_flt(int ii);
"EmKey_C",             //  name[10]
0,                     //  n_dev
&Ekd,                  //  *ICP_dd
EKD_ST,                //  ⥪��� �㭪��
ToutAns_Ekd,           //  timeout �⢥�
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
struct COM_rqst Ekd_Rqst_c1={
0,                     //  ���ﭨ� �����
ffgets_com_Ekd,        //  gets_com(int ii);
f_Ekd_KBD_ans,         //  answ_com(int ii);
f_Ekd_Flt,             //  answ_flt(int ii);
"EmKey_C1",            //  name[10]
0,                     //  n_dev
&Ekd,                  //  *ICP_dd
EKD_ST,                //  ⥪��� �㭪��
ToutAns_Ekd,           //  timeout �⢥�
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
""                     //  com[]
};
/*----------------------------*/
int f_empty_Ekd(int ii)
{
int tmp;
     if(IsCom(ii)==0)
     {
        return 0;
     }
     tmp=ReadCom(ii);
     cb_COM[ii][n_bcom[ii]]=tmp;
     if( (++n_bcom[ii]) >= n_bufcom )
      {
//      cb_COM[ii][n_bufcom-1]=0;
        n_bcom[ii]--;
  /*
        printf("\n Command buffer overflow !\n--");
        puts(cb_COM1);
  */
      }
      return 0;
}

/*----------------------------*/
int ffgets_com_Ekd( int ii)
{
//  ���� �㭪樨 �㤥� ��⠭����� � 㪠��⥫�  ff_gets_com[Port]
//  �� ��ࠡ�⪥ ����� Ekd_Rqst.
//  �㭪�� �ਭ����� ᨬ���� �� ���� COM ii  � ���� cb_COM[ii].
//  �᫨ ��諮 �६� ToutAns[ii] �� � ������ �ਥ�� ��᫥����� ᨬ����
// ��� � ������ ��।�� ������� - 䨪������ ⠩����, � � 㪠��⥫�
// �㭪樨 ��ࠡ��稪� ff_answ_com �����뢠���� ���� �㭪樨 ��� ��ࠡ�⪨
// ⠩���� ( ���� ���ன ����ᠭ � 㪠��⥫� ff_answ_flt).
//

/* ii - ����� COM   1...4 */
 char tmp;
 int i;
struct COM_rqst *request;

m1:
      if(IsCom(ii)==0)
      {
        if(!IsTxBufEmpty(ii)) time_snd[ii] = TimeStamp;
  //    if(!IsCom3OutBufEmpty()) time_snd[ii] = TimeStamp;

        if( f_timer(time_snd[ii],ToutAns[ii]))
        { // ��⥪ ⠩���� �ਥ�� �⢥� �� �������
  /*
          if(n_bcom[ii])
          {
             printf("\n\rRcv=%d==",n_bcom[ii]);
             for(i=0;i<n_bcom[ii];i++)
               putch(cb_COM[ii][i]);
          }
  */
//    printf("\nt_send= %ld",time_snd[ii]-request->time_stamp );
//    printf("\nt_cur= %ld",TimeStamp-request->time_stamp );

          return 1;
        }
        return 0;
      }
     tmp=ReadCom(ii);
     cb_COM[ii][n_bcom[ii]]=tmp;
     if( (++n_bcom[ii]) >= n_bufcom )
      {
 //       cb_COM[ii][n_bufcom-1]=0;
        n_bcom[ii]--;
  /*
        printf("\n Command buffer overflow !\n--");
        puts(cb_COM1);
  */
      }
   time_snd[ii]= TimeStamp;
   goto m1;
}
/* ---------------------------------------- */

void f_ekd_err()
{
  page_Ekd_c=-1;
  Ekd_fl=0;
  Curs_stt=0;
  if(sw_fst_ekd != 0) sw_fst_ekd=fst_ESC;
}
/* ---------------------------------------- */
int ekd_key_in=0,ekd_key_out=0;

void f_Ekd_KBD_ans(int ii)
{
/* �� ����稨 ������� ������, ������ ���� ������ � ����楢�� ����  ekdkey[]
   ekd_key_out - 㪠������ �� �� �ᯮ�짮����� ᨬ���
   ekd_key_in  - 㪠�뢠�� �� ������ �祩�� ��� ����� ᨬ����
   ekd_key_out==ekd_key_in - ���� ����
*/

 struct COM_rqst *request;
 char *Result;
 int j,key,nextidx,i;
 int count;
 int flag_ok;

  Result=cb_COM[ii];        // �������
  count=n_bcom[ii];  // ����� �������
  request=COM_que[ii].request[COM_que[ii].empt_ptr];

  flag_ok=0;

  for(j=0;j<count;j++)
  {
     if(Result[j] != 0x1b)
     {
       goto m1;
     }

     if((Result[j+1]=='#')&& (Result[j+2]=='P'))
      {
       flag_ok=1;
       j+=2;
       continue;
      }
     else if((Result[j+1]=='[') && (Result[j+2]=='1') && (Result[j+4]==0x7e) )
     {
      if(Result[j+3]=='1')
      {
       key = F1;
       j+=4;
       goto m2;
      }
      else if(Result[j+3]=='2')
      {
       key = F2;
       j+=4;
       goto m2;
      }
      else if(Result[j+3]=='3')
      {
       key = F3;
       j+=4;
       goto m2;
      }
      else if(Result[j+3]=='4')
      {
       key = Prev;
       j+=4;
       goto m2;
      }
      else if(Result[j+3]=='5')
      {
       key = Next;
       j+=4;
       goto m2;
      }
     }
     else if(Result[j+1]=='[')
     {
      if(Result[j+2]=='A')
      {
       key = ARR_UP;
       j+=2;
       goto m2;
      }
      else if(Result[j+2]=='B')
      {
       key = ARR_DN;
       j+=2;
       goto m2;
      }
      if(Result[j+2]=='C')
      {
       key = ARR_RGT;
       j+=2;
       goto m2;
      }
      if(Result[j+2]=='D')
      {
       key = ARR_LFT;
       j+=2;
       goto m2;
      }
     }
     else
      {
  /*
           printf("\n\r unknown ,count=%d",count);

           printf("\n\rRcv=%d==",n_bcom[ii]);
            for(i=0;i<n_bcom[ii];i++)
               putch(cb_COM[ii][i]);
  */
 goto m_err;
      }

m1:
          key=Result[j];
          if(key=='#')
          {
     /*
           printf("\n\r #,count=%d",count);

           printf("\n\rRcv=%d==",n_bcom[ii]);
            for(i=0;i<n_bcom[ii];i++)
               putch(cb_COM[ii][i]);
     */
             goto m_err;
          }
m2:  // ���� ᨬ���� key � ����

   //   printf("\n\r%c",key);

          nextidx=(ekd_key_in+1) & KEY_SIZ1;
          if(nextidx != ekd_key_out)
          {
            ekdkey[ekd_key_in]=key;
            ekd_key_in=nextidx;
          }
  }
  if(request->function==EKD_KBD)
  {
   if(flag_ok==1)
    {
       Ekd.n_success++;
       Ekd_inp_err=0;
    }
    else
    {
m_err:
      Ekd_inp_err++;
      Ekd.n_timeout_error++;
      if(Ekd_inp_err>=Ekd_max_inp_err)
      {
        Ekd_inp_err=Ekd_max_inp_err-3;
        f_ekd_err();
    //    f_icp_error(&Ekd,RD_ERR );
      }
    }
  }
  else
      Ekd.n_success++;

  f_queue_free(ii,request);
//  f_queue_free(ii,&Ekd_Rqst);
  ff_gets_com[ii]=f_empty_Ekd;
}
/* ---------------------------------------- */
/* ---------------------------------------- */
void f_Ekd_Flt(int ii)
{
// �ந��襫 ⠩���� �� �ਥ�� �⢥�.
// ���⪠ ��।� � ����⨪�.
 struct COM_rqst *request;

 Ekd.time_stamp_rcv=0;
 Ekd.n_timeout_error++;
 /*
 Ekd_inp_err++;
   if(Ekd_inp_err>=Ekd_max_inp_err)
   {
     Ekd_inp_err=Ekd_max_inp_err;
 //    f_icp_error(&Ekd,RD_ERR );
   }
 */
 // f_ekd_err();
// printf("\n\r'f_Ekd_Flt'");
// f_queue_free(ii,&Ekd_Rqst);
 request=COM_que[ii].request[COM_que[ii].empt_ptr];
 f_queue_free(ii,request);
 Ekd.n_CRC_error++;
 Ekd_Rqst.status=Req_Flt;
}
/* ---------------------------------------- */
void f_Ekd_SetAns(int ii)
{
 // ��ࠡ�⪠ �⢥� �� Ekd �� ������� ��������� ��࠭���
 // � ����� ᨬ����(��).������� ����⢨� �� �ॡ����.
 // �᢮�������� ��।� � ����⨪� १����.

//if(cb_COM[ii][0]=='!')
   {
     Ekd.n_success++;
     Ekd_out_err=0;
     f_queue_free(ii,&Ekd_Rqst);
   }
//   printf("\n\r'f_Ekd_SetAns'");
/*
 else
   {
    f_queue_free(ii,&Ekd_Rqst);
    Ekd_Rqst.status=Req_Flt;
   }
*/
}
/* ---------------------------------------- */

int  fst_n_ekd=1;

char fst_str_ekd[22];
int  sw_fst_ekd=0;
int  x_fst=0;
int  y_fst=0;

void f_start_fst( int nn)
{
          fst_n_ekd=nn;
          sw_fst_ekd=1;
          x_fst=x_ekd;
          y_fst=y_ekd;
}
//-------------------------------------
int fstinEkd()
/* �뢮��� ᮤ�ন��� ��ப� fst_str_ekd  �� ��࠭ Ekd  � ।������ �� ,
   १���� � ⮩ �� ��ப�.

   fst_str_ekd - ��ப�;
   fst_n_ekd ����� ���� ।���஢����.

   �ਬ�� �ᯮ�짮����� :

     sprintf( fst_str_ekd,"%f",num_float);
     EkdGotoxy(4,6);
     f_start_fst(2);

    ���� �����襭 , �᫨ sw_fst_ekd==10;
*/
{
 int i,itmp,itmp1;
 int  key;

 switch(sw_fst_ekd)
 {
 case 0: break;
 case fst_OK: break;   //  ����� 'Enter' - ���� ������ �����襭
 case fst_ESC: break;  // ����� 'ESC' - ��室 ��� ����� ������
 case 1:
 m1:
  fst_in_n=0;
//if ( fst_str_ekd[0] )
      {
        strcpy(str_tmp,fst_str_ekd);

        EkdGotoxy(x_fst,y_fst);
        EkdShowCursor();
        EkdPuts(fst_str_ekd);
//      if(x_ekd>(x_fst+fst_n_ekd-1)) x_ekd=x_fst+fst_n_ekd-1;
//      EkdShowZone(fst_n_ekd);
      }
      sw_fst_ekd=2;

 case 2:

// Curs_stt=1;
   if( EkdKbhit()) key= EkdGetch();
   else break;

   if( key == ENTER )
   {
    sw_fst_ekd=fst_OK;
    EkdCursorOff();
    break;
   }
   else if( key == Prev )
   {
    sw_fst_ekd=fst_ESC;
    EkdCursorOff();
    break;
   }

 /* ��࠭�� �� ���� ��ப� */

     itmp=strlen(fst_str_ekd);
     if(itmp<fst_n_ekd) itmp=fst_n_ekd;

     EkdGotoxy(x_fst,y_fst);
     for(i=0;i<itmp;i++) EkdPutch(' ');
     EkdGotoxy(x_fst,y_fst);
     EkdShowCursor();

     sw_fst_ekd=3;
     fst_in_n=0;
     goto m3;
 case 3:
   if( EkdKbhit()) key= EkdGetch();
   else break;
 m3:
   if( isdigit(key)  || (key == '.') || ( ( key == '-') && (fst_in_n==0))  )
   {
    if(fst_in_n <  (fst_n_ekd))
    {
     fst_str_ekd[fst_in_n]=key;
     EkdGotoxy(x_fst+fst_in_n,y_fst);
     EkdPutch(key);
     fst_in_n+=1;
     if(fst_in_n < fst_n_ekd)
       EkdShowCursor();
     else
     {
      EkdGotoxy(x_fst+fst_in_n-1,y_fst);
      EkdShowCursor();
     }
    }
    else
    {
      fst_str_ekd[fst_in_n-1]=key;
      EkdGotoxy(x_fst+fst_in_n-1,y_fst);
      EkdShowCursor();
      EkdPutch(key);
    }
   }
   else if((key == DEL) && (fst_in_n > 0)  )
   {
    EkdGotoxy(x_fst+fst_in_n-1,y_fst);
    EkdPutch(' ');
    fst_in_n--;
    EkdGotoxy(x_fst+fst_in_n,y_fst);
    EkdShowCursor();
   }
   else if(key == ENTER)
   {
    fst_str_ekd[fst_in_n]=0;
    sw_fst_ekd=fst_OK;
    EkdCursorOff();
    break;
   }
   else if(key == Prev)
   {
     strcpy(fst_str_ekd,str_tmp);
     itmp=strlen(fst_str_ekd);
     if(itmp<fst_n_ekd) itmp=fst_n_ekd;
     x_ekd=x_fst;
     for(i=0;i<itmp;i++) EkdPutch(' ');
     goto m1;
   }
 }
 return sw_fst_ekd;
}
/*================================================================*/
/* ---------------------------------------- */
int sw_ekd=0;
int f_menu_Ekd()
{
  time_t ltime1;
  time_t ltime2;
  int year,month,day,hour,min,sec;
  int key;
  char str[31];
  int i,j,k;
  struct s_icp_dev *ICPl_DEV;
struct arch_str *ar_str_ptr;
long int ltmp;
 struct tm *tdat;


   GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
   EkdMoveToXY(0,15);
   EkdPrintf(" %02d.%02d.%02d   %02d:%02d:%02d" ,day,month,year-2000 ,hour,min,sec);


//   s_MVD[0].Press=( ADC[0] + analog_offset[0]) * analog_scale[0];
   sw_mmi_slv= sw_ekd;
   if((i=fstinEkd())==fst_OK)
   {  // ������� �᫮��� ���祭��
     key=DATA;  // �ਧ��� ����砭�� ����� ��ப�
     sw_fst_ekd=0;
   }
   else if(i==fst_ESC)
   {
     key=DATA_ESC;  // �ਧ��� ����砭�� ����� ��ப�
     sw_fst_ekd=0;
   }
   else if( EkdKbhit())
   {  // ���� ����㯨�襣� ᨬ���� �� ����������
    key=EkdGetch();
   }
   else if(sw_fst_ekd==0)
   {
    key=PASS; // �ਧ��� 宫��⮣� ��室� (��� ���������� ᨬ����)
              // �ᯮ������ ��� ������樨 ����������� ���祭��
   }
   else return 0;

   switch(sw_ekd)
    {
     case 0: /*���������*/

//---------------------------------
       if(key==F2)
        {
       m_0f2:
          EkdGotoxy(1,1);
//        f_clr_scr_Ekd();
          f_clr_scr_Ekd_p(2);
          EkdPuts("Input page number:" );
          sprintf( fst_str_ekd,"%d",num_page);
//        EkdGotoxy(10,3);
          EkdGotoxy(6,3);
          f_start_fst(8);
          SetEkdPage(2); // ���� ���祭��
          break;
        }
       else if(key==DATA)
        {
          sscanf(fst_str_ekd,"%d",&num_page);
          if(num_page<0) { num_page=0; goto m_0f2;}
          else if(num_page > 63) {num_page=63; goto m_0f2;}

          SetEkdPage((int)num_page);
          sw_ekd=100;
          f_clr_scr_Ekd_p(2);
          break;
        }
       else if(key==DATA_ESC)
        {
          goto m_main;
        }
        else
//---------------------------------
        if (key==Sht_ESC)
        {  // ���⪠ �訡��

          f_clr_scr_Ekd();
          SetEkdPage(EmptPage);
          sw_ekd=26;
          f_cl_error();
          EkdMoveToXY(5,2);    EkdPuts("   Errors erased   ");
          EkdMoveToXY(0,4);    EkdPuts(" Enter - continue ");

        }
        else if (key==Sht_F1)
        {  // �ᯥ�⪠ �訡��

          f_clr_scr_Ekd();
          SetEkdPage(EmptPage);
          sw_ekd=26;
          f_disp_error();
        }
        else if(key==Sht_F2)
        {  // MVD zeroing

          f_clr_scr_Ekd();
          SetEkdPage(EmptPage);

          if(f_reset_zer_MVD() == 0)
          {
          EkdMoveToXY(0,3);
          EkdPrintf(" Error of start zeroing");
          f_wr_evt(evt_MVD_CL_BS2);
  m_SF2:
          EkdMoveToXY(0,6); EkdPuts("Prev,ENTER  Return");
          sw_ekd=-1;
          break;
          }
          f_wr_evt(evt_MVD_CL_strt);
          sw_ekd=18;
          zero_time=TimeStamp;
          fl_zeroing=1;
          break;
        }
       else if((key==Prev) || (key==ENTER))
       {
m0_f1:
main_menu:
          f_clr_scr_Ekd();
          SetEkdPage(EmptPage);
          f_prepare_t(&adate00);
          sw_ekd=120;
          PassW=0;

            EkdMoveToXY(0,0);    EkdPuts("     Menu ");
            EkdMoveToXY(0,1);    EkdPuts("1  Delivery ");
            EkdMoveToXY(0,2);    EkdPuts("2  Counter");
            EkdMoveToXY(0,3);    EkdPuts("3  Jurnal");
            EkdMoveToXY(0,4);    EkdPuts("4  Sevice");
            EkdMoveToXY(0,5);    EkdPuts("5  Flowmeter");
            EkdMoveToXY(0,6);    EkdPuts("Prev  Return");

          break;
       }
       else if(key==PASS)
        {  // �⮡ࠦ���� ���� � �६���
         if((FL_err|CRC_err) == 0)
         {
          EkdMoveToXY( 9,3);  EkdPrintf("%9.2f kg",s_MVD[0].MassI);
          EkdMoveToXY( 9,4);
      //  if(I7017C[0].status)
          {
            EkdPrintf("%9.3f MPa       ",s_MVD[0].Press);
            EkdMoveToXY( 9,5);
          }
          EkdPrintf("%9.2f C ",s_MVD[0].TempR);
         // EkdMoveToXY(18,4);
    //    f_prn_comps();
         }
         GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
         EkdMoveToXY(0,7);
         EkdPrintf(" %02d.%02d.%02d   %02d:%02d:%02d" ,day,month,year-2000 ,hour,min,sec);
/*
         GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
         EkdMoveToXY(0,15);
         EkdPrintf(" %02d.%02d.%02d   %02d:%02d:%02d" ,day,month,year-2000 ,hour,min,sec);
*/
        }
        break;
 /*========================================*/
 /*========================================*/
    case 120:  // �᭮���� ����

       if((key==Prev)||(key==ENTER))    /* ���室 � ���� */
       {
           sw_ekd=0;
           SetEkdPage(ZeroPage);
           f_clr_scr_Ekd();
           f_prn_beginEkd();
           break;
       }
/*================================================================*/
       default:
       if((key==Prev)||(key==ENTER))  /* ���室 �� ��砫��� ��࠭��� */
       {
 m_main:
           SetEkdPage(ZeroPage);
           sw_ekd=0;
           f_clr_scr_Ekd();
           f_prn_beginEkd();
           break;
       }
       break;
    }
    return 0;
}
/*-----------------------------------------*/
void  f_prn_beginEkd()
{
//  if(CRC_err) f_prn_CRC_error();
//  else if(FL_err) f_prn_error();
//  else
  {
    f_clr_scr_Ekd();
            //  EkdPrintf("                              ");
EkdMoveToXY(0,0);  EkdPrintf(" FACOM Technologies");
EkdMoveToXY(0,1);  EkdPrintf("   Oil flow check");
            //             "  Counters:    "
EkdMoveToXY(0,3) ; EkdPrintf("Mass       :    ");
EkdMoveToXY(0,4) ; EkdPrintf("Pressure   :    ");
EkdMoveToXY(0,5) ; EkdPrintf("Temperat.  :    ");

//EkdMoveToXY(0,4); EkdPrintf("        |         |          ");
//EkdMoveToXY(0,5); EkdPrintf("        |         |          ");
//EkdMoveToXY(4,6); EkdPrintf("%02d.%02d.%02d   %02d:%02d:%02d" ,day,month,year-2000 ,hour,min,sec);
  }
}
//-------------------------------------
//-------------------------------------
