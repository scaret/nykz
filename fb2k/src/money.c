/* 思源湖 交易市场代码 2005.07.19 */
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "bbs.h"
#include "money.h"
//#ifdef ALLOWGAME
#define CHANGE_MAX	1000000

//pax@sjtubbs 05-07
//int  BANK_CURRDEPO_RATE[6]={3,7,11,15,19,24};//银行利率 
double  BANK_CURRDEPO_RATE[6]={2.5,5.25,8.5,12.0,16.0,20.0};//银行利率 
//2.5?两个月5.25？三个月8.25？四个月11.5
//5个月15？ 6个月18.75

double callrate=0.000133;
//int  CALL_RATE＝2;//活期利率
char mail_title[STRLEN]={0};
char mail_buffer[1024] = {0};//pax@sjtubbs 05 07 16
char bankdatetime[20];
char tmpbname[STRLEN];
//end by pax;





int inGame(void)
{
	FILE *fp;
	char buf[STRLEN];
	extern int NowInGame ;
	
	if (!strcmp("guest", currentuser.userid)) return 1;
	setuserfile(buf,"inGame");
	if(dashf(buf)){
		clear();
		move(12,18);
		prints("对不起，您已经有另外一个窗口在使用本站的游戏功能。\n");
		pressanykey();
		return 1;
	} 
	NowInGame = YEA;
	fp = fopen(buf,"w+");
	fclose(fp);
	return 0;
}

int gotomarket(char *title)
{
	if(inGame()==1) return 1;
	modify_user_mode(MARKET);
	clear();
	set_safe_record();
	move(2,0);
	prints("欢迎进入 [[32m%s[37m]....\n\n",title);
	return 0;
}

void outgame(void)
{
	char buf[STRLEN];
	extern int NowInGame ;
	setuserfile(buf,"inGame");
	if(dashf(buf))unlink(buf);
	NowInGame = NA;
}



