#ifndef _WAVEDUMP_H_
#define _WAVEDUMP_H_

#define _GNU_SOURCE // for CPU affinity etc...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <CAENDigitizer.h>
#include <CAENDigitizerType.h>

#include <unistd.h>
#include <stdint.h>   /* C99 compliant compilers: uint64_t */
#include <ctype.h>    /* toupper() */
#include <sys/time.h>

#include <sys/socket.h>
#include <netinet/in.h>	// for "sockaddr_in"
#include <netdb.h>	// for "gethostbyname"

/* GC added */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>        /* necessaria per tm- structure (dereferencing pointer to incomplete type)*/
#include <sched.h>
#include <pthread.h>
#include <semaphore.h>
#include <setjmp.h>
#include "zlib.h"
#include <unistd.h>   // getch_
#include <termios.h>  // getch_

#define      _PACKED_      __attribute__ ((packed, aligned(1)))
#define      _INLINE_      __inline__ 
#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define DEFAULT_CONFIG_FILE    "WaveDumpConfig.txt"
#define DEFAULT_CONFIG_FILE_0  "WaveDumpConfig_0.txt"
#define DEFAULT_CONFIG_FILE_1  "WaveDumpConfig_1.txt"
#define DEFAULT_CONFIG_FILE_2  "WaveDumpConfig_2.txt"
#define DEFAULT_CONFIG_FILE_3  "WaveDumpConfig_3.txt"
#define DEFAULT_CONFIG_FILE_4  "WaveDumpConfig_4.txt"
#define DEFAULT_CONFIG_FILE_5  "WaveDumpConfig_5.txt"
#define DEFAULT_CONFIG_FILE_6  "WaveDumpConfig_6.txt"
#define DEFAULT_CONFIG_FILE_7  "WaveDumpConfig_7.txt"
#define DEFAULT_CONFIG_FILE_8  "WaveDumpConfig_8.txt"
#define DEFAULT_CONFIG_FILE_9  "WaveDumpConfig_9.txt"
#define GNUPLOT_DEFAULT_PATH ""

#define OUTFILENAME "wave"  /* The actual file name is wave_n.txt, where n is the channel */
#define MAX_WD   6       /* max. number of digitizers */
#define NUM_WD   2       /* number of digitizers */        
#define MAX_CH  64       /* max. number of channels */
#define MAX_SET 16       /* max. number of independent settings */

#define MAX_GW  1000     /* max. number of generic write commads */

#define PLOT_REFRESH_TIME 1000

#define VME_INTERRUPT_LEVEL      1
#define VME_INTERRUPT_STATUS_ID  0xAAAA
#define INTERRUPT_MODE           CAEN_DGTZ_IRQ_MODE_ROAK
#define INTERRUPT_TIMEOUT        200  // ms

#define PLOT_WAVEFORMS   0
#define PLOT_FFT         1
#define PLOT_HISTOGRAM   2

#define MESSLEN          256

/******************************************************************************
* Digitizer Registers (not documented) Address Map 
******************************************************************************/
#define CAEN_DGTZ_TEST_IO_ADD                        0x8168
#define CAEN_DGTZ_START_DELAY_ADD                    0x8170
#define CAEN_DGTZ_SYNC_ADD                           0x813C
#define CAEN_DGTZ_ALMOSTFULL                         0x816C
// 20170610 GC
#define CAEN_DGTZ_LVDS_NEWFEATURES                   0x81A0

/* ###########################################################################
   Typedefs
   ###########################################################################
*/

typedef enum {
     OFF_BINARY= 0x00000001,         // Bit 0: 1 = BINARY, 0 =ASCII
     OFF_HEADER= 0x00000002,         // Bit 1: 1 = include header, 0 = just samples data
} OUTFILE_FLAGS;


typedef struct WaveDumpConfig_t {
    int nBoards;
    int LinkType;
    int LinkNum;
    int ConetNode;
    uint32_t BaseAddress;
    CAEN_DGTZ_BoardInfo_t BoardInfo;
    int Nch;
    int Nbit;
    float Ts;
    int InputDyn;
    int NumEvents;
    int RecordLength;
    int PostTrigger;
    int InterruptNumEvents;
    int TestPattern;
    int DesMode;
    int SWZScopy;
    int FPIOtype;
    CAEN_DGTZ_TriggerMode_t ExtTriggerMode;
    uint8_t EnableMask;
    uint8_t TriggerMask;
    uint8_t SWZSoff;
    uint8_t SWZSwrt;
    uint32_t DCoffset[MAX_SET];
    CAEN_DGTZ_TriggerMode_t ChannelTriggerMode[MAX_SET];
    uint32_t Threshold[MAX_SET];
    uint32_t Hysteresis[MAX_SET];
    CAEN_DGTZ_TriggerPolarity_t TriggerEdge[MAX_SET]; // GC TM 20120420 - in 2011 era int
    int SWZSon[MAX_SET];
    uint32_t SWZSthr[MAX_SET];
    uint32_t SWZShyst[MAX_SET];
    int SWZSedge[MAX_SET];
    uint8_t GroupTrgEnableMask[MAX_SET];
   
    int GWn;
    uint32_t GWaddr[MAX_GW];
    uint32_t GWdata[MAX_GW];
    OUTFILE_FLAGS OutFileFlags;

//to be added asap - TM 2012
   //double dynamic_range;

} WaveDumpConfig_t;



