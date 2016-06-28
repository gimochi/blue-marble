#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>


struct player_info  //����ü ����
{
	int shuttle; //��Ʋ �̿�� �����ִ�
	int money; //��
	int land_possess; //���� �� ���
	int count;


};

struct land_info
{
	int land_price;
	int building;// ���� 3��
	int belongs_who;

};

struct player_info players[3] = { { 0, 200, 0, 0, }, { 0, 200, 0, 0, }, { 0, 200, 0, 0, } }; //����ü ���� ���� [3]�� �÷��̾� ���ڸ� �ǹ��Ѵ�.
struct land_info lands[28] = { { 5, 0, 0 }, { 8, 0, 0 }, { 0, 0, 0 }, { 12, 0, 0 }, { 20, 0, 0 }, { 10, 0, 0 }, { 10, 0, 0 },  //�� ����
{ 12, 0, 0 }, { 12, 0, 0 }, { 14, 0, 0 }, { 16, 0, 0 }, { 20, 0, 0 }, { 18, 0, 0 }, { 18, 0, 0 }, //������
{ 20, 0, 0 }, { 22, 0, 0 }, { 23, 0, 0 }, { 24, 0, 0 }, { 24, 0, 0 }, { 50, 0, 0 }, { 26, 0, 0 }, //�Ʒ���
{ 28, 0, 0 }, { 30, 0, 0 }, { 30, 0, 0 }, { 45, 0, 0 }, { 32, 0, 0 }, { 32, 0, 0 }, { 35, 0, 0 } }; //���� ;;;;;;;;;;���� ���� 28��, �̺�Ʈĭ ����!
struct land_info lands_early[28] = { { 5, 0, 0 }, { 8, 0, 0 }, { 0, 0, 0 }, { 12, 0, 0 }, { 20, 0, 0 }, { 10, 0, 0 }, { 10, 0, 0 },  //�� ����
{ 12, 0, 0 }, { 12, 0, 0 }, { 14, 0, 0 }, { 16, 0, 0 }, { 20, 0, 0 }, { 18, 0, 0 }, { 18, 0, 0 }, //������
{ 20, 0, 0 }, { 22, 0, 0 }, { 23, 0, 0 }, { 24, 0, 0 }, { 24, 0, 0 }, { 50, 0, 0 }, { 26, 0, 0 }, //�Ʒ���
{ 28, 0, 0 }, { 30, 0, 0 }, { 30, 0, 0 }, { 45, 0, 0 }, { 32, 0, 0 }, { 32, 0, 0 }, { 35, 0, 0 } };

int want_to_go;//������� ����� ��ȣ(1~28)�� ������ ����
int playersnum;//���� ����
int a[3] = { 1, 1, 1 };//a�� ���� �������������� ���ʼ��������� ����ִ����˷��ִ°�
int coordinate[3][2] = { 0 };//coordinate[�÷��̾��][0=x,1=y]
int wait[3] = { 0 };//����Ű�� �迭
int dead = 100;
int dead_cnt = 0;
int land_pos[28][2] = { { 16, 5 }, { 28, 5 }, { 40, 5 }, { 52, 5 }, { 64, 5 }, { 76, 5 }, { 88, 5 },
{ 100, 11 }, { 100, 17 }, { 100, 23 }, { 100, 29 }, { 100, 35 }, { 100, 41 }, { 100, 47 },
{ 88, 53 }, { 76, 53 }, { 64, 53 }, { 52, 53 }, { 40, 53 }, { 28, 53 }, { 16, 53 },
{ 4, 47 }, { 4, 41 }, { 4, 35 }, { 4, 29 }, { 4, 23 }, { 4, 17 }, { 4, 11 } }; //�� ��ǥ





void settextcolor(int color, int bgcolor)//���� �ִ� �Լ�
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4 | color));

}

void gotoxy(int x, int y)//��ǥ�̵� �Լ�
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//////////////////////Ʋ/////////////////////////////////

void BOARD();//������ ��� �Լ� ����
void BOARD_Status();//�������ͽ� ȭ�� ��� 

//////////////////////Ʋ/////////////////////////////////

///////////////////�� �����̱�///////////////////////////

void player_move(int i, int random);//�� �����̴� �Լ�
int dice(int i);//�ֻ��� �Լ�

///////////////////�� �����̱�///////////////////////////

////////////////////���й���/////////////////////////////
void shuttle(int want_to_go, int i);
///////////////////���й���//////////////////////////////

/////////////////////�� ����/////////////////////////////

/////////////////////�� ����//////////////////////////////

/////////////////////Ȳ�ݿ���////////////////////////////
void golden_key(int i);

void output_delete(char c);
/////////////////////Ȳ�ݿ���////////////////////////////

/////////////////////�̺�Ʈ ĭ ����////////////////////
void clear_event();
/////////////////////�̺�Ʈ ĭ ����////////////////////

/////////////////////�÷��̾� ��Ȳ��/////////////////////
void show_playerstat(int i);
/////////////////////�÷��̾� ��Ȳ��/////////////////////

/////////////////////�����, �ǹ�����////////////////////
void structure(int i);
////////////////////�����, �ǹ�����.////////////////////

/////////////////////����////////////////////////////////
void blackout(int i);
/////////////////////����////////////////////////////////

//////////////////////�нǹ� ����////////////////////////
void lost_and_found(int i);
//////////////////////�нǹ� ����////////////////////////



void take_over(int a, int b);//(�÷��̾�ѹ�,����ȣ)
////////////////////////�μ��μ��μ�///////////////////
void sell_the_land(int a, int b);//////////////���������Ⱦƾ���//////////////
void bankrup(int a);/////////�Ļ�

int main()
{
	int i = 0, random;//playersnum �÷��̾�� b�� �װ� �����ٿ����Ѿ�� �����ٷ� ������ üũ���ִ°� rest�� �����ٿ��������̰���ĭ���� �����ٷ� ������ random�� �ֻ������޾Ƽ��־�,j�� �ϴ� ����, g�� �����Է¹޴°�


	///////////////////////////////////////////////���� ���ۺκ�
	system("mode con cols=160 lines=100");

	gotoxy(25, 15);
	puts("���� �� �ҷ����� ��"); Sleep(500);
	gotoxy(25, 15);
	puts("���� �� �ҷ����� ��."); Sleep(500);
	gotoxy(25, 15);
	puts("���� �� �ҷ����� ��.."); Sleep(500);
	gotoxy(25, 15);
	puts("���� �� �ҷ����� ��..."); Sleep(500);
	gotoxy(25, 15);
	puts("���� �� �ҷ����� ��...."); Sleep(500);
	gotoxy(25, 15);
	puts("���� �� �ҷ����� ��....."); Sleep(500);


	system("cls");//�ҷ����� �� �� �����

	gotoxy(25, 15);
	puts("�ҷ����� �Ϸ�!");
	Sleep(3000);///3�� ����

	system("cls");//�ε� ȭ�� �� �����


	/////////////////////////////////////////////////���ӽ��ۺκ�

START:
	gotoxy(40, 20);
	printf("���� �ο� [ �ּ� 2�� | �ִ� 3�� ] : ");//�����ο����޾ƾ���
	gotoxy(44, 22);
	printf("[2 �Ǵ� 3 �Է� �� Enter!]");
	gotoxy(76, 20);
	scanf_s("%d", &playersnum); 	//�޾Ҿ�!

	if (playersnum == 1)
	{
		gotoxy(0, 0);
		puts("MMMBMBMBMBMBMBMBMBMBMBMBMBMBMMMBMBMBMBMBMMMBMBMBMBMBMBMBMMMMMBMBMBMBMBMBMBMBMBMBKrvuxBMBBBMBMBMMMBMB"); Sleep(50);
		puts("BMBMBMBMBMBMBMBMBMBMBMBMBBBMBMBMBMBBBMBMBMBMBMBMBBBMBMBMBMBMBMBMBMBMBBBMBMBMBOzi    viMMBMBMBMBMBMBM"); Sleep(50);
		puts("MBMBMBMBMBBBBBMBMBMBMBMBMBMBBBMBMBMBMBMBMBMBMBMBMBMBMBMBMBMBMBMBMBMBMBMBMSi  c    u ci GMBMBBBBBMBMB"); Sleep(50);
		puts("MBBMBMBMBMBMBMBMBMBMBMBMBBBMBMBMBMBMBMBMBMBMBMBMBMBMBMBMBMBBBBBBBREzJrri     SUiS Biii  SMBMBBBMBMBM"); Sleep(50);
		puts("MBMBMBMBMBMBMBMBMBMBMBBBMBMBMBMBMBBBMBMBMBMBMBMBMBMBMBBBMBMBMBiioZv     xBv  FHiS MJiL i JMBMBMBBBMB"); Sleep(50);
		puts("BMBBBMBMBMBMBMBMBBBBBMBMBMBMBMBBBMBMBMBMBBBMBMBMBMBMBMBMBMBMBO iBMOv    JBo izpiX RD u ii LMBBBMBMBM"); Sleep(50);
		puts("MBMBMBMBMBMBMBBBMBMBMBBBMBMBMBMBMBBBBBMBMBMBMBMBBBMBMBMBBBMBMB iMBDr    iBX  oKiI UB S iii rBBMBBBMB"); Sleep(50);
		puts("BMBMBMBMBMBMBBBMBMBBBMBMBMBBBBBMBMBMBMBMBMBMBMBMBMBMBBBMBMBBBM  RMMv    iMR  LErxirM u  ii  iBBMBMBM"); Sleep(50);
		puts("MBMBMBMBMBMBMBMBMBBBMBMBMBMBBBMBMBMBBBBBMBMBBBMBMBMBMBMEovii W  OBMx     BM  vKxrv B Li iiii iBBMBMB"); Sleep(50);
		puts("BMBMBBBMBBBMBMBMBMBMBMBMBMBMBMBMBMBBBMBMBMBMBMBMBZori     ir    OMBJ     BB  iKIio Miir iiiii iMBMBM"); Sleep(50);
		puts("MBMBMBMBMBMBBBMBMBBBMBMBMBMBMBMBMBMBMBMBMBMMHxi        iiicr    EBBF     BMi iSPiZ BJiL iiiiii iBBMB"); Sleep(50);
		puts("BMBBBMBMBMBMBBBMBMBMBMBMBBBMBMBMBMBMBMWIvi       iiiriiiiiJi vr PMBF     GBi iFPiK RO o iiiii   BMBM"); Sleep(50);
		puts("MBBBMBMBBBBBBBBBMBMBBBMBMBMBBBBBMZor            ii    iiiiJ  Os IBBH     FMv iuZiP UB o ii i    RBMB"); Sleep(50);
		puts("BMBBBMBBBMBMBMBBBMBMBMBMBMMFJii       i      ii ii    i irs  Mu JBBS     vBx  xHiJiiB x  i      FMBM"); Sleep(50);
		puts("MBMBMBMBBBBBMBMBMBMBMBMBBB     ii       i      iii i iii ui  BJ vBMHi    iMH  vSrvi M ri i      iBMB"); Sleep(50);
		puts("MMBMBMBMBMBMBMBBBMBMBMBBBJ    ii              iii iiiiiiiIi iMF rRBFi    iBK  iuLic Biii i       BBM"); Sleep(50);
		puts("MBMBMBMBMBBBMBMBMBMBMBMBB    ii             iii    i    iF  zBX iWRFi     RD  iLsiJ Or r i       BMB"); Sleep(50);
		puts("BMBMBMBBBMBMBMBMBBBMBMBMi    ii            ii     i     Jx  BBB iHWui     ZG  ivziS KuiI         RBM"); Sleep(50);
		puts("MBMBBBMBMBBBMBBBMBBBMBBS     iiirii ii ivLLi   iiiii   iSr  MBI iSZUc     ZRiiroorIioSiS         HMB"); Sleep(50);
		puts("MMBMBMBMBMBBBMBMBMBMBMB   iiiiiiviriiiriii    iiiiiiiriiEi     ioXZUo     zXi  ic v iF r         vBM"); Sleep(50);
		puts("MBMBMBMBBBMBBBBBMBMBMBi iiiiiiii iii          iiii  ii Lpi      iLJLi     rSi  iviri H iii       iMB"); Sleep(50);
		puts("BMBMBMBMBMBMBBBMBMBMBS iiiiiiiiiiii                    PS   i   ivuvr     ioi  iriii I iiii       BM"); Sleep(50);
		puts("MBBBMBBBMBMBBBMBMr  ii i                               Bv  i i   vLvi     iJi  iiiir Ji i         MB"); Sleep(50);
		puts("BMBMBMBMBMBMBMBMG   r                iiii             iMi i i i  rLri     ivr  iii r vi r i       SM"); Sleep(50);
		puts("MBMBMBMBMBMBMBMB   v         i                        FR   i i   irri      vr  iir v ii r         iB"); Sleep(50);
		puts("BMBMBMBMBBBMBMBi  ii                                  BJ  i i    irii      ir   ir r ir r          M"); Sleep(50);
		puts("MBMBMBMBMBMBMBD   i                                  iMi     i   iiii      iii  ii i  i ii         B"); Sleep(50);
		puts("BMBMBMBMBMBBBM   iviiiiiiiiiiiiivvLcLcLLLLscsLxLJJIISSS         iiiii      ii   ii i  i ii         M"); Sleep(50);
		puts("MBMBBBMBMBBBBr   i rJvirvvrrriiiivvLvLLxLLiiiiirrcvssUi  iiiii  ii i       iii   iiii i  i         M"); Sleep(50);
		puts("BMBMBMBMBMBMR      BBMBMBMBMBBBMU         sxLrrrvvvrvrrrooUISvi iiiii      iii  iii i i  i         z"); Sleep(50);
		puts("MBBBMBMBBBMBMBMBDi  iBBBMBBBMBBBP         MBMBMBMBMBMBL irvJziiiiiiiii     iii   ii i ii i         i"); Sleep(50);
		puts("BMBMBMBMBROHHxsxJi    i iii iivL          iZocriBMBMBMOi          iii      iri  iii i ii i         i"); Sleep(50);
		puts("RZLriiiiiiiiiiiiiiiiii          iiiiii          vBMBBBMBMBM     i                                 vB"); Sleep(50);
		puts("r         iiiiriiiiiiiiiiiriiiiiiii      i  iJBMi OMBBBLrri                                     HMBM"); Sleep(50);
		puts("MBMOSsii         iiiiiiiiiiiiii     iruRMB iRRSEJ  oMBBosuuSSvirSRIsvviiiiii                 iOBMBMB"); Sleep(50);
		puts("BBBMBBBMBMBMBMRpXUusLrriiiiiiicpRMBMBMBMBBorBr  xRi BMBMBMBMBMBMBMBMBBBMBMBMBMBMBBBMBOWGDKOMBMBMBMBM"); Sleep(50);

		gotoxy(20, 42);
		puts("�١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١� AI �غ����Դϴ� �١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١�"); Sleep(3000);
		gotoxy(20, 44);
		puts("�١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١� ����ȭ������ �Ѿ�ϴ� �١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١ڡ١�"); Sleep(3000);
		system("cls");
		goto START;
	}
	system("cls");

	//////////////////////////////////////////////////

	gotoxy(0, 0);
	BOARD();//���� �� �ҷ�����
	show_playerstat(i);//�÷��̾� ���� ������ִ� �Լ�
	settextcolor(21, 0);// �� ����, Enter Ű�� ���� �ֻ����� �������� ���� ����

	//////////////////////////////////////////////////������ ��������������

	for (i = 0; i<playersnum; i++)
	{
		gotoxy(4 + 2 * i, 5);
		if (i == 0)
		{
			settextcolor(3, 0);
			printf("��");
		}

		else if (i == 1)
		{
			settextcolor(4, 0);
			printf("��");
		}
		else
		{
			settextcolor(6, 0);
			printf("��");
		}
		coordinate[i][0] = 4 + 2 * i;
		coordinate[i][1] = 5;   //ù��°�����ǿ� �������� ������ ��ġ�� ������
	}

	//(4,6,8) (16,18,20) (28,30,32) (40,42,44) (52,54,56) (64,66,68) (76,78,80) (88,90,92) (100,102,104) : x
	//   5        11         17         23         29         35         41         47           53      : y

	//random�� �ֻ���

	while ((players[0].count<20) && (players[1].count<20) && (players[2].count<20))//�׳� 1�־�� ���ӳ��������� �־����
	{
		for (i = 0; i < playersnum; i++)//�������� ����������
		{


			////        �ֻ���
			///////////////////////
			if (i == 0)
				settextcolor(3, 0);
			else if (i == 1)
				settextcolor(4, 0);
			else
				settextcolor(6, 0);
			/////////////////////// �÷��̾� ���� ���� ����.

			if (wait[i] != 0)//����
			{
				wait[i]--;
				goto jump_dice;
			}//

			if (players[i].shuttle == 1)//��Ʋ����
			{
				shuttle(want_to_go, i);
				goto jump_dice;
			}//

			clear_event();//�̺�Ʈâ �����

			random = dice(i);

			player_move(i, random);

			structure(i);

			blackout(i);
		jump_dice:

			if (((coordinate[i][0] == 40 + 2 * i) && (coordinate[i][1] == 5)) ||
				((coordinate[i][0] == 100 + 2 * i) && (coordinate[i][1] == 23)) ||
				((coordinate[i][0] == 28 + 2 * i) && (coordinate[i][1] == 53)) ||
				((coordinate[i][0] == 4 + 2 * i) && (coordinate[i][1] == 23)) == 1)
			{
				golden_key(i);
			}


			///���࿭��ĭ���� �Ǵ�///
			if ((coordinate[i][0] == 4 + 2 * i) && (coordinate[i][1] == 53))
			{
				players[i].shuttle = 1;
			}
			///
			fflush(stdin);

			if ((coordinate[i][0] == 100 + 2 * i) && (coordinate[i][1] == 5))
			{
				lost_and_found(i);
			}

			fflush(stdin);

			show_playerstat(i);

			fflush(stdin);//Ȳ�ݿ��� ���� ����(�ֻ��� �ڵ� ������ ����)
		}
	}

	for (i = 0; i<3; i++)
	{
		if (players[i].count > 20)
		{
			gotoxy(25, 35);
			printf("player %d �� �¸��Ͽ����ϴ�.!!!", i); Sleep(5000);
			system("cls");
			gotoxy(0, 0);
			puts("                                                                                                    "); Sleep(50);
			puts("                                                                                                    "); Sleep(50);
			puts("                                                                                                    "); Sleep(50);
			puts("                                                                                                    "); Sleep(50);
			puts("sRDDBMBMBMBMBMW.     FMEOMBMBMM;     .DODOBMBBBMM:LMBMBRDDR;    :DRGRMBBBMBMBR"); Sleep(50);
			puts("MBMBKJ2UUF1ULBMM    ;BMBMFJ2sPMB     XMBMBJ11F3uEBB2JFLRMBMB    OBMBML3F1U2FKu"); Sleep(50);
			puts("BROM.        ;B2    ,MRRB     BR     JBWRZ	    ,BB    cBDBM    0BOMU       "); Sleep(50);
			puts("MREB.               7BDRM    .MB     JMWRR	    :BM    JMWMR    HBGMU         "); Sleep(50);
			puts("BRDOO7  rMBMBMH   ,RBWDDMMBMBMBMBU   xBZDRBu    :MB    JBDBR    PMDDRZMMBMBMBM"); Sleep(50);
			puts("BRZEMB3      UMW  SBWDZOM7;77r:;MB   JMWZWMB.   :BB    3MOMR    HBEGDBM7i77Lx7"); Sleep(50);
			puts("BRGZOMx      xBH  uMD0EOB       BM   xBEDERM    ;BB    JBGBR    KMGZGRM       "); Sleep(50);
			puts("BRZZGBv      3MS  xBZZZRM       BB   JMW0GRB    :BM    3MWMR    HBEGZMB       "); Sleep(50); Sleep(50);
			puts("BOG0WML      xBF  JMDKGOB       BM   xBEEZRM    :MB    uBGBR    KMW0ERB                  "); Sleep(50);
			puts("MMZEEBr      LMH  JBGGGMM       MB   uMWEWOB    :BB    3MRMR    XBGEGRR                  "); Sleep(50);
			puts("BMMRBBZ:rr7r;EBG  XMBMBMB       BM   HBMBMBM.   iMB    PBMBM    RBBRBBB;r777xi           "); Sleep(50);
			puts("1BMMORMBBBMBMBM:  :BMRRBS       BB.  :MBRRMS    ;BM    :MBML    ;BRRORRBMBMBMB           "); Sleep(50);
			puts("                                                                                         "); Sleep(50);
			puts(";:;:;:;::,:,,:;      ::::;        :7;     ;::;i;;;ir7rc.    :;;:::;:;:;::               "); Sleep(50);
			puts(":BMBMBMBMBMBMBBBBB   iBBBBMBM       2BM   ;BMBMBMBMBMBMBMP   HMBBBMBMBMBMBMB              "); Sleep(50);
			puts("iMB       iMRDODBM   LBOODOBB       sMM   7MRWB7             ZBWRM.      :BM              "); Sleep(50);
			puts(":BM       rBRDEDRB   7MOEEZMM       vBR   ;BDWMr             HMWDB       .MB              "); Sleep(50);
			puts(":MB        ZBREZMM   rBDEZGRM       LMM   rMWGB7        .    SBEDM.      :BMx             "); Sleep(50);
			puts(":BM         ,RODRB   7MW0GEMM       7BR   ;BDDDRMBMBMBBBBK   HMDEOMBMBMBMBMBBB;           "); Sleep(50);
			puts(":MB          EBEMM   rBDEZDRB       7BR   rMOEDDMM;:::;:r.   SBGZZOMZ.::::,.RMP           "); Sleep(50);
			puts(":BM          MMDRB   sMOGDEMR      ;MBB   iBDEZGOB           HMD0ZEBJ       OBu           "); Sleep(50);
			puts(":MB          RMEMM   :BMOEGRB     MMBMK   iMOEEERM           SBZEKWMF       RM2           "); Sleep(50);
			puts(":BM          RMGRB     uMOEMM     MBD,    iBDEZDRB           HMW0ZGBU       RBJ           "); Sleep(50);
			puts(":MB          OBEMM      RRERR     BM:     iMOZG0MM           SBEZKWMS       MM3           "); Sleep(50);
			puts(":BB          WMOMB      BBWBO     MB;     rBWODORB           0MOZDDBX       MB3           "); Sleep(50);
			puts(":MBMBMBMBMBMBMBBBM      BBBMBMBMBMBMi     rMBMBMBMBMBMBMB2   KBMBMBMX       BME           "); Sleep(50);
			puts(":u21U2U1F1UuxcxL.       LLLL22131J;       :JcvcvLuU1U2F0i    7svcL7        xHi           "); Sleep(50);

			gotoxy(0, 33);
			settextcolor(35, 5);
			printf("��        "); Sleep(1000);
			settextcolor(24, 2);
			printf("��        "); Sleep(1000);
			settextcolor(15, 1);
			printf("��        "); Sleep(1000);
			settextcolor(14, 4);
			printf("��        "); Sleep(1000);



		}
	}

	////////////////////////////////////////////////////////////�������̱�




	return 0;

}