int withdraw(void)
{
	char sysbuf[STRLEN];
	char msg[3];
	char money[7];
	int opt;
	int num_money;
	int id;
	time_t now;	
	//pax
	struct bankbill Bill;
	//struct callbill cBill;
	struct rate bankrate ;
	long size;
	struct stat kkk;
	char fname[80];
	int fd;
	int fd_rate;
	char length[3];
	int num_length;//定额存款的存款时间长度
	char BillNo[9];
	int num_bill;
	//end by pax*/
	
	FILE *fp;
	struct Money mdata;
	char buf[STRLEN]; 
	int result=0;
	id = getuser(currentuser.userid);
	
	// get game coin
	/*	sethomefile(buf,currentuser.userid,"game_money.dat");
	if(dashf(buf)) update_game_money();
	fp = fopen(buf,"r");
	if(fp==NULL) return 0;
	result = fread(&mdata,sizeof(mdata),1,fp);
	fclose(fp);
	*/
	// modified by pax 05-08-02 
	//没有游戏币纪录的ID自动创建游戏币文件
	sethomefile(buf,currentuser.userid,"game_money.dat");
	if(dashf(buf)) update_game_money();
	fp = fopen(buf,"r");
	
	if(fp==NULL) 
	{
		mdata.money=0;
	}
	else
	{
		result = fread(&mdata,sizeof(mdata),1,fp);
		fclose(fp);
	}
	
	// get end	
	
	if(gotomarket("思源湖大银行")) 
		return 0;
	prints("欢迎光临 思源湖大银行\n");
	sprintf(fname,"您目前携带现金 %d 元，银行帐户结余 %d 元，游戏币 %ld 元。\n", currentuser.money, currentuser.inbank, mdata.money);

	prints(fname);

		if ((currentuser.inbank<0) || (currentuser.money<0 ))
		{
			move(8,0);
			prints("对不起，您的金融数据异常，请和系统管理员联系！\n");
			pressanykey();
			outgame();
			return 0;
		}
//	prints("\n[1]活期存款 [2]活期取款 [3]购买游戏币 [4]出售游戏币 [5]定期存款 [6]定期取款\n");
	prints("\n[1]活期存款 [2]活期取款 [3]购买游戏币 [4]定期存款 [5]定期取款\n");
	
	getdata( 8, 0,"请输入操作:", msg, 2, DOECHO, /*NULL,*/ YEA);
	if((msg[0] == '\n') || (msg[0] == '\0'))
	{
		outgame();
		return 0;
	}
	opt = atoi(msg);
	if (!isdigit(msg[0]) || opt <= 0 || opt > 5 || msg[0] == '\n' || msg[0] == '\0')
	{
		move(10,0);
		prints("对不起，思源湖大银行不提供此项服务！\n");
		pressanykey();
		outgame();
		return 0;
	}
	
	if(opt == 2)
	{
		//update_deposit(currentuser.userid);//更新利息 pax@2005-8-8

		/*判断是否存款文件是否存在*/
		/*sprintf (fname,"bank/%s.bank", currentuser.userid);
		if(fd=open(fname,O_RDWR )==-1)
		{
			prints("对不起，您尚未开通活期存款帐户\n");
			pressanykey();
			outgame();
			return 0;		
		}
	    get_record (fname, &cBill,sizeof(struct callbill), 1);	//modified
		move( 9, 0);*/

		/*显示可以提取的资金以及利息*/
		sprintf (mail_buffer,"现在可提取的实际金额：%d 湖币\n",currentuser.inbank);
		prints (mail_buffer);
		
		/*判断取款的数目和合法性*/
		getdata(10, 0, "请输入你要取款的金额 [1 - 50000] (必须为整数):", money, 6, DOECHO, YEA);
		num_money = (int)atoi(money);
		if (!isdigit(money[0]) || num_money <= 0 || num_money > 50000 || msg[0] == '\n' || msg[0] == '\0')
		{
			move( 12, 0);
			prints("对不起，取款金额必须在 1 - 50000 之间！\n");
			pressanykey();
			outgame();
			return 0;
		}
		
		if(currentuser.inbank < num_money)
		{
			move(12, 0);
			prints("对不起，您的思源实业银行帐户不能透支！\n");
			pressanykey();
			outgame();
			return 0;
		}

		/*更新.bank文件*/
		/*将利息放到银行账户和本金上，利息清空，随后本金中扣除取款数目*/
		/*	cBill.principal+=(int)cBill.interest;
			currentuser.inbank += (int)cBill.interest;
			cBill.interest-=(int)cBill.interest;	
			cBill.principal-=num_money;		
			cBill.date=time(0);
			substitute_record(fname, &cBill,sizeof(struct callbill), 1);*/

		/*更新PASSFILE文件*/
		currentuser.money += num_money;//增加本金
		currentuser.inbank -= num_money;//扣除本金
		substitute_record(PASSFILE, &currentuser, sizeof(struct userec),id);
		/*发信通知*/
		getdatestring (time(0), NA);
		sprintf (mail_title, "%s %.23s 取款 %d 元", currentuser.userid,datestring + 6,num_money);
		securityreport(mail_title);		
		//pax@sjtubbs 05-07 取款发信
		if (askyn ("要给您发备份信件吗？", YEA, NA) == YEA)//pax@05-07 确认
		{
			sprintf (mail_buffer,
				"发信站: 思源实业银行发信系统\n\n"
				"\n亲爱的用户%s，您于【%.23s】\n\n"
				"\n 在思源湖银行活期取款 %d 元",currentuser.userid, datestring + 6,num_money);
			strcpy (tmpbname, currboard);
			strcpy (currboard, MONEYADMIN_BOARDNAME);
			autoreport (mail_title, mail_buffer, currentuser.userid, NULL);
			strcpy (currboard, tmpbname);
			//bankreport(mail_title, mail_buffer, currentuser.userid, NULL);
		}
		//end by pax
		move(14,0);
		prints("您的存款手续已经完成\n");
		pressanykey();
		outgame();
		return 1;
		
	}
	if(opt == 1)
	{
		//update_deposit(currentuser.userid);//更新利息 pax@2005-8-8
		getdata(10, 0, "请输入你存款的金额 [1 - 50000] (必须为整数):", money, 6, DOECHO, YEA);
		num_money = (int)atoi(money);
		
		if (!isdigit(money[0]) || num_money <= 0 || num_money > 50000 || msg[0] == '\n' || msg[0] == '\0')
		{
			move( 12, 0);
			prints("对不起，存款金额必须在 1 - 50000 之间！\n");
			pressanykey();
			outgame();
			return 0;
		}
		
		if(currentuser.money < num_money)
		{
			move(12, 0);
			prints("对不起，您的现金不足！\n");
			pressanykey();
			outgame();
			return 0;
		}
		
		/*sprintf (fname,"bank/%s.bank", currentuser.userid);
		if(fd=open(fname,O_RDWR )==-1)//第一次存款
		{
			fd=open(fname,O_RDWR | O_CREAT ,0644 );		
			cBill.date=time(0);			
			cBill.principal=num_money;	
			cBill.interest=0;
			write(fd,&cBill,sizeof(struct callbill));//重写文件
			close(fd);		
		}
		else{

			get_record (fname, &cBill,sizeof(struct callbill), 1);
			cBill.principal+=(int )cBill.interest;
			cBill.interest-=(int )cBill.interest;	
			cBill.principal+=num_money;		
			cBill.date=time(0);
			substitute_record(fname, &cBill,sizeof(struct callbill), 1);
		}*/
		currentuser.money -= num_money;
		currentuser.inbank += num_money;
		substitute_record(PASSFILE, &currentuser, sizeof(struct userec),id);
		getdatestring (time(0), NA);		
		sprintf (mail_title, "%s %.23s 存款 %d 元", currentuser.userid,datestring + 6,num_money);
		securityreport(mail_title);		
		//pax@sjtubbs 05-07 存款发信
		if (askyn ("要给您发备份信件吗？", YEA, NA) == YEA)//pax@05-07 确认
		{

			sprintf (mail_buffer,
				"发信站: 思源实业银行发信系统\n\n"
				"\n亲爱的用户%s，您于【%.23s】\n\n"
				"\n 在思源湖银行活期存款 %d 元",currentuser.userid, datestring + 6,num_money);
			//bankreport(mail_title, mail_buffer, currentuser.userid, NULL);
			strcpy (tmpbname, currboard);
			strcpy (currboard, MONEYADMIN_BOARDNAME);
			autoreport (mail_title, mail_buffer, currentuser.userid, NULL);
			strcpy (currboard, tmpbname);

		}
		//end by pax
		move(14,0);
		prints("您的存款手续已经完成\n");
		pressanykey();
		outgame();
		return 1;
		
	}
	if(opt == 3)//buy
	{
		getdata(10, 0, "请输入你需要购买的游戏币数目 [1 - 50000] :", money, 6, DOECHO, YEA);
		num_money = atoi(money);
		if (!isdigit(money[0]) || num_money <= 0 || num_money > 50000 || msg[0] == '\n' || msg[0] == '\0')
		{
			move( 12, 0);
			prints("对不起，购买数量必须在 1 - 50000 之间！\n");
			pressanykey();
			outgame();
			return 0;
		}
		
		if(currentuser.money < num_money)
		{
			move(12, 0);
			prints("对不起，您的现金不足！\n");
			pressanykey();
			outgame();
			return 0;
		}
		
		currentuser.money -= num_money;
		mdata.money += num_money;
		substitute_record(PASSFILE, &currentuser, sizeof(struct userec),id);
		fp = fopen(buf,"w");
		if(fp != NULL)
		{
			result = fwrite(&mdata, sizeof(mdata), 1, fp);
			fclose(fp);
		}		
		move(12,0);
		prints("您的游戏币兑换手续已经完成\n");

		sprintf(sysbuf, "%s 购买游戏币 %d 元", currentuser.userid, num_money);
		securityreport(sysbuf);
		
		//pax@sjtubbs 05-07 购买游戏币
		if (askyn ("要给您发备份信件吗？", YEA, NA) == YEA)//pax@05-07 确认
		{
			now = time (0);	
			getdatestring (now, NA);
			sprintf (mail_title, "%.23s 购买游戏币 %d 元", datestring + 6,num_money);
			sprintf (mail_buffer,
				"\n发信站: 思源实业银行发信系统\n\n"
				"\n亲爱的用户%s，您于【%.23s】\n\n"
				"\n 购买游戏币 %d 元",currentuser.userid, datestring + 6,num_money);
			//bankreport(mail_title, mail_buffer, currentuser.userid, NULL);
			strcpy (tmpbname, currboard);
			strcpy (currboard, MONEYADMIN_BOARDNAME);
			autoreport (mail_title, mail_buffer, currentuser.userid, NULL);
			strcpy (currboard, tmpbname);
		}
		pressanykey();
		outgame();
		return 1;
	}
	
/*	if(opt == 4)//sell
	{
		getdata(10, 0, "请输入你需要出售的游戏币数目 [1 - 50000] :", money, 6, DOECHO, YEA);
		num_money = atoi(money);
		if (!isdigit(money[0]) || num_money <= 0 || num_money > 50000 || msg[0] == '\n' || msg[0] == '\0')
		{
			move( 12, 0);
			prints("对不起，出售数量必须在 1 - 50000 之间！\n");
			pressanykey();
			outgame();
			return 0;
		}
		
		if(mdata.money < num_money)
		{
			move(12, 0);
			prints("对不起，您没有这么多游戏币套现！\n");
			pressanykey();
			outgame();
			return 0;
		}
		mdata.money -= num_money;		
		currentuser.money += (num_money/10);
		
		substitute_record(PASSFILE, &currentuser, sizeof(struct userec),id);
		fp = fopen(buf,"w");
		if(fp != NULL)
		{
			result = fwrite(&mdata, sizeof(mdata), 1, fp);
			fclose(fp);
		}		
		move(12,0);
		prints("您的游戏币兑换手续已经完成\n");
		sprintf(sysbuf, "%s 出售游戏币 %d 元", currentuser.userid, num_money);
		securityreport(sysbuf);
		//pax@sjtubbs 05-07 出售游戏币
		if (askyn ("要给您发备份信件吗？", YEA, NA) == YEA)//pax@05-07 确认
		{
			now = time (0);
			getdatestring (now, NA);
			sprintf (mail_title, "%.23s 出售游戏币 %d 元", datestring + 6,num_money);
			sprintf (mail_buffer,
				"\n发信站: 思源实业银行发信系统\n\n"
				"\n亲爱的用户%s，您于【%.23s】\n\n"
				"\n 出售游戏币 %d 元",currentuser.userid, datestring + 6,num_money);
			bankreport(mail_title, mail_buffer, currentuser.userid, NULL);
		}
		//end by pax
		pressanykey();
		outgame();
		return 1;
	}
*/	
	if(opt == 4)
			//if(opt == 5)
	{
		getdata(10, 0, "请输入你存款的金额 [1 - 50(单位 千元)] :", money, 6, DOECHO, YEA);
		num_money = atoi(money);
		
		if (!isdigit(money[0]) || num_money <= 0 || num_money > 50 || msg[0] == '\n' || msg[0] == '\0')
		{
			move( 12, 0);
			prints("对不起，存款金额必须在 1 - 50 (千元) 之间！\n");
			pressanykey();
			outgame();
			return 0;
		}
		num_money=num_money*1000;
		if(currentuser.money < num_money)
		{
			move(12, 0);
			prints("对不起，您的现金不足！\n");
			pressanykey();
			outgame();
			return 0;
		}
		
		
		getdata(12, 0, "请输入你此次存款的时间长度 [1 - 6]（单位：月） :", length, 2, DOECHO, YEA);
		num_length = atoi(length);
		
		if (!isdigit(length[0]) || num_length <= 0 || num_length > 6 || msg[0] == '\n' || msg[0] == '\0')
		{
			move( 14, 0);
			prints("对不起，存款的时间长度必须在 1 - 6 之间！\n");
			pressanykey();
			outgame();
			return 0;
		}
		
		fd=open("bank/bank",O_APPEND | O_WRONLY | O_CREAT ,0644 );
		stat("bank/bank",&kkk);
		size=kkk.st_size/sizeof(struct bankbill);
		strcpy(Bill.userid,currentuser.userid);
		Bill.usernum=size+1;
		Bill.datein=time(0);			
		Bill.money=num_money;		
		Bill.dateout=0;	
		Bill.length=num_length;
		write(fd,&Bill,sizeof(struct bankbill));
		close(fd);
		
		currentuser.money -= num_money;
		//currentuser.inbank += num_money;
		substitute_record(PASSFILE, &currentuser, sizeof(struct userec),id);
		//定期存款强制发信
		//		if (askyn ("要给您发备份信件吗？", YEA, NA) == YEA)//pax@05-07 确认
		//		{
		now = time (0);
		getdatestring (now, NA);
		sprintf (mail_title, "%s 办理 %d月期存款 %d 元", currentuser.userid,num_length,num_money);
		sprintf (mail_buffer,
			"发信站: 思源实业银行发信系统\n"
			"\n亲爱的用户%s，您于【%.23s】\n"
			"\n 在思源湖银行存款 %d 元，存期为 %d 个月\n"
			"\n帐单号为：%ld",currentuser.userid, datestring + 6,num_money ,num_length,size+1);
		//bankreport(mail_title, mail_buffer, currentuser.userid, NULL);
		strcpy (tmpbname, currboard);
		strcpy (currboard, MONEYADMIN_BOARDNAME);
		autoreport (mail_title, mail_buffer, currentuser.userid, NULL);
		strcpy (currboard, tmpbname);
		//		}
		move(12,0);
		prints("您的存款手续已经完成\n");
		sprintf(sysbuf, "%s 办理%d 月期存款 %d 元，帐单号为 %ld", currentuser.userid,num_length, num_money, Bill.usernum);
		securityreport(sysbuf);
		pressanykey();
		outgame();
		return 1;
		
	}
	if(opt == 5)//	if(opt == 6)
	{
		getdata(10, 0, "请输入你存款的帐单号 :", BillNo, 8, DOECHO, YEA);
		num_bill = atoi(BillNo);
		
		if (!isdigit(BillNo[0]) || msg[0] == '\n' || msg[0] == '\0')
		{
			move( 12, 0);
			prints("对不起，请输入正确帐单号！\n");
			pressanykey();
			outgame();
			return 0;
		}
		sprintf (fname,"bank/bank");
		get_record (fname, &Bill,sizeof(struct bankbill), num_bill);
		
		//if(strcmp("pax",currentuser.userid)!=0)//判断是否为当前用户的账户，判断是否已经被取走了
		{
			if((strcmp(Bill.userid,currentuser.userid)!=0) || (Bill.dateout!=0))
			{
				move( 12, 0);
				prints("对不起，请输入正确帐单号！\n");
				pressanykey();
				outgame();
				return 0;
			}
		}
		Bill.dateout=time(0);//设定取出时间
		//callrate=0.0005;
		if(Bill.dateout < Bill.datein + Bill.length*86400*30)
		{
			if(askyn ("你确定要提早结束吗", NA, NA) != YEA)
			{
				move (2, 0);
				prints ("取消操作\n");
				pressreturn ();
				outgame();
				return 0;
			}
			else
			{
				num_money=(int)Bill.money*(1+callrate*(double)(Bill.dateout-Bill.datein)/86400);//按照活期的算
			}
		}
		else
		{
			if(fd_rate=open("bank/rate",O_RDONLY)!=1)
			{
				fd_rate=open("bank/rate",O_RDONLY);
				stat("bank/rate",&kkk);
				size=kkk.st_size/sizeof(struct rate);
				read(fd_rate,&bankrate,sizeof(struct rate));
    			//while((size>1) && (bankrate.time<Bill.datein))
				while(size>0)
				{
					read(fd_rate,&bankrate,sizeof(struct rate));	
					BANK_CURRDEPO_RATE[0]=bankrate.timedepo_01;
					BANK_CURRDEPO_RATE[1]=bankrate.timedepo_02;
					BANK_CURRDEPO_RATE[2]=bankrate.timedepo_03;
					BANK_CURRDEPO_RATE[3]=bankrate.timedepo_04;
					BANK_CURRDEPO_RATE[4]=bankrate.timedepo_05;
					BANK_CURRDEPO_RATE[5]=bankrate.timedepo_06;
					size--;
				}	
				close(fd_rate);
			}

			num_money =(int)Bill.money*(1+BANK_CURRDEPO_RATE[Bill.length-1]);//按照定期的算
			num_money +=(int)Bill.money*(callrate*(double)(Bill.dateout-(Bill.datein + Bill.length*86400*30))/86400);//按照活期的算			

		}
		sprintf (fname,"bank/bank");
		substitute_record(fname, &Bill,sizeof(struct bankbill), num_bill);
		currentuser.money += num_money;//现金加上本金和利息
		//currentuser.inbank -= Bill.money;//帐户只扣去本金
		substitute_record(PASSFILE, &currentuser, sizeof(struct userec),id);
//		close(fd);
		
		//pax@sjtubbs 05-07 存款发信
		if (askyn ("要给您发备份信件吗？", YEA, NA) == YEA)//pax@05-07 确认
		{
			getdatestring (Bill.dateout, NA);
			sprintf (mail_title, "%s 取款 %d月期取款 %d 元", currentuser.userid,Bill.length,num_money );
			sprintf (mail_buffer,
				"发信站: 思源实业银行发信系统\n"
				"\n亲爱的用户%s，您于【%.23s】\n"
				"\n 在思源湖银行取款 %d 元\n"
				"\n帐单号为：%ld",currentuser.userid, datestring + 6,num_money ,Bill.usernum);
			//bankreport(mail_title, mail_buffer, currentuser.userid, NULL);
			strcpy (tmpbname, currboard);
			strcpy (currboard, MONEYADMIN_BOARDNAME);
			autoreport (mail_title, mail_buffer, currentuser.userid, NULL);
			strcpy (currboard, tmpbname);
		}
		prints("您的取款手续已经完成\n");
		sprintf(sysbuf, "%s 领取存款 %d 元，帐单号为 %ld", currentuser.userid, num_money,Bill.usernum);
		
		securityreport(sysbuf);
		pressanykey();
		outgame();
		return 1;
	}
	
	move(12,0);
	prints("今日思源湖大银行休息:)\n");
	pressanykey();
	outgame();
	return 1;
	
	
}


