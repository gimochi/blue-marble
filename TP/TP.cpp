#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>


struct player_info  //구조체 선언
{
	int shuttle; //셔틀 이용권 갖고있니
	int money; //돈
	int land_possess; //소유 땅 몇개니
	int count;


};

struct land_info
{
	int land_price;
	int building;// 빌딩 3개
	int belongs_who;

};

struct player_info players[3] = { { 0, 200, 0, 0, }, { 0, 200, 0, 0, }, { 0, 200, 0, 0, } }; //구조체 변수 선언 [3]은 플레이어 숫자를 의미한다.
struct land_info lands[28] = { { 5, 0, 0 }, { 8, 0, 0 }, { 0, 0, 0 }, { 12, 0, 0 }, { 20, 0, 0 }, { 10, 0, 0 }, { 10, 0, 0 },  //맨 윗줄
{ 12, 0, 0 }, { 12, 0, 0 }, { 14, 0, 0 }, { 16, 0, 0 }, { 20, 0, 0 }, { 18, 0, 0 }, { 18, 0, 0 }, //오른줄
{ 20, 0, 0 }, { 22, 0, 0 }, { 23, 0, 0 }, { 24, 0, 0 }, { 24, 0, 0 }, { 50, 0, 0 }, { 26, 0, 0 }, //아랫줄
{ 28, 0, 0 }, { 30, 0, 0 }, { 30, 0, 0 }, { 45, 0, 0 }, { 32, 0, 0 }, { 32, 0, 0 }, { 35, 0, 0 } }; //왼줄 ;;;;;;;;;;땅의 갯수 28개, 이벤트칸 제외!
struct land_info lands_early[28] = { { 5, 0, 0 }, { 8, 0, 0 }, { 0, 0, 0 }, { 12, 0, 0 }, { 20, 0, 0 }, { 10, 0, 0 }, { 10, 0, 0 },  //맨 윗줄
{ 12, 0, 0 }, { 12, 0, 0 }, { 14, 0, 0 }, { 16, 0, 0 }, { 20, 0, 0 }, { 18, 0, 0 }, { 18, 0, 0 }, //오른줄
{ 20, 0, 0 }, { 22, 0, 0 }, { 23, 0, 0 }, { 24, 0, 0 }, { 24, 0, 0 }, { 50, 0, 0 }, { 26, 0, 0 }, //아랫줄
{ 28, 0, 0 }, { 30, 0, 0 }, { 30, 0, 0 }, { 45, 0, 0 }, { 32, 0, 0 }, { 32, 0, 0 }, { 35, 0, 0 } };

int want_to_go;//가고싶은 장소의 번호(1~28)를 저장할 변수
int playersnum;//전역 변수
int a[3] = { 1, 1, 1 };//a는 말이 위에가로줄인지 왼쪽세로줄인지 어디있는지알려주는거
int coordinate[3][2] = { 0 };//coordinate[플레이어수][0=x,1=y]
int wait[3] = { 0 };//대기시키는 배열
int dead = 100;
int dead_cnt = 0;
int land_pos[28][2] = { { 16, 5 }, { 28, 5 }, { 40, 5 }, { 52, 5 }, { 64, 5 }, { 76, 5 }, { 88, 5 },
{ 100, 11 }, { 100, 17 }, { 100, 23 }, { 100, 29 }, { 100, 35 }, { 100, 41 }, { 100, 47 },
{ 88, 53 }, { 76, 53 }, { 64, 53 }, { 52, 53 }, { 40, 53 }, { 28, 53 }, { 16, 53 },
{ 4, 47 }, { 4, 41 }, { 4, 35 }, { 4, 29 }, { 4, 23 }, { 4, 17 }, { 4, 11 } }; //땅 좌표





void settextcolor(int color, int bgcolor)//색깔 넣는 함수
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4 | color));

}

