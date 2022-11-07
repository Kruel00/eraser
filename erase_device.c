//
// Created By: Javier Castorena
//
#include "common.h"

int verbose = 0;
static struct flags_t oflag;
static struct stat nvme_stat;
const char *devicename;
const int blocks = 128;

uint8_t erasmosign[] = 
    {
    0x51, 0x75, 0x61, 0x6E, 0x74, 0x75, 0x6D, 0x20, 0x65, 0x72, 0x61, 0x73,
    0x6D, 0x6F, 0x28, 0x52, 0x29, 0x20, 0x62, 0x79, 0x20, 0x4D, 0x6F, 0x62,
    0x69, 0x6C, 0x69, 0x74, 0x79, 0x20, 0x54, 0x65, 0x61, 0x6D, 0x0a, 0x0a
    };


int erase_sg_device(storage_device_t sg_erasing_device)
{

    int outfd;
    unsigned char *wrkPos, *fprint;
    long long seek = 1;
    static int blk_sz = 512;
    int scsi_cdbsz_out = DEF_SCSI_CDBSZ;
    unsigned char *wrkBuff;
    unsigned char *wrkBuff2;

    static int verbose = 0;
    
    int device_blocks = sg_erasing_device.total_sectors - 1;
    size_t psz = getpagesize();

    long long int tfwide = blk_sz * blocks + psz;
    wrkBuff = MALLOC(tfwide);
    wrkBuff2 = MALLOC(1);

    uint8_t data[tfwide];
    int res, k, t, buf_sz, dio_tmp, flags, fl, sg_fd;

    //byte to write on disk
    memset(data, 0x83, sizeof(data));
    
    wrkPos = wrkBuff;
    memcpy(wrkPos, &data, sizeof(data));

    free(fprint);

    
    fprint = wrkBuff2;
    memcpy(fprint, &erasmosign, sizeof(erasmosign));

    // open device.
    if ((outfd = sg_cmds_open_device(sg_erasing_device.sg_name, 1, verbose)) < 0)
    {
        fprintf(stderr, ME " Device %s dont exist\n%s\n", sg_erasing_device.sg_name, safe_strerror(-sg_fd));
    }

    dio_tmp = 0;
    for (long long int i = 1; i < (device_blocks / blocks); i++)
    {
        res = sg_write(outfd, wrkPos, blocks, seek, blk_sz, scsi_cdbsz_out, oflag.fua, oflag.dpo, &dio_tmp);
        seek += blocks;
        printf("block %lli of %i\n",seek,device_blocks);
    }

    int blk_remains = device_blocks - seek;
    if (seek > 0)
    {

        for (int i = 1; i < blk_remains + 2; i++)
        {
            res = sg_write(outfd, wrkPos, 1, seek, blk_sz, scsi_cdbsz_out, oflag.fua, oflag.dpo, &dio_tmp);
            seek++;
            printf("block %lli of %i\n",seek,device_blocks);

        }
    }

    res = sg_write(outfd, fprint, 1, 0, blk_sz, scsi_cdbsz_out, oflag.fua, oflag.dpo, &dio_tmp);

    json_save(sg_erasing_device);
    upload_erase_test_result(sg_erasing_device);

    free(wrkBuff);
    free(wrkBuff2);

    return 0;
}

// erase nvme device
void *erase_nvme_device(void *device_nvme)
{
    int res, k, t, buf_sz, dio_tmp, flags, fl, sg_fd, bl_sz, ret;
    GtkTextBuffer *buffer;
    char *device_serial = (gchar *)device_nvme;
    char inf[512];
    storage_device_list_t device_list;

    storage_device_t nvme_erasing_device;
    size_t psz = getpagesize();

    // write
    __u16 nlb = 0, control = 0;
    __u16 appmask = 0;
    __u16 apptag = 0;
    __u32 reftag = 0UL;
    __u64 slba = 1;

    find_device_by_serial(device_list, &nvme_erasing_device, device_serial);
    
    bl_sz = 4096;
    buffer = MALLOC(bl_sz * blocks + psz);
    strcpy(inf, nvme_erasing_device.name);
    nvme_open_dev(inf);

    // write zeros
    struct nvme_passthru_cmd cmd = {
        .opcode = nvme_cmd_write_zeroes,
        .nsid = 1,
        .cdw10 = slba & 0xffffffff,        // star block
        .cdw11 = slba >> 32,               // star block
        .cdw12 = nlb | (control << 16),    // block count | control
        .cdw14 = reftag,                   // ref_tag set 1 to enables protection information 0 disabled
        .cdw15 = apptag | (appmask << 16), // app_tag | (app_tag_mask)
    };

    for (int i = 0; i < nvme_erasing_device.total_sectors - 1; i++)
    {
        printf("erasing %i sector of %lli sectors\n", i, nvme_erasing_device.total_sectors - 1);
        cmd.cdw10 = i; // Logical Block Address we want to read
        ret = ioctl(res, NVME_IOCTL_IO_CMD, &cmd);
    }
    return 0;
}