int 
bank_record(){
	struct bankbill Bill;
	long size;
	struct stat kkk;
	//	char fname[80];
	int i;
	long count;
	int fd;
	char mail_title[STRLEN]={0};
	char mail_buffer[1024] = {0};
	int all;
	char ans[5];
	char temp[65535];
	char someoneID[20];
	clear();
	all = 0;
	
	//-----------------------------------------------------------------
    modify_user_mode (QUERY);
    usercomplete ("请输入您想查询的帐号: ", someoneID);
    if (someoneID[0] == 0)
    {
        getdata (0, 30, "查询所有的作者吗?[Y/N]: ", ans, 7, DOECHO, YEA);
        if ((*ans != 'Y') && (*ans != 'y'))
        {
            return;
        }
        else
            all = 1;
    }
	else if (!getuser (someoneID))
    {
        prints ("不正确的使用者代号\n");
        pressreturn ();
        return;
    }
	
	
	if (all==1){
		fd=open("bank/bank",O_RDWR | O_CREAT );
		stat("bank/bank",&kkk);
		size=kkk.st_size/sizeof(struct bankbill);
		i=0;
		count=0;
		sprintf (temp,"所有人的银行记录\n");
		sprintf (mail_title,"所有人的银行记录");		
		sprintf(mail_buffer,"%8s %12s %20s %20s %8s %s\n", "帐单号", "ID", "存入日期","取款日期","金额","存期");
		strcat (temp,mail_buffer);
		
		while (i<size)
		{
			read(fd,&Bill,sizeof(struct bankbill));
			if(Bill.dateout==0){
				count=count+Bill.money;}			
			//	sprintf (mail_buffer,"\n%8d %12s %20s %20s %8d %d",Bill.usernum,Bill.userid,getdatebank(Bill.datein),getdatebank(Bill.dateout),Bill.money,Bill.length);
			getdatebank(Bill.datein);
			sprintf (mail_buffer,"\n%8d %12s %s ",Bill.usernum,Bill.userid,bankdatetime);
			strcat (temp, mail_buffer);
			getdatebank(Bill.dateout);
			sprintf (mail_buffer,"%s %8d %2d",bankdatetime,Bill.money,Bill.length);
			strcat (temp, mail_buffer);
			
			i++;
		}
		sprintf (mail_buffer,"\n总额:%ld",count);
		strcat (temp, mail_buffer);
		//bankreport(mail_title, temp, currentuser.userid, NULL);
		
		strcpy (tmpbname, currboard);
		strcpy (currboard, "");
		autoreport (mail_title, temp, currentuser.userid, NULL);
		strcpy (currboard, tmpbname);

		close(fd);	
	}
	else{
		fd=open("bank/bank",O_RDWR | O_CREAT );
		stat("bank/bank",&kkk);
		size=kkk.st_size/sizeof(struct bankbill);	
		i=0;
		count=0;
		
		sprintf (mail_title,"%s的银行记录",someoneID);	
		sprintf (temp,"%s的银行记录\n",someoneID);				
		sprintf(mail_buffer,"%8s %20s %20s %8s %s\n", "帐单号", "存入日期","取款日期","金额","存期");
		strcat (temp,mail_buffer);
		while (i<size)
		{
			read(fd,&Bill,sizeof(struct bankbill));
			//if(strcmp(Bill.userid,someoneID)==0)
			if((strcmp(Bill.userid,someoneID)==0)&&(Bill.dateout==0))
			{
				count=count+Bill.money;
				getdatebank(Bill.datein);
				sprintf (mail_buffer,"\n%8d  %s ",Bill.usernum,bankdatetime);
				strcat (temp, mail_buffer);
				getdatebank(Bill.dateout);
				sprintf (mail_buffer,"%s %8d %2d",bankdatetime,Bill.money,Bill.length);
				strcat (temp, mail_buffer);
			}
			i++;
		}
		sprintf (mail_buffer,"\ncount=%ld,size=%d,id=%s",count,size,someoneID);
		strcat (temp, mail_buffer);
		//bankreport(mail_title, temp, currentuser.userid, NULL);
		strcpy (tmpbname, currboard);
		strcpy (currboard, "");
		autoreport (mail_title, temp, currentuser.userid, NULL);
		strcpy (currboard, tmpbname);
		close(fd);	}
}



