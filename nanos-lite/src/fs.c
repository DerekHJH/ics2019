#include "fs.h"

size_t ramdisk_read(void *buf, size_t offset, size_t len);//hjh
size_t ramdisk_write(const void *buf, size_t offset, size_t len);//hjh

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
	size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

//begin{hjh}
size_t getsize(int fd)
{
	return file_table[fd].size; 
}
size_t getdisk(int fd)
{
	return file_table[fd].disk_offset; 
}
size_t getopen(int fd)
{
	return file_table[fd].open_offset; 
}
//end{hjh}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char *path,int flags,int mode)
{
  for(int i=0;i<NR_FILES;i++)
	{
		if(strcmp(file_table[i].name,path)==0)
		{
			file_table[i].open_offset=0;
			return i;
		}
	}
	panic("I did not find the correct file!!!\n");
	return -1;
}

int fs_close(int fd)
{
	return 0;
}

int fs_read(int fd,void *buf,size_t len)
{
  if(file_table[fd].open_offset+len>file_table[fd].size)len=file_table[fd].size-file_table[fd].open_offset;
	ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
  file_table[fd].open_offset+=len;
	return len;
}
int fs_write(int fd,void *buf,size_t len)
{
	if(fd==0)panic("We cannot write to fd==0!!!\n");
  if(fd==1||fd==2)
	{
		for(int i=0;i<len;i++)
		{
			_putc(*((char *)buf+i));
		}
	}
	else
	{
    if(file_table[fd].open_offset+len>file_table[fd].size)
		{
			printf("Caution!! We are wrting too much words into fd==%d\n",fd);
			len=file_table[fd].size-file_table[fd].open_offset;
		}
		ramdisk_write(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
    file_table[fd].open_offset+=len;
	}
	return len;
}
__off_t fs_lseek(int fd,__off_t offset,int whence)
{
	__off_t temp=0;
  switch(whence)
	{
		case SEEK_SET:
		{
      temp=offset;
      break;
		}
		case SEEK_CUR:
		{
			temp=file_table[fd].open_offset+offset;
			break;
		}
		case SEEK_END:
		{
			temp=file_table[fd].size+offset;
			break;
		}
		default:panic("There is no such whence type!!!\n");
	}
	if(temp>file_table[fd].size)
	{
		printf("caution!!Your cursor has nearly moved out of the file!!\n");
		temp=file_table[fd].size;
	}
	file_table[fd].open_offset=temp;
  return temp;
}
