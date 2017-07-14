/*
    Pirate Bulletin Board System
    Copyright (C) 1990, Edward Luke, lush@Athena.EE.MsState.EDU
    Eagles Bulletin Board System
    Copyright (C) 1992, Raymond Rocker, rocker@rock.b11.ingr.com
                        Guy Vega, gtvega@seabass.st.usm.edu
                        Dominic Tynes, dbtynes@seabass.st.usm.edu
    Firebird Bulletin Board System
    Copyright (C) 1996, Hsien-Tsung Chang, Smallpig.bbs@bbs.cs.ccu.edu.tw
                        Peng Piaw Foong, ppfoong@csie.ncu.edu.tw
	Firebird Bulletin Board System  -- Version 3.0 series
	Copyright (C) 1999, Peng Piaw Foong, Harimau.bbs@MSIA.pine.ncu.edu.tw
	Edward Ping-Da Chuang, edwardc.bbs@bbs.seed.net.tw
						
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 1, or (at your option)
    any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/
/*
$Id: bbs.h,v 1.4 2004/07/15 10:14:56 bbs Exp $
*/

#ifndef  _BBS_H_
#define _BBS_H_

#ifndef BBSIRC

/* Global includes, needed in most every source file... */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#ifndef BSD44
#include <stdlib.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <dirent.h>
#include "config.h"             /* User-configurable stuff */

#include "glossary.h"
#include "functions.h"          /* you can enable functions that you want */
#include "permissions.h"
#include "userstat.h"

