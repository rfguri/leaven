typedef struct Info {

	char filesystem[8];
	char system_name[8];
	unsigned short sector_size;
	unsigned char cluster_sectors;
	unsigned short reserved_sectors;
	unsigned char fats_number;
	unsigned short max_root_entries;
	unsigned short fat_sectors;
	char label[11];

} filesystem_info;