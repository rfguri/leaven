void print_ext2_info (filesystem_EXT2 ext2_info) {

	char aux[100];

	write (1, "\n------ Filesystem information ------\n\n", strlen("\n------ Filesystem information ------\n\n"));

	sprintf(aux, "Filesystem: %s\n", ext2_info.filesystem);
	write (1, aux, strlen(aux));

	write (1, "\nINFO INODE\n", strlen("\nINFO INODE\n"));

	sprintf(aux, "Mida del inode: %d\n", ext2_info.inode_size);
	write (1, aux, strlen(aux));

	sprintf(aux, "Num Inode: %d\n", ext2_info.num_inode);
	write (1, aux, strlen(aux));

	sprintf(aux, "Primer Inode: %d\n", ext2_info.first_inode);
	write (1, aux, strlen(aux));

	sprintf(aux, "Inodes Grup: %d\n", ext2_info.inode_group);
	write (1, aux, strlen(aux));

	sprintf(aux, "Inodes Lliures: %d\n", ext2_info.free_inode);
	write (1, aux, strlen(aux));

	write (1, "\nINFO BLOC\n", strlen("\nINFO BLOC\n"));

	sprintf(aux, "Mida Bloc: %d\n", ext2_info.block_size);
	write (1, aux, strlen(aux));

	sprintf(aux, "Blocs Reservats: %d\n", ext2_info.reserved_block);
	write (1, aux, strlen(aux));

	sprintf(aux, "Blocs Lliures: %d\n", ext2_info.free_block);
	write (1, aux, strlen(aux));

	sprintf(aux, "Total Blocs: %d\n", ext2_info.total_block);
	write (1, aux, strlen(aux));

	sprintf(aux, "Primer Bloc: %d\n", ext2_info.first_block);
	write (1, aux, strlen(aux));

	sprintf(aux, "Block Grup: %d\n", ext2_info.block_group);
	write (1, aux, strlen(aux));

	sprintf(aux, "Frags Grup: %d\n", ext2_info.frags_group);
	write (1, aux, strlen(aux));

	write (1, "\nINFO VOLUM\n", strlen("\nINFO VOLUM\n"));

	sprintf(aux, "Nom volum: %s\n", ext2_info.volume_name);
	write (1, aux, strlen(aux));

	sprintf(aux, "Ultima comprov: %s", ext2_info.last_check);
	write (1, aux, strlen(aux));

	sprintf(aux, "Ultim muntatge: %s", ext2_info.last_mount);
	write (1, aux, strlen(aux));

	sprintf(aux, "Ultima escriptura: %s", ext2_info.last_write);
	write (1, aux, strlen(aux));

	write (1, "\n", 1);

}

void print_fat_info (filesystem_FAT fat_info) {

	char aux[100];

	write (1, "\n------ Filesystem information ------\n\n", strlen("\n------ Filesystem information ------\n\n"));

	sprintf(aux, "Filesystem: %s\n", fat_info.filesystem);
	write (1, aux, strlen(aux));

	sprintf(aux, "System Name: %s\n", fat_info.system_name);
	write (1, aux, strlen(aux));

	sprintf(aux, "Mida del sector: %d\n", fat_info.sector_size);
	write (1, aux, strlen(aux));

	sprintf(aux, "Sectors per cluster: %d\n", fat_info.cluster_sectors);
	write (1, aux, strlen(aux));

	sprintf(aux, "Sectors reservats: %d\n", fat_info.reserved_sectors);
	write (1, aux, strlen(aux));

	sprintf(aux, "Número de FAT's: %d\n", fat_info.fat_number);
	write (1, aux, strlen(aux));

	sprintf(aux, "MaxRootEntries: %d\n", fat_info.max_root_entries);
	write (1, aux, strlen(aux));

	sprintf(aux, "Sectors per FAT: %d\n", fat_info.fat_sectors);
	write (1, aux, strlen(aux));

	sprintf(aux, "Label: %s\n", fat_info.label);
	write (1, aux, strlen(aux));

	write (1, "\n", 1);

}