void BOARD(void)
{
	/////////////////////////////���� ��///////////////////////////////////
	settextcolor(26, 0); //�� �׵θ� �� ����
	puts("��������������������������������������������������������������������������������������������������������������"); Sleep(80);
	puts("��  START!  ��  ��ġ��  ��  ���  ��*Ȳ�ݿ���*��  ���뿪  �� �б����� ��  �ȱ���  �� �湫�ο� ���нǹ����ͦ�"); Sleep(80);
	puts("��          ��          ��          ��          ��          ��          ��          ��          ��          ��"); Sleep(80);
	puts("��   �б�   ��          ��          ��          ��          ��          ��          ��          ��          ��"); Sleep(80);
	puts("��          ��          ��          ��          ��          ��          ��          ��          ��          ��"); Sleep(80);
	puts("��          ��          ��          ��          ��          ��          ��          ��          ��          ��"); Sleep(80); //y 5
	puts("��������������������������������������������������������������������������������������������������������������"); Sleep(80);
	puts("��  �ȾϿ�  ��                                                                                  �� �սʸ��� ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                         [ �� �� Ʈ â]                                           ��          ��"); Sleep(80);
	puts("��          ��    ����������������������������������������������������������                    ��          ��"); Sleep(80); //y 11
	puts("��������������    ��                                                      ��                    ��������������"); Sleep(80);
	puts("��  ��뿪  ��    ��                                                      ��                    ��  ��û��  ��"); Sleep(80);
	puts("��          ��    ��                                                      ��                    ��          ��"); Sleep(80);
	puts("��          ��    ��                                                      ��                    ��          ��"); Sleep(80);
	puts("��          ��    ��                                                      ��                    ��          ��"); Sleep(80);
	puts("��          ��    ��                                                      ��                    ��          ��"); Sleep(80); //y 17
	puts("��������������    ��                                                      ��                    ��������������"); Sleep(80);
	puts("��*Ȳ�ݿ���*��    ��                                                      ��                    ��*Ȳ�ݿ���*��"); Sleep(80);
	puts("��          ��    ����������������������������������������������������������                    ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80); // y 23
	puts("��������������                                                                                  ��������������"); Sleep(80);
	puts("��  ������  ��                                                                                  ��  �Ｚ��  ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80); // y 29
	puts("��������������                                                                                  ��������������"); Sleep(80);
	puts("��  ���  ��                                                                                  ��  ������  ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80); // y 35
	puts("��������������                                                                                  ��������������"); Sleep(80);
	puts("��  �Ŵ翪  ��                                                                                  ��  ������  ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80); // y 41
	puts("��������������                                                                                  ��������������"); Sleep(80);
	puts("��  �����  ��                                                                                  �� �ŵ����� ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80);
	puts("��          ��                                                                                  ��          ��"); Sleep(80); // y 47
	puts("��������������������������������������������������������������������������������������������������������������"); Sleep(80);
	puts("�� ���࿭�� ��  ������  ��*Ȳ�ݿ���*��  ������  ��  ���￪  �� ���빮�� �� �뷮���� ��  ȸ�⿪  �� ������� ��"); Sleep(80);
	puts("��          ��          ��          ��          ��          ��          ��          ��          ��          ��"); Sleep(80);
	puts("��          ��          ��          ��          ��          ��          ��          ��          ��          ��"); Sleep(80);
	puts("��          ��          ��          ��          ��          ��          ��          ��          ��          ��"); Sleep(80);
	puts("��          ��          ��          ��          ��          ��          ��          ��          ��          ��"); Sleep(80); // y 53
	puts("��������������������������������������������������������������������������������������������������������������"); Sleep(80);


	settextcolor(4, 0);//�� ��� �ؽ�Ʈ ���� ����
	gotoxy(48, 27);
	printf("�� ");
	settextcolor(6, 0);
	printf("�� ");
	settextcolor(7, 0);
	printf("�� ");
	settextcolor(8, 0);
	printf("�� ");

	gotoxy(42, 7);// �÷��̾� ��ǥ�� ( �÷��̾� ���� ���� ����� �޶�����)
	if (playersnum == 2)
	{
		settextcolor(3, 0);
		printf("P1 : ��  ");

		settextcolor(4, 0);
		printf("P2 : ��  ");
	}
	else if (playersnum == 3)
	{
		settextcolor(3, 0);
		printf("P1 : ��  ");

		settextcolor(4, 0);
		printf("P2 : ��  ");

		settextcolor(6, 0);
		printf("P3 : ��  ");
	}





	/////////////////////////////���� ��//////////////////////////////////
	/////////////////////////�������ͽ� â////////////////////////////////

	BOARD_Status();
}

void BOARD_Status(void)
{
	settextcolor(15, 0);
	gotoxy(120, 0);
	puts("< �������ͽ� >");

	if (playersnum == 2)
	{
		gotoxy(114, 1);
		settextcolor(8, 0);
		puts("������������������������"); Sleep(80); gotoxy(114, 2);
		puts("��      Player1       ��"); Sleep(80); gotoxy(114, 3);
		puts("��                    ��"); Sleep(80); gotoxy(114, 4);
		puts("��                    ��"); Sleep(80); gotoxy(114, 5);
		puts("��                    ��"); Sleep(80); gotoxy(114, 6);
		puts("��                    ��"); Sleep(80); gotoxy(114, 7);
		puts("��                    ��"); Sleep(80); gotoxy(114, 8);
		puts("������������������������"); Sleep(80); gotoxy(114, 9);
		puts("������������������������"); Sleep(80); gotoxy(114, 10);
		puts("��      Player2       ��"); Sleep(80); gotoxy(114, 11);
		puts("��                    ��"); Sleep(80); gotoxy(114, 12);
		puts("��                    ��"); Sleep(80); gotoxy(114, 13);
		puts("��                    ��"); Sleep(80); gotoxy(114, 14);
		puts("��                    ��"); Sleep(80); gotoxy(114, 15);
		puts("��                    ��"); Sleep(80); gotoxy(114, 16);
		puts("������������������������"); Sleep(80); gotoxy(114, 17);
	}
	else if (playersnum == 3)
	{
		gotoxy(114, 1);
		settextcolor(8, 0);
		puts("������������������������"); Sleep(80); gotoxy(114, 2);
		puts("��      Player1       ��"); Sleep(80); gotoxy(114, 3);
		puts("��                    ��"); Sleep(80); gotoxy(114, 4);
		puts("��                    ��"); Sleep(80); gotoxy(114, 5);
		puts("��                    ��"); Sleep(80); gotoxy(114, 6);
		puts("��                    ��"); Sleep(80); gotoxy(114, 7);
		puts("��                    ��"); Sleep(80); gotoxy(114, 8);
		puts("������������������������"); Sleep(80); gotoxy(114, 9);
		puts("������������������������"); Sleep(80); gotoxy(114, 10);
		puts("��      Player2       ��"); Sleep(80); gotoxy(114, 11);
		puts("��                    ��"); Sleep(80); gotoxy(114, 12);
		puts("��                    ��"); Sleep(80); gotoxy(114, 13);
		puts("��                    ��"); Sleep(80); gotoxy(114, 14);
		puts("��                    ��"); Sleep(80); gotoxy(114, 15);
		puts("��                    ��"); Sleep(80); gotoxy(114, 16);
		puts("������������������������"); Sleep(80); gotoxy(114, 17);
		puts("������������������������"); Sleep(80); gotoxy(114, 18);
		puts("��      Player3       ��"); Sleep(80); gotoxy(114, 19);
		puts("��                    ��"); Sleep(80); gotoxy(114, 20);
		puts("��                    ��"); Sleep(80); gotoxy(114, 21);
		puts("��                    ��"); Sleep(80); gotoxy(114, 22);
		puts("��                    ��"); Sleep(80); gotoxy(114, 23);
		puts("��                    ��"); Sleep(80); gotoxy(114, 24);
		puts("������������������������"); Sleep(80); gotoxy(114, 25);
	}

	gotoxy(115, 27); printf("������������Ʈ��������");
	gotoxy(115, 28); printf("1. ��ġ��");
	gotoxy(115, 29); printf("2. ���"); settextcolor(14, 0);
	gotoxy(115, 30); printf("3. *Ȳ�ݿ���*"); settextcolor(8, 0);
	gotoxy(115, 31); printf("4. ���뿪");
	gotoxy(115, 32); printf("5. �б�����");//5
	gotoxy(115, 33); printf("6. �ȱ���");
	gotoxy(115, 34); printf("7. �湫�ο�");
	gotoxy(115, 35); printf("8. �սʸ���");
	gotoxy(115, 36); printf("9. ��û��"); settextcolor(14, 0);
	gotoxy(115, 37); printf("10. *Ȳ�ݿ���*"); settextcolor(8, 0);//10 
	gotoxy(115, 38); printf("11. �Ｚ��");
	gotoxy(115, 39); printf("12. ������");
	gotoxy(115, 40); printf("13. ������");
	gotoxy(115, 41); printf("14. �ŵ�����");
	gotoxy(115, 42); printf("15. ȸ�⿪");//15
	gotoxy(115, 43); printf("16. �뷮����");
	gotoxy(115, 44); printf("17. ���빮��");
	gotoxy(115, 45); printf("18. ���￪");
	gotoxy(115, 46); printf("19. ������"); settextcolor(14, 0);
	gotoxy(115, 47); printf("20. *Ȳ�ݿ���*"); settextcolor(8, 0);//20
	gotoxy(115, 48); printf("21. ������");
	gotoxy(115, 49); printf("22. �����");
	gotoxy(115, 50); printf("23. �Ŵ翪");
	gotoxy(115, 51); printf("24. ���");
	gotoxy(115, 52); printf("25. ������"); settextcolor(14, 0);//25
	gotoxy(115, 53); printf("26. *Ȳ�ݿ���*"); settextcolor(8, 0);
	gotoxy(115, 54); printf("27. ��뿪");
	gotoxy(115, 55); printf("28. �ȾϿ�");//28
	gotoxy(115, 56); printf("����������������������");

	settextcolor(14, 0);
	gotoxy(110, 58); printf("�� Ȳ�ݿ��� �κ����δ� �����ʴ� ���� �����ϴ� ��");
}

void show_playerstat(int i)
{
	for (i = 0; i < playersnum; i++)
	{
		if (dead == i)
		{
			gotoxy(116, 3 + 8 * i);
			printf("�Ļ�");
		}
		else
			gotoxy(116, 3 + 8 * i); settextcolor(14, 0);
		printf("������ �� : %d ����", players[i].money);
		gotoxy(116, 4 + 8 * i);
		printf("������ �� : %d ��", players[i].land_possess);
		gotoxy(116, 5 + 8 * i);
		printf("���� �� : %d", players[i].count);
		Sleep(150);
	}
}

////////////////////////////////////�ֻ���///////////////////////////////////////////

int dice(int i)
{
	int g, random;

	gotoxy(20, 12);
	printf("�÷��̾� %d �� ���Դϴ�!             \n", i + 1);
	gotoxy(20, 15);
	printf("                                                     ");
	gotoxy(20, 13);
	srand(time(NULL));
	random = rand() % 6 + 1;
	printf("Enter Ű�� ���� �ֻ����� ��������.");

	fflush(stdin);//���� ����

	scanf_s("%c", &g);  //enter �Է¹���, Ȥ�� �ٸ��� �ԷµǸ� ����������
	gotoxy(20, 15);
	printf("�ֻ��� ���� �� : %d (EnterŰ�� ������ ���� �̵��մϴ�)", random);
	gotoxy(20, 16);
	scanf_s("%c", &g); //enter�� ������ ���� �̵��ϱ� ����
	gotoxy(20, 16);

	Sleep(1000);//1�� ����� �ֻ����� ������ ��.

	return random;
}

///////////////////////////////////�� �����̱�/////////////////////////////////

void player_move(int i, int random)
{
	int rest;
	if (dead == i);
	else
	{
		while (random >= 1)
		{

			if (a[i] == 1)//a[i]�� 1�̸� ���� �����ٿ��� ���̿��������ϴ°�
			{
				if (coordinate[i][0] + 12 > 100 + 2 * i)// ���� ������ ĭ���� �Ѿ�ٸ� 
				{
					rest = (coordinate[i][0] + 12 - (100 + 2 * i)) / 12;//�Ѿ�¼���ŭ�����ϰ�
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");
					gotoxy(100 + 2 * i, 5 + (6 * rest));
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					//printf("%d", playerscharc[i]);//
					Sleep(300);
					coordinate[i][0] = 100 + 2 * i;
					coordinate[i][1] = 5 + (6 * rest);
					a[i] = 2;
					if (coordinate[i][1] == 53)//�ǳ��̸�
					{
						coordinate[i][0] = 100 + 2 * i;
						coordinate[i][1] = 53;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 3;
					}
				}
				else//������ ĭ���� �ȳѾ�ٸ�
				{
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");
					gotoxy(coordinate[i][0] + 12, coordinate[i][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					//printf("%d", playerscharc[i]);//
					Sleep(300);
					coordinate[i][0] = coordinate[i][0] + 12;//��ǥ����

					if (coordinate[i][0] == 100 + 2 * i)//���ೡĭ���Ӵٸ�
					{
						coordinate[i][0] = 100 + 2 * i;//������ �����γ��������յ��� ��ǥ���ٲٰ�
						coordinate[i][1] = 5;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 2;//�ǳ��̸�
					}
				}
			}
			else if (a[i] == 2)
			{
				if (coordinate[i][1] + 6 > 53)//�����
				{
					rest = (coordinate[i][1] + 6 - 53) / 6;
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");//�����
					gotoxy(100 + 2 * i - (12 * rest), 53);
					/////////////////���� ���
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					/////////////////
					//printf("%d", playerscharc[i]);//
					Sleep(500);
					coordinate[i][0] = 100 + 2 * i - (12 * rest);
					coordinate[i][1] = 53;
					a[i] = 3;
					if (coordinate[i][0] == 4 + 2 * i)//�ǳ��̸�
					{
						coordinate[i][0] = 4 + 2 * i;
						coordinate[i][1] = 53;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 4;
					}

				}
				else
				{
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");
					gotoxy(coordinate[i][0], coordinate[i][1] + 6);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					//printf("%d", playerscharc[i]);//
					Sleep(300);
					coordinate[i][1] = coordinate[i][1] + 6;//�ȳѾ��

					if (coordinate[i][1] == 53)//�ǳ��̸�
					{
						coordinate[i][0] = 100 + 2 * i;
						coordinate[i][1] = 53;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 3;
					}
				}
			}
			else if (a[i] == 3)
			{
				if ((coordinate[i][0] - 12) < (4 + 2 * i))//�Ѿ�ٸ�
				{
					rest = (coordinate[i][0] - 12 - (4 + 2 * i)) / 12;
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");
					gotoxy(4 + 2 * i, 53 + (6 * rest));
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					//printf("%d", playerscharc[i]);//
					Sleep(300);
					coordinate[i][0] = 4 + 2 * i;
					coordinate[i][1] = 53 + (6 * rest);
					a[i] = 4;
					if (coordinate[i][1] == 5)
					{
						coordinate[i][0] = 4 + 2 * i;
						coordinate[i][1] = 5;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 1;//�ǳ��̸�
					}
				}
				else
				{
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");
					gotoxy(coordinate[i][0] - 12, coordinate[i][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(300);
					coordinate[i][0] = coordinate[i][0] - 12;//�ȳѾ��

					if (coordinate[i][0] == 4 + 2 * i)//�ǳ��̸�
					{
						coordinate[i][0] = 4 + 2 * i;
						coordinate[i][1] = 53;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 4;
					}
				}
			}
			else if (a[i] == 4)
			{
				if (coordinate[i][1] - 6 < 3)//�Ѿ�ٹ�
				{
					rest = -((coordinate[i][1] - 6 - 5)) / 6;
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");//�����
					gotoxy(4 + 2 * i + (12 * rest), 5);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					//printf("%d", playerscharc[i]);//
					Sleep(300);
					coordinate[i][0] = 4 + 2 * i + (12 * rest);
					coordinate[i][1] = 5;
					a[i] = 1;
					players[i].count++;
					if (coordinate[i][0] == 100 + 2 * i)//���ೡĭ���Ӵٸ�
					{
						coordinate[i][0] = 100 + 2 * i;//������ �����γ��������յ��� ��ǥ���ٲٰ�
						coordinate[i][1] = 5;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 2;//�ǳ��̸�
						players[i].count++;
					}
				}
				else
				{
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");
					gotoxy(coordinate[i][0], coordinate[i][1] - 6);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					//printf("%d", playerscharc[i]);//
					Sleep(300);
					coordinate[i][1] = coordinate[i][1] - 6;//�ȳѾ��

					if (coordinate[i][1] == 5)
					{
						coordinate[i][0] = 4 + 2 * i;
						coordinate[i][1] = 5;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 1;//�ǳ��̸�
					}
				}
			}
			if ((coordinate[i][0] == 4 + 2 * i) && (coordinate[i][1] == 5))
			{
				players[i].count++;
				players[i].money += 30;
			}

			random--;
		}


	}



}


void golden_key(int i)
{
	int key, money, d, mal, j;
	srand(time(NULL));
	key = rand() % 11 + 1;

	gotoxy(20, 30);
	settextcolor(6, 0);
	puts("����������������������������������������������������������������������"); Sleep(80); gotoxy(20, 31);
	puts("��                             Ȳ�ݿ���                             ��"); Sleep(80); gotoxy(20, 32);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 33);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 34);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 35);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 36);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 37);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 38);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 39);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 40);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 41);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 42);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 43);
	puts("��                                                                  ��"); Sleep(80); gotoxy(20, 44);
	puts("����������������������������������������������������������������������"); Sleep(80); gotoxy(20, 45);


	switch (key)
	{
	case 1: gotoxy(23, 34); printf("<����!>"); Sleep(2000);
		gotoxy(23, 36); printf("���� �����մϴ�! ¦¦"); Sleep(2000);
		gotoxy(23, 38); printf("ģ������ ���� ������ ��� �ɴϴ�."); Sleep(2000);
		gotoxy(23, 40); printf("\"��ο��� 20������ ��������\""); Sleep(2000);

		for (j = 0; j < playersnum; j++)
		{
			if (j == i)
				players[j].money += (playersnum - 1) * 20;
			else
				players[j].money -= 20;
		}

		break;

	case 2: gotoxy(23, 34); printf("<���࿭��>"); Sleep(2000);
		gotoxy(23, 36); printf("�̹� ������ ���࿭���Դϴ�."); Sleep(2000);
		gotoxy(23, 38); printf("���࿭���� Ÿ�� ���ϴ� ������ �̵��ϼ���."); Sleep(2000);
		gotoxy(23, 40); printf("\"���࿭���� ����\""); Sleep(2000);

		gotoxy(coordinate[i][0], coordinate[i][1]);
		printf("  ");
		coordinate[i][0] = 4 + 2 * i;
		coordinate[i][1] = 53;
		a[i] = 4;
		gotoxy(coordinate[i][0], coordinate[i][1]);
		if (i == 0)
		{
			settextcolor(3, 0);
			printf("��");
		}
		else if (i == 1)
		{
			settextcolor(4, 0);
			printf("��");
		}
		else
		{
			settextcolor(6, 0);
			printf("��");
		}

		players[i].shuttle = 1;
		break;

	case 3: gotoxy(23, 34); printf("<������>"); Sleep(2000);
		gotoxy(23, 36); printf("����� �б��� �����⸦ �ΰ� �Խ��ϴ�."); Sleep(2000);
		gotoxy(23, 38); printf("�б��� �ٽ� ���ư�����."); Sleep(2000);
		gotoxy(23, 40); printf("\"�б��� ����\""); Sleep(2000);

		gotoxy(coordinate[i][0], coordinate[i][1]);
		printf("  ");
		coordinate[i][0] = 4 + 2 * i;
		coordinate[i][1] = 5;
		a[i] = 1;
		for (i = 0; i<playersnum; i++)
		{
			gotoxy(coordinate[i][0], coordinate[i][1]);
			if (i == 0)
			{
				settextcolor(3, 0);
				printf("��");
			}
			else if (i == 1)
			{
				settextcolor(4, 0);
				printf("��");
			}
			else
			{
				settextcolor(6, 0);
				printf("��");
			}
		}
		// �̵� & ��

		break;

	case 4: gotoxy(23, 34); printf("<����ڵ�>"); Sleep(2000);
		gotoxy(23, 36); printf("����� ���� ����ڵ��� �Ͽ� �ſ� �ǰ��մϴ�."); Sleep(2000);
		gotoxy(23, 38); printf("����ö���� ���� ���� ��� ���� ����ġ�� ���ҽ��ϴ�."); Sleep(2000);
		gotoxy(23, 40); printf("\"�ݴ��� ����ö�� Ÿ�� ���ư��� ���� 3�� �ҿ�\""); Sleep(2000);

		wait[i] = 3;

		break;

	case 5: gotoxy(23, 34); printf("<�����>"); Sleep(2000);
		gotoxy(23, 36); printf("����ö �ȿ��� ������� �������ϴ�."); Sleep(2000);
		gotoxy(23, 38); printf("�ҽ��� ������ ������ ������ ���� �� �� ���� ���� ���������ϴ�."); Sleep(2000);
		gotoxy(23, 40); printf("���� ���Ƚ��ϴ�."); Sleep(2000);
		gotoxy(23, 42); printf("\"20������ ������\""); Sleep(2000);

		players[i].money -= 20;

		break;

	case 6: gotoxy(23, 34); printf("<�� ��� �ҸӴ�>"); Sleep(2000);
		gotoxy(23, 36); printf("����� ����ö���� ���ſ� ���� ��� �ҸӴϸ� �������ϴ�."); Sleep(2000);
		gotoxy(23, 38); printf("(1 : �����ش�, 2 : �����Ѵ�) : "); Sleep(2000);
		scanf_s("%d", &j);
		gotoxy(23, 40);
		if (j == 1)
		{
			printf("����� �ҸӴϸ� �����־����ϴ�."); Sleep(2000);
			gotoxy(23, 42); printf("����� 1���� �Ҹ������� �ҸӴϿ��� ������ �޾ҽ��ϴ�."); Sleep(2000);

			wait[i] = 1;

			players[i].money += 30;
		}
		else if (j == 2)
		{
			printf("����� �ҸӴϸ� �����߽��ϴ�."); Sleep(2000);
			gotoxy(23, 42); printf("�ƹ� �ϵ� �Ͼ�� �ʾҽ��ϴ�."); Sleep(2000);
		}

		break;

	case 7: gotoxy(23, 34); printf("<Ȳ�� ���迡 ���� �� ����>"); Sleep(2000);
		gotoxy(23, 36); printf("�� ���� �� ����"); Sleep(2000);
		gotoxy(23, 38); printf("���̳� ���� ������."); Sleep(2000);

		money = (rand() % 5 + 1) * 10;

		gotoxy(23, 40); printf("\"�� %d���� ����>\"", money); Sleep(2000);

		players[i].money += money;

		break;

	case 8: gotoxy(23, 34); printf("<��������>"); Sleep(2000);
		gotoxy(23, 36); printf("���������� �ϴµ� ������ ����� �����ϴ�."); Sleep(2000);
		gotoxy(23, 38); printf("���� �� �ϳ��� ���� �̵��ؼ� ��ƿ�����."); Sleep(2000);
		gotoxy(23, 40); printf("\"�ٸ� �÷��̾� �� �ϳ��� ���� �ִ� ������ ������\""); Sleep(2000);
		gotoxy(23, 42);
		printf("(");
		settextcolor(3, 0); printf("1 : �� ");
		settextcolor(4, 0); printf("2 : �� ");
		settextcolor(6, 0);
		if (playersnum == 3)
		{
			printf("3 : ��");
		}
		printf(") ���ϴ� ����? : ");
		scanf_s("%d", &mal);
		Sleep(1000);

		for (j = 0; j < playersnum; j++)
		{
			if (mal == (j + 1))
			{
				gotoxy(coordinate[i][0], coordinate[i][1]);
				printf("  ");
				coordinate[i][0] = coordinate[j][0] - 2 * j + 2 * i;
				coordinate[i][1] = coordinate[j][1];
				a[i] = a[j];
				gotoxy(coordinate[i][0], coordinate[i][1]);

				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
			}
		}

		Sleep(1000);

		break;

	case 9: gotoxy(23, 34); printf("<������ ������>"); Sleep(2000);
		gotoxy(23, 36); printf("������ ���ϴ�. 3ĭ ����"); Sleep(2000);
		gotoxy(23, 38); printf("\"������ 3ĭ ����\""); Sleep(2000);

		player_move(i, 3);

		break;

	case 10: gotoxy(23, 34); printf("�������� �Ҹ�ġ�⸦ �������ϴ�."); Sleep(2000);
		gotoxy(23, 36); printf("���� ī�带 �Ҿ���Ƚ��ϴ�."); Sleep(2000);
		gotoxy(23, 38); printf("�нǹ� ���ͷ� ���� ����ī�带 ã�ƺ�����.");
		gotoxy(coordinate[i][0], coordinate[i][1]);
		printf("  ");
		coordinate[i][0] = 100 + 2 * i;
		coordinate[i][1] = 5;
		a[i] = 2;
		gotoxy(coordinate[i][0], coordinate[i][1]);
		if (i == 0)
		{
			settextcolor(3, 0);
			printf("��");
		}
		else if (i == 1)
		{
			settextcolor(4, 0);
			printf("��");
		}
		else
		{
			settextcolor(6, 0);
			printf("��");
		}
		break;

	case 11:gotoxy(23, 34); printf("������ ���迡 ó�߽��ϴ�."); Sleep(2000);
		gotoxy(23, 36); printf("�ýø� Ÿ���մϴ�."); Sleep(2000);
		gotoxy(23, 38); printf("�ýú� �����ϰ� �ȾϿ����� ������.(�ýú�:2����)"); Sleep(2000);
		players[i].money -= 2;
		gotoxy(coordinate[i][0], coordinate[i][1]);
		printf("  ");
		coordinate[i][0] = 4 + 2 * i;
		coordinate[i][1] = 11;
		a[i] = 4;
		gotoxy(coordinate[i][0], coordinate[i][1]);
		if (i == 0)
		{
			settextcolor(3, 0);
			printf("��");
		}
		else if (i == 1)
		{
			settextcolor(4, 0);
			printf("��");
		}
		else
		{
			settextcolor(6, 0);
			printf("��");
		}
		break;


	default: gotoxy(23, 34); printf("���װ� Ž���Ǿ����ϴ�."); Sleep(2000);
		gotoxy(23, 36); printf("���α׷��� �����մϴ�."); Sleep(2000);
	}


	for (d = 30; d <= 45; d++)
	{
		gotoxy(20, d);
		puts("                                                                      ");
		Sleep(80);
	}

	return;
}

