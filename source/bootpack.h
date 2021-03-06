/* asmhead.nas */
struct BOOTINFO { /* 0x0ff0-0x0fff */
	char cyls; /* 启动区读磁盘读到此为止 */
	char leds; /* 启动时键盘的LED的状态 */
	char vmode; /* 显卡模式为多少位彩色 */
	char reserve;
	short scrnx, scrny; /* 画面分辨率 */
	short *vram;
};
#define ADR_BOOTINFO	0x00000ff0
#define ADR_DISKIMG		0x00100000


/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
int io_in8(int port);
int io_in16(int port);		//*2
void io_out8(int port, int data);
void io_out16(int port, int data);//*2
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
int load_cr0(void);
void store_cr0(int cr0);
void load_tr(int tr);
void asm_inthandler0c(void);
void asm_inthandler0d(void);
void asm_inthandler20(void);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);
unsigned int memtest_sub(unsigned int start, unsigned int end);
void farjmp(int eip, int cs);
void farcall(int eip, int cs);
void asm_hrb_api(void);
void start_app(int eip, int cs, int esp, int ds, int *tss_esp0);
void asm_end_app(void);


/* fifo.c */
struct FIFO32 {
	int *buf;
	int p, q, size, free, flags;
	struct TASK *task;
};
void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task);
int fifo32_put(struct FIFO32 *fifo, int data);
int fifo32_get(struct FIFO32 *fifo);
int fifo32_status(struct FIFO32 *fifo);

/* LJH new */
struct xhl {
	volatile int lock;//use to make the operation atomic
	volatile int mutex;//�M����
	volatile int size;
	volatile int waiting[20];
};
//void init_cpt(struct CPT cpt, char *name1, char *name2);
void Reader();
void Write();
void mutex_init(volatile struct xhl* m);
void signal(volatile struct xhl* m);
void wait(volatile struct xhl* m, int i);
int TestAndSet(volatile int* dest);


/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned short *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen8(short *vram, int x, int y, char *fname);
void putfont8(short *vram, int xsize, int x, int y, unsigned char c, char *font);
void putfonts8_asc_pre(short *vram, int xsize, int x, int y, unsigned char c, unsigned char *s);
void putfonts8_asc(short *vram, int xsize, int x, int y, unsigned char c, unsigned char *s);
void init_mouse_cursor8(short *mouse, unsigned char bc);
void putblock8_8(short *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, short *buf, int bxsize);
//int read_picture(int *fat, short *vram, int x, int y);
int read_picture(int *fat, short *vram, int x, int y, char *fname);
unsigned short rgb2pal(int r, int g, int b, int x, int y, int cb);
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15
#define COL8_336666     16
#define COL8_3D7878     17
#define COL8_408080     18
#define COL8_4F9D9D     19
#define COL8_5CADAD     20
#define COL8_6FB7B7     21
#define COL8_81C0C0     22
#define COL8_95CACA     23
#define COL8_A3D1D1     24
#define COL8_B3D9D9     25
#define COL8_C4E1E1     26
#define COL8_D1E9E9     27


/* dsctbl.c */
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff

#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff

#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff

#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_TSS32		0x0089
#define AR_INTGATE32	0x008e


/* int.c */
void init_pic(void);
void inthandler27(int *esp);
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

/* keyboard.c */
void inthandler21(int *esp);
void wait_KBC_sendready(void);
void init_keyboard(struct FIFO32 *fifo, int data0);
#define PORT_KEYDAT		0x0060
#define PORT_KEYCMD		0x0064

/* mouse.c */
struct MOUSE_DEC {
	unsigned char buf[3], phase;
	int x, y, btn;
};
void inthandler2c(int *esp);
void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);


/* memory.c */
#define MEMMAN_FREES 4090 /* 大约是32KB*/
#define MEMMAN_ADDR			0x003c0000
struct FREEINFO { /* 可用信息 */
	unsigned int addr, size;
};
struct MEMMAN { /* 内存管理 */
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO free[MEMMAN_FREES];
};
unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size);
int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size);


/* sheet.c */
#define MAX_SHEETS		256
struct SHEET {
	unsigned short *buf;
	int bxsize, bysize, vx0, vy0, col_inv, height, flags;
	struct SHTCTL *ctl;
	struct TASK *task;
};
struct SHTCTL {
	unsigned short *vram;
	unsigned char *map;
	int xsize, ysize, top;
	struct SHEET *sheets[MAX_SHEETS];
	struct SHEET sheets0[MAX_SHEETS];
};
struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned short *vram, int xsize, int ysize);
struct SHEET *sheet_alloc(struct SHTCTL *ctl);
void sheet_setbuf(struct SHEET *sht, unsigned short *buf, int xsize, int ysize, int col_inv);
void sheet_updown(struct SHEET *sht, int height);
void sheet_refresh(struct SHEET *sht, int bx0, int by0, int bx1, int by1);
void sheet_slide(struct SHEET *sht, int vx0, int vy0);
void sheet_free(struct SHEET *sht);


