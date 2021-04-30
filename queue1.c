
// ��।� ����㯠 � COM ���⠬

struct COM_q COM_que[que_wdth];

//----------------------------
void f_queue_init()
{  // init ��᫥ ����祭�� ��⠭��

//printf("\n f_queue_init");
 _fmemset( &COM_que[0].busy_ptr,0, sizeof( COM_que) );
}
//----------------------------
unsigned long int TimeClear=0;

void f_queue_clr( int port)
{  // ���⪠ ��।� ������ ����
   // ���ன�⢠ ������ ���� ������ ���� � ��।�
   // �����㦨� ���਩��� �����襭�� �����
int i;
  struct s_icp_dev *ICPl_DEV;

  for(i=COM_que[port].empt_ptr;i != COM_que[port].busy_ptr;)
  {
    if(COM_que[port].request[i] != NULL)
     {
        // �맮� �㭪権 ���਩���� �����襭�� �����
       (*COM_que[port].request[i]->answ_flt)(port);

        // ��⠭���� ����� ���ਨ � ��ࠡ�⪥ �����
        COM_que[port].request[i]->status=Req_Flt;
     }

    i=(i+1) & que_lgth1;
  }
 _fmemset( &COM_que[port].busy_ptr,0, sizeof( struct COM_q) );
    for(i=0;i<5;i++)
     last_out[i]=-1;

    for(i=0;i<icp_lst_max;i++)
    {
      ICPl_DEV=ICP_dd[i];
      ICPl_DEV->n_transaction =0;
      ICPl_DEV->n_success     =0;
      ICPl_DEV->n_timeout_error =0;
      ICPl_DEV->n_CRC_error     =0;
    }
  TimeClear=TimeStamp;
}

//==========================================================
int f_queue_put(int port, struct COM_rqst *request)
{ // ���⠭���� � ��।� ����ᮢ �� ���饭�� � COM
int itmp;
if(flag_prn)
  printf("\n f_queue_put");
  itmp= (COM_que[port].busy_ptr+1) & que_lgth1;

  if( itmp == COM_que[port].empt_ptr )
  {
if(flag_prn)
  printf(" ER1\n");

	  return 1;// ��।� ��९������
  }

   if(request->status==Req_Que)
   {
	   if(flag_prn)
       printf(" ER2\n");

	   return 1;
   }
   request->status=Req_Que;
   COM_que[port].request[COM_que[port].busy_ptr]=request;
   COM_que[port].busy_ptr=itmp;

   if(flag_prn)
  printf(" OK\n");

   return 0;
}
//==========================================================
int f_queue_chk(int port )
{  // �஢�ઠ  ��।� �� ��९�������
   // �᫨ ���� ���� ��� �����  - �����頥� 1
   // ���� 0

  int itmp;


// printf("\n f_queue_chk");
  itmp= (COM_que[port].busy_ptr+1) & que_lgth1;

  if( itmp == COM_que[port].empt_ptr ) return 0;// ��।� ��९������
  else
    return 1;
}
//==========================================================

int f_queue_free(int port, struct COM_rqst *request)
{  // ����� ���㦥�, �த������� ��।�

//printf("\n f_queue_free");

  if( COM_que[port].request[COM_que[port].empt_ptr] == request)
  {
//    COM_que[port].request[COM_que[port].empt_ptr] = NULL;
    COM_que[port].empt_ptr = (COM_que[port].empt_ptr+1) & que_lgth1;

    if(request != NULL) request->status =0;
    return 0;
  }
  else return 1;
}
//==========================================================

int f_queue_Flt(int port)
{  // ����� ���㦥� ���਩��, �த������� ��।�

    struct COM_rqst *request;

//printf("\n f_queue_Flt");

    request=COM_que[port].request[COM_que[port].empt_ptr];
//    COM_que[port].request[COM_que[port].empt_ptr] = NULL;
    COM_que[port].empt_ptr = (COM_que[port].empt_ptr+1) & que_lgth1;
    if(request != NULL) request->status =10;

    return 0;
}

//==========================================================