void shuttle(int want_to_go, int i)
{
	int rest;// �� �̰� 28�� ���� �Ѿ�� ���� ���ؼ� �̵���ų��� �����Ѱ�.
	int j, now;//for �� �����
	int re_coordinate[28][2];
	double shuttleposition = 21.5;


	/////////// ��ǥ�� ĭ 1��1 ������Ű�� //////////
	re_coordinate[0][0] = 16;
	re_coordinate[0][1] = 5;

	re_coordinate[1][0] = 28;
	re_coordinate[1][1] = 5;

	re_coordinate[2][0] = 40;
	re_coordinate[2][1] = 5;

	re_coordinate[3][0] = 52;
	re_coordinate[3][1] = 5;

	re_coordinate[4][0] = 64;
	re_coordinate[4][1] = 5;

	re_coordinate[5][0] = 76;
	re_coordinate[5][1] = 5;

	re_coordinate[6][0] = 88;
	re_coordinate[6][1] = 5;

	re_coordinate[7][0] = 100;
	re_coordinate[7][1] = 11;

	re_coordinate[8][0] = 100;
	re_coordinate[8][1] = 17;

	re_coordinate[9][0] = 100;
	re_coordinate[9][1] = 23;

	re_coordinate[10][0] = 100;
	re_coordinate[10][1] = 29;

	re_coordinate[11][0] = 100;
	re_coordinate[11][1] = 35;

	re_coordinate[12][0] = 100;
	re_coordinate[12][1] = 41;

	re_coordinate[13][0] = 100;
	re_coordinate[13][1] = 47;

	re_coordinate[14][0] = 88;
	re_coordinate[14][1] = 53;

	re_coordinate[15][0] = 76;
	re_coordinate[15][1] = 53;

	re_coordinate[16][0] = 64;
	re_coordinate[16][1] = 53;

	re_coordinate[17][0] = 52;
	re_coordinate[17][1] = 53;

	re_coordinate[18][0] = 40;
	re_coordinate[18][1] = 53;

	re_coordinate[19][0] = 28;
	re_coordinate[19][1] = 53;

	re_coordinate[20][0] = 16;
	re_coordinate[20][1] = 53;

	re_coordinate[21][0] = 4;
	re_coordinate[21][1] = 47;

	re_coordinate[22][0] = 4;
	re_coordinate[22][1] = 41;

	re_coordinate[23][0] = 4;
	re_coordinate[23][1] = 35;

	re_coordinate[24][0] = 4;
	re_coordinate[24][1] = 29;

	re_coordinate[25][0] = 4;
	re_coordinate[25][1] = 23;

	re_coordinate[26][0] = 4;
	re_coordinate[26][1] = 17;

	re_coordinate[27][0] = 4;
	re_coordinate[27][1] = 11;

	////////// ��ǥ�� ĭ 1��1 ������Ű�� //////////

	clear_event(); //�̺�Ʈĭ ����

	gotoxy(20, 12);
	printf("������� ���� ��ȣ�� �Է��ϼ��� (1 ~ 28) : "); scanf_s("%d", &want_to_go);

	for (j = 0; j < 28; j++)
	{
		if ((coordinate[i][0] == (land_pos[j][0] + 2 * i)) && (coordinate[i][1] == land_pos[j][1]))
		{
			now = j;  //���� ��ġ
			break;
		}
	}



	if (now - (want_to_go - 1)>0)   //������� �������� ���������� �Ǻ��ϱ� ����
	{
		if ((want_to_go != 3) && (want_to_go != 10) && (want_to_go != 20) && (want_to_go != 26))  //Ȳ�ݿ���ĭ�� ��� ���ܷ� ħ
		{
			players[i].count++;
			players[i].money += 30;
		}
	}


	if (shuttleposition>want_to_go)
		rest = want_to_go;

	switch (want_to_go)
	{
	case 1:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("������ġ������ �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���
		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}


				//////gotoxy(coordinate[i][0], coordinate[i][1]);//���� ��ǥ�� �� ��ǥ�� ���� �� �ű��

				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[0][0] + 2 * i;
			coordinate[i][1] = 5; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}


				//////gotoxy(coordinate[i][0], coordinate[i][1]);//���� ��ǥ�� �� ��ǥ�� ���� �� �ű��

				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 1; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);


		clear_event();

		structure(i);
		break;


	case 2:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("����������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���
		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[1][0] + 2 * i;
			coordinate[i][1] = 5; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 1; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 3:
		clear_event();
		gotoxy(20, 12);
		settextcolor(4, 0);
		printf("��ɺθ��� �ȵ˴ϴ�^^* ������ �м��m"); Sleep(3000);//�ȳ���

		players[i].shuttle = 0;
		break;
	case 4:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�������뿪���� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[3][0] + 2 * i;
			coordinate[i][1] = 5; ////////////////////
		}
		else
		{
			for (j = 22; j < (want_to_go - 1); j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 1; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 5:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�����б��������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���


		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[4][0] + 2 * i;
			coordinate[i][1] = 5; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 1; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 6:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�����ȱ������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���


		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[5][0] + 2 * i;
			coordinate[i][1] = 5; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 1; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 7:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�����湫�ο����� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[6][0] + 2 * i;
			coordinate[i][1] = 5; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 1; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;

	case 8:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�����սʸ������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[7][0] + 2 * i;
			coordinate[i][1] = 11; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i; ///////////////////�̰� ���� �� ĭ ����� �̵����Ķ�
			coordinate[i][1] = 11;
		}

		a[i] = 2; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 9:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("������û������ �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[8][0] + 2 * i;
			coordinate[i][1] = 17; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 2; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 10:
		clear_event();
		gotoxy(20, 12);
		settextcolor(4, 0);
		printf("��ɺθ��� �ȵ˴ϴ�^^* ������ �м��m"); Sleep(3000);//�ȳ���

		players[i].shuttle = 0;
		break;
	case 11:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�����Ｚ������ �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[10][0] + 2 * i;
			coordinate[i][1] = 29; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 2; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 12:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�������������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[11][0] + 2 * i;
			coordinate[i][1] = 35; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 2; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 13:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�������������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[12][0] + 2 * i;
			coordinate[i][1] = 41; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 2; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 14:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�����ŵ��������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[13][0] + 2 * i;
			coordinate[i][1] = 47; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 2; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;

	case 15:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("����ȸ�⿪���� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[14][0] + 2 * i;
			coordinate[i][1] = 53; ////////////////////
		}
		else
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 3; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 16:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�����뷮�������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[15][0] + 2 * i;
			coordinate[i][1] = 53; ////////////////////
		}
		else
		{
			for (j = 22; j < want_to_go; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 3; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 17:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�������빮������ �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[16][0] + 2 * i;
			coordinate[i][1] = 53; ////////////////////
		}
		else
		{
			for (j = 22; j < want_to_go; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 3; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 18:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�������￪���� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[17][0] + 2 * i;
			coordinate[i][1] = 53; ////////////////////
		}
		else
		{
			for (j = 22; j < want_to_go; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 3; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 19:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�������������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[18][0] + 2 * i;
			coordinate[i][1] = 53; ////////////////////
		}
		else
		{
			for (j = 22; j < want_to_go; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 3; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 20:
		clear_event();
		gotoxy(20, 12);
		settextcolor(4, 0);
		printf("��ɺθ��� �ȵ˴ϴ�^^*"); Sleep(3000);//�ȳ���

		players[i].shuttle = 0;

		break;
	case 21:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�������������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[20][0] + 2 * i;
			coordinate[i][1] = 53; ////////////////////
		}
		else
		{
			for (j = 22; j < want_to_go; j++)
			{
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 47;
		}

		a[i] = 3; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("�� ���� ��� ������ 30������ ���޵˴ϴ� ��");
		Sleep(2000);
		clear_event();

		structure(i);
		break;

	case 22:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("����������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[15][0] + 2 * i;
			coordinate[i][1] = 53; ////////////////////
		}
		else
		{
			gotoxy(4 + 2 * i, 53);
			printf("  ");// ��Ʋĭ �����
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//�� �����
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					Sleep(100);
				}
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 41;
		}

		a[i] = 4; ///////////////////////

		clear_event();

		structure(i);
		break;
	case 23:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�����Ŵ翪���� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[22][0] + 2 * i;
			coordinate[i][1] = 53; ////////////////////
		}
		else
		{
			gotoxy(4 + 2 * i, 53);
			printf("  ");// ��Ʋĭ �����
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//�� �����
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					Sleep(100);
				}
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 35;
		}

		a[i] = 4; ///////////////////////

		clear_event();

		structure(i);
		break;
	case 24:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("����������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[23][0] + 2 * i;
			coordinate[i][1] = 29; ////////////////////
		}
		else
		{
			gotoxy(4 + 2 * i, 53);
			printf("  ");// ��Ʋĭ �����
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//�� �����
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					Sleep(100);
				}
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 4; ///////////////////////

		clear_event();

		structure(i);
		break;
	case 25:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�������������� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[24][0] + 2 * i;
			coordinate[i][1] = 53; ////////////////////
		}
		else
		{
			gotoxy(4 + 2 * i, 53);
			printf("  ");// ��Ʋĭ �����
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//�� �����
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					Sleep(100);
				}
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 23;
		}

		a[i] = 4; ///////////////////////

		clear_event();

		structure(i);
		break;
	case 26:
		clear_event();
		gotoxy(20, 12);
		settextcolor(4, 0);
		printf("��ɺθ��� �ȵ˴ϴ�^��^!"); Sleep(3000);//�ȳ���

		players[i].shuttle = 0;

		break;
	case 27:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("������뿪���� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[26][0] + 2 * i;
			coordinate[i][1] = 53; ////////////////////
		}
		else
		{
			gotoxy(4 + 2 * i, 53);
			printf("  ");// ��Ʋĭ �����
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//�� �����
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					Sleep(100);
				}
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 17;
		}

		a[i] = 4; ///////////////////////

		clear_event();

		structure(i);
		break;
	case 28:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("�����ȾϿ����� �̵��մϴ�~_~����"); Sleep(3000);//�ȳ���

		gotoxy(4 + 2 * i, 53);
		printf("  ");//��Ʋ������ �� ����

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//��Ʋ���� �� �����
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//�� �����
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("��");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("��");
				}
				else
				{
					settextcolor(6, 0);
					printf("��");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28��° ĭ �� �����
			for (j = 0; j < rest; j++) //rest �̵���Ű��
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
				}


				Sleep(100);
			}
			coordinate[i][0] = re_coordinate[27][0] + 2 * i;
			coordinate[i][1] = 53; ////////////////////
		}
		else
		{
			gotoxy(4 + 2 * i, 53);
			printf("  ");// ��Ʋĭ �����
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//�� �����
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("��");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("��");
					}
					else
					{
						settextcolor(6, 0);
						printf("��");
					}
					Sleep(100);
				}
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 4; ///////////////////////

		clear_event();

		structure(i);
		break;



	}
	players[i].shuttle = 0;
}