int
getdatebank (time_t now)
{
	struct tm *tm;
	
	tm = localtime (&now);
	sprintf (bankdatetime, "%4d-%02d-%02d %02d:%02d:%02d",
		tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec);
	return 0;
}
int lending()
{
	//	char sysbuf[STRLEN];
	int     id, canlending=0,maxnum = 0, num = 0,u_money=0;
	char	ans[STRLEN];
	//char	tmpbname[STRLEN];
	char ps[5][STRLEN];
	
	int i = 0;
	time_t 	now=time(0);
	extern int gettheuserid();
	
	if(gotomarket("交易市场")) return 0;
	maxnum = currentuser.money - 100;
	prints("[m欢迎使用[1;33m"BBSNAME"[m交易市场[32m友情转帐[m功能");
	prints("\n\n您目前的情况是：\n注册天数([32m%d[m 天) "
		"上站总时数([32m%d[m 小时) "
		"[44;37m可转帐资金([32m%d[37m 元)[m",
		(now - currentuser.firstlogin)/86400,
		currentuser.stay/3600,currentuser.money-100);
	if(currentuser.stay<=3600||now-currentuser.firstlogin<=259200||maxnum<=0){
		prints("\n\n目前市场规定： 参与[32m友情转帐[m的 ID 必须具备以下"
			"全部条件：\n"
			"    1. 本帐号注册天数超过 3 天;\n"
			"    2. 总上站时数超过 1 小时;\n"
			"    3. 最终拥有存款超过 100 元.\n");
		prints("\n\n根据市场规定，您目前尚没有[32m友情转帐[m的资格。\n");
		pressanykey();
		outgame();
		return 0;
	}
	if (!gettheuserid(9,"您想转帐到谁的帐户上？请输入他的帐号: ",&id))
	{
		outgame();
		return 0;
	}
	get_record(PASSFILE, &lookupuser, sizeof(lookupuser), id);//+neige@100511-lending-lookupuser-dirtydata (@100511);
	u_money = lookupuser.money+lookupuser.medals*10000+ lookupuser.inbank;
	if(!strcmp(currentuser.userid,lookupuser.userid) ||u_money > CHANGE_MAX){
		prints("\n对不起，%s 目前尚不需要您的转帐！",lookupuser.userid);
		pressanykey();
		outgame();
		return 0;
	}
	move(10,0);
	canlending = (maxnum>(CHANGE_MAX-u_money))?(CHANGE_MAX-u_money):maxnum;
	prints("您将转帐给 [[1;32m%s[m]，转帐最多金额为 [1;33m%d[m 元。",
		lookupuser.userid, canlending);
	getdata(12, 0, "确实要转帐，请输入转帐数目，否则，请直接回车取消转帐: ",
		ans, 6, DOECHO, YEA);
	num = atoi(ans);
	if ( num <= 0 || num > canlending ) {
		prints("\n输入数目有错误。 取消交易 ...");
		pressanykey();
		outgame();
		return 0;
	}
	set_safe_record();
	if(currentuser.money - 100  != maxnum) {
		prints("\n对不起，您的可转帐资金有所变化，取消此次交易！");
		prints("\n请重新执行本交易。");
		pressanykey();
		outgame();
		return 0;
	}
	
	for (i = 0; i < 5; i++)
		ps[i][0] = '\0';
	move (14, 0);
	prints ("请输入转帐附言(最多五行，按 Enter 结束)");
	for (i = 0; i < 5; i++)
	{
		getdata (i + 15, 0, ": ", ps[i], STRLEN - 5, DOECHO, YEA);
		if (ps[i][0] == '\0')
			break;
	}
	if (askyn ("确认转帐？", YEA, NA) == YEA)//pax@05-07 转帐确认
	{
		currentuser.money -= num;
		substitute_record(PASSFILE, &currentuser, sizeof(currentuser),usernum);
		lookupuser.money += num;
		substitute_record(PASSFILE, &lookupuser, sizeof(lookupuser), id);
		
		prints("\n谢谢您，您已经给 [1;32m%s[m 转帐 [1;33m%d[m 元。",
			lookupuser.userid,num);
		prints("\n为表示对你的转帐行为的感谢，本站已经用信件通知了他。");
		sprintf(genbuf,"给 %s 转帐 %d 元",lookupuser.userid,num);
		gamelog(genbuf);
		sprintf(ans,"%s 给 %s 寄来了 %d 元友情转帐",currentuser.userid, lookupuser.userid, num);
		sprintf(genbuf,"\n %s :\n\n\t您好！\n\n\t您的朋友 %s 给您寄来了 %d 元友情转帐资金。\n",lookupuser.userid,currentuser.userid,num);
		//   bankreport(ans,genbuf,NA,lookupuser.userid);
		
		for (i = 0; i < 5; i++)
		{
			if (ps[i][0] == '\0')
				break;
			if (i == 0)
				strcat (genbuf, "\n\n转帐附言：\n");

			strcat (genbuf, ps[i]);
			strcat (genbuf, "\n");
		}
		
		strcpy (currboard, MONEYADMIN_BOARDNAME);
		autoreport(ans,genbuf, lookupuser.userid, NULL);
		//add by pax@sjtubbs 05-07 转帐备份给自己
		if (askyn ("备份给自己？", YEA, NA) == YEA)//pax@05-07 转帐确认
		{
			sprintf(ans,"%s 给 %s 寄去了 %d 元友情转帐",currentuser.userid, lookupuser.userid, num);
			sprintf(genbuf,"\n %s :\n\n\t您好！\n\n\t 您给您的朋友 %s 寄去了 %d 元友情转帐资金。\n",currentuser.userid,lookupuser.userid,num);
			for (i = 0; i < 5; i++)
			{
				if (ps[i][0] == '\0')
					break;
				if (i == 0)
					strcat (genbuf, "\n\n转帐附言：\n");
				strcat (genbuf, ps[i]);
				strcat (genbuf, "\n");
			}
			//bankreport(ans,genbuf,currentuser.userid, NULL);			
			strcpy (tmpbname, currboard);
			strcpy (currboard, "");
			autoreport (ans, genbuf, currentuser.userid, NULL);
			strcpy (currboard, tmpbname);

		}
		//end by pax
		
	}//pax
	pressanykey();
	outgame();
	return 1;
}

