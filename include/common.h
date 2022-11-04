//
// Created By: Javier Castorena
//
#include <sys/ioctl.h>
#include <gtk/gtk.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>

#include "gui.h"
#include "storage_devices.h"
#include "leak_memory.h"
#include "config.h"
#include "erase_log.h"
#include "nvme_ioctl.h"
#include "sg_cmds.h"
#include "sg_lib.h"
#include "nvme.h"
#include "sg_io_linux.h"
#include "erase_device.h"
#include "build_device.h"



#define DEF_TIMEOUT 60000 /* 60,000 millisecs == 60 seconds */
#define MAX_SCSI_CDBSZ 16
#define SENSE_BUFF_LEN 64 /* Arbitrary, could be larger */
#define ME "erasmo"
#define SG_DD_BYPASS 999 /* failed but coe set */
#define SG_PATH_SIZE 512
#define DEF_SCSI_CDBSZ 10
#define unlikely(x) x
#define SIZE 4096



struct flags_t {
    bool append;
    bool dio;
    bool direct;
    bool dpo;
    bool dsync;
    bool excl;
    bool flock;
    bool ff;
    bool fua;
    bool nocreat;
    bool random;
    bool sgio;
    bool sparse;
    bool zero;
    int cdbsz;
    int cdl;
    int coe;
    int nocache;
    int pdt;
    int retries;
};

