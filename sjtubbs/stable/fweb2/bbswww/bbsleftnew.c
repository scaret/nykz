#include "www.h"
static int count_new_mails()
{
    struct fileheader x1;
    int unread = 0;
    char buf[1024];
    FILE *fp;

    if (currentuser.userid[0] == 0)
        return 0;
    sprintf(buf, "%s/mail/%c/%c/%s/.DIR", BBSHOME,
            toupper(currentuser.userid[0]), toupper(currentuser.userid[1]),
            currentuser.userid);
    fp = fopen(buf, "r");
    if (fp == 0)
        return 0;
    while (fread(&x1, sizeof(x1), 1, fp) > 0)
        if (!(x1.accessed[0] & FILE_READ))
            unread++;
    fclose(fp);
    return unread;
}

static int check_mybrd()
{
    FILE *fp;
    char file[200];
    char mybrd[GOOD_BRC_NUM][80], mybrd2[GOOD_BRC_NUM][80];
    int i, total = 0, mybrdnum = 0;

    if (!loginok)
        return 0;
    mybrdnum = LoadMyBrdsFromGoodBrd(currentuser.userid, mybrd);
    total = 0;
    for (i = 0; i < mybrdnum; i++)
    {
        if (has_read_perm(&currentuser, mybrd[i]))
        {
            strcpy(mybrd2[total], mybrd[i]);
            total++;
        }
    }
    if (total == mybrdnum)
        return 0;
    sethomefile(file, currentuser.userid, ".goodbrd");
    fp = fopen(file, "w");
    fwrite(mybrd2, 80, total, fp);
    fclose(fp);
    return 0;
}