#if defined (c_plusplus) || defined (__cplusplus)
extern "C"
{
#endif

#define DENY_POST_BOARDNAME "penalty"   //add by alt

#define TRAIN_ANNOUNCE_BOARDNAME	"TrainingAdmin" //add by rwzmm

#ifndef LOCK_EX
#define LOCK_EX         2       /* exclusive lock */
#define LOCK_UN         8       /* unlock */
#endif

#ifdef XINU
    extern int errno;
#endif

#define YEA (1)                 /* Booleans  (Yep, for true and false) */
#define NA  (0)

#define DOECHO (1)              /* Flags to getdata input function */
#define NOECHO (0)

    char *bfile();

    extern FILE *ufp;           /* External variable declarations */
    extern long ti;

#endif                          /* BBSIRC */

#define MAXFRIENDS (200)
#define MAXREJECTS (32)
#define NUMPERMS   (32)
#define REG_EXPIRED         360 /* 重做身份确认期限 */
#define MAXGUEST			 2000   /* 最多 guest 帐号上站个数 */
#define MAXVOTEPERIP     3      /*同一个ip不同账号最多投票数 */

#define SectNum         	    12      /*add by rwzmm , 现有分区数 */
#define FILE_BUFSIZE            255     /* max. length of a file in SHM */
#define FILE_MAXLINE            25      /* max. line of a file in SHM */
#define MAX_WELCOME             15      /* 欢迎画面数 */
#define MAX_GOODBYE             15      /* 离站画面数 */
#define MAX_ISSUE               15      /* 最大进站画面数 */
#define MAX_DIGEST              1000    /* 最大文摘数 */
#define MAX_TOP				    5       /* 最大置底文章数 */
#define MAX_POSTRETRY           100
#define MAX_MSG_SIZE            1024    /* for multi_msg, by cf */
#define MSGDIR                  ".msgdir"
#define MORE_BUFSIZE            4096
#define MAX_DIRNUM		        100     /* the max number of one user's collection */
#define MAX_FOLDERNUM		    500
#define ANONY_RANSTR_SIZE       6

#ifdef BIGGER_MOVIE
#define MAXMOVIE		8
#else
#define MAXMOVIE		6       /* lines of  activity board  */
#endif

#define ACBOARD_BUFSIZE         256 /* max. length of each line for activity board */
//#define ACBOARD_MAXLINE      210  /* max. lines of  activity board  */
#define ACBOARD_MAXLINE         2000       /* max. lines of  activity board  */
#define STRLEN                  80  /* Length of most string data */
#define NAMELEN                 40  /* Length of username/realname */
#define IDLEN                   12  /* Length of userids */

#ifdef MD5                      /* MD5 cryptographic support */
#define ENCPASSLEN         35
#else
#define ENCPASSLEN         14   /* Length of encrypted passwd field */
#endif

#define PASSLEN		 	14      /* User's password length (13 chars) */

#ifdef CODE_VALID
#define RNDPASSLEN             10       /* 暗码认证的暗码长度 (适宜范围 4~10) */
#endif

    /*#define MAXGOPHERITEMS     9999 *//* max of gopher items */
#define PASSFILE     ".PASSWDS" /* Name of file User records stored in */
#define ULIST_BASE   ".UTMP"    /* Names of users currently on line */
#define RETURN_DIR  ".RETURNDIR"        /* 回收站, add by mudboy */
#define TOPFILE_DIR ".TOPFILE"
    extern char ULIST[];

#ifndef BBSIRC

#define FLUSH       ".PASSFLUSH"        /* Stores date for user cache flushing */
#define BOARDS      ".BOARDS"   /* File containing list of boards */
#define DOT_DIR     ".DIR"      /* Name of Directory file info */
#define THREAD_DIR  ".THREAD"   /* Name of Thread file info */
#define DIGEST_DIR  ".DIGEST"   /* Name of Digest file info */
#define MARKED_DIR  ".MARKEDDIR"
#define AUTHOR_DIR  ".AUTHORDIR"
#define KEY_DIR	    ".KEYDIR"

#define INBOARD_FILE    "inboard_users"

#define QUIT 0x666              /* Return value to abort recursive functions */

#define FILE_READ           0x1         /* Ownership flags used in fileheader structure */
#define FILE_OWND           0x2         /* accessed array */
#define FILE_VISIT          0x4
#define FILE_MARKED         0x8
#define FILE_DIGEST         0x10        /* Digest Mode */
#define FILE_RECOMMENDED    0x8         /*Recommend post */
#define FILE_FORWARDED      0x20        /* Undelete file */
#define MAIL_REPLY          0x20        /* Mail Reply */
#define FILE_NOREPLY        0x40        /* No Allow Replay */
#define FILE_DELETED        0x80
#define FILE_TOP1           0x2         /*modified from 0x320 to 0x2,by acdsee */

#define VOTE_FLAG           0x1
#define NOZAP_FLAG          0x2
#define OUT_FLAG            0x4
#define ANONY_FLAG          0x8
#define NOREPLY_FLAG        0x10        /* No reply board */
#define POSITIVE_ANONY_FLAG 0x20        /* modified by littlesword@SJTUbbs,positive_anony */
#define NOPOST_FLAG 	    0x40
#define PUSH_FLAG 		    0x80
//use flag2[0]
#define BOARD_DIR_FLAG	    0x1
#define MODULE_FLAG         0x2

#define ZAPPED              0x1         /* For boards...tells if board is Zapped */

#define REG_ACCEPT	0x1;
#define	REG_REJECT	0x2;
#define	REG_DELETE	0x4;

/* these are flags in userec.flags[0] */
#define PAGER_FLAG   0x1        /* true if pager was OFF last session */
#define CLOAK_FLAG   0x2        /* true if cloak was ON last session */
#define SIG_FLAG     0x8        /* true if sig was turned OFF last session */
#define BRDSORT_FLAG 0x20       /* true if the boards sorted alphabetical */
#define CURSOR_FLAG  0x80       /* true if the cursor mode open */   /* 现在不用了 */
#define ACTIVE_BOARD 0x200      /* true if user toggled active movie board on */

#define MSG_SEPERATOR   "\
———————————————————————————————————————"
#define MULTI_LOGINS	3       /* 同时可上站 ID 数 */
#define DEF_VALUE		"★★ 欢迎光临饮水思源站! ★★"
#define DEF_FILE		"etc/whatdate"

#define ALL_PAGER       0x1
#define FRIEND_PAGER    0x2
#define ALLMSG_PAGER    0x4
#define FRIENDMSG_PAGER 0x8

#define SHIFTMODE(usernum,mode) ((usernum<MAXUSERS)?mode:mode<<4)

#define SETFILEMODE(array,usernum,mode) \
     (array[usernum%MAXUSERS] |= ((usernum<MAXUSERS)?mode:mode<<4))

#define CLRFILEMODE(array,usernum,mode) \
          (array[usernum%MAXUSERS] &= ((usernum<MAXUSERS)?~mode:~(mode<<4)))

#define CHECKFILEMODE(array,usernum,mode) \
       (array[usernum%MAXUSERS] & ((usernum<MAXUSERS)?mode:mode<<4))
#define USERIDSIZE (16)
#define USERNAMESZ (24)
#define TERMTYPESZ (10)
/* END */

#endif                          /* BBSIRC */

#if defined (c_plusplus) || defined (__cplusplus)
}
#endif
#ifndef NO_STRUCT_H
#include "struct.h"
#endif
#include "modes.h"              /* The list of valid user modes */
#if defined (c_plusplus) || defined (__cplusplus)
extern "C"
{
#endif

#define DONOTHING       0       /* Read menu command return states */
#define FULLUPDATE      1       /* Entire screen was destroyed in this oper */
#define PARTUPDATE      2       /* Only the top three lines were destroyed */
#define DOQUIT          3       /* Exit read menu was executed */
#define NEWDIRECT       4       /* Directory has changed, re-read files */
#define READ_NEXT       5       /* Direct read next file */
#define READ_PREV       6       /* Direct read prev file */
#define GOTO_NEXT       7       /* Move cursor to next */
#define DIRCHANGED      8       /* Index file was changed */
#define MODECHANGED     9       /* ... */

#define I_TIMEOUT   (-2)        /* Used for the getchar routine select call */
#define I_OTHERDATA (-333)      /* interface, (-3) will conflict with chinese */

#define SCREEN_SIZE (23)        /* Used by read menu  */

    extern int scrint;          /* Set when screen has been initialized */

/* Used by namecomplete *only* */

    extern int digestmode;      /*To control Digestmode */
    extern int returnmode;      /*To control detailed mode in return mode */

#ifndef NO_STRUCT_H             /* NO_STRUCT_H */

    extern struct userec currentuser;   /*  user structure is loaded from passwd */

/*  file at logon, and remains for the   */
/*  entire session */

    extern struct user_info uinfo;      /* Ditto above...utmp entry is stored here
                                           and written back to the utmp file when
                                           necessary (pretty darn often). */
    extern int usernum;         /* Index into passwds file user record */
    extern int utmpent;         /* Index into this users utmp file entry */
    extern int count_friends, count_users;      /*Add by SmallPig for count users and friends */

    extern int t_lines, t_columns;      /* Screen size / width */
    extern struct userec lookupuser;    /* Used when searching for other user info */

#endif                          /* NO_STRUCT_H */
    extern char someoneDIR[];
    extern int nettyNN;
    extern char netty_path[];
    extern char netty_board[];
    extern char currboard[];    /* name of currently selected board */
    extern char currBM[];       /* BM of currently selected board */

    extern int selboard;        /* THis flag is true if above is active */
    extern char blog_user_id[32];
    extern char blog_id[32];
    extern char blog_catId[32];
    extern char genbuf[1024];   /* generally used global buffer */

    extern struct commands *cmdlist;    /* main menu command list */

    extern jmp_buf byebye;      /* Used for exception condition like I/O error */

    extern struct commands *xyzlist;    /* These are command lists for all the */
    extern struct commands *talklist;   /* sub-menus */
    extern struct commands *maillist;
    extern struct commands *dellist;
    extern struct commands *maintlist;

    extern char save_title[];   /* These are used by the editor when inserting */
    extern int in_mail;
    extern int dumb_term;
    extern int showansi;
    extern time_t uptime;       /* save user last key-in time, up every 1min */

#define Min(a,b) ((a<b)?a:b)
#define Max(a,b) ((a>b)?a:b)

/*SREAD Define*/
#define SR_BMBASE       (10)
#define SR_BMDEL	    (11)
#define SR_BMMARK       (12)
#define SR_BMDIGEST     (13)
#define SR_BMIMPORT     (14)
#define SR_BMTMP        (15)
#define SR_BMUNDERLINE  (16)
/*SREAD Define*/

#define MORE_MODE_COMMON        0
#define MORE_MODE_POST          1
#define MORE_MODE_SREAD         2
#define MORE_MODE_AUSER         3
#define MORE_MODE_MAIL          4
#define MORE_MODE_STUFF         5
#define MORE_MODE_MESSAGE       6
#define MORE_MODE_WIKI          7
#define MORE_MODE_READING       8
#define MORE_MODE_DIGEST        9

#ifndef EXTEND_KEY
#define EXTEND_KEY
#define KEY_TAB         9
#define KEY_ESC         27
#define KEY_UP          0x0101
#define KEY_DOWN        0x0102
#define KEY_RIGHT       0x0103
#define KEY_LEFT        0x0104
#define KEY_HOME        0x0201
#define KEY_INS         0x0202
#define KEY_DEL         0x0203
#define KEY_END         0x0204
#define KEY_PGUP        0x0205
#define KEY_PGDN        0x0206
#endif

#define BBS_SHM_KEY (30002)

#define Ctrl(c)         ( c & 037 )
#define isprint2(c)     ( (c & 0x80) || isprint(c) )

#ifdef  SYSV
#define bzero(tgt, len)         memset( tgt, 0, len )
#define bcopy(src, tgt, len)    memcpy( tgt, src, len)

#define usleep(usec)            {               \
    struct timeval t;                           \
    t.tv_sec = usec / 1000000;                  \
    t.tv_usec = usec % 1000000;                 \
    select( 0, NULL, NULL, NULL, &t);           \
}