void clear_event()
{
	gotoxy(20, 12);
	printf("                                                     ");
	gotoxy(20, 13);
	printf("                                                     ");
	gotoxy(20, 14);
	printf("                                                     ");
	gotoxy(20, 15);
	printf("                                                     ");
	gotoxy(20, 16);
	printf("                                                     ");
}

void structure(int i)

{
	char answer;
	int j;
	int k;




	for (j = 0; j < 28; j++)
	{

		if ((coordinate[i][0] == (land_pos[j][0] + 2 * i)) && (coordinate[i][1] == land_pos[j][1])) //ã�Ҵٸ�
		{
			if (j == 2 || j == 9 || j == 19 || j == 25)  //�� ���� Ȳ�ݿ��� ���̶��
				break; //�극��ũ

			if (lands[j].belongs_who == 0) //�� ���� �����ְ� ���ٸ�
			{
				if (players[i].money >= lands[j].land_price)
				{
					fflush(stdin);
					gotoxy(25, 35);
					printf("���� ��ðڽ��ϱ�? (yes:Y  no:N)  : ");
					scanf_s("%c", &answer);
					if ((answer == 'Y') || (answer == 'y'))
					{
						lands[j].belongs_who = i + 1; //�� ������ player�� ���Դϴ� �ϰ� ����
						players[i].money -= lands[j].land_price; //���� ��ٰ� ���� ��� �����ݿ��� �� ������ ������ ��ŭ ��������
						players[i].land_possess++;

						//////////////////////////////////�� ������ ����Ʈ ���� ����///////////////////////////////////
						if (j == 0)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("1. ��ġ��");
						}
						if (j == 1)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("2. ���");
						}
						if (j == 3)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("4. ���뿪");
						}
						if (j == 4)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("5. �б�����");
						}
						if (j == 5)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("6. �ȱ���");
						}
						if (j == 6)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("7. �湫�ο�");
						}
						if (j == 7)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("8. �սʸ���");
						}
						if (j == 8)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("9. ��û��");
						}
						if (j == 10)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("11. �Ｚ��");
						}
						if (j == 11)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("12. ������");
						}
						if (j == 12)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("13. ������");
						}
						if (j == 13)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("14. �ŵ�����");
						}
						if (j == 14)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("15. ȸ�⿪");
						}
						if (j == 15)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("16. �뷮����");
						}
						if (j == 16)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("17. ���빮��");
						}
						if (j == 17)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("18. ���￪");
						}
						if (j == 18)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("19. ������");
						}
						if (j == 20)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("21. ������");
						}
						if (j == 21)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("22. �����");
						}
						if (j == 22)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("23. �Ŵ翪");
						}
						if (j == 23)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("24. ���");
						}
						if (j == 24)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("25. ������");
						}
						if (j == 26)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("27. ��뿪");
						}
						if (j == 27)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("28. �ȾϿ�");
						}


						////////////////////////////////////�� ������ ����Ʈ ���� ����//////////////////////////////////
						//�� ������ �� ���� �ٲٴ°� �ʿ�
					}
					else if ((answer == 'N') || (answer == 'n'))
					{
						gotoxy(25, 36);
						printf("���� ���� �ʾҽ��ϴ�. ���� ������ �Ѿ�ϴ�.");
						Sleep(1000);
					}

					gotoxy(25, 35);
					printf("                                                          ");
					gotoxy(25, 36);
					printf("                                                          ");
					fflush(stdin);
					gotoxy(coordinate[i][0], coordinate[i][1]);

				}
				else
				{
					gotoxy(25, 35);
					printf("�������� �����Ͽ� ���� �� �� �����ϴ�.");
					gotoxy(coordinate[i][0], coordinate[i][1]);
					Sleep(1000);
					gotoxy(25, 35);
					printf("                                                           ");
					gotoxy(25, 36);
					printf("                                                           ");
				}
			}
			else if (lands[j].belongs_who == i + 1) //�� ���� �����ְ� �÷��̾���
			{
				k = 0;
				fflush(stdin);


				if (lands[j].building == 0)
				{
					gotoxy(25, 35);
					printf("��ö�� �Ǽ��Ͻðڽ��ϱ�? (yes:Y  no:N)  : ");
					scanf_s("%c", &answer);
					gotoxy(25, 35);
					printf("                                                          ");
					if ((answer == 'Y') || (answer == 'y'))
					{
						lands[j].building++;
						players[i].money -= (lands[j].land_price)*0.5;   //�����ݿ��� ���� ����
						lands[j].land_price *= 1.5;  // ������� ���.
						gotoxy(coordinate[i][0] - i, coordinate[i][1] - 3);
						printf("N");
						gotoxy(coordinate[i][0], coordinate[i][1]);
					}
				}
				else if (lands[j].building == 1)
				{
					gotoxy(25, 35);
					printf("��ö�� ��ӿ����� ���׷��̵� �Ͻðڽ��ϱ�? (yes:Y  no:N)  : ");
					scanf_s("%c", &answer);
					gotoxy(25, 35);
					printf("                                                             ");
					if ((answer == 'Y') || (answer == 'y'))
					{
						lands[j].building++;
						players[i].money -= (lands[j].land_price) * 1;
						lands[j].land_price *= 2;
						gotoxy(coordinate[i][0] - i, coordinate[i][1] - 3);
						printf("F");
						gotoxy(coordinate[i][0], coordinate[i][1]);
					}
				}
				else if (lands[j].building == 2)
				{
					gotoxy(25, 35);
					printf("��ӿ����� KTX�� ���׷��̵� �Ͻðڽ��ϱ�? (yes:Y  no:N)  : ");
					scanf_s("%c", &answer);
					gotoxy(25, 35);
					printf("                                                            ");
					if ((answer == 'Y') || (answer == 'y'))
					{
						lands[j].building++;
						players[i].money -= (lands[j].land_price) * 2;
						lands[j].land_price *= 3;
						gotoxy(coordinate[i][0] - i, coordinate[i][1] - 3);
						printf("K");
						gotoxy(coordinate[i][0], coordinate[i][1]);
					}
				}
				else if (lands[j].building == 3)
				{
					gotoxy(25, 35);
					printf("���̻� ���׷��̵��Ҽ������ϴ�. ���������մϴ�."); Sleep(5000);
					gotoxy(25, 35);
					printf("                                            ");
				}
				gotoxy(25, 35);
				printf("                                                          ");
				gotoxy(25, 36);
				printf("                                                          ");
			}
			else
			{
				if (players[i].money < lands[j].land_price)  //�����ݺ��� ���� �� ���� �� ���� ���
				{

					gotoxy(25, 35);
					printf("                           ");
					gotoxy(25, 35);
					printf("�����Ͻ������̺����մϴ�"); Sleep(5000);
					gotoxy(25, 35);
					printf("                       ");
					gotoxy(25, 35);
					printf("���Ͻô� ��ȣ�� �Է��ϼ���");
					gotoxy(25, 36);
					printf("                      ");
					gotoxy(25, 36);
					printf("1.�Ű� 2.�Ļ�");
					gotoxy(25, 37);
					scanf_s("%d", &k);
					if (k == 1)
					{
						sell_the_land(i, j);
					}
					else
					{


						gotoxy(25, 35);
						printf("�÷��̾� %d�� �Ļ��Ͽ����ϴ�.", i + 1);
						bankrup(i);

					}

				}

				else
				{
					players[i].money -= lands[j].land_price;   //�ڱ� ���� �ƴѰ�� ����
					gotoxy(25, 35);
					printf("�� ���� player %d�� ���Դϴ�. ����Ḧ �����ϼ���.", lands[j].belongs_who);
					players[lands[j].belongs_who - 1].money += lands[j].land_price;
					Sleep(2000);
					gotoxy(25, 35);
					printf("                                                           ");
					gotoxy(25, 35);
					printf("�μ��Ͻðڽ��ϱ�??(�μ���� : )(yes:Y  no:N) :");
					scanf_s("%c", &answer);
					gotoxy(25, 35);
					printf("                                                           ");

					if ((answer == 'Y') || (answer == 'y'))
					{
						take_over(i, j);
					}

				}

			}

		}
	}

	gotoxy(coordinate[i][0], coordinate[i][1]); //�ٽ� ���ư��� �ֻ����� ������

}