int bbsleftnew_main()
{
    char *ptr;
    char buf[256];

    init_all();
    check_mybrd();
    printf("<html>");
    printf
        ("<meta HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=gb2312\">");
    printf("<link rel=stylesheet type=text/css href='/css/bbs.css'>");
    printf("<link rel=stylesheet type=text/css href='/css/blue.css'>");
    printf("<input type=hidden name=\"temp\" id=\"temp\" value=\"\">");
    printf("<script src=\"/js/bbsleft.js\"></script>");
    printf
        ("\n           <script>\n           function closebut(x, y) {\n           if(document.img0) document.img0.src='"
         UPLOAD_SERVER_HEADER
         "/folder.gif';\n           if(document.img1) document.img1.src='"
         UPLOAD_SERVER_HEADER
         "/folder.gif';\n           if(document.img2) document.img2.src='"
         UPLOAD_SERVER_HEADER
         "/folder.gif';\n           if(document.img3) document.img3.src='"
         UPLOAD_SERVER_HEADER
         "/folder.gif';\n           if(document.img4) document.img4.src='"
         UPLOAD_SERVER_HEADER
         "/folder.gif';\n           if(document.img5) document.img5.src='"
         UPLOAD_SERVER_HEADER
         "/folder.gif';\n           if(document.getElementById('div0')) document.getElementById('div0').style.display='none';\n           if(document.getElementById('div1')) document.getElementById('div1').style.display='none';\n           if(document.getElementById('div2')) document.getElementById('div2').style.display='none';\n           if(document.getElementById('div3')) document.getElementById('div3').style.display='none';\n           if(document.getElementById('div4')) document.getElementById('div4').style.display='none';\n           if(document.getElementById('div5')) document.getElementById('div5').style.display='none';\n           x.style.display='block';\n           y.src='"
         UPLOAD_SERVER_HEADER
         "/folder2.gif';\n       }\n           function t(x, y) {\n           if(x.style.display!='none') {\n           x.style.display='none';\n           y.src='"
         UPLOAD_SERVER_HEADER
         "/folder.gif';\n       }\n           else\n           closebut(x, y);\n       }\n           function openchat() {\n           url='bbschat';\n           chatWidth=screen.availWidth;\n           chatHeight=screen.availHeight;\n           winPara='toolbar=no,location=no,status=no,menubar=no,scrollbars=auto,resizable=yes,left=0,top=0,'+'width='+(chatWidth-10)+',height='+(chatHeight-27);\n           window.open(url,'_blank',winPara);\n       }\n           </script>\n           ");
    printf("<nobr>\n");
    printf("<body class=left>\n");
    printf
        ("<iframe id=\"framehide\" name=\"framehide\" width=\"0\" height=\"0\" src=\"\" frameborder=\"0\" scrolling=\"no\"></iframe>");
    if (!loginok)
    {
        printf
            ("\n               <form action=bbslogin target=_top method=post><br>\n               �û���¼:<br>\n               �ʺ� <input style='height:20px;' type=text name=id maxlength=12 size=8><br>\n               ���� <input style='height:20px;' type=password name=pw maxlength=12 size=8><br>\n               <input style='width:45px; ' type=submit value=��¼>\n	       <input style='width:45px; ' name=\"button\" type=\"button\" onClick=\"open('bbsreg', '', 'width=620,height=550')\" value=\"ע��\" class=\"button\">\n               <hr class=left width=94px align=left></form>");
    }
    else
    {
        strcpy(buf, "δע���û�");
        printf
            ("<br>\n�û�: <a class=left href=bbsqry?userid=%s target=f3>%s</a><br>",
             currentuser.userid, currentuser.userid);

        if (currentuser.userlevel & PERM_LOGINOK)
            strcpy(buf, "��ͨվ��");
        if (currentuser.userlevel & PERM_BOARDS)
            strcpy(buf, "����");
        if (currentuser.userlevel & PERM_XEMPT)
            strcpy(buf, "�����ʺ�");
        if (currentuser.userlevel & PERM_CHATCLOAK)
            strcpy(buf, "��վ����");
        if (currentuser.userlevel & PERM_SYSOP)
            strcpy(buf, "��վվ��");
        printf("����: %s<br>", buf);
        printf
            ("<a class=left href=bbslogout target=_top>[ע�����ε�¼]</a>\n                <hr class=left width=94px align=left>\n");
    }
    printf("<form action=bbssel target=f3>\n");
    printf
        ("ѡ��������: <br><input type=text name=board maxlength=15 size=8>");
    printf(" <input type=submit value=go>");
    printf("</form>\n");
    printf("<hr class=left width=94px align=left>\n");
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/link0.gif><a class=left target=f3 href=bbssec>��������ҳ</a><br>\n");
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/link0.gif><a class=left target=f3 href=bbs0an>����������</a><br>\n");
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/link0.gif><a class=left target=f3 href=php/bbsindex.html>WEB����</a><br>\n");
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/link0.gif><a class=left target=f3 href=wiki>ˮԴ�ٿ�</a><br>\n");
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/link0.gif><a class=left target=f3 href=bbstopb10>����������</a><br>\n");
    if (loginok)
    {
        printf
            ("<a href=javascript:submenu(\"main.hck\",1) target=\"_self\"><img id=\"img_main.hck\" src=\"/gif/folderclose.gif\" class=pm></a><a class=left href=\"bbsshowhome?boardfile=main.hck\" target=\"f3\">�ҵ��ղؼ�</a><br>\n");
        printf("<div id=\"main.hck\" style='display:none'></div>");
    }
    printf("\n           <img src=" UPLOAD_SERVER_HEADER
           "/folder.gif name=img1><a class=left href=\"javascript: t(document.getElementById('div1'), document.img1)\">����������</a><br>\n           <div id=div1 style='display:none'>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=0> BBSϵͳ </a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=1> �Ϻ�����</a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=2> ѧ��ԺУ</a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=3> ���Լ���</a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=4> ѧ����ѧ</a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=5> �����Ļ�</a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=6> �����˶�</a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=7> ��������</a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=8> ֪�Ը���</a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=9> �����Ϣ</a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=10> ����Ⱥ��</a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsboa?sec=11> ��Ϸר��</a><br>\n           <img src="
           UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsall> ȫ��������</a><br>\n           </div>\n           ");
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/folder.gif name=img2><a class=left href=\"javascript: t(document.getElementById('div2'), document.img2)\"≯��˵����</a><br>\n");
    printf("<div id=div2 style='display:none'>\n");
    if (loginok)
    {
        printf("<img src=" UPLOAD_SERVER_HEADER
               "/link.gif><a class=left href=bbsfriend target=f3> ���ߺ���</a><br>\n");
    }
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/link.gif><a class=left href=bbsusr target=f3> �����ķ�</a><br>\n");
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/link.gif><a class=left href=bbsqry target=f3> ��ѯ����</a><br>\n");
    if (currentuser.userlevel & PERM_PAGE)
    {
        printf("<img src=" UPLOAD_SERVER_HEADER
               "/link.gif><a class=left href=bbssendmsg target=f3> ����ѶϢ</a><br>\n");
        printf("<img src=" UPLOAD_SERVER_HEADER
               "/link.gif><a class=left href=bbsmsg target=f3> �鿴����ѶϢ</a><br>\n");
    }
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/link.gif><a class=left href=bbsshowinfo target=f3> �����������</a><br>\n");
    printf("</div>\n");
    ptr = "";
    if (currentuser.userlevel & PERM_CLOAK)
        ptr = "<img src=" UPLOAD_SERVER_HEADER
            "/link.gif><a class=left target=f3 onclick='return confirm(\"ȷʵ�л�����״̬��?\")' href=bbscloak> �л�����</a><br>\n";

    if (loginok)
    {
            printf("\n               <img src=" UPLOAD_SERVER_HEADER
                   "/folder.gif name=img3><a class=left href=\"javascript: t(document.getElementById('div3'), document.img3)\">���˹�����</a><br>\n               <div id=div3 style='display:none'>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/link.gif><a class=left target=f3 href=bbsinfo> ��������</a><br>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/link.gif><a class=left target=f3 href=bbsplan> ��˵����<a><br>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/link.gif><a class=left target=f3 href=bbssig> ��ǩ����<a><br>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/link.gif><a class=left target=f3 href=bbspwd> �޸�����</a><br>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/link.gif><a class=left target=f3 href=bbsparm> �޸ĸ��˲���</a><br>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/link.gif><a class=left target=f3 href=bbsmywww> WWW���˶���</a><br>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/link.gif><a class=left target=f3 href=bbsnick> ��ʱ���ǳ�</a><br>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/link.gif><a class=left target=f3 href=bbsfall> �趨����</a><br>\n               %s</div>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/folder.gif name=img5><a class=left href=\"javascript: t(document.getElementById('div5'), document.img5)\">�����ż���</a><br>\n"
                   , ptr);
            printf("<div id=div5 style='display:none'>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/link.gif><a class=left target=f3 href=bbsnewmail> �������ʼ�</a><br>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/link.gif><a class=left target=f3 href=bbsmail> �����ʼ�</a><br>\n               <img src="
                   UPLOAD_SERVER_HEADER
                   "/link.gif><a class=left target=f3 href=bbspstmail> �����ʼ�</a><br>\n               </div>\n               ");
                   
    }
    
    printf("<img src=" UPLOAD_SERVER_HEADER "/folder.gif name=img4>");
    printf
        ("<a class=left href=\"javascript: t(document.getElementById('div4'), document.img4)\">�ر������</a><br>\n");
    printf("<div id=div4 style='display:none'>\n");
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsnotepad>�������԰�</a><br>\n");
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/link.gif><a class=left target=f3 href=bbsalluser>����ʹ����</a><br>\n");

    if (currentuser.userlevel & PERM_OBOARDS)
    {
        printf("<img src=" UPLOAD_SERVER_HEADER
               "/link.gif><a class=left target=f3 href=bbsallf>�������</a><br>\n");
        printf("<img src=" UPLOAD_SERVER_HEADER
               "/link.gif><a class=left target=f3 href=bbsfind2>���ز�ѯ</a><br>\n");
    }
    printf("</div>\n");
    if (loginok)
    {
        if (currentuser.userlevel & PERM_REALNAME)
        {
            printf("<img src=" UPLOAD_SERVER_HEADER
                   "/link0.gif><a class=left href=bbsfind target=f3>���²�ѯ</a><br>\n");
        }
    }

    int juhuiRegIsOpen = 0;
    FILE *fp;

    if ((fp = fopen(REGIST_OPENCHECK_FILE, "r")) != NULL)
    {
        fscanf(FCGI_ToFILE(fp), "%d", &juhuiRegIsOpen);
        fclose(fp);
    }

    if (juhuiRegIsOpen || (currentuser.userlevel & PERM_OVOTE))
    {
        printf("<img src=" UPLOAD_SERVER_HEADER
               "/link0.gif><a class=left href=bbsregistview target=f3>վ�챨��</a><br>\n");
    }

    printf("<img src=" UPLOAD_SERVER_HEADER
           "/link0.gif><a class=left href='javascript:top.f3.document.location.reload()'>ˢ�°���</a><br>\n");
    printf("<img src=" UPLOAD_SERVER_HEADER
           "/telnet.gif><a class=left href='telnet:%s'>Telnet��¼</a><br>\n",
           BBSHOST);
    if (!loginok)
        printf("<img src=" UPLOAD_SERVER_HEADER
               "/link0.gif><a class=left href=\"javascript:void open('bbsreg', '', 'width=620,height=550')\">���û�ע��</a><br>\n");
    if (loginok)
    {

        if (HAS_PERM(PERM_LOGINOK) && !HAS_PERM(PERM_POST))
            printf
                ("<script>alert('���������ȫվ�������µ�Ȩ��, ��ο�penalty�幫��, ��������sysop��������. ��������, ����sysop���������.')</script>\n");
        if (count_new_mails() > 0)
            printf("<script>alert('�������ż�!')</script>\n");
    }
    if (loginok && !(currentuser.userlevel & PERM_LOGINOK)
        && !has_fill_form())
        printf
            ("<br><a class=left style='color:red; font-size:15px' target=f3 href=bbsform><b>��дע�ᵥ</b></a><br>\n");
    printf("<script>function addiv() { \n \
           if(document.getElementById('ad')) { \
               var addiv = document.getElementById('ad'); \n \
               var adb = document.getElementById('adb'); \n \
               if(addiv.style.visibility == 'hidden' ) { \n \
                   addiv.style.visibility = 'visible'; \n \
                   adb.innerHTML = '�ر�'; \n \
               } else { \n \
                   addiv.style.visibility = 'hidden'; \n \
                   adb.innerHTML = 'չ��'; \n \
              } \n \
           } }\n \
           </script>");

    printf
        ("<br>[���]&nbsp;<a href=# class=left><span id=adb onclick='javascript:addiv();'>�ر�</span></a><br /><div id=ad>");

    if ((fp = fopen("etc/Ads_Left", "r")) != NULL)
    {
        while (fgets(buf, STRLEN, fp))
        {
            if (buf == NULL || strlen(buf) == 0 || buf[0] == '#')
                continue;
            printf("%s\n", buf);
        }
        fclose(fp);
    }

    printf("</div>");

    printf
        ("<script src=\"http://www.google-analytics.com/urchin.js\" type=\"text/javascript\"></script><script type=\"text/javascript\"> _uacct = \"UA-144567-2\"; _usample=13.6;urchinTracker(); </script>");
    printf("</body></html>");

    return 0;
}