/* timer.c */
#define MAX_TIMER		500
struct TIMER {
	struct TIMER *next;
	unsigned int timeout, flags;
	struct FIFO32 *fifo;
	int data;
};
struct TIMERCTL {
	unsigned int count, next;
	struct TIMER *t0;
	struct TIMER timers0[MAX_TIMER];
};
extern struct TIMERCTL timerctl;
void init_pit(void);
struct TIMER *timer_alloc(void);
void timer_free(struct TIMER *timer);
void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data);
void timer_settime(struct TIMER *timer, unsigned int timeout);
void inthandler20(int *esp);


/* mtask.c */
#define MAX_TASKS 1000	/*最大任务数量*/
#define TASK_GDT0 3		/*定义从GDT的几号开始分配给TSS */
#define MAX_TASKLEVELS  3
#define MAX_FAT		2880	// 磁盘空间扇区数

#define TASK_UNUSE 0
#define TASK_IDLE 1
#define TASK_RUNNING 2


#define MAX_RT_PRIO 100
#define MAX_PRIO  140
#define USER_PRIO(p) ((p)-MAX_RT_PRIO)  
#define MAX_USER_PRIO       (USER_PRIO(MAX_PRIO)) 
#define NICE_TO_PRIO(nice)  (MAX_RT_PRIO + (nice) + 20)
#define max(a,b) a>b?a:b
#define SCALE_PRIO(x, prio)  x*(140 - prio)/1000
# define HZ     1000      
#define DEF_TIMESLICE       100

struct TSS32 {
	int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	int es, cs, ss, ds, fs, gs;
	int ldtr, iomap;
};
struct TASK {
	int 	sel, flags;		/* sel用来存放GDT的编号*/
	int 	level, priority; /* 优先级 */
	int 	nice;//100-139
	int 	wait_time;
	struct 	FIFO32 fifo;
	struct 	TSS32 tss;
	struct 	TASK *next;
	int 	cons_stack;
	char 	name[16];

	/*desktop*/
	unsigned char langmode, langbyte1;
};
struct TASKLEVEL {
	int running_now;
	int running; /*正在运行的任务数量*/
	struct TASK *now; /*这个变量用来记录当前正在运行的是哪个任务*/
	struct TASK *tasks;
};
struct TASKCTL {
	int now_lv; /*现在活动中的LEVEL */
	char lv_change; /*在下次任务切换时是否需要改变LEVEL */
	struct TASKLEVEL level[MAX_TASKLEVELS];
	struct TASK tasks0[MAX_TASKS];
};

extern struct TIMER *task_timer;
struct TASK *task_init(struct MEMMAN *memman);
struct TASK *task_alloc(void);
struct TASK *task_now(void);
void task_run(struct TASK *task, int level, int priority);
void task_switch(void);
void task_sleep(struct TASK *task);
void task_remove(struct TASK *task);
void printInfo();

/* window.c */
void make_window8(unsigned short *buf, int xsize, int ysize, char *title, char act);
void make_wtitle8(unsigned short *buf, int xsize, char *title, char act);

void putfonts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l);
void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy, int c);

/* file.c */
#define DIR_MAXSIZE 224						//* 目录下文件信息数量上限
struct FILEINFO {
	unsigned char name[8], ext[3], type;
	char reserve[10];
	unsigned short time, date, clustno;
	unsigned int size;
};
struct DIRINFO {							//* 当前目录信息
	struct FILEINFO *adr_dir;				// 本级目录地址
	struct DIRINFO *adr_parent;				// 上级目录地址
	unsigned int clustno;					// 存放当前目录的起始扇区
	unsigned int maxsize;					// 当前目录的最大finfo数
	char name[9];							// 本级目录名
};
//-----------------disk-------------------
void dskin(unsigned int cluster, unsigned int fatadr);
void dskout(unsigned int cluster, unsigned int fatadr);
char dsk_fat_read(int *fat);
char dsk_fat_write(int *fat);
char dsk_write(int *fat, unsigned int clustno, unsigned char *buf, unsigned int size);
char dsk_read(int *fat, unsigned int clustno, unsigned char *buf, unsigned int size);

