typedef struct FAT_vol {

	char *filesystem;
	char *system_name;
	unsigned short sector_size;
	unsigned char cluster_sectors;
	unsigned short reserved_sectors;
	unsigned char fat_number;
	unsigned short max_root_entries;
	unsigned short fat_sectors;
	char *label;

} filesystem_FAT;

typedef struct EXT2 {

	char filesystem[8];
	unsigned short inode_size;
	unsigned int num_inode;
	unsigned int first_inode;
	unsigned int inode_group;
	unsigned int free_inode;
	unsigned int block_size;
	unsigned int reserved_block;
	unsigned int free_block;
	unsigned int total_block;
	unsigned int first_block;
	unsigned int block_group;
	unsigned int frags_group;
	char *volume_name;
	char *last_check;
	char *last_mount;
	char *last_write;

} filesystem_EXT2;

typedef struct FAT_dir {

	char *filename;
	char *extension;
	char attr;
	unsigned int size;

} dir_FAT;

typedef struct EXT2_inode {

	unsigned short i_mode;
	unsigned short i_uid;
	unsigned int i_size;
	unsigned int i_atime;
	unsigned int i_ctime;
	unsigned int i_mtime;
	unsigned int i_dtime;
	unsigned short i_gid;
	unsigned short i_links_count;
	unsigned int i_blocks;
	unsigned int i_flags;
	unsigned int i_osdl;
	unsigned int i_block_pointer[15];
	unsigned int i_generation;
	unsigned int i_file_acl;
	unsigned int i_dir_acl;
	unsigned int i_faddr;
	double i_osd2;

} inode_EXT2;

typedef struct EXT2_dir {

	unsigned int num_inode;
	unsigned short register_length;
	char name_length;
	char file_type;
	char *name;
	unsigned int file_size;

} dir_EXT2;

typedef struct EXT2_group {

	unsigned int init_block_bitmap;
	unsigned int init_inode_bitmap;
	unsigned int init_inode_table;
	unsigned short free_blocks;
	unsigned short free_inodes;
	unsigned short used_dirs;

} group_EXT2;
