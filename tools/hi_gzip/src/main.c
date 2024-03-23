#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
static void usage()
{
	printf("input err!!!!!!!!!!!!!!!! \n");
    printf("usage: ./hi_gzip input_filename output_filename\n");
    exit(1);
}

#define HEAD_SIZE   0X10
#define COMPRESSED_SIZE_OFFSET      0X0
#define UNCOMPRESSED_SIZE_OFFSET    0X4

#define HEAD_MAGIC_NUM0 0X70697A67/*'g''z''i''p'*/
#define HEAD_MAGIC_NUM0_OFFSET 0X8
#define HEAD_MAGIC_NUM1 0X64616568/*'h''e''a''d'*/
#define HEAD_MAGIC_NUM1_OFFSET 0XC
int main(int argc, char **argv)
{
	if(argc != 3)
	{
		usage();
	}
	char *input_file = argv[1];
	char *output_file = argv[2];
	int gzip = 1;/* atoi(argv[3]); */

	int input_fd, output_fd;
	int input_len, output_len;
	input_fd = open(input_file, O_RDONLY);
	if(input_fd < 0)
	{
		printf("open file %s failed \n", input_file);
		exit(1);
	}
	
	/* statistics the length of the input file */
	struct stat s;
	if(fstat(input_fd, &s))
	{
		printf("get input file len failed \n");
		exit(1);
	}
	input_len = s.st_size;
	printf("input file len: %d \n", input_len);
	close(input_fd);

	#define BUF_LEN   (2*input_len)
	output_len = BUF_LEN;
	char *output_buf = malloc(BUF_LEN);
	char *output_buf1 = malloc(BUF_LEN);	
	char *tmp_buf = malloc(BUF_LEN);

	memset(output_buf, 0, BUF_LEN);
	memset(output_buf1, 0, BUF_LEN);
	memset(tmp_buf, 0, BUF_LEN);
	
	if((tmp_buf == 0) || (output_buf == 0) || (output_buf1 == 0))
	{
		printf("malloc failed \n");
		exit(1);
	}
    char *tmp_str = output_file;
    if(gzip) {
        char buf[0x2000+0x10];
        if(strlen(input_file)>0x1000) {
            printf("Error:input_file name is too long(more than 0x1000)!\n");
		    exit(1);
        }
        sprintf(buf,"./gzip -c %s > %s",input_file,tmp_str);
        printf("%s\n",buf);
        system(buf);
    } else {
        tmp_str = input_file;
    }
	int tmp_fd = open(tmp_str, O_RDONLY);
	if(tmp_fd < 0)
	{
		printf("open file %s failed \n",tmp_str);
		exit(1);
	}
	
	/* statistic the length of temporary files */
	if(fstat(tmp_fd, &s))
	{
		printf("get %s file len failed \n",tmp_str);
		exit(1);
	}
	int tmp_len = s.st_size;
	printf("%s file len: %d \n", tmp_str, tmp_len);
    
	int readlen = read(tmp_fd, tmp_buf, tmp_len);
	if(readlen != tmp_len)
	{
		printf("read tmp len error !\n");
		exit(1);
	}
    output_len = readlen;
    memcpy(output_buf,tmp_buf,output_len);
	/* writes to the output file */
	if((output_fd = open(output_file, O_RDWR|O_CREAT, 0777)) < 0)
	{
		printf("create output file failed \n");
		exit(1);
	}
	
	/* Write a custom head to the file
	Storage Length Information | The original length must be stored 
	| Magic number 0 | Magic number 1 */

	*(int *)&output_buf1[COMPRESSED_SIZE_OFFSET] = output_len;
	*(int *)&output_buf1[UNCOMPRESSED_SIZE_OFFSET] = input_len;
	*(int *)&output_buf1[HEAD_MAGIC_NUM0_OFFSET] = HEAD_MAGIC_NUM0;
	*(int *)&output_buf1[HEAD_MAGIC_NUM1_OFFSET] = HEAD_MAGIC_NUM1;
	
	memcpy(output_buf1 + HEAD_SIZE,output_buf,output_len);

	output_len += HEAD_SIZE;

	int writelen;
	writelen = write(output_fd, output_buf1, output_len);
	if(writelen != output_len)
	{
		printf("write output file failed \n");
		exit(1);
	}
	close(output_fd);
	free(tmp_buf);
	free(output_buf);
	free(output_buf1);
    /* if(gzip)
        system("rm -rf hi_gzip_tmp_file"); */
	printf("-------------------compress successfully--------------------\n ");
	return 0;
}