int popshop(void)
{
	int no,num,maxnum/*,templog*/;
	char ans[10];
	//char sysbuf[STRLEN];
	EXCHANGES exchanges[10] = {
		//      {"上站次数",2},
		//      {"文章数",5},
		//      {"奖章数",10000},
		{"隐身术",40000},
		{"看穿隐身术",100000},
		{"帐号永久保留",200000},
		{"强制呼叫",20000},
		{"延长发呆时间",10000},
		{"666 生命力",20000},
		{"信箱无上限",40000}
	};
	if(gotomarket("市场典当行")) return 1;
	prints("今日典当报价:");
	for(no = 0; no < 7; no ++) {
		move(5+no, 2);
		prints("(%2d) %s",no+1,exchanges[no].title);
		move(5+no, 20);
		prints("==> %6d 元", exchanges[no].value);
	}
	//   move(16,0);
	//   prints("[0;1m您目前的情况是: (1)上站次数([32m%d[m)  "
	//          "(2)文章数([32m%d[37m)  (3)奖章数.([32m%d[37m)\n",
	//	  currentuser.numlogins,currentuser.numposts,currentuser.medals);
	getdata(18, 0, "您想典当哪一项？", ans, 10, DOECHO, YEA);
	no = atoi(ans);
	if ( no < 1 || no > 7/*10*/ ) {
		prints("\n输入错误，取消交易 ...");
		pressanykey();
		outgame();
		return 0;
	}
	move(18, 30);
	prints("你选择典当“[32m%s[m”。",exchanges[no-1].title);
	if(no>0/*3*/){
		set_safe_record();
		maxnum = exchanges[no-1].value;
		switch(no) {
		case 1:
			if(!HAS_PERM(PERM_CLOAK)) { num = 0; break; }
			break;
		case 2:
			if(!HAS_PERM(PERM_SEECLOAK)) { num = 0; break; }
			break;
		case 3:
			if(!HAS_PERM(PERM_XEMPT)) { num = 0; break; }
			break;
		case 4:
			if(!HAS_PERM(PERM_FORCEPAGE)) { num = 0; break; }
			break;
		case 5:
			if(!HAS_PERM(PERM_EXT_IDLE)) { num = 0; break; }
			break;
		case 6:
			if(!HAS_PERM(PERM_666LIFE)) { num = 0; break; }
			break;
		case 7:
			if(!HAS_PERM(PERM_MAILINFINITY)) { num = 0; break; }
			break;
		}
		prints("\n\n");
		if(!num) {
			prints("对不起, 你还没有这种权限. ");
			pressanykey();
			outgame();
			return 0;
		}
		if(askyn("您确定要典当吗？",NA,NA) == NA ) {
			move(23,0);clrtoeol();
			prints("交易取消 ...");
			pressanykey();
			outgame();
			return 0;
		}
		set_safe_record();
		switch(no) {
		case 1:
			num = HAS_PERM(PERM_CLOAK);
			currentuser.userlevel &= ~PERM_CLOAK ;
			break;
		case 2:
			num = HAS_PERM(PERM_SEECLOAK);
			currentuser.userlevel &= ~PERM_SEECLOAK ;
			break;
		case 3:
			num = HAS_PERM(PERM_XEMPT);
			currentuser.userlevel &= ~PERM_XEMPT ;
			break;
		case 4:
			num = HAS_PERM(PERM_FORCEPAGE);
			currentuser.userlevel &= ~PERM_FORCEPAGE ;
			break;
		case 5:
			num = HAS_PERM(PERM_EXT_IDLE);
			currentuser.userlevel &= ~PERM_EXT_IDLE ;
			break;
		case 6:
			num = HAS_PERM(PERM_666LIFE);
			currentuser.userlevel &= ~PERM_666LIFE ;
			break;
		case 7:
			num = HAS_PERM(PERM_MAILINFINITY);
			currentuser.userlevel &= ~PERM_MAILINFINITY ;
			break;
		}
		if(!num) {
			prints("对不起, 你的数据发生了变化, 你目前没有这种权限. ");
			pressanykey();
			outgame();
			return 0;
		}
		} /*
		  else {
		  if( no == 1 )maxnum = currentuser.numlogins-1;
		  else if ( no == 2) maxnum = currentuser.numposts;  
		  else	maxnum = currentuser.medals;
		  templog = maxnum;
		  sprintf(genbuf,"您想典当多少呢(最多%d)？",maxnum);
		  getdata(19, 0, genbuf,ans, 10, DOECHO, YEA);
		  num = atoi(ans);
		  if ( num <= 0 || num > maxnum ) {
		  prints("输入错误。取消交易 ...");
		  pressanykey();
		  outgame();
		  return 0;
		  }
		  maxnum = num*exchanges[no-1].value;
		  move(19,0);
		  prints("您共计典当%s.[32m%d.[m 份， %s .[33m%d.[m 元。\n",
		  exchanges[no-1].title,num,"可以获得",maxnum);
		  if(askyn("您确定要典当吗？",NA,NA) == NA ) {
		  move(21,0);clrtoeol();
		  prints("取消交易 ...");
		  pressanykey();
		  outgame();
		  return 0;
		  }
		  set_safe_record();
		  if (no == 1) {
		  if(templog==currentuser.numlogins-1)
		  currentuser.numlogins -= num;
		  else templog = -1;
		  } else if (no == 2)  {
		  if(templog == currentuser.numposts)
		  currentuser.numposts -= num;
		  else templog = -1;
		  } else {
		  if(templog == currentuser.medals)
		  currentuser.medals -= num;
		  else templog = -1;
		  }
		  if( templog == -1) {
		  move(21,0); clrtoeol();
		  prints("对不起, 在交易过程中您的数据发生了变化.\n");
		  prints("为保证交易的正常进行, 此次交易取消.\n您可以重新进行本交易.");
		  pressanykey();
		  outgame();
		  return 0;
		  }
	  }*/
	  
	  currentuser.money += maxnum;
	  if( currentuser.money > 900000000 ){
		  move(21,0); clrtoeol();
		  prints("对不起，交易数据过大，产生溢出，请重新交易！\n");
		  prints("建议您将一部分金钱存入银行。");
		  pressanykey();
		  outgame();
		  return 0;
	  }
	  substitute_record(PASSFILE, &currentuser, sizeof(currentuser), usernum);
	  move(21,0); clrtoeol();
	  prints("顺利完成交易，欢迎您的再次惠顾。;P");
	  if(no<=3)
	  {
		  sprintf(genbuf,"典当%s %d 份，花费 %d 元.",exchanges[no-1].title,num,maxnum);
	  }
	  else
	  {
		  sprintf(genbuf,"典当%s，花费 %d 元.",exchanges[no-1].title,maxnum);
	  }
	  securityreport(genbuf);
	  gamelog(genbuf);
	  pressanykey();
	  outgame();
	  return 1;
}