////////***���Ű���***/////////   �� ������  x��� ������
//���� 0.5x    ���� 1x    ȣ�� 2x//


///////***�����***////////
///���� 1.5x   ���� 2x    ȣ��  3x///

void lost_and_found(int i)
{
	int random1 = rand() % 5 + 1;
	char answer;
	int answerN;

	clear_event();
	switch (random1)
	{
	case 1:
		gotoxy(20, 12);
		printf("�нǹ� ���Ϳ��� D.S.L.R ĳ���� ���� �� !"); Sleep(2000);
		gotoxy(20, 13);
		printf("�߰�� ���˽ô� : 80���� GET ��"); Sleep(2000);
		clear_event();

		players[i].money += 80;
		fflush(stdin);
		break;
	case 2:
		gotoxy(20, 12);
		printf("�нǹ� ���Ϳ��� �����..... ���� �� !"); Sleep(2000);
		gotoxy(20, 13);
		printf("������ ���� �����ϴ� : �׳� �ڱⰡ���ϴ�.."); Sleep(2000);
		clear_event();

		players[i].money += 0;
		fflush(stdin);
		break;
	case 3:
		gotoxy(20, 12);
		printf("�нǹ� ���Ϳ��� �������� ������ ���� �� !"); Sleep(2000);
		gotoxy(20, 13);
		printf("���� �ȿ� ������ ����ֽ��ϴ�.."); Sleep(2000);
		gotoxy(20, 14);
		printf("�������ðڽ��ϱ�? (Y) or (N) :"); scanf_s("%c", &answer);

		if ((answer == 'Y') || (answer == 'y'))
		{
			gotoxy(20, 15);
			printf("���� ��������� ���� 20������ ���ϴ� ��"); Sleep(2000);
			players[i].money -= 20;
		}
		else if ((answer == 'N') || (answer == 'n'))
		{
			gotoxy(20, 15);
			printf("������ ��� : ���� 30������ �޽��ϴ� ��"); Sleep(2000);
			players[i].money += 30;
		}

		clear_event();
		fflush(stdin);

		break;
	case 4:
		gotoxy(20, 12);
		printf("�нǹ� ���Ϳ��� ������ C��� �������� ���� �� !"); Sleep(2000);
		gotoxy(20, 13);
		printf("�߰� ���� �˶�� �˴ϴ� : 2���� GET ��"); Sleep(2000);
		clear_event();

		players[i].money += 2;
		fflush(stdin);
		break;
	case 5:
		gotoxy(20, 12);
		printf("�нǹ� ���Ϳ��� �ϳ� üũī�带 ���� �� !"); Sleep(2000);
		gotoxy(20, 13);
		printf("������ ���ϴ� ��"); Sleep(2000);
		gotoxy(20, 14);
		printf("�󸶸� �������ðڽ��ϱ�? (������ ����) :"); scanf_s("%d", &answerN);

		if (answerN > 50)
		{
			gotoxy(20, 15);
			printf("�������� �׼��׿�. ������ 40���� ��"); Sleep(2000);

			players[i].money -= 40;
		}
		else if (answerN <= 50)
		{
			gotoxy(20, 15);
			printf("���� ���� �� ���½��ϴ� ��"); Sleep(2000);

			players[i].money += answerN;
		}
		clear_event();
		fflush(stdin);


		break;

	}
}