#endif                          /* SYSV */

/* =============== ANSI EDIT ================== */
#define   ANSI_RESET    "\033[0m"
#define   ANSI_REVERSE  "\033[7m\033[4m"
    extern int editansi;
    extern int KEY_ESC_arg;

/* ============================================ */

/* Added by deardragon 1999.11.2 */
    extern char datestring[];

/* Added End. */

//add by rwzmm @ sjtubbs, the max length in multi-line message
#define		EACH_LINE_LENGTH	61
//add end

//add by rwzmm @ sjtubbs, 新帐号注册一天以后方可以发文章
//如果要关闭此功能，将下面的宏定义注释掉即可。
#define		POST_AFTER_ONE_DAY

// add by rwzmm @ sjtubbs
// filter some key words in sjtubbs
#define KEYWORD_FILTER
// add end

/*启用实名制*/
#define 	REAL_NAME_EMAIL
#define 	REAL_NAME_EMAIL_LEN 40
#define		REAL_NAME_FILE "real_email"
#define		REAL_NAME_CHK_CODE "real_email_chk_code"
#define		REAL_NAME_EMAIL_RULE "etc/rmail_rule"
#define		REAL_NAME_EMAIL_RULE_DSC "etc/rmail_rule_dsc"

/* 聚会报名*/
#define MAX_REGIST_NUM 5005
#define REGIST_DIR "etc/juhui_regist"
#define REGIST_DATA_FILE REGIST_DIR"/data"
#define REGIST_OPENCHECK_FILE REGIST_DIR"/open_check"
#define REGIST_MAXALLOWNUM_FILE REGIST_DIR"/max_allow_num"
#define REGIST_NOTE REGIST_DIR"/note"
#define REGIST_LOG REGIST_DIR"/log"
#define REGIST_DATAVIEW_FILE REGIST_DIR"/dataview"