/*
int shopping()//modified by smartfish, 2003.09.08.
{
	int no,hasperm=1,maxnum;
	char ans[10];
	EXCHANGES exchanges[10] = {
		{"隐身术",100000},
		{"看穿隐身术",4000000},
		{"帐号永久保留",4000000},
		{"强制呼叫",4000000},
		{"延长发呆时间",400000},
		{"666生命力",1000000},
		{"无上限信箱",9900000}
	};
	if(gotomarket("市场购物中心")) return 1;
	prints("今日商品报价:");
	for(no = 0; no < 7; no ++) {
		move(5+no, 2);
		prints("(%2d) %s",no+1,exchanges[no].title);
		move(5+no, 20);
		prints("==> %6d 元", exchanges[no].value);
	}
	move(16,0);
	prints("您目前尚有 %d 元 (奖章 %d 个)\n",
		currentuser.money,currentuser.medals);
	getdata(18, 0, "您想购买哪一件商品？", ans, 10, DOECHO, YEA);
	no = atoi(ans);
	if ( no < 1 || no > 7 ) {
		prints("\n输入错误。取消交易 ...");
		pressanykey();
		outgame();
		return 0;
	}
	set_safe_record();
	maxnum = exchanges[no-1].value;
	switch(no) {
	case 1:
		hasperm = HAS_PERM(PERM_CLOAK);
		break;
	case 2:
		hasperm = HAS_PERM(PERM_SEECLOAK);
		break;
	case 3:
		hasperm = HAS_PERM(PERM_XEMPT);
		break;
	case 4:
		hasperm = HAS_PERM(PERM_FORCEPAGE);
		break;
	case 5:
		hasperm = HAS_PERM(PERM_EXT_IDLE);
		break;
	case 6:
		hasperm = HAS_PERM(PERM_666LIFE);
		break;
	case 7:
		hasperm = HAS_PERM(PERM_MAILINFINITY);
		break;
	}
	prints("\n\n");
	if(hasperm) {
		prints("您已经有这种权限, 不需要再购买. ");
		pressanykey();
		outgame();
		return 0;
	}
	if(currentuser.money < maxnum) {
		prints("\n对不起, 你没有足够的钱购买这种权限.");
		pressanykey();
		outgame();
		return 0;
	}
	if(askyn("您确定要购买吗？",NA,NA) == NA ) {
		move(23,0);clrtoeol();
		prints("取消交易。");
		pressanykey();
		outgame();
		return 0;
	}
	set_safe_record();
	switch(no) {
	case 1:
		hasperm = HAS_PERM(PERM_CLOAK);
		currentuser.userlevel |= PERM_CLOAK ;
		break;
	case 2:
		hasperm = HAS_PERM(PERM_SEECLOAK);
		currentuser.userlevel |= PERM_SEECLOAK ;
		break;
	case 3:
		hasperm = HAS_PERM(PERM_XEMPT);
		currentuser.userlevel |= PERM_XEMPT ;
		break;
	case 4:
		hasperm = HAS_PERM(PERM_FORCEPAGE);
		currentuser.userlevel |= PERM_FORCEPAGE ;
		break;
	case 5:
		hasperm = HAS_PERM(PERM_EXT_IDLE);
		currentuser.userlevel |= PERM_EXT_IDLE ;
		break;
	case 6: 
		hasperm = HAS_PERM(PERM_666LIFE); 
		currentuser.userlevel |= PERM_666LIFE ; 
		break;
	case 7: 
		hasperm = HAS_PERM(PERM_MAILINFINITY); 
		currentuser.userlevel |= PERM_MAILINFINITY ;
		break;
	}
	if(hasperm) {
		prints("在交易进行前您已经有了这种权限, 所以取消此次交易. ");
		pressanykey();
		outgame();
		return 0;
	}
	if(currentuser.money < maxnum) {
		prints("\n对不起, 你没有足够的钱购买这种权限.");
		pressanykey();
		outgame();
		return 0;
	}
	currentuser.money -= maxnum;
	substitute_record(PASSFILE, &currentuser, sizeof(currentuser), usernum);        move(23,0); clrtoeol();
	prints("顺利完成交易，欢迎您的再次惠顾。;P");
	sprintf(genbuf,"购买[%s]，花费 %d 元.",exchanges[no-1].title,maxnum);
	securityreport(genbuf);
	gamelog(genbuf);
	pressanykey();
	outgame();
	return 1;
}	


*/