void blackout(int i)
{
	if ((coordinate[i][0] == 100 + 2 * i) && (coordinate[i][1] == 53))
	{
		gotoxy(25, 30);
		printf("����ö�� �����Ǿ����ϴ�!"); Sleep(2000);
		gotoxy(25, 31);
		printf("3�� ���� ������ �� �����ϴ�."); Sleep(2000);
		wait[i] = 3;
		gotoxy(25, 30);
		printf("                                 ");
		gotoxy(25, 31);
		printf("                                 ");
	}
}

void take_over(int a, int b)//(�÷��̾�ѹ�,����ȣ)
{
	int take_over_money = { 0 };

	if (lands[b].belongs_who == 0)
	{
		gotoxy(25, 35);
		printf("                                    ");
		gotoxy(25, 35);
		printf("�����ְ����¶��� �μ��Ҽ������ϴ�^^"); Sleep(5000);
		gotoxy(25, 35);
		printf("                                   ");


	}
	else
	{


		if (lands[b].building == 0)
		{
			take_over_money = 2 * lands[b].land_price;

			if (players[a].money >= take_over_money)
			{

				players[lands[b].belongs_who - 1].land_possess--;

				lands[b].belongs_who = a + 1;

				players[a].money -= take_over_money;
				players[a].land_possess++;

				//////////////////////////////////�� ������ ����Ʈ ���� ����///////////////////////////////////
				if (b == 0)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("1. ��ġ��");
				}
				if (b == 1)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("2. ���");
				}
				if (b == 3)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("4. ���뿪");
				}
				if (b == 4)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("5. �б�����");
				}
				if (b == 5)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("6. �ȱ���");
				}
				if (b == 6)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("7. �湫�ο�");
				}
				if (b == 7)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("8. �սʸ���");
				}
				if (b == 8)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("9. ��û��");
				}
				if (b == 10)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("11. �Ｚ��");
				}
				if (b == 11)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("12. ������");
				}
				if (b == 12)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("13. ������");
				}
				if (b == 13)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("14. �ŵ�����");
				}
				if (b == 14)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("15. ȸ�⿪");
				}
				if (b == 15)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("16. �뷮����");
				}
				if (b == 16)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("17. ���빮��");
				}
				if (b == 17)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("18. ���￪");
				}
				if (b == 18)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("19. ������");
				}
				if (b == 20)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("21. ������");
				}
				if (b == 21)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("22. �����");
				}
				if (b == 22)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("23. �Ŵ翪");
				}
				if (b == 23)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("24. ���");
				}
				if (b == 24)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("25. ������");
				}
				if (b == 26)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("27. ��뿪");
				}
				if (b == 27)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("28. �ȾϿ�");
				}


				////////////////////////////////////�� ������ ����Ʈ ���� ����//////////////////////////////////
				//�� ������ �� ���� �ٲٴ°� �ʿ�
			}
			else
			{
				gotoxy(25, 35);
				printf("                                    ");
				gotoxy(25, 35);
				printf("�����ְ����¶��� �μ��Ҽ������ϴ�^^"); Sleep(5000);
				gotoxy(25, 35);
				printf("                                   ");

			}
		}
		else
		{
			take_over_money = 1.5*(lands[b].land_price);
			if (players[a].money >= take_over_money)
			{

				players[lands[b].belongs_who - 1].land_possess--;

				lands[b].belongs_who = a + 1;

				players[a].money -= take_over_money;
				players[a].land_possess++;
				if (b == 0)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("1. ��ġ��");
				}
				if (b == 1)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("2. ���");
				}
				if (b == 3)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("4. ���뿪");
				}
				if (b == 4)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("5. �б�����");
				}
				if (b == 5)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("6. �ȱ���");
				}
				if (b == 6)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("7. �湫�ο�");
				}
				if (b == 7)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("8. �սʸ���");
				}
				if (b == 8)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("9. ��û��");
				}
				if (b == 10)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("11. �Ｚ��");
				}
				if (b == 11)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("12. ������");
				}
				if (b == 12)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("13. ������");
				}
				if (b == 13)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("14. �ŵ�����");
				}
				if (b == 14)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("15. ȸ�⿪");
				}
				if (b == 15)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("16. �뷮����");
				}
				if (b == 16)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("17. ���빮��");
				}
				if (b == 17)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("18. ���￪");
				}
				if (b == 18)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("19. ������");
				}
				if (b == 20)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("21. ������");
				}
				if (b == 21)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("22. �����");
				}
				if (b == 22)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("23. �Ŵ翪");
				}
				if (b == 23)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("24. ���");
				}
				if (b == 24)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("25. ������");
				}
				if (b == 26)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("27. ��뿪");
				}
				if (b == 27)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("28. �ȾϿ�");
				}


				////////////////////////////////////�� ������ ����Ʈ ���� ����//////////////////////////////////
				//�� ������ �� ���� �ٲٴ°� �ʿ�


			}
			else
			{
				gotoxy(25, 35);
				printf("                                    ");
				gotoxy(25, 35);
				printf("�����ְ����¶��� �μ��Ҽ������ϴ�^^"); Sleep(5000);
				gotoxy(25, 35);
				printf("                                   ");

			}
		}
	}
}