void save_ext2 (int fd, filesystem_EXT2 *ext2_info) {

	int superblock = 1024;
	char tmp[16];
	time_t clk;

	strcpy((*ext2_info).filesystem, "EXT2");

	// Get inode's size
	lseek (fd, 88 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).inode_size, 2);

	// Get inode's numbers
	lseek (fd, 0 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).num_inode, 4);

	// Get first inode
	lseek (fd, 84 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).first_inode, 4);

	// Get inode's group
	lseek (fd, 40 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).inode_group, 4);

	// Get free inode
	lseek (fd, 16 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).free_inode, 4);

	// Get block's size
	lseek (fd, 24 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).block_size, 4);
	(*ext2_info).block_size = 1024 << (*ext2_info).block_size;
	
	// Get reserved's block
	lseek (fd, 8 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).reserved_block, 4);

	// Get free blocks
	lseek (fd, 12 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).free_block, 4);

	// Get total blocks
	lseek (fd, 4 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).total_block, 4);

	// Get first block
	lseek (fd, 20 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).first_block, 4);

	// Get block's group
	lseek (fd, 32 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).block_group, 4);

	// Get frags' group
	lseek (fd, 36 + superblock, SEEK_SET);
	read (fd, &(*ext2_info).frags_group, 4);

	// Get volume name
	lseek (fd, 120 + superblock, SEEK_SET);
	read (fd, tmp, 16);
	(*ext2_info).volume_name = strdup(tmp);

	// Get last check
	lseek (fd, 64 + superblock, SEEK_SET);
	read (fd, &clk, 4);
	(*ext2_info).last_check = strdup(ctime(&clk));

	// Get last mount
	lseek (fd, 44 + superblock, SEEK_SET);
	read (fd, &clk, 4);
	(*ext2_info).last_mount = strdup(ctime(&clk));

	// Get last write
	lseek (fd, 48 + superblock, SEEK_SET);
	read (fd, &clk, 4);
	(*ext2_info).last_write = strdup(ctime(&clk));

}

void save_fat (int fd, filesystem_FAT *fat_info) {

	int n, i, done;
	char tmp[8], tmp2[11];

	// Get file system
	lseek (fd, 54, SEEK_SET);
	read (fd, tmp, 8);
	(*fat_info).filesystem = strdup(tmp);

	// Get system name
	lseek (fd, 3, SEEK_SET);
	n = read (fd, tmp, 8);
	(*fat_info).system_name = strdup(tmp);

	// Get sector size
	lseek (fd, 11, SEEK_SET);
	read (fd, &(*fat_info).sector_size, 2);

	// Get cluster's sectors
	lseek (fd, 13, SEEK_SET);
	read (fd, &(*fat_info).cluster_sectors, 1);

	// Get reserved sectors
	lseek (fd, 14, SEEK_SET);
	read (fd, &(*fat_info).reserved_sectors, 2);

	// Get FATs' number
	lseek (fd, 16, SEEK_SET);
	read (fd, &(*fat_info).fat_number, 1);

	// Get number of root entries
	lseek (fd, 17, SEEK_SET);
	read (fd, &(*fat_info).max_root_entries, 2);

	// Get FATs' sector
	lseek (fd, 22, SEEK_SET);
	read (fd, &(*fat_info).fat_sectors, 2);

	// Get label
	lseek (fd, 43, SEEK_SET);
	n = read (fd, tmp2, 11);

	// Get rid of innecessary characters
	done = 0;
	for (i = 0; i < n; i++) {
		if (tmp2[i] == ' '){
			tmp2[i] = '\0';
			(*fat_info).label = strdup(tmp2);
			i = n;
			done = 1;
		}
	}
	if (done == 0) {
		(*fat_info).system_name = strdup(tmp2);
	}

}