int shopping()//modified by pax, 2005.09.21.
{
	int i,no,maxnum;
	int id,counts=0;
	int fd;
	char	ans[STRLEN];
	char ps[5][STRLEN];
	char datafile[80] = BBSHOME "/etc/shop.ini";
	int type[64];

	struct override fh;    
	char filepath[STRLEN];
	extern int cmpfnames ();

	//char tmpbname[STRLEN];
	EXCHANGES exchanges[64];
    FILE *fp;
    char t[256], *t1, *t2, *t3;
	struct gift cgift;

    fp = fopen (datafile, "r");
    if (fp == NULL)
        return;
    while (fgets (t, 255, fp) && counts <= 10)
    {
        t1 = strtok (t, " \t");
        t2 = strtok (NULL, " \t\n");
        t3 = strtok (NULL, " \t\n");
        if (t1[0] == '#' || t1 == NULL || t2 == NULL)
            continue;
		strncpy (exchanges[counts].title, t1, 16);
		exchanges[counts].value=atoi(strncpy (ans, t2, 36));
		type[counts] = t3 ? atoi (t3):0;
        counts++;
    }
    fclose (fp);

	if(gotomarket("市场购物中心")) return 1;
	prints("今日商品报价:");
	for(no = 0; no < counts; no ++) {
		move(5 + no/2, 2 + 38*(no%2));
		prints("(%2d) %s",no+1,exchanges[no].title);
		move(5 + no/2, 20 + 38*(no%2));
		prints("==> %6d 元", exchanges[no].value);
	}
	move(counts/2+6,0);
	prints("您目前尚有 %d 元 (奖章 %d 个)\n",	currentuser.money,currentuser.medals);
	if (!gettheuserid(counts/2+7,"您想给谁买礼物？请输入他/她的帐号: ",&id))
	{
		prints("\n输入错误。取消交易 ...");
		pressanykey();
		outgame();
		return 0;
	}

	//检查黑名单
    sethomefile (filepath, lookupuser.userid, "rejects");
    if (search_record(filepath, &fh, sizeof (fh), cmpfnames, currentuser.userid))
	{
		prints("\n对方不想收到您的礼物");
		pressanykey();
		outgame();
		return 0;
	}


	getdata(counts/2+8, 0, "您想购买哪一件商品？", ans, 3, DOECHO, YEA);
	no = atoi(ans);
	if ( no < 1 || no > counts ) {
		prints("\n输入错误。取消交易 ...");
		pressanykey();
		outgame();
		return 0;
	}
	maxnum = exchanges[no-1].value;
	if(currentuser.money < maxnum) {
		prints("\n对不起, 您的资金不够。");
		pressanykey();
		outgame();
		return 0;
	}
	if(askyn("您确定要购买吗？",NA,NA) == NA ) {
		move(23,0);
		clrtoeol();
		prints("取消交易。");
		pressanykey();
		outgame();
		return 0;
	}
	currentuser.money -= maxnum;
	substitute_record(PASSFILE, &currentuser, sizeof(currentuser), usernum);
	sprintf(ans,"%s 给 %s 寄来了礼物",currentuser.userid, lookupuser.userid);
	sprintf(genbuf,"\n %s :\n\n\t您好！\n\n\t您的朋友 %s 给您寄来礼物： %s。\n",lookupuser.userid,currentuser.userid,exchanges[no-1].title);
	for (i = 0; i < 5; i++)
		ps[i][0] = '\0';
	move (counts/2+10, 0);
	prints ("请输入转帐附言(最多五行，按 Enter 结束)");
	for (i = 0; i < 5; i++)
	{
		getdata (i + counts/2 + 11, 0, ": ", ps[i], STRLEN - 5, DOECHO, YEA);
		if (ps[i][0] == '\0')
			break;
	}

	for (i = 0; i < 5; i++)
	{
		if (ps[i][0] == '\0')
			break;
		if (i == 0)
			strcat (genbuf, "\n\n附言：\n");
		strcat (genbuf, ps[i]);
		strcat (genbuf, "\n");
	}		
	sprintf(ans,"%s 为 %s 购买[%s]，花费 %d 元.",currentuser.userid,lookupuser.userid,exchanges[no-1].title,maxnum);

	strcpy (tmpbname, currboard);
	strcpy (currboard, GIFTADMIN_BOARDNAME);
	if(askyn("是否公开此次交易行为？",YEA,YEA) == YEA ) {
			autoreport(ans,genbuf,lookupuser.userid, GIFT_BOARDNAME);
	}
	else
	{
			autoreport (ans, genbuf, lookupuser.userid, NULL);
	}
	strcpy (currboard, tmpbname);

	/*写入收礼人的个人文件*/
	if(type[no-1]){
		sethomefile(genbuf,lookupuser.userid,"gift.dat");
		fd=open(genbuf,O_APPEND | O_WRONLY | O_CREAT ,0644 );
		strcpy(cgift.id,currentuser.userid);
		strcpy(cgift.name,exchanges[no-1].title);
		lseek(fd, 0, SEEK_END);
		write(fd,&cgift,sizeof(cgift));
		close(fd);
	}
	securityreport(ans);	
	gamelog(ans);
	move(22,0);
	prints("顺利完成交易，欢迎您的再次惠顾。;P");
	pressanykey();
	outgame();
	return 1;
}	
/*
void
bankreport (title, str, userid, sysboard)
char *title;
char *str;
char *userid;
char *sysboard;
{
	FILE *se;
	char fname[STRLEN];
	int savemode;
	savemode = uinfo.mode;
	report (title);
	sprintf (fname, "tmp/%s.%s.%05d", BBSID, currentuser.userid, uinfo.pid);
	if ((se = fopen (fname, "w")) != NULL)
	{
		fprintf (se, "%s", str);
		fclose (se);
		if (userid != NULL)
			mail_file (fname, userid, title);
		unlink (fname);
		modify_user_mode (savemode);
	}
}
*/

/*调整银行存款*/

