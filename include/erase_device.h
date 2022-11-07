//
// Created By: Javier Castorena
//
int erase_sg_device(storage_device_t sg_erasing_device);

static int sg_write(int sg_fd, unsigned char *buff, int blocks, long long to_block, int bs, int cdbsz, int fua, int dpo, int *diop);
static int nvme_open_dev(const char *dev);
static int sg_build_scsi_cdb(unsigned char *cdbp, int cdb_sz, unsigned int blocks, long long start_block, int write_true, int fua, int dpo);