/* tek.c */
int tek_getsize(unsigned char *p);
int tek_decomp(unsigned char *p, char *q, int size);

void file_readfat(int *fat, unsigned char *img);
void file_loadfile(int clustno, int size, char *buf, int *fat, char *img);
char *file_loadfile2(int clustno, int *psize, int *fat);
struct FILEINFO *file_search(char *name, struct FILEINFO *finfo, int max);
void finfo_init(struct FILEINFO * finfo);					// FILEINFO初始化
struct FILEINFO * file_addfile(struct FILEINFO *finfo, struct FILEINFO *af, int max);//*
int file_remove(int *fat, struct FILEINFO *delf);			//
//int file_writefile(int *fat, unsigned char *img, int clustno, struct FILEINFO *fileinf, int max);	//**
int allo_fat(int *fat);										//
void encoding_fat(int *fat, unsigned char *img_fat);		// fat编码
int test_encoding(int *fat, char *img_fat);					// 测试encoding 不正确则返回零


/* console.c */
struct CONSOLE {
	struct SHEET *sht;
	int cur_x, cur_y, cur_c;
	struct DIRINFO *dir_info;					//* 文件目录地址
};
struct FILEHANDLE {								//*3 -------
	char *buf;
	int size;
	int pos;
};
void printmem(void);
void console_task(struct SHEET *sheet, unsigned int memtotal);
void cons_putchar(struct CONSOLE *cons, int chr, char move);
void cons_newline(struct CONSOLE *cons);
void cons_putstr0(struct CONSOLE *cons, char *s);
void cons_putstr1(struct CONSOLE *cons, char *s, int l);
void cons_runcmd(char *cmdline, struct CONSOLE *cons, int *fat, unsigned int memtotal);
void cmd_mem(struct CONSOLE *cons, unsigned int memtotal);
void cmd_cls(struct CONSOLE *cons);
void cmd_dir(struct CONSOLE *cons);//wzh----
void cmd_ls(struct CONSOLE *cons);
void cmd_mkf(struct CONSOLE *cons, char *cmdline);			
void cmd_mkd(struct CONSOLE *cons, int *fat, char *cmdline);
void cmd_del(struct CONSOLE *cons, int *fat, char *cmdline);
char get_type(char *cmdline);								 
void cmd_cd(struct CONSOLE *cons, char *cmdline, int *fat);	
void cmd_finf(struct CONSOLE *cons, char *cmdline);
void cmd_infc(struct CONSOLE *cons, char *cmdline);
void test_disk(struct CONSOLE *cons, int *fat);
void show_fat(struct CONSOLE *cons, int *fat);
void cmd_fat_test(struct CONSOLE *cons, int *fat);
void cmd_type(struct CONSOLE *cons, int *fat, char *cmdline);
int cmd_app(struct CONSOLE *cons, int *fat, char *cmdline);
int *hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax);
int *inthandler0c(int *esp);
int *inthandler0d(int *esp);
// --- file.c
char dsk_read0(struct CONSOLE *cons, int *fat, unsigned int clustno, unsigned char *buf, unsigned int size);
char dsk_write0(struct CONSOLE *cons, int *fat, unsigned int clustno, unsigned char *buf, unsigned int size);
void cd_up(struct CONSOLE *cons);
int cd_down(struct CONSOLE *cons, char *dirname, int *fat);


/* Desktop */

/*jpeg.c*/
struct DLL_STRPICENV {	/* 64KB */
	int work[64 * 1024 / 4];
};
struct RGB {
	unsigned char b, g, r, t;
};


/* bmp.nasm */
int info_BMP(struct DLL_STRPICENV *env, int *info, int size, unsigned char *fp);
int decode0_BMP(struct DLL_STRPICENV *env, int size,unsigned char *fp, int b_type,unsigned char *buf, int skip);


/* jpeg.c */
int info_JPEG(struct DLL_STRPICENV *env, int *info, int size, unsigned char *fp);
int decode0_JPEG(struct DLL_STRPICENV *env, int size, unsigned char *fp, int b_type, unsigned char *buf, int skip);

void init_open(unsigned short *vram1, int xsize1, struct FIFO32 *fifo);

/* LJH */
struct TASK *open_constask(struct SHEET *sht, unsigned int memtotal);
struct SHEET *open_console(struct SHTCTL *shtctl, unsigned int memtotal);
struct SHEET *open_console1(struct SHTCTL *shtctl, unsigned int memtotal);
void task_run_in2(struct TASK *task,int Nice);