/*int shopping_admin()//modified by pax, 2005.09.21.
{
	int i,no,hasperm=1,maxnum;
	int id,id2,counts=0;
	int fd;
	char	ans[STRLEN];
	char ps[5][STRLEN];
	char datafile[80] = BBSHOME "/etc/shop.ini";
	int type[64];
	EXCHANGES exchanges[64];
    FILE *fp;
    char t[256], *t1, *t2, *t3;
	struct gift cgift;

    fp = fopen (datafile, "r");
    if (fp == NULL)
        return;
    while (fgets (t, 255, fp) && counts <= 10)
    {
        t1 = strtok (t, " \t");
        t2 = strtok (NULL, " \t\n");
        t3 = strtok (NULL, " \t\n");
        if (t1[0] == '#' || t1 == NULL || t2 == NULL)
            continue;
		strncpy (exchanges[counts].title, t1, 16);
		exchanges[counts].value=atoi(strncpy (ans, t2, 36));
		type[counts] = t3 ? atoi (t3):0;
        counts++;
    }
    fclose (fp);

	prints("今日商品报价:");
	for(no = 0; no < counts; no ++) {
		move(5+no, 2);
		prints("(%2d) %s",no+1,exchanges[no].title);
		move(5+no, 20);
		prints("==> %6d 元", exchanges[no].value);
	}
	move(14,0);
	if (!gettheuserid(15,"获益者: ",&id2))
	{
		prints("\n输入错误。取消交易 ...");
		pressanykey();
		outgame();
		return 0;
	}
	getdata(15, 0, "您想购买哪一件商品？", ans, 3, DOECHO, YEA);
	no = atoi(ans);
	if ( no < 1 || no > counts ) {
		prints("\n输入错误。取消交易 ...");
		pressanykey();
		outgame();
		return 0;
	}
	
	if(askyn("确定要购买吗？",NA,NA) == NA ) {
		move(23,0);
		clrtoeol();
		prints("取消交易。");
		pressanykey();
		outgame();
		return 0;
	}
	/*写入收礼人的个人文件
	if(type[no-1]){
	sethomefile(genbuf,lookupuser.userid,"gift.dat");
	fd=open(genbuf,O_APPEND | O_WRONLY | O_CREAT ,0644 );

	if (!gettheuserid(15,"购买者: ",&id))
	{
		prints("\n输入错误。取消交易 ...");
		pressanykey();
		outgame();
		return 0;
	}

	strcpy(cgift.id,lookupuser.userid);
	strcpy(cgift.name,exchanges[no-1].title);
	lseek(fd, 0, SEEK_END);
	write(fd,&cgift,sizeof(cgift));
	close(fd);
}

	pressanykey();
	return 1;
}	*/
/*int refreshbank(){

struct bankbill Bill;
struct userec user;	
struct callbill cBill;
long size,size1;
int money;
struct stat kkk;
struct stat kkk1;
int id;
int i;
long count;
int fd,fd1,fd2;
char temp[60000];
char temp2[60000];
char mail_title[STRLEN]={0};
char mail_buffer[1024] = {0};
char someoneID[20];
char fname[80];
clear();

  
	fd=open(PASSFILE,O_RDWR);
	stat(PASSFILE,&kkk);
	size=kkk.st_size/sizeof(struct userec);
	sprintf (mail_title,"%s","测试查询");
	
	  while(size>0){
	  read(fd,&user,sizeof(struct userec));
	  fd1=open("bank/bank",O_RDWR | O_CREAT );
	  stat("bank/bank",&kkk1);
	  size1=kkk1.st_size/sizeof(struct bankbill);	
	  i=0;
	  count=0;
	  while (i<size1)
	  {
	  read(fd1,&Bill,sizeof(struct bankbill));
	  if((strcmp(Bill.userid,user.userid)==0)&&(Bill.dateout==0))
	  {
	  count=count+Bill.money;
	  }
	  i++;
	  }
	  sprintf (mail_buffer,"\n%-15s,总账户:%ld,定期:%ld,",user.userid,user.inbank,count);
	  strcat (temp, mail_buffer);
	  
		sprintf (fname,"bank/%s.bank", user.userid);
		if(fd2=open(fname,O_RDONLY )!=-1)
		{
		fd2=open(fname,O_RDWR);
		read(fd2,&cBill,sizeof(struct callbill));//读取原有信息
		close(fd2);
		}
		else
		{
		cBill.principal=0;
		}
		money=user.inbank-count-cBill.principal;
		sprintf (mail_buffer,"活期=%5ld,原先存款=%5ld",cBill.principal,money);
		strcat (temp, mail_buffer);
		if(money!=0)
		{
		
		  sprintf (mail_buffer,"\n%-15s，原先存款：%-5ld",user.userid,money);
		  strcat (temp2, mail_buffer);
		  
			id = getuser(user.userid);
			user.money += money;
			user.inbank -= money;
			substitute_record(PASSFILE, &user, sizeof(struct userec),id);
			
			  
				}
				close(fd1);	
				size--;
				}
				close(fd);
				bankreport(mail_title, temp, currentuser.userid, NULL);
	bankreport(mail_title, temp2, currentuser.userid, NULL);
	
}*/

/*
int research_current_deposit(){
	struct userec user;	
	struct callbill cBill;
	struct stat kkk;
	int all;
	int fd,fd1;
	int size;
	int cmoney;
	double cinterest;
	char temp[65535]={0};
	char mail_title[STRLEN]={0};
	char mail_buffer[1024] = {0};
	char someoneID[20];
	char fname[80];
	char ans[10];


	
	cmoney=0;
	cinterest=0.0;
	clear();
	all=0;
    modify_user_mode (QUERY);
    usercomplete ("请输入您想查询的帐号: ", someoneID);
    if (someoneID[0] == 0)
    {
        getdata (0, 30, "查询所有的作者吗?[Y/N]: ", ans, 7, DOECHO, YEA);
        if ((*ans != 'Y') && (*ans != 'y'))
        {
            return;
        }
        else
            all = 1;
    }
	else if (!getuser (someoneID))
    {
        prints ("不正确的使用者代号\n");
        pressreturn ();
        return;
    }
	
	fd=open(PASSFILE,O_RDWR);
	stat(PASSFILE,&kkk);
	size=kkk.st_size/sizeof(struct userec);
	
	if(all==1){
		
		sprintf (mail_title,"%s(累计%d)","所有用户活期利率查询",size);
		while(size>0){


	


			sprintf (fname,"bank/%s.bank", user.userid);
			if(fd1=open(fname,O_RDONLY )!=-1)
			{	
				get_record (fname, &cBill,sizeof(struct callbill), 1);	//modified
				cmoney+=cBill.principal;
				cinterest+=cBill.interest;
			}
			else
			{
				cBill.principal=0;
				cBill.interest=0.0;
			}
			sprintf (mail_buffer,"\n%-15s,活期：%16ld,利息：%8f",user.userid,cBill.principal,cBill.interest);
			strcat (temp, mail_buffer);
			size--;
		}
		sprintf (mail_buffer,"\n总数 %ld,总利息：%8.4f",cmoney,cinterest);
		strcat (temp, mail_buffer);
		
	}
	else
	{	
		sprintf (mail_title,"%s活期利率查询",someoneID);
		sprintf (fname,"bank/%s.bank", someoneID);
		if(fd1=open(fname,O_RDONLY )!=-1)
		{
			get_record (fname, &cBill,sizeof(struct callbill), 1);	//modified

		}
		else
		{
			cBill.principal=0;
		}
		sprintf (temp,"\n%-15s,活期：%6ld,利息：%8f",someoneID,cBill.principal,cBill.interest);	
	}
	close(fd);
	bankreport(mail_title, temp, currentuser.userid, NULL);
}*/