void gotoxy(int x, int y)//좌표이동 함수
{
	COORD Pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//////////////////////틀/////////////////////////////////

void BOARD();//게임판 출력 함수 선언
void BOARD_Status();//스테이터스 화면 출력 

//////////////////////틀/////////////////////////////////

///////////////////말 움직이기///////////////////////////

void player_move(int i, int random);//말 움직이는 함수
int dice(int i);//주사위 함수

///////////////////말 움직이기///////////////////////////

////////////////////셔털버스/////////////////////////////
void shuttle(int want_to_go, int i);
///////////////////셔털버스//////////////////////////////

/////////////////////돈 관련/////////////////////////////

/////////////////////돈 관련//////////////////////////////

/////////////////////황금열쇠////////////////////////////
void golden_key(int i);

void output_delete(char c);
/////////////////////황금열쇠////////////////////////////

/////////////////////이벤트 칸 비우기////////////////////
void clear_event();
/////////////////////이벤트 칸 비우기////////////////////

/////////////////////플레이어 상황판/////////////////////
void show_playerstat(int i);
/////////////////////플레이어 상황판/////////////////////

/////////////////////땅사기, 건물짓기////////////////////
void structure(int i);
////////////////////땅사기, 건물짓기.////////////////////

/////////////////////정전////////////////////////////////
void blackout(int i);
/////////////////////정전////////////////////////////////

//////////////////////분실물 센터////////////////////////
void lost_and_found(int i);
//////////////////////분실물 센터////////////////////////



void take_over(int a, int b);//(플레이어넘버,땅번호)
////////////////////////인수인수인수///////////////////
void sell_the_land(int a, int b);//////////////돈없으면팔아야지//////////////
void bankrup(int a);/////////파산

int main()
{
	int i = 0, random;//playersnum 플레이어수 b는 그거 가로줄에서넘어가면 세로줄로 가도록 체크해주는거 rest는 가로줄에서움직이고남은칸수를 세로줄로 보내줌 random은 주사위수받아서넣어,j는 일단 보류, g는 엔터입력받는거


	///////////////////////////////////////////////게임 시작부분
	system("mode con cols=160 lines=100");

	gotoxy(25, 15);
	puts("게임 판 불러오는 중"); Sleep(500);
	gotoxy(25, 15);
	puts("게임 판 불러오는 중."); Sleep(500);
	gotoxy(25, 15);
	puts("게임 판 불러오는 중.."); Sleep(500);
	gotoxy(25, 15);
	puts("게임 판 불러오는 중..."); Sleep(500);
	gotoxy(25, 15);
	puts("게임 판 불러오는 중...."); Sleep(500);
	gotoxy(25, 15);
	puts("게임 판 불러오는 중....."); Sleep(500);


	system("cls");//불러오는 중 싹 지우기

	gotoxy(25, 15);
	puts("불러오기 완료!");
	Sleep(3000);///3초 지연

	system("cls");//로딩 화면 싹 지우기


	/////////////////////////////////////////////////게임시작부분

START:
	gotoxy(40, 20);
	printf("게임 인원 [ 최소 2명 | 최대 3명 ] : ");//게임인원을받아야지
	gotoxy(44, 22);
	printf("[2 또는 3 입력 후 Enter!]");
	gotoxy(76, 20);
	scanf_s("%d", &playersnum); 	//받았어!

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
		puts("☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★ AI 준비중입니다 ☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★"); Sleep(3000);
		gotoxy(20, 44);
		puts("☆★☆★☆★☆★☆★☆★☆★☆★☆★ 선택화면으로 넘어갑니다 ☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★"); Sleep(3000);
		system("cls");
		goto START;
	}
	system("cls");

	//////////////////////////////////////////////////

	gotoxy(0, 0);
	BOARD();//게임 판 불러오기
	show_playerstat(i);//플레이어 상태 출력해주는 함수
	settextcolor(21, 0);// 말 색깔, Enter 키를 눌러 주사위를 굴리세요 색깔 설정

	//////////////////////////////////////////////////자이제 말을움직여볼까

	for (i = 0; i<playersnum; i++)
	{
		gotoxy(4 + 2 * i, 5);
		if (i == 0)
		{
			settextcolor(3, 0);
			printf("▣");
		}

		else if (i == 1)
		{
			settextcolor(4, 0);
			printf("◈");
		}
		else
		{
			settextcolor(6, 0);
			printf("⊙");
		}
		coordinate[i][0] = 4 + 2 * i;
		coordinate[i][1] = 5;   //첫번째시작판에 말을놓고 말들의 위치를 저장함
	}

	//(4,6,8) (16,18,20) (28,30,32) (40,42,44) (52,54,56) (64,66,68) (76,78,80) (88,90,92) (100,102,104) : x
	//   5        11         17         23         29         35         41         47           53      : y

	//random이 주사위

	while ((players[0].count<20) && (players[1].count<20) && (players[2].count<20))//그냥 1넣어놈 게임끝날조건을 넣어야해
	{
		for (i = 0; i < playersnum; i++)//포문으로 말을돌리자
		{


			////        주사위
			///////////////////////
			if (i == 0)
				settextcolor(3, 0);
			else if (i == 1)
				settextcolor(4, 0);
			else
				settextcolor(6, 0);
			/////////////////////// 플레이어 마다 색깔 설정.

			if (wait[i] != 0)//감옥
			{
				wait[i]--;
				goto jump_dice;
			}//

			if (players[i].shuttle == 1)//셔틀버스
			{
				shuttle(want_to_go, i);
				goto jump_dice;
			}//

			clear_event();//이벤트창 지우기

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


			///급행열차칸인지 판단///
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

			fflush(stdin);//황금열쇠 버퍼 비우기(주사위 자동 굴리기 방지)
		}
	}

	for (i = 0; i<3; i++)
	{
		if (players[i].count > 20)
		{
			gotoxy(25, 35);
			printf("player %d 가 승리하였습니다.!!!", i); Sleep(5000);
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
			printf("게        "); Sleep(1000);
			settextcolor(24, 2);
			printf("임        "); Sleep(1000);
			settextcolor(15, 1);
			printf("오        "); Sleep(1000);
			settextcolor(14, 4);
			printf("버        "); Sleep(1000);



		}
	}

	////////////////////////////////////////////////////////////말움직이기




	return 0;

}