/*启用新匿名模块*/
#define ANONYMOUS_MODULE
#define ANONYFILE   ".ANONYMOUS" /* Name of Anonymous Users */

/*log到数据库*/
#define LOGHANDLER

#ifdef LOGHANDLER

//#define LOGFILE_NUMBER (2) /*how many log files to be used*/
//#define ROTATING_TIME (1800) /*how many seconds to ratate log file*/
//#define LOGFILE_PREFIX "reclog/sqllog"
#define LOGHANDLER_RANDOMINDEX_NUM (2)
#define LOGHANDLER_TIMEINDEX_NUM (2)
#define LOGHANDLER_ROTATING_TIME (1800)
#define LOGHANDLER_PATH_FORMT "reclog/log%d/%d"
#define LOGHANDLER_SQL "sql "
    extern int mysql_escape_string(char *src, char *dst, int len);
    extern int loghander_write(char *sql, int len);
    extern int loghandler_get_fd(void);
#endif
//added by kongyang 2005-05-03
//#define NO_POST
//#define NO_POST_OUT

//for speed limit
#define SPEED_LIMIT

/* turn on/off static-html like url
 * for example, url bbscon?board=bbs&file=M.12345.A
 * will print as bbscon,board,bbs,file,M.12345.A.html
 * this feature is designed for searh engine optimizing
 * only important URL will be reformated
 * such as bbscon, bbsdoc, bbs0an ...
 * if you turn on it, please set proper URL-Rewrite rules in http server
 */
#define STATIC_URL
/* turn on/off hugetlb shm feature
 * please check if your system support it 
 */
/* #define USE_SHM_HUGETLB */

#ifdef USE_SHM_HUGETLB
#define SHM_HUGETLB_SIZE (2*1024*1024)
#define BBS_SHM_SIZE ((sizeof(struct SHM_t)/SHM_HUGETLB_SIZE+1)*SHM_HUGETLB_SIZE)
#else
#define BBS_SHM_SIZE (sizeof(struct SHM_t))
#endif

#if defined (c_plusplus) || defined (__cplusplus)
}
#endif
#ifdef INCLUDE_FBFUNC_H
#include "fbfunc.h"
#endif
#endif                          /* of _BBS_H_ */
