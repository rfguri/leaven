int entries_detected;

void print_fat_dir_info (dir_FAT dir_fat[8]) {

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

			//printf("i: %d, filename: %s, extension: %s, attributes: %x, size: %d\n", i,dir_fat[i].filename, dir_fat[i].extension, dir_fat[i].attr, dir_fat[i].size);
	
		}

	}

}