typedef struct WaveDumpRun_t {
    int Quit;
    int AcqRun;
    int PlotType;
    int ContinuousTrigger;
    int ContinuousWrite;
    int SingleWrite;
    int ContinuousPlot;
    int SinglePlot;
    int SetPlotOptions;
    int GroupPlotIndex;
    char ChannelPlotMask;
    int Restart;
    int RunHisto;
    uint32_t *Histogram[MAX_CH];
    FILE *fout[MAX_CH];
} WaveDumpRun_t;

typedef struct ChannelEventHeader_t {
   uint32_t ChannelEventSize;
   uint32_t BoardId;
   uint32_t Pattern;
   uint32_t ChannelId;
   uint32_t EventCounter;
   uint32_t TriggerTimeTag;
} _PACKED_ ChannelEventHeader_t;

/* GC add */
/* output file structure */

/* 
   EventHeader (SOB)
   SOBurst 
   WaveDumpConfig Board 0
   WaveDumpConfig Board 1
   EventHeader (Event)
   ... Event Data
   EventHeader (Event)
   ... Event Data
   ...
   EventHeader (EOB)
   EOBurst 
*/

typedef enum {
     SOB = 0x00000001,         // 
     EVT = 0x00000002,         // 
     EOB = 0x00000004,
} evtflag_t;

typedef struct EventHeader_t { /* Block info + Event info*/
   evtflag_t EventType;  
   uint32_t EventSize;
   uint32_t BoardId;
   uint32_t Pattern;
   uint32_t ChannelSize[MAX_CH];
   uint32_t EventCounter;
   uint32_t TriggerTimeTag;
// ...
} _PACKED_ EventHeader_t;

typedef struct SOBurst_t { /* start of burst info*/
   uint32_t RunNumber;
   uint32_t BurstCounter;
   struct tm StartBurstTime;
// ...
} _PACKED_ SOBurst_t;

typedef struct EOBurst_t { /* end of burst info*/
   uint32_t BurstCounter;
   struct tm EndBurstTime;
   uint32_t BurstLength; /* ms */
   uint32_t nEvents[MAX_WD];
   float DeadTime[MAX_WD];
   uint32_t TBOXscale[3][8];
   uint32_t AUXscale[3][16];
   uint32_t nLiveCycles[3];
// ...
} _PACKED_ EOBurst_t;

/* Error messages */

typedef enum  {
   ERR_NONE= 0,
   ERR_CONF_FILE_NOT_FOUND,
   ERR_LOG_FILE_NOT_FOUND,
   ERR_RUNINFO_FILE_NOT_FOUND,
   ERR_RUNDATA_FILE_NOT_FOUND,
   ERR_DGZ_OPEN,
   ERR_BOARD_INFO_READ,
   ERR_INVALID_BOARD_TYPE,
   ERR_DGZ_PROGRAM,
   ERR_MALLOC,
   ERR_RESTART,
   ERR_INTERRUPT,
   ERR_READOUT,
   ERR_EVENT_BUILD,
   ERR_HISTO_MALLOC,
   ERR_UNHANDLED_BOARD,
   ERR_OUTFILE_WRITE,
   ERR_FORCED_EXIT,
   ERR_ACQ_NOT_READY,
   ERR_ACQ_NO_TRGOUT,
   ERR_ACQ_PLL_UNLOCK,
   ERR_WRITE_DATA,
   ERR_BUFFER_OVERFLOW,
   ERR_DUMMY_LAST,
} ERROR_CODES;

static char ErrMsg[ERR_DUMMY_LAST][100] = {
   "No Error",                                         /* ERR_NONE */
   "Configuration File not found",                     /* ERR_CONF_FILE_NOT_FOUND */
   "Log File not found",                               /* ERR_LOG_FILE_NOT_FOUND */
   "run.info File not found",                          /* ERR_RUNINFO_FILE_NOT_FOUND */
   "run.data File not found",                          /* ERR_RUNDATA_FILE_NOT_FOUND */
   "Can't open the digitizer",                         /* ERR_DGZ_OPEN */
   "Can't read the Board Info",                        /* ERR_BOARD_INFO_READ */
   "Can't run WaveDump for this digitizer",            /* ERR_INVALID_BOARD_TYPE */
   "Can't program the digitizer",                      /* ERR_DGZ_PROGRAM */
   "Can't allocate the memory for the readout buffer", /* ERR_MALLOC */
   "Restarting Error",                                 /* ERR_RESTART */
   "Interrupt Error",                                  /* ERR_INTERRUPT */
   "Readout Error",                                    /* ERR_READOUT */
   "Event Build Error",                                /* ERR_EVENT_BUILD */
   "Can't allocate the memory fro the histograms",     /* ERR_HISTO_MALLOC */
   "Unhandled board type",                             /* ERR_UNHANDLED_BOARD */
   "Output file write error",                          /* ERR_OUTFILE_WRITE */
   "Forced Exit",                                      /* ERR_FORCED_EXIT */ /* TEST MODE */
   "ACQ not Ready",                                    /* ERR_ACQ_NOT_READY */
   "ACQ no TRGOUT",                                    /* ERR_ACQ_NO_TRGOUT */
   "PLL unlock",                                       /* ERR_ACQ_PLL_UNLOCK */
   "Write data to file",			       /* ERR_WRITE_DATA */
   "Buffer Overflow",				       /* ERR_BUFFER_OVERFLOW */
};

/* Function prototypes */
int ParseConfigFile(FILE *f_ini, WaveDumpConfig_t *WDcfg);

#endif /* _WAVEDUMP__H */