void sell_the_land(int a, int b)
{

	players[a].money += lands[b].land_price;//ȯ�ҿϷ�
	lands[b] = lands_early[b];//�ʱ�ȭ�Ϸ�
}

void bankrup(int a)
{
	int b = 0;
	dead_cnt++;
	if (playersnum - 1 == dead_cnt)
	{
		gotoxy(25, 36);
		printf("�����մϴ�! �¸��ϼ̽��ϴ�."); Sleep(5000);
		system("cls");
		gotoxy(0, 0);
		puts("                                                                                                    "); Sleep(50);
		puts("                                                                                                    "); Sleep(50);
		puts("                                                                                                    "); Sleep(50);
		puts("                                                                                                    "); Sleep(50);
		puts("sRDDBMBMBMBMBMW.     FMEOMBMBMM;     .DODOBMBBBMM:LMBMBRDDR;    :DRGRMBBBMBMBR"); Sleep(50);
		puts("MBMBKJ2UUF1ULBMM    ;BMBMFJ2sPMB     XMBMBJ11F3uEBB2JFLRMBMB    OBMBML3F1U2FKu"); Sleep(50);
		puts("BROM.        ;B2    ,MRRB     BR     JBWRZ	    ,BB    cBDBM    0BOMU       "); Sleep(50);
		puts("MREB.               7BDRM    .MB     JMWRR	    :BM    JMWMR    HBGMU         "); Sleep(50);
		puts("BRDOO7  rMBMBMH   ,RBWDDMMBMBMBMBU   xBZDRBu    :MB    JBDBR    PMDDRZMMBMBMBM"); Sleep(50);
		puts("BRZEMB3      UMW  SBWDZOM7;77r:;MB   JMWZWMB.   :BB    3MOMR    HBEGDBM7i77Lx7"); Sleep(50);
		puts("BRGZOMx      xBH  uMD0EOB       BM   xBEDERM    ;BB    JBGBR    KMGZGRM       "); Sleep(50);
		puts("BRZZGBv      3MS  xBZZZRM       BB   JMW0GRB    :BM    3MWMR    HBEGZMB       "); Sleep(50); Sleep(50);
		puts("BOG0WML      xBF  JMDKGOB       BM   xBEEZRM    :MB    uBGBR    KMW0ERB                  "); Sleep(50);
		puts("MMZEEBr      LMH  JBGGGMM       MB   uMWEWOB    :BB    3MRMR    XBGEGRR                  "); Sleep(50);
		puts("BMMRBBZ:rr7r;EBG  XMBMBMB       BM   HBMBMBM.   iMB    PBMBM    RBBRBBB;r777xi           "); Sleep(50);
		puts("1BMMORMBBBMBMBM:  :BMRRBS       BB.  :MBRRMS    ;BM    :MBML    ;BRRORRBMBMBMB           "); Sleep(50);
		puts("                                                                                         "); Sleep(50);
		puts(";:;:;:;::,:,,:;      ::::;        :7;     ;::;i;;;ir7rc.    :;;:::;:;:;::               "); Sleep(50);
		puts(":BMBMBMBMBMBMBBBBB   iBBBBMBM       2BM   ;BMBMBMBMBMBMBMP   HMBBBMBMBMBMBMB              "); Sleep(50);
		puts("iMB       iMRDODBM   LBOODOBB       sMM   7MRWB7             ZBWRM.      :BM              "); Sleep(50);
		puts(":BM       rBRDEDRB   7MOEEZMM       vBR   ;BDWMr             HMWDB       .MB              "); Sleep(50);
		puts(":MB        ZBREZMM   rBDEZGRM       LMM   rMWGB7        .    SBEDM.      :BMx             "); Sleep(50);
		puts(":BM         ,RODRB   7MW0GEMM       7BR   ;BDDDRMBMBMBBBBK   HMDEOMBMBMBMBMBBB;           "); Sleep(50);
		puts(":MB          EBEMM   rBDEZDRB       7BR   rMOEDDMM;:::;:r.   SBGZZOMZ.::::,.RMP           "); Sleep(50);
		puts(":BM          MMDRB   sMOGDEMR      ;MBB   iBDEZGOB           HMD0ZEBJ       OBu           "); Sleep(50);
		puts(":MB          RMEMM   :BMOEGRB     MMBMK   iMOEEERM           SBZEKWMF       RM2           "); Sleep(50);
		puts(":BM          RMGRB     uMOEMM     MBD,    iBDEZDRB           HMW0ZGBU       RBJ           "); Sleep(50);
		puts(":MB          OBEMM      RRERR     BM:     iMOZG0MM           SBEZKWMS       MM3           "); Sleep(50);
		puts(":BB          WMOMB      BBWBO     MB;     rBWODORB           0MOZDDBX       MB3           "); Sleep(50);
		puts(":MBMBMBMBMBMBMBBBM      BBBMBMBMBMBMi     rMBMBMBMBMBMBMB2   KBMBMBMX       BME           "); Sleep(50);
		puts(":u21U2U1F1UuxcxL.       LLLL22131J;       :JcvcvLuU1U2F0i    7svcL7        xHi           "); Sleep(50);

		gotoxy(0, 33);
		settextcolor(35, 5);
		printf("��        "); Sleep(1000);
		settextcolor(24, 2);
		printf("��        "); Sleep(1000);
		settextcolor(15, 1);
		printf("��        "); Sleep(1000);
		settextcolor(14, 4);
		printf("��        "); Sleep(1000);
	}
	dead = a;
	gotoxy(coordinate[a][0], coordinate[a][1]);
	printf(" ");
	for (b = 0; b<28; b++)
	{

		if (lands[b].belongs_who == a + 1)
		{
			lands[b] = lands_early[b];
		}
	}//���ʱ�ȭ
}
