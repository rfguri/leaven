# Leaven

Application in C that allows executing different types of queries on a disk 
volume formatted in FAT16 or EXT2 without mounting it.

## How to build executable

To build the executable file just run: 
`make`

## How to clean the project

To clean the entire project and get rid of executables and linkers just run:
`make clean`

## Usage

`./leaven <operation> <volume_name>`

### Operation types

* /info

## Usage example

`./leaven /info Fat16_1024_Binary.bin`

That will produce the following output:

	------ Filesystem Information ------

	Filesystem: FAT16
	System Name: mkdosfs
	Mida del sector: 1024
	Sectors per cluster: 1
	Sectors reservats: 1
	Número de FAT's: 2
	MaxRootEntries: 512
	Sectors per FAT: 16
	Label: TEST2
