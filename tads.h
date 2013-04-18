typedef struct FAT {

	char *filesystem;
	char *system_name;
	unsigned short sector_size;
	unsigned char cluster_sectors;
	unsigned short reserved_sectors;
	unsigned char fats_number;
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