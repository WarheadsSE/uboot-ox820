#include <common.h>
#include <command.h>
#include <diag.h>


#define MVDIAG_MEMTEST_START_ADDR	0x61000000
#define MVDIAG_MEMTEST_END_ADDR		0x68000000


diag_func_t *diag_sequence[] =
{
    mem_data_bus_test,      /* DRAM Data bus test */
    mem_address_bus_test,   /* DRAM Address bus test */
    mem_device_test,        /* DRAM device test */
    mvNandDetectTest,
    mvNandBadBlockTest,
    mvNandReadWriteTest,
    NULL,
};


unsigned int *mem_test_start_offset = MVDIAG_MEMTEST_START_ADDR;
unsigned int *mem_test_end_offset = MVDIAG_MEMTEST_END_ADDR;


int do_mv_diag (cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
        diag_func_t **diag_func_ptr;

        for (diag_func_ptr = diag_sequence; *diag_func_ptr; ++diag_func_ptr)
        {
                printf("\n");
                if((*diag_func_ptr)())
                        break;
        }

        if(*diag_func_ptr == NULL)
                printf("\nDiag completed\n");
        else
                printf("\nDiag FAILED\n");

        return 0;
}

U_BOOT_CMD(
        mv_diag, 1, 0, do_mv_diag,
        "mv_diag - perform board diagnostics\n",
        " - run all available tests\n"
);