void BOARD(void)
{
	/////////////////////////////게임 판///////////////////////////////////
	settextcolor(26, 0); //판 테두리 색 설정
	puts("┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐"); Sleep(80);
	puts("│  START!  │  대치역  │  도곡역  │*황금열쇠*│  교대역  │ 압구정역 │  안국역  │ 충무로역 │분실물센터│"); Sleep(80);
	puts("│          │          │          │          │          │          │          │          │          │"); Sleep(80);
	puts("│   학교   │          │          │          │          │          │          │          │          │"); Sleep(80);
	puts("│          │          │          │          │          │          │          │          │          │"); Sleep(80);
	puts("│          │          │          │          │          │          │          │          │          │"); Sleep(80); //y 5
	puts("├─────┼─────┴─────┴─────┴─────┴─────┴─────┴─────┼─────┤"); Sleep(80);
	puts("│  안암역  │                                                                                  │ 왕십리역 │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                         [ 이 벤 트 창]                                           │          │"); Sleep(80);
	puts("│          │    ┌───────────────────────────┐                    │          │"); Sleep(80); //y 11
	puts("├─────┤    │                                                      │                    ├─────┤"); Sleep(80);
	puts("│  고대역  │    │                                                      │                    │  시청역  │"); Sleep(80);
	puts("│          │    │                                                      │                    │          │"); Sleep(80);
	puts("│          │    │                                                      │                    │          │"); Sleep(80);
	puts("│          │    │                                                      │                    │          │"); Sleep(80);
	puts("│          │    │                                                      │                    │          │"); Sleep(80); //y 17
	puts("├─────┤    │                                                      │                    ├─────┤"); Sleep(80);
	puts("│*황금열쇠*│    │                                                      │                    │*황금열쇠*│"); Sleep(80);
	puts("│          │    └───────────────────────────┘                    │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80); // y 23
	puts("├─────┤                                                                                  ├─────┤"); Sleep(80);
	puts("│  보문역  │                                                                                  │  삼성역  │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80); // y 29
	puts("├─────┤                                                                                  ├─────┤"); Sleep(80);
	puts("│  월곡역  │                                                                                  │  강남역  │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80); // y 35
	puts("├─────┤                                                                                  ├─────┤"); Sleep(80);
	puts("│  신당역  │                                                                                  │  선릉역  │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80); // y 41
	puts("├─────┤                                                                                  ├─────┤"); Sleep(80);
	puts("│  약수역  │                                                                                  │ 신도림역 │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80);
	puts("│          │                                                                                  │          │"); Sleep(80); // y 47
	puts("├─────┼─────┬─────┬─────┬─────┬─────┬─────┬─────┼─────┤"); Sleep(80);
	puts("│ 급행열차 │  수원역  │*황금열쇠*│  종각역  │  서울역  │ 동대문역 │ 노량진역 │  회기역  │ 정전사고 │"); Sleep(80);
	puts("│          │          │          │          │          │          │          │          │          │"); Sleep(80);
	puts("│          │          │          │          │          │          │          │          │          │"); Sleep(80);
	puts("│          │          │          │          │          │          │          │          │          │"); Sleep(80);
	puts("│          │          │          │          │          │          │          │          │          │"); Sleep(80); // y 53
	puts("└─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘"); Sleep(80);


	settextcolor(4, 0);//판 가운데 텍스트 색깔 변경
	gotoxy(48, 27);
	printf("등 ");
	settextcolor(6, 0);
	printf("교 ");
	settextcolor(7, 0);
	printf("마 ");
	settextcolor(8, 0);
	printf("블 ");

	gotoxy(42, 7);// 플레이어 말표시 ( 플레이어 수에 따라 출력이 달라진다)
	if (playersnum == 2)
	{
		settextcolor(3, 0);
		printf("P1 : ▣  ");

		settextcolor(4, 0);
		printf("P2 : ◈  ");
	}
	else if (playersnum == 3)
	{
		settextcolor(3, 0);
		printf("P1 : ▣  ");

		settextcolor(4, 0);
		printf("P2 : ◈  ");

		settextcolor(6, 0);
		printf("P3 : ⊙  ");
	}





	/////////////////////////////게임 판//////////////////////////////////
	/////////////////////////스테이터스 창////////////////////////////////

	BOARD_Status();
}

void BOARD_Status(void)
{
	settextcolor(15, 0);
	gotoxy(120, 0);
	puts("< 스테이터스 >");

	if (playersnum == 2)
	{
		gotoxy(114, 1);
		settextcolor(8, 0);
		puts("┌──────────┐"); Sleep(80); gotoxy(114, 2);
		puts("│      Player1       │"); Sleep(80); gotoxy(114, 3);
		puts("│                    │"); Sleep(80); gotoxy(114, 4);
		puts("│                    │"); Sleep(80); gotoxy(114, 5);
		puts("│                    │"); Sleep(80); gotoxy(114, 6);
		puts("│                    │"); Sleep(80); gotoxy(114, 7);
		puts("│                    │"); Sleep(80); gotoxy(114, 8);
		puts("└──────────┘"); Sleep(80); gotoxy(114, 9);
		puts("┌──────────┐"); Sleep(80); gotoxy(114, 10);
		puts("│      Player2       │"); Sleep(80); gotoxy(114, 11);
		puts("│                    │"); Sleep(80); gotoxy(114, 12);
		puts("│                    │"); Sleep(80); gotoxy(114, 13);
		puts("│                    │"); Sleep(80); gotoxy(114, 14);
		puts("│                    │"); Sleep(80); gotoxy(114, 15);
		puts("│                    │"); Sleep(80); gotoxy(114, 16);
		puts("└──────────┘"); Sleep(80); gotoxy(114, 17);
	}
	else if (playersnum == 3)
	{
		gotoxy(114, 1);
		settextcolor(8, 0);
		puts("┌──────────┐"); Sleep(80); gotoxy(114, 2);
		puts("│      Player1       │"); Sleep(80); gotoxy(114, 3);
		puts("│                    │"); Sleep(80); gotoxy(114, 4);
		puts("│                    │"); Sleep(80); gotoxy(114, 5);
		puts("│                    │"); Sleep(80); gotoxy(114, 6);
		puts("│                    │"); Sleep(80); gotoxy(114, 7);
		puts("│                    │"); Sleep(80); gotoxy(114, 8);
		puts("└──────────┘"); Sleep(80); gotoxy(114, 9);
		puts("┌──────────┐"); Sleep(80); gotoxy(114, 10);
		puts("│      Player2       │"); Sleep(80); gotoxy(114, 11);
		puts("│                    │"); Sleep(80); gotoxy(114, 12);
		puts("│                    │"); Sleep(80); gotoxy(114, 13);
		puts("│                    │"); Sleep(80); gotoxy(114, 14);
		puts("│                    │"); Sleep(80); gotoxy(114, 15);
		puts("│                    │"); Sleep(80); gotoxy(114, 16);
		puts("└──────────┘"); Sleep(80); gotoxy(114, 17);
		puts("┌──────────┐"); Sleep(80); gotoxy(114, 18);
		puts("│      Player3       │"); Sleep(80); gotoxy(114, 19);
		puts("│                    │"); Sleep(80); gotoxy(114, 20);
		puts("│                    │"); Sleep(80); gotoxy(114, 21);
		puts("│                    │"); Sleep(80); gotoxy(114, 22);
		puts("│                    │"); Sleep(80); gotoxy(114, 23);
		puts("│                    │"); Sleep(80); gotoxy(114, 24);
		puts("└──────────┘"); Sleep(80); gotoxy(114, 25);
	}

	gotoxy(115, 27); printf("┌───리스트───┐");
	gotoxy(115, 28); printf("1. 대치역");
	gotoxy(115, 29); printf("2. 도곡역"); settextcolor(14, 0);
	gotoxy(115, 30); printf("3. *황금열쇠*"); settextcolor(8, 0);
	gotoxy(115, 31); printf("4. 교대역");
	gotoxy(115, 32); printf("5. 압구정역");//5
	gotoxy(115, 33); printf("6. 안국역");
	gotoxy(115, 34); printf("7. 충무로역");
	gotoxy(115, 35); printf("8. 왕십리역");
	gotoxy(115, 36); printf("9. 시청역"); settextcolor(14, 0);
	gotoxy(115, 37); printf("10. *황금열쇠*"); settextcolor(8, 0);//10 
	gotoxy(115, 38); printf("11. 삼성역");
	gotoxy(115, 39); printf("12. 강남역");
	gotoxy(115, 40); printf("13. 선릉역");
	gotoxy(115, 41); printf("14. 신도림역");
	gotoxy(115, 42); printf("15. 회기역");//15
	gotoxy(115, 43); printf("16. 노량진역");
	gotoxy(115, 44); printf("17. 동대문역");
	gotoxy(115, 45); printf("18. 서울역");
	gotoxy(115, 46); printf("19. 종각역"); settextcolor(14, 0);
	gotoxy(115, 47); printf("20. *황금열쇠*"); settextcolor(8, 0);//20
	gotoxy(115, 48); printf("21. 수원역");
	gotoxy(115, 49); printf("22. 약수역");
	gotoxy(115, 50); printf("23. 신당역");
	gotoxy(115, 51); printf("24. 월곡역");
	gotoxy(115, 52); printf("25. 보문역"); settextcolor(14, 0);//25
	gotoxy(115, 53); printf("26. *황금열쇠*"); settextcolor(8, 0);
	gotoxy(115, 54); printf("27. 고대역");
	gotoxy(115, 55); printf("28. 안암역");//28
	gotoxy(115, 56); printf("└─────────┘");

	settextcolor(14, 0);
	gotoxy(110, 58); printf("ㅱ 황금열쇠 부분으로는 가지않는 것이 좋습니다 ㅱ");
}

void show_playerstat(int i)
{
	for (i = 0; i < playersnum; i++)
	{
		if (dead == i)
		{
			gotoxy(116, 3 + 8 * i);
			printf("파산");
		}
		else
			gotoxy(116, 3 + 8 * i); settextcolor(14, 0);
		printf("보유한 돈 : %d 만원", players[i].money);
		gotoxy(116, 4 + 8 * i);
		printf("소유한 땅 : %d 개", players[i].land_possess);
		gotoxy(116, 5 + 8 * i);
		printf("바퀴 수 : %d", players[i].count);
		Sleep(150);
	}
}

////////////////////////////////////주사위///////////////////////////////////////////

int dice(int i)
{
	int g, random;

	gotoxy(20, 12);
	printf("플레이어 %d 의 턴입니다!             \n", i + 1);
	gotoxy(20, 15);
	printf("                                                     ");
	gotoxy(20, 13);
	srand(time(NULL));
	random = rand() % 6 + 1;
	printf("Enter 키를 눌러 주사위를 굴리세요.");

	fflush(stdin);//버퍼 삭제

	scanf_s("%c", &g);  //enter 입력받음, 혹시 다른거 입력되면 지워버리기
	gotoxy(20, 15);
	printf("주사위 눈금 수 : %d (Enter키를 누르면 말이 이동합니다)", random);
	gotoxy(20, 16);
	scanf_s("%c", &g); //enter를 누르면 말이 이동하기 시작
	gotoxy(20, 16);

	Sleep(1000);//1초 쉬어야 주사위가 난수가 됨.

	return random;
}

///////////////////////////////////말 움직이기/////////////////////////////////

void player_move(int i, int random)
{
	int rest;
	if (dead == i);
	else
	{
		while (random >= 1)
		{

			if (a[i] == 1)//a[i]가 1이면 위쪽 가로줄에서 말이움직여야하는거
			{
				if (coordinate[i][0] + 12 > 100 + 2 * i)// 만약 그줄의 칸수를 넘어간다면 
				{
					rest = (coordinate[i][0] + 12 - (100 + 2 * i)) / 12;//넘어가는수만큼저장하고
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");
					gotoxy(100 + 2 * i, 5 + (6 * rest));
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}
					//printf("%d", playerscharc[i]);//
					Sleep(300);
					coordinate[i][0] = 100 + 2 * i;
					coordinate[i][1] = 5 + (6 * rest);
					a[i] = 2;
					if (coordinate[i][1] == 53)//맨끝이면
					{
						coordinate[i][0] = 100 + 2 * i;
						coordinate[i][1] = 53;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 3;
					}
				}
				else//그줄의 칸수를 안넘어간다면
				{
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");
					gotoxy(coordinate[i][0] + 12, coordinate[i][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}
					//printf("%d", playerscharc[i]);//
					Sleep(300);
					coordinate[i][0] = coordinate[i][0] + 12;//좌표저장

					if (coordinate[i][0] == 100 + 2 * i)//만약끝칸에왓다면
					{
						coordinate[i][0] = 100 + 2 * i;//말들이 밑으로내려갈수잇도록 좌표를바꾸고
						coordinate[i][1] = 5;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 2;//맨끝이면
					}
				}
			}
			else if (a[i] == 2)
			{
				if (coordinate[i][1] + 6 > 53)//지우기
				{
					rest = (coordinate[i][1] + 6 - 53) / 6;
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");//지우기
					gotoxy(100 + 2 * i - (12 * rest), 53);
					/////////////////말판 출력
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}
					/////////////////
					//printf("%d", playerscharc[i]);//
					Sleep(500);
					coordinate[i][0] = 100 + 2 * i - (12 * rest);
					coordinate[i][1] = 53;
					a[i] = 3;
					if (coordinate[i][0] == 4 + 2 * i)//맨끝이면
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}
					//printf("%d", playerscharc[i]);//
					Sleep(300);
					coordinate[i][1] = coordinate[i][1] + 6;//안넘어갈때

					if (coordinate[i][1] == 53)//맨끝이면
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
				if ((coordinate[i][0] - 12) < (4 + 2 * i))//넘어간다면
				{
					rest = (coordinate[i][0] - 12 - (4 + 2 * i)) / 12;
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");
					gotoxy(4 + 2 * i, 53 + (6 * rest));
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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

						a[i] = 1;//맨끝이면
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}

					Sleep(300);
					coordinate[i][0] = coordinate[i][0] - 12;//안넘어갈때

					if (coordinate[i][0] == 4 + 2 * i)//맨끝이면
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
				if (coordinate[i][1] - 6 < 3)//넘어간다묜
				{
					rest = -((coordinate[i][1] - 6 - 5)) / 6;
					gotoxy(coordinate[i][0], coordinate[i][1]);
					printf("  ");//지우기
					gotoxy(4 + 2 * i + (12 * rest), 5);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}
					//printf("%d", playerscharc[i]);//
					Sleep(300);
					coordinate[i][0] = 4 + 2 * i + (12 * rest);
					coordinate[i][1] = 5;
					a[i] = 1;
					players[i].count++;
					if (coordinate[i][0] == 100 + 2 * i)//만약끝칸에왓다면
					{
						coordinate[i][0] = 100 + 2 * i;//말들이 밑으로내려갈수잇도록 좌표를바꾸고
						coordinate[i][1] = 5;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 2;//맨끝이면
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}
					//printf("%d", playerscharc[i]);//
					Sleep(300);
					coordinate[i][1] = coordinate[i][1] - 6;//안넘어갈때

					if (coordinate[i][1] == 5)
					{
						coordinate[i][0] = 4 + 2 * i;
						coordinate[i][1] = 5;
						gotoxy(coordinate[i][0], coordinate[i][1]);

						a[i] = 1;//맨끝이면
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
	puts("┌─────────────────────────────────┐"); Sleep(80); gotoxy(20, 31);
	puts("│                             황금열쇠                             │"); Sleep(80); gotoxy(20, 32);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 33);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 34);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 35);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 36);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 37);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 38);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 39);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 40);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 41);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 42);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 43);
	puts("│                                                                  │"); Sleep(80); gotoxy(20, 44);
	puts("└─────────────────────────────────┘"); Sleep(80); gotoxy(20, 45);


	switch (key)
	{
	case 1: gotoxy(23, 34); printf("<생일!>"); Sleep(2000);
		gotoxy(23, 36); printf("생일 축하합니다! 짝짝"); Sleep(2000);
		gotoxy(23, 38); printf("친구들이 생일 선물을 들고 옵니다."); Sleep(2000);
		gotoxy(23, 40); printf("\"모두에게 20만원씩 받으세요\""); Sleep(2000);

		for (j = 0; j < playersnum; j++)
		{
			if (j == i)
				players[j].money += (playersnum - 1) * 20;
			else
				players[j].money -= 20;
		}

		break;

	case 2: gotoxy(23, 34); printf("<급행열차>"); Sleep(2000);
		gotoxy(23, 36); printf("이번 열차는 급행열차입니다."); Sleep(2000);
		gotoxy(23, 38); printf("급행열차를 타고 원하는 곳으로 이동하세요."); Sleep(2000);
		gotoxy(23, 40); printf("\"급행열차로 ㄱㄱ\""); Sleep(2000);

		gotoxy(coordinate[i][0], coordinate[i][1]);
		printf("  ");
		coordinate[i][0] = 4 + 2 * i;
		coordinate[i][1] = 53;
		a[i] = 4;
		gotoxy(coordinate[i][0], coordinate[i][1]);
		if (i == 0)
		{
			settextcolor(3, 0);
			printf("▣");
		}
		else if (i == 1)
		{
			settextcolor(4, 0);
			printf("◈");
		}
		else
		{
			settextcolor(6, 0);
			printf("⊙");
		}

		players[i].shuttle = 1;
		break;

	case 3: gotoxy(23, 34); printf("<충전기>"); Sleep(2000);
		gotoxy(23, 36); printf("당신은 학교에 충전기를 두고 왔습니다."); Sleep(2000);
		gotoxy(23, 38); printf("학교로 다시 돌아가세요."); Sleep(2000);
		gotoxy(23, 40); printf("\"학교로 ㄱㄱ\""); Sleep(2000);

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
				printf("▣");
			}
			else if (i == 1)
			{
				settextcolor(4, 0);
				printf("◈");
			}
			else
			{
				settextcolor(6, 0);
				printf("⊙");
			}
		}
		// 이동 & 돈

		break;

	case 4: gotoxy(23, 34); printf("<밤샘코딩>"); Sleep(2000);
		gotoxy(23, 36); printf("당신은 어제 밤샘코딩을 하여 매우 피곤합니다."); Sleep(2000);
		gotoxy(23, 38); printf("지하철에서 깜빡 잠이 들어 역을 지나치고 말았습니다."); Sleep(2000);
		gotoxy(23, 40); printf("\"반대편 지하철을 타고 돌아가기 위해 3턴 소요\""); Sleep(2000);

		wait[i] = 3;

		break;

	case 5: gotoxy(23, 34); printf("<잡상인>"); Sleep(2000);
		gotoxy(23, 36); printf("지하철 안에서 잡상인을 만났습니다."); Sleep(2000);
		gotoxy(23, 38); printf("불쌍해 보여서 물건을 샀지만 역시 한 번 쓰고 나니 망가졌습니다."); Sleep(2000);
		gotoxy(23, 40); printf("돈만 버렸습니다."); Sleep(2000);
		gotoxy(23, 42); printf("\"20만원을 내세요\""); Sleep(2000);

		players[i].money -= 20;

		break;

	case 6: gotoxy(23, 34); printf("<짐 드신 할머니>"); Sleep(2000);
		gotoxy(23, 36); printf("당신은 지하철에서 무거운 짐을 드신 할머니를 만났습니다."); Sleep(2000);
		gotoxy(23, 38); printf("(1 : 도와준다, 2 : 무시한다) : "); Sleep(2000);
		scanf_s("%d", &j);
		gotoxy(23, 40);
		if (j == 1)
		{
			printf("당신은 할머니를 도와주었습니다."); Sleep(2000);
			gotoxy(23, 42); printf("당신은 1턴을 소모했지만 할머니에게 보답을 받았습니다."); Sleep(2000);

			wait[i] = 1;

			players[i].money += 30;
		}
		else if (j == 2)
		{
			printf("당신은 할머니를 무시했습니다."); Sleep(2000);
			gotoxy(23, 42); printf("아무 일도 일어나지 않았습니다."); Sleep(2000);
		}

		break;

	case 7: gotoxy(23, 34); printf("<황금 열쇠에 넣을 게 없어>"); Sleep(2000);
		gotoxy(23, 36); printf("아 넣을 게 없다"); Sleep(2000);
		gotoxy(23, 38); printf("돈이나 가져 가세요."); Sleep(2000);

		money = (rand() % 5 + 1) * 10;

		gotoxy(23, 40); printf("\"돈 %d만원 받음>\"", money); Sleep(2000);

		players[i].money += money;

		break;

	case 8: gotoxy(23, 34); printf("<조별과제>"); Sleep(2000);
		gotoxy(23, 36); printf("조별과제를 하는데 팀원이 잠수를 탔습니다."); Sleep(2000);
		gotoxy(23, 38); printf("팀원 중 하나의 말로 이동해서 잡아오세요."); Sleep(2000);
		gotoxy(23, 40); printf("\"다른 플레이어 중 하나의 말이 있는 곳으로 가세요\""); Sleep(2000);
		gotoxy(23, 42);
		printf("(");
		settextcolor(3, 0); printf("1 : ▣ ");
		settextcolor(4, 0); printf("2 : ◈ ");
		settextcolor(6, 0);
		if (playersnum == 3)
		{
			printf("3 : ⊙");
		}
		printf(") 원하는 말은? : ");
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
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
			}
		}

		Sleep(1000);

		break;

	case 9: gotoxy(23, 34); printf("<앞으로 가세요>"); Sleep(2000);
		gotoxy(23, 36); printf("앞으로 갑니다. 3칸 정도"); Sleep(2000);
		gotoxy(23, 38); printf("\"앞으로 3칸 ㄱㄱ\""); Sleep(2000);

		player_move(i, 3);

		break;

	case 10: gotoxy(23, 34); printf("열차에서 소매치기를 만났습니다."); Sleep(2000);
		gotoxy(23, 36); printf("교통 카드를 잃어버렸습니다."); Sleep(2000);
		gotoxy(23, 38); printf("분실물 센터로 가서 교통카드를 찾아보세요.");
		gotoxy(coordinate[i][0], coordinate[i][1]);
		printf("  ");
		coordinate[i][0] = 100 + 2 * i;
		coordinate[i][1] = 5;
		a[i] = 2;
		gotoxy(coordinate[i][0], coordinate[i][1]);
		if (i == 0)
		{
			settextcolor(3, 0);
			printf("▣");
		}
		else if (i == 1)
		{
			settextcolor(4, 0);
			printf("◈");
		}
		else
		{
			settextcolor(6, 0);
			printf("⊙");
		}
		break;

	case 11:gotoxy(23, 34); printf("지각의 위험에 처했습니다."); Sleep(2000);
		gotoxy(23, 36); printf("택시를 타야합니다."); Sleep(2000);
		gotoxy(23, 38); printf("택시비를 지불하고 안암역으로 가세요.(택시비:2만원)"); Sleep(2000);
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
			printf("▣");
		}
		else if (i == 1)
		{
			settextcolor(4, 0);
			printf("◈");
		}
		else
		{
			settextcolor(6, 0);
			printf("⊙");
		}
		break;


	default: gotoxy(23, 34); printf("버그가 탐지되었습니다."); Sleep(2000);
		gotoxy(23, 36); printf("프로그램을 종료합니다."); Sleep(2000);
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
	int rest;// 음 이건 28번 땅을 넘어가는 갯수 구해서 이동시킬라고 설정한거.
	int j, now;//for 문 제어변수
	int re_coordinate[28][2];
	double shuttleposition = 21.5;


	/////////// 좌표랑 칸 1대1 대응시키기 //////////
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

	////////// 좌표랑 칸 1대1 대응시키기 //////////

	clear_event(); //이벤트칸 비우기

	gotoxy(20, 12);
	printf("가고싶은 곳의 번호를 입력하세요 (1 ~ 28) : "); scanf_s("%d", &want_to_go);

	for (j = 0; j < 28; j++)
	{
		if ((coordinate[i][0] == (land_pos[j][0] + 2 * i)) && (coordinate[i][1] == land_pos[j][1]))
		{
			now = j;  //현재 위치
			break;
		}
	}



	if (now - (want_to_go - 1)>0)   //출발점을 지나는지 안지나는지 판별하기 위해
	{
		if ((want_to_go != 3) && (want_to_go != 10) && (want_to_go != 20) && (want_to_go != 26))  //황금열쇠칸의 경우 예외로 침
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
		printf("▷▶대치역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문
		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}


				//////gotoxy(coordinate[i][0], coordinate[i][1]);//말의 좌표를 땅 좌표로 설정 후 옮기기

				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}


				//////gotoxy(coordinate[i][0], coordinate[i][1]);//말의 좌표를 땅 좌표로 설정 후 옮기기

				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i;
			coordinate[i][1] = 11;
		}

		a[i] = 1; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);


		clear_event();

		structure(i);
		break;


	case 2:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶도곡역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문
		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 3:
		clear_event();
		gotoxy(20, 12);
		settextcolor(4, 0);
		printf("욕심부리면 안됩니다^^* 열차권 압수헿"); Sleep(3000);//안내문

		players[i].shuttle = 0;
		break;
	case 4:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶교대역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 5:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶압구정역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문


		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 6:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶안국역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문


		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 7:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶충무로역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;

	case 8:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶왕십리역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			coordinate[i][0] = 4 + 2 * i; ///////////////////이건 왼쪽 줄 칸 경우임 이따고쳐라
			coordinate[i][1] = 11;
		}

		a[i] = 2; ///////////////////////
		players[i].count++;
		players[i].money += 30;
		gotoxy(20, 12);
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 9:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶시청역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 10:
		clear_event();
		gotoxy(20, 12);
		settextcolor(4, 0);
		printf("욕심부리면 안됩니다^^* 열차권 압수헿"); Sleep(3000);//안내문

		players[i].shuttle = 0;
		break;
	case 11:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶삼성역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 12:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶강남역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 13:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶선릉역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 14:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶신도림역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;

	case 15:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶회기역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 16:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶노량진역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 17:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶동대문역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 18:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶서울역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 19:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶종각역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;
	case 20:
		clear_event();
		gotoxy(20, 12);
		settextcolor(4, 0);
		printf("욕심부리면 안됩니다^^*"); Sleep(3000);//안내문

		players[i].shuttle = 0;

		break;
	case 21:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶수원역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
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
		printf("ㅸ 통학 경로 정복금 30만원이 지급됩니다 ㅸ");
		Sleep(2000);
		clear_event();

		structure(i);
		break;

	case 22:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷약수역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
			printf("  ");// 셔틀칸 지우기
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//말 지우기
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
		printf("▷▶신당역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
			printf("  ");// 셔틀칸 지우기
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//말 지우기
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
		printf("▷▶월곡역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
			printf("  ");// 셔틀칸 지우기
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//말 지우기
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
		printf("▷▶보문역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
			printf("  ");// 셔틀칸 지우기
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//말 지우기
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
		printf("욕심부리면 안됩니다^ㅗ^!"); Sleep(3000);//안내문

		players[i].shuttle = 0;

		break;
	case 27:
		clear_event();
		gotoxy(20, 12);
		settextcolor(26, 0);
		printf("▷▶고대역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
			printf("  ");// 셔틀칸 지우기
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//말 지우기
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
		printf("▷▶안암역으로 이동합니다~_~▷▶"); Sleep(3000);//안내문

		gotoxy(4 + 2 * i, 53);
		printf("  ");//셔틀버스쪽 말 삭제

		if (shuttleposition > want_to_go)
		{
			for (j = 22; j < 27; j++)
			{
				gotoxy(4 + 2 * i, 53);//셔틀버스 말 지우기
				printf("  ");
				gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);//말 지우기
				printf("  ");
				gotoxy(re_coordinate[j + 1][0] + 2 * i, re_coordinate[j + 1][1]);
				if (i == 0)
				{
					settextcolor(3, 0);
					printf("▣");
				}
				else if (i == 1)
				{
					settextcolor(4, 0);
					printf("◈");
				}
				else
				{
					settextcolor(6, 0);
					printf("⊙");
				}
				Sleep(100);
			}
			gotoxy(re_coordinate[27][0] + 2 * i, re_coordinate[27][1]);
			printf("  ");////////////28번째 칸 말 지우기
			for (j = 0; j < rest; j++) //rest 이동시키기
			{
				if (j == 0)
				{
					gotoxy(re_coordinate[0][0] + 2 * i, re_coordinate[0][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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
			printf("  ");// 셔틀칸 지우기
			for (j = 21; j < want_to_go; j++)
			{
				if (j == 21)
				{
					gotoxy(re_coordinate[21][0] + 2 * i, re_coordinate[21][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
					}

					Sleep(100);
				}
				else
				{
					gotoxy(re_coordinate[j - 1][0] + 2 * i, re_coordinate[j - 1][1]);//말 지우기
					printf("  ");
					gotoxy(re_coordinate[j][0] + 2 * i, re_coordinate[j][1]);
					if (i == 0)
					{
						settextcolor(3, 0);
						printf("▣");
					}
					else if (i == 1)
					{
						settextcolor(4, 0);
						printf("◈");
					}
					else
					{
						settextcolor(6, 0);
						printf("⊙");
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

		if ((coordinate[i][0] == (land_pos[j][0] + 2 * i)) && (coordinate[i][1] == land_pos[j][1])) //찾았다면
		{
			if (j == 2 || j == 9 || j == 19 || j == 25)  //그 땅이 황금열쇠 땅이라면
				break; //브레이크

			if (lands[j].belongs_who == 0) //그 땅의 소유주가 없다면
			{
				if (players[i].money >= lands[j].land_price)
				{
					fflush(stdin);
					gotoxy(25, 35);
					printf("땅을 사시겠습니까? (yes:Y  no:N)  : ");
					scanf_s("%c", &answer);
					if ((answer == 'Y') || (answer == 'y'))
					{
						lands[j].belongs_who = i + 1; //이 지역은 player의 땅입니다 하고 선언
						players[i].money -= lands[j].land_price; //땅을 산다고 했을 경우 소지금에서 그 지역의 대지료 만큼 빠져나감
						players[i].land_possess++;

						//////////////////////////////////땅 샀을때 리스트 색깔 변경///////////////////////////////////
						if (j == 0)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("1. 대치역");
						}
						if (j == 1)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("2. 도곡역");
						}
						if (j == 3)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("4. 교대역");
						}
						if (j == 4)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("5. 압구정역");
						}
						if (j == 5)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("6. 안국역");
						}
						if (j == 6)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("7. 충무로역");
						}
						if (j == 7)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("8. 왕십리역");
						}
						if (j == 8)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("9. 시청역");
						}
						if (j == 10)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("11. 삼성역");
						}
						if (j == 11)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("12. 강남역");
						}
						if (j == 12)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("13. 선릉역");
						}
						if (j == 13)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("14. 신도림역");
						}
						if (j == 14)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("15. 회기역");
						}
						if (j == 15)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("16. 노량진역");
						}
						if (j == 16)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("17. 동대문역");
						}
						if (j == 17)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("18. 서울역");
						}
						if (j == 18)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("19. 종각역");
						}
						if (j == 20)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("21. 수원역");
						}
						if (j == 21)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("22. 약수역");
						}
						if (j == 22)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("23. 신당역");
						}
						if (j == 23)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("24. 월곡역");
						}
						if (j == 24)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("25. 보문역");
						}
						if (j == 26)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("27. 고대역");
						}
						if (j == 27)
						{
							if (i == 0)
								settextcolor(3, 0);
							else if (i == 1)
								settextcolor(4, 0);
							else
								settextcolor(6, 0);

							gotoxy(115, 28 + j); printf("28. 안암역");
						}


						////////////////////////////////////땅 샀을때 리스트 색깔 변경//////////////////////////////////
						//땅 샀을때 땅 색깔 바꾸는거 필요
					}
					else if ((answer == 'N') || (answer == 'n'))
					{
						gotoxy(25, 36);
						printf("땅을 사지 않았습니다. 다음 턴으로 넘어갑니다.");
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
					printf("소지금이 부족하여 땅을 살 수 없습니다.");
					gotoxy(coordinate[i][0], coordinate[i][1]);
					Sleep(1000);
					gotoxy(25, 35);
					printf("                                                           ");
					gotoxy(25, 36);
					printf("                                                           ");
				}
			}
			else if (lands[j].belongs_who == i + 1) //그 땅의 소유주가 플레이어라면
			{
				k = 0;
				fflush(stdin);


				if (lands[j].building == 0)
				{
					gotoxy(25, 35);
					printf("전철을 건설하시겠습니까? (yes:Y  no:N)  : ");
					scanf_s("%c", &answer);
					gotoxy(25, 35);
					printf("                                                          ");
					if ((answer == 'Y') || (answer == 'y'))
					{
						lands[j].building++;
						players[i].money -= (lands[j].land_price)*0.5;   //소지금에서 돈이 나감
						lands[j].land_price *= 1.5;  // 대지료는 상승.
						gotoxy(coordinate[i][0] - i, coordinate[i][1] - 3);
						printf("N");
						gotoxy(coordinate[i][0], coordinate[i][1]);
					}
				}
				else if (lands[j].building == 1)
				{
					gotoxy(25, 35);
					printf("전철을 고속열차로 업그레이드 하시겠습니까? (yes:Y  no:N)  : ");
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
					printf("고속열차를 KTX로 업그레이드 하시겠습니까? (yes:Y  no:N)  : ");
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
					printf("더이상 업그레이드할수없습니다. 턴을종료합니다."); Sleep(5000);
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
				if (players[i].money < lands[j].land_price)  //소지금보다 내야 할 돈이 더 많은 경우
				{

					gotoxy(25, 35);
					printf("                           ");
					gotoxy(25, 35);
					printf("보유하신현금이부족합니다"); Sleep(5000);
					gotoxy(25, 35);
					printf("                       ");
					gotoxy(25, 35);
					printf("원하시는 번호를 입력하세요");
					gotoxy(25, 36);
					printf("                      ");
					gotoxy(25, 36);
					printf("1.매각 2.파산");
					gotoxy(25, 37);
					scanf_s("%d", &k);
					if (k == 1)
					{
						sell_the_land(i, j);
					}
					else
					{


						gotoxy(25, 35);
						printf("플레이어 %d는 파산하였습니다.", i + 1);
						bankrup(i);

					}

				}

				else
				{
					players[i].money -= lands[j].land_price;   //자기 땅이 아닌경우 돈냄
					gotoxy(25, 35);
					printf("이 땅은 player %d의 땅입니다. 통행료를 지불하세요.", lands[j].belongs_who);
					players[lands[j].belongs_who - 1].money += lands[j].land_price;
					Sleep(2000);
					gotoxy(25, 35);
					printf("                                                           ");
					gotoxy(25, 35);
					printf("인수하시겠습니까??(인수비용 : )(yes:Y  no:N) :");
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

	gotoxy(coordinate[i][0], coordinate[i][1]); //다시 돌아가서 주사위를 굴리자

}

////////***구매가격***/////////   땅 가격을  x라고 했을때
//별장 0.5x    빌딩 1x    호텔 2x//


///////***통행료***////////
///별장 1.5x   빌딩 2x    호텔  3x///

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
		printf("분실물 센터에서 D.S.L.R 캐무라를 습득 ㆅ !"); Sleep(2000);
		gotoxy(20, 13);
		printf("중고상에 되팝시다 : 80만원 GET ㆅ"); Sleep(2000);
		clear_event();

		players[i].money += 80;
		fflush(stdin);
		break;
	case 2:
		gotoxy(20, 12);
		printf("분실물 센터에서 우산을..... 습득 ㆅ !"); Sleep(2000);
		gotoxy(20, 13);
		printf("내다팔 곳이 없습니다 : 그냥 자기가씁니다.."); Sleep(2000);
		clear_event();

		players[i].money += 0;
		fflush(stdin);
		break;
	case 3:
		gotoxy(20, 12);
		printf("분실물 센터에서 누군가의 지갑을 습득 ㆅ !"); Sleep(2000);
		gotoxy(20, 13);
		printf("지갑 안에 만원이 들어있습니다.."); Sleep(2000);
		gotoxy(20, 14);
		printf("꺼내가시겠습니까? (Y) or (N) :"); scanf_s("%c", &answer);

		if ((answer == 'Y') || (answer == 'y'))
		{
			gotoxy(20, 15);
			printf("절도 현행범으로 벌금 20만원을 냅니다 ㆅ"); Sleep(2000);
			players[i].money -= 20;
		}
		else if ((answer == 'N') || (answer == 'n'))
		{
			gotoxy(20, 15);
			printf("정직한 사람 : 보상 30만원을 받습니다 ㆅ"); Sleep(2000);
			players[i].money += 30;
		}

		clear_event();
		fflush(stdin);

		break;
	case 4:
		gotoxy(20, 12);
		printf("분실물 센터에서 새내기 C언어 전공서를 습득 ㆅ !"); Sleep(2000);
		gotoxy(20, 13);
		printf("중고 서점 알라딘에 팝니다 : 2만원 GET ㆅ"); Sleep(2000);
		clear_event();

		players[i].money += 2;
		fflush(stdin);
		break;
	case 5:
		gotoxy(20, 12);
		printf("분실물 센터에서 하나 체크카드를 습득 ㆅ !"); Sleep(2000);
		gotoxy(20, 13);
		printf("인출기로 갑니다 ㆅ"); Sleep(2000);
		gotoxy(20, 14);
		printf("얼마를 꺼내가시겠습니까? (단위는 만원) :"); scanf_s("%d", &answerN);

		if (answerN > 50)
		{
			gotoxy(20, 15);
			printf("비양심적인 액수네요. 벌금형 40만원 ㆅ"); Sleep(2000);

			players[i].money -= 40;
		}
		else if (answerN <= 50)
		{
			gotoxy(20, 15);
			printf("몰래 돈을 잘 빼냈습니다 ㆅ"); Sleep(2000);

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
		printf("지하철이 정전되었습니다!"); Sleep(2000);
		gotoxy(25, 31);
		printf("3턴 동안 움직일 수 없습니다."); Sleep(2000);
		wait[i] = 3;
		gotoxy(25, 30);
		printf("                                 ");
		gotoxy(25, 31);
		printf("                                 ");
	}
}

void take_over(int a, int b)//(플레이어넘버,땅번호)
{
	int take_over_money = { 0 };

	if (lands[b].belongs_who == 0)
	{
		gotoxy(25, 35);
		printf("                                    ");
		gotoxy(25, 35);
		printf("소유주가없는땅은 인수할수없습니다^^"); Sleep(5000);
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

				//////////////////////////////////땅 샀을때 리스트 색깔 변경///////////////////////////////////
				if (b == 0)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("1. 대치역");
				}
				if (b == 1)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("2. 도곡역");
				}
				if (b == 3)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("4. 교대역");
				}
				if (b == 4)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("5. 압구정역");
				}
				if (b == 5)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("6. 안국역");
				}
				if (b == 6)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("7. 충무로역");
				}
				if (b == 7)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("8. 왕십리역");
				}
				if (b == 8)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("9. 시청역");
				}
				if (b == 10)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("11. 삼성역");
				}
				if (b == 11)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("12. 강남역");
				}
				if (b == 12)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("13. 선릉역");
				}
				if (b == 13)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("14. 신도림역");
				}
				if (b == 14)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("15. 회기역");
				}
				if (b == 15)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("16. 노량진역");
				}
				if (b == 16)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("17. 동대문역");
				}
				if (b == 17)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("18. 서울역");
				}
				if (b == 18)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("19. 종각역");
				}
				if (b == 20)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("21. 수원역");
				}
				if (b == 21)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("22. 약수역");
				}
				if (b == 22)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("23. 신당역");
				}
				if (b == 23)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("24. 월곡역");
				}
				if (b == 24)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("25. 보문역");
				}
				if (b == 26)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("27. 고대역");
				}
				if (b == 27)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("28. 안암역");
				}


				////////////////////////////////////땅 샀을때 리스트 색깔 변경//////////////////////////////////
				//땅 샀을때 땅 색깔 바꾸는거 필요
			}
			else
			{
				gotoxy(25, 35);
				printf("                                    ");
				gotoxy(25, 35);
				printf("소유주가없는땅은 인수할수없습니다^^"); Sleep(5000);
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

					gotoxy(115, 28 + b); printf("1. 대치역");
				}
				if (b == 1)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("2. 도곡역");
				}
				if (b == 3)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("4. 교대역");
				}
				if (b == 4)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("5. 압구정역");
				}
				if (b == 5)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("6. 안국역");
				}
				if (b == 6)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("7. 충무로역");
				}
				if (b == 7)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("8. 왕십리역");
				}
				if (b == 8)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("9. 시청역");
				}
				if (b == 10)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("11. 삼성역");
				}
				if (b == 11)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("12. 강남역");
				}
				if (b == 12)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("13. 선릉역");
				}
				if (b == 13)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("14. 신도림역");
				}
				if (b == 14)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("15. 회기역");
				}
				if (b == 15)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("16. 노량진역");
				}
				if (b == 16)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("17. 동대문역");
				}
				if (b == 17)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("18. 서울역");
				}
				if (b == 18)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("19. 종각역");
				}
				if (b == 20)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("21. 수원역");
				}
				if (b == 21)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("22. 약수역");
				}
				if (b == 22)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("23. 신당역");
				}
				if (b == 23)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("24. 월곡역");
				}
				if (b == 24)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("25. 보문역");
				}
				if (b == 26)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("27. 고대역");
				}
				if (b == 27)
				{
					if (a == 0)
						settextcolor(3, 0);
					else if (a == 1)
						settextcolor(4, 0);
					else
						settextcolor(6, 0);

					gotoxy(115, 28 + b); printf("28. 안암역");
				}


				////////////////////////////////////땅 샀을때 리스트 색깔 변경//////////////////////////////////
				//땅 샀을때 땅 색깔 바꾸는거 필요


			}
			else
			{
				gotoxy(25, 35);
				printf("                                    ");
				gotoxy(25, 35);
				printf("소유주가없는땅은 인수할수없습니다^^"); Sleep(5000);
				gotoxy(25, 35);
				printf("                                   ");

			}
		}
	}
}

void sell_the_land(int a, int b)
{

	players[a].money += lands[b].land_price;//환불완료
	lands[b] = lands_early[b];//초기화완료
}

void bankrup(int a)
{
	int b = 0;
	dead_cnt++;
	if (playersnum - 1 == dead_cnt)
	{
		gotoxy(25, 36);
		printf("축하합니다! 승리하셨습니다."); Sleep(5000);
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
		printf("게        "); Sleep(1000);
		settextcolor(24, 2);
		printf("임        "); Sleep(1000);
		settextcolor(15, 1);
		printf("오        "); Sleep(1000);
		settextcolor(14, 4);
		printf("버        "); Sleep(1000);
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
	}//땅초기화
}