static int sg_write(int sg_fd, unsigned char *buff, int blocks, long long to_block, int bs, int cdbsz, int fua, int dpo, int *diop)
{

    unsigned char wrCmd[MAX_SCSI_CDBSZ];
    unsigned char senseBuff[SENSE_BUFF_LEN];
    int res, info_valid;
    unsigned long long io_addr = 0;
    static int recovered_errs = 0;
    

    if (sg_build_scsi_cdb(wrCmd, cdbsz, blocks, to_block, 1, fua, dpo))
    {
        fprintf(stderr, ME " bad wr cdb build, to_block=%lld, blocks=%d\n", to_block, blocks);
        return -1;
    }

    struct sg_io_hdr io_hdr = {
        .interface_id = 'S',
        .cmd_len = cdbsz,
        .cmdp = wrCmd,
        .dxfer_direction = SG_DXFER_TO_DEV,
        .dxfer_len = bs * blocks,
        .dxferp = buff,
        .mx_sb_len = SENSE_BUFF_LEN,
        .sbp = senseBuff,
        .timeout = DEF_TIMEOUT,
        .pack_id = (int)to_block,
    };

    if (diop && *diop)
        io_hdr.flags |= SG_FLAG_DIRECT_IO;

    while (((res = ioctl(sg_fd, SG_IO, &io_hdr)) < 0) && (EINTR == errno))
    {
        if (res < 0)
        {
            if (ENOMEM == errno)
                return -2;
            perror("writing (SG_IO) on sg device, error");
            return -1;
        }

        switch (sg_err_category3(&io_hdr))
        {
        case SG_LIB_CAT_CLEAN:
            break;
        case SG_LIB_CAT_RECOVERED:
            ++recovered_errs;
            info_valid = sg_get_sense_info_fld(io_hdr.sbp, io_hdr.sb_len_wr, &io_addr);

            if (info_valid)
            {
                fprintf(stderr, "    lba of last recovered error in this WRITE=0x%llx\n", io_addr);
                if (verbose > 1)
                    sg_chk_n_print3("writing", &io_hdr, 1);
            }
            else
            {
                fprintf(stderr, "Recovered error: [no info] writing to "
                                "block=0x%llx, num=%d\n",
                        to_block, blocks);
                sg_chk_n_print3("writing", &io_hdr, verbose > 1);
            }
            break;
        case SG_LIB_CAT_MEDIA_CHANGED:
            if (verbose > 1)
                sg_chk_n_print3("writing", &io_hdr, 1);
            return -3;
        default:
            sg_chk_n_print3("writing", &io_hdr, verbose > 1);
            if (oflag.coe)
            {
                fprintf(stderr, ">> ignored errors for out blk=%lld for "
                                "%d bytes\n",
                        to_block, bs * blocks);
                return 0; /* fudge success */
            }
            else
                return -1;
        }
    }
    if (diop && *diop &&
        ((io_hdr.info & SG_INFO_DIRECT_IO_MASK) != SG_INFO_DIRECT_IO))
        *diop = 0; /* flag that dio not done (completely) */
    return 0;
}

static int nvme_open_dev(const char *dev)
{
    int err, fd;
    devicename = dev;
    err = open(dev, O_RDONLY);
    fd = err;
    err = fstat(fd, &nvme_stat);
    return fd;
}

static int sg_build_scsi_cdb(unsigned char *cdbp, int cdb_sz, unsigned int blocks, long long start_block, int write_true, int fua, int dpo)
{
    int rd_opcode[] = {0x8, 0x28, 0xa8, 0x88};
    int wr_opcode[] = {0xa, 0x2a, 0xaa, 0x8a};
    int sz_ind = 1;

    memset(cdbp, 0, cdb_sz);
    if (dpo)
        cdbp[1] |= 0x10;
    if (fua)
        cdbp[1] |= 0x8;
    cdbp[0] = (unsigned char)(write_true ? wr_opcode[sz_ind] : rd_opcode[sz_ind]);
    cdbp[2] = (unsigned char)((start_block >> 24) & 0xff);
    cdbp[3] = (unsigned char)((start_block >> 16) & 0xff);
    cdbp[4] = (unsigned char)((start_block >> 8) & 0xff);
    cdbp[5] = (unsigned char)(start_block & 0xff);
    cdbp[7] = (unsigned char)((blocks >> 8) & 0xff);
    cdbp[8] = (unsigned char)(blocks & 0xff);
    return 0;
}
