#include "libs.h"
#include "tads.h"
#include "leaven_info.c"
#include "leaven_dir.c"

int check_filesystem (int fd, filesystem_FAT *fat_info, filesystem_EXT2 *ext2_info) {

	int type = 0, n, i;
	char tmp[8];

	// Get file system
	lseek (fd, 54, SEEK_SET);
	n = read (fd, tmp, 8);

	// Get rid of innecessary characters
	for (i = 0; i < n; i++) {
		if (tmp[i] == ' '){
			tmp[i] = '\0';
			i = n;
		}
	}

	if (strcmp(tmp, "FAT16") == 0) {

		save_fat (fd, &(*fat_info));

		type = 1;

	} else {

		unsigned short tmp2;
		int superblock = 1024;

		lseek (fd, 56 + superblock, SEEK_SET);
		read (fd, &tmp2, 2);

		if (tmp2 == 0xEF53) {

			save_ext2 (fd, &(*ext2_info));

			type = 2;

		} else {

			write (1, "Error. Volum no formatat en FAT16 ni EXT2.\n", strlen("Error. Volum no formatat en FAT16 ni EXT2.\n"));
		
		}

	}

	return (type);

}

int read_file (char **argv, int *fd, filesystem_FAT *fat_info, filesystem_EXT2 *ext2_info) {

    if ((*fd = open (argv[2], O_RDONLY)) < 0) {
    	
    	write (1, "Error. Volum no trobat.\n", strlen("Error. Volum no trobat.\n"));
    	
    	return (0);
    
    } else {

		return (check_filesystem(*fd, &(*fat_info), &(*ext2_info)));
	}

}

int main ( int argc, char **argv ) {

	if ( argc != 3 ) {
	
		write (1, "Error. Nombre de paràmetres incorrecte.\n", strlen("Error. Nombre de paràmetres incorrecte.\n"));
    
    } else {

    	int fd;

    	filesystem_FAT fat_info;
    	filesystem_EXT2 ext2_info;

    	if (strcmp(argv[1], "/info") == 0) {

	    	int type = read_file (argv, &fd, &fat_info, &ext2_info);

	    	switch (type) {
	    		case 1:
					print_fat_info (fat_info);
	    		break;
	    		case 2:
					print_ext2_info (ext2_info);
	    		break;
	    	}

    	} else if (strcmp(argv[1], "/dir") == 0) {
    	
			dir_FAT *dir_fat = (dir_FAT *) malloc (sizeof(dir_FAT) * fat_info.max_root_entries);

	    	int type = read_file (argv, &fd, &fat_info, &ext2_info);

	    	int reserved_region, fat_region, root_dir_init_addr;

	    	switch (type) {
	    		case 1:
					
					reserved_region = fat_info.reserved_sectors * fat_info.sector_size;
					fat_region = fat_info.fat_number * fat_info.fat_sectors * fat_info.sector_size;
					root_dir_init_addr = reserved_region + fat_region;

					get_fat_dir_info (fd, dir_fat, fat_info, root_dir_init_addr);

					print_fat_dir_info (dir_fat);

	    		break;
	    		case 2:

	    		break;
	    	}

    	} else {

    		write (1, "Error. Operació no permesa.\n", strlen("Error. Operació no permesa.\n"));

    	}

    	close (fd);

    }

	return (0);

}