int entries_detected;

void print_fat_dir_info (dir_FAT *dir_fat) {

	int i, j, ascii;
	char aux[100];

	for (i = 0; i < entries_detected; i++) {

		ascii = 0;

		if (dir_fat[i].size != 0xffffffff) {

			if (dir_fat[i].attr & 0x02){
				// File Hidden

			} else {

				// Check if filename contains non ascii character
				for (j = 0; j < strlen(dir_fat[i].filename); j++){
					if ((int)dir_fat[i].filename[j] < 0) {
						ascii = 1;
						j = strlen(dir_fat[i].filename);
					}
				}

				if (!ascii) {

					write (1, dir_fat[i].filename, strlen(dir_fat[i].filename));
					write (1, ".", 1);
					write (1, dir_fat[i].extension, 3);
					if (strlen(dir_fat[i].filename) + strlen(dir_fat[i].extension) < 7 || strlen(dir_fat[i].filename) < 4) {
						write (1, "\t\t", 2);
					} else {
						write (1, "\t", 1);
					}

					if (dir_fat[i].attr & 0x20){
						sprintf(aux, "<file>\t\t");
					} else if (dir_fat[i].attr & 0x08) {
						sprintf(aux, "<VOL_LABEL>\t");
					} else {
						sprintf(aux, "<dir>\t\t");
					}
					write(1, aux, strlen(aux));

					sprintf (aux, "%d", dir_fat[i].size);
					write (1, aux, strlen(aux));
					write (1, "\n", 1);

				}

			}

		}

	}

}

void print_ext2_block_info (dir_EXT2 *dir_ext2, int i) {

	char aux[100];

	sprintf(aux, "%s\t", dir_ext2[i].name);
	write (1, aux, strlen(aux));

	if (dir_ext2[i].file_type & 0x01) {

		sprintf(aux, "<file>\t");

	} else if (dir_ext2[i].file_type & 0x02) {

		sprintf(aux, "<dir>\t");

	}

	write(1, aux, strlen(aux));

	if (dir_ext2[i].file_type & 0x01) {

		// Print size
		sprintf(aux, "\t %d\n", dir_ext2[i].file_size);
		write (1, aux, strlen(aux));

	} else {

		write (1, "\n", 1);

	}

}

void read_block (int i, int fd, dir_EXT2 *dir_ext2, int block_init_addr, int block_acc, int i_table_init_addr, filesystem_EXT2 ext2_info) {

	char tmp[100];

	lseek (fd, block_init_addr + block_acc, SEEK_SET);
	read (fd, &dir_ext2[i].num_inode, 4);

	read (fd, &dir_ext2[i].register_length, 2);

	read (fd, &dir_ext2[i].name_length, 1);

	read (fd, &dir_ext2[i].file_type, 1);

	int name_lenght_aux = (int)dir_ext2[i].name_length;

	int n = read (fd, tmp, name_lenght_aux);

	tmp[n] = '\0';
	dir_ext2[i].name = strdup(tmp);

	if (dir_ext2[i].file_type & 0x01) {

		// Go to inode
		int i_addr = i_table_init_addr + (dir_ext2[i].num_inode - 1) * ext2_info.inode_size;

		lseek (fd, 4 + i_addr, SEEK_SET);
		read (fd, &dir_ext2[i].file_size, 4);

	}

}

void get_ext2_dir_info (int fd, dir_EXT2 *dir_ext2, group_EXT2 *group_ext2, filesystem_EXT2 ext2_info) {

	inode_EXT2 inode_ext2;

	int i, addr_block = 40;

	// Get inode table init block
	lseek (fd, 8 + 2048, SEEK_SET);
	read (fd, &(*group_ext2).init_inode_table, 4);

	// Get inode table init address
	int i_table_init_addr = ((*group_ext2).init_inode_table * ext2_info.block_size);

	// Go to second inode (root)
	int i_root_init_addr = i_table_init_addr + ext2_info.inode_size;

	// Get inode size
	lseek (fd, 4 + i_root_init_addr, SEEK_SET);
	read (fd, &inode_ext2.i_size, 4);

	// Get direct pointer number
	int max_direct_pointers = inode_ext2.i_size / ext2_info.block_size;

	int acc = 0;

	dir_ext2 = (dir_EXT2 *) malloc (sizeof(dir_EXT2) * max_direct_pointers);

	// Iterate trough direct inodes
	for (i = 0; i < max_direct_pointers; i++) {

		lseek (fd, (addr_block + (4 * i)) + i_root_init_addr, SEEK_SET);
		read (fd, &inode_ext2.i_block_pointer[i], 4);

		// Check last inode
		if (inode_ext2.i_block_pointer[i] == 0) {

			i = 11;

		} else {

			// Get block init address
			int block_init_addr = inode_ext2.i_block_pointer[i] * ext2_info.block_size;

			int block_acc = 0;

			while (acc < inode_ext2.i_size && block_acc < ext2_info.block_size) {

				read_block(i, fd, dir_ext2, block_init_addr, block_acc, i_table_init_addr, ext2_info);

				print_ext2_block_info (dir_ext2, i);

				acc = acc + dir_ext2[i].register_length;
				block_acc = block_acc + dir_ext2[i].register_length;

			}

		}

	}

}

void get_fat_dir_info (int fd, dir_FAT *dir_fat, filesystem_FAT fat_info, int root_dir_init_addr) {

	int i, j, n, done;
	char tmp[8], tmp2[3];

	for (i = 0; i < fat_info.max_root_entries; i++) {

		// Get filename
		lseek (fd, root_dir_init_addr + (32 * i), SEEK_SET);
		n = read (fd, tmp, 8);

		done = 0;
		for (j = 0; j < n; j++) {
			if (tmp[j] == ' '){
				tmp[j] = '\0';
				dir_fat[i].filename = strdup(tmp);
				j = n;
				done = 1;
			}
		}

		if (done == 0) {
			dir_fat[i].filename = strdup(tmp);
		}

		if (strcmp(dir_fat[i].filename, "") == 0){

			entries_detected = i;
			i = fat_info.max_root_entries;

		} else {

			// Get extension
			lseek (fd, root_dir_init_addr + 8 + (32 * i), SEEK_SET);
			n = read (fd, tmp2, 3);

			done = 0;
			for (j = 0; j < n; j++) {
				if (tmp2[j] == ' '){
					tmp2[j] = '\0';
					dir_fat[i].extension = strdup(tmp2);
					j = n;
					done = 1;
				}
			}
			if (done == 0) {
				dir_fat[i].extension = strdup(tmp2);
			}

			// Get attributes
			lseek (fd, root_dir_init_addr + 11 + (32 * i), SEEK_SET);
			read (fd, &dir_fat[i].attr, 1);

			// Get size
			lseek (fd, root_dir_init_addr + 28 + (32 * i), SEEK_SET);
			read (fd, &dir_fat[i].size, 4);

		}

	}

}
