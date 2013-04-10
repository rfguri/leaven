#include "tads.h"
#include "libs.h"

void print_info (filesystem_info info) {

	char aux[100];

	write (1, "\n", 1);
	write (1, "------ Filesystem Information ------", strlen("------ Filesystem Information ------"));
	write (1, "\n\n", 2);

	write (1, "Filesystem: ", strlen("Filesystem: "));
	write (1, info.filesystem, strlen(info.filesystem));
	write (1, "\n", 1);

	write (1, "System Name: ", strlen("System Name: "));
	write (1, info.system_name, strlen(info.system_name));
	write (1, "\n", 1);

	sprintf(aux, "Mida del sector: %d\n", info.sector_size);
	write (1, aux, strlen(aux));

	sprintf(aux, "Sectors per cluster: %d\n", info.cluster_sectors);
	write (1, aux, strlen(aux));

	sprintf(aux, "Sectors reservats: %d\n", info.reserved_sectors);
	write (1, aux, strlen(aux));

	sprintf(aux, "Número de FAT's: %d\n", info.fats_number);
	write (1, aux, strlen(aux));

	sprintf(aux, "MaxRootEntries: %d\n", info.max_root_entries);
	write (1, aux, strlen(aux));

	sprintf(aux, "Sectors per FAT: %d\n", info.fat_sectors);
	write (1, aux, strlen(aux));

	write (1, "Label: ", strlen("Label: "));
	write (1, info.label, strlen(info.label));
	write (1, "\n", 1);

	write (1, "\n", 1);

}

int check_filesystem (char filesystem[8]) {

	if (strcmp(filesystem, "FAT16") == 0 || strcmp(filesystem, "EXT2") == 0) {

		return (1);

	} else {

		write (1, "Error. Volumn no formatat en FAT16 ni EXT2.\n", strlen("Error. Volumn no formatat en FAT16 ni EXT2.\n"));
		
		return (0);
	
	}
}

int read_file (char **argv, filesystem_info *info) {

	int fd, n, i;
    char tmp[8];

    if ((fd = open (argv[2], O_RDONLY)) < 0) {
    	
    	write (1, "Error. Volum no trobat.\n", strlen("Error. Volum no trobat.\n"));
    	
    	return (0);
    
    } else {

		// Get file system
		lseek (fd, 54, SEEK_SET);
		n = read (fd, tmp, 8);

		// Get rid of innecessary characters
		for (i = 0; i < n; i++) {
			if (tmp[i] == ' '){
				tmp[i] = '\0';
				strcpy((*info).filesystem, tmp);
				i = n;
			}
		}

		if (check_filesystem((*info).filesystem)) {

			// Get system name
			lseek (fd, 3, SEEK_SET);
			n = read (fd, tmp, 8);

			// Get rid of innecessary characters
			for (i = 0; i < n; i++) {
				if (tmp[i] == ' '){
					tmp[i] = '\0';
					strcpy((*info).system_name, tmp);
					i = n;
				}
				strcpy((*info).system_name, tmp);
			}

			// Get sector size
			lseek (fd, 11, SEEK_SET);
			read (fd, &(*info).sector_size, 2);

			// Get cluster's sectors
			lseek (fd, 13, SEEK_SET);
			read (fd, &(*info).cluster_sectors, 1);

			// Get reserved sectors
			lseek (fd, 14, SEEK_SET);
			read (fd, &(*info).reserved_sectors, 2);

			// Get FATs' number
			lseek (fd, 16, SEEK_SET);
			read (fd, &(*info).fats_number, 1);

			// Get reserved sectors
			lseek (fd, 17, SEEK_SET);
			read (fd, &(*info).max_root_entries, 2);

			// Get FATs' sector
			lseek (fd, 22, SEEK_SET);
			read (fd, &(*info).fat_sectors, 2);

			// Get label
			lseek (fd, 43, SEEK_SET);
			n = read (fd, tmp, 11);

			// Get rid of innecessary characters
			for (i = 0; i < n; i++) {
				if (tmp[i] == ' '){
					tmp[i] = '\0';
					strcpy((*info).label, tmp);
					i = n;
				}
				strcpy((*info).label, tmp);
			}

			close (fd);

			return (1);

		} else {

			return (0);

		}

	}

}

int main ( int argc, char **argv ) {

	if ( argc != 3 ) {
	
		write (1, "Error. Nombre de paràmetres incorrecte.\n", strlen("Error. Nombre de paràmetres incorrecte.\n"));
    
    } else {

    	if (strcmp(argv[1], "/info") == 0) {

    		filesystem_info info;

	    	int ok = read_file (argv, &info);

	    	if (ok) print_info (info);

    	} else {

    		write (1, "Error. Operació no permesa.\n", strlen("Error. Operació no permesa.\n"));

    	}

    }
	
	return (0);

}