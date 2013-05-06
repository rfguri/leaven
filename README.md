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
* /dir

## Usage examples

`./leaven /info Fat16_1024_Binary.bin`

Will produce the following output:

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

`./leaven /dir Fat16_2048_Con-info.bin`

Will produce the following output:

	TEST2.		<VOL_LABEL>	0
	ALLOCA.H	<file>		1289
	BYTESWAP.H	<file>		1489
	CONIO.H		<file>		1543
	EXECINFO.H	<file>		1606
	HDPARM.		<dir>		0
	I686.		<dir>		0
	LASTLOG.H	<file>		126
	LIBGEN.H	<file>		1465
	MEMORY.H	<file>		1029
	POLL.H		<file>		22
	PTY.H		<file>		1610
	RE_COMP.H	<file>		1024
	SGTTY.H		<file>		1431
	SINNAD~1.	<dir>		0
	STAB.H		<file>		264
	SYSCALL.H	<file>		25
	SYSLOG.H	<file>		24
	TERMIO.H	<file>		214
	ULIMIT.H	<file>		1657
	USTAT.H		<file>		23
	UTIME.H		<file>		1603
	WAIT.H		<file>		22
	XLOCALE.H	<file>		1764

`./leaven /dir Ext2_1024_Con-info.ex2`

Will produce the following output:

	.			<dir>
	..			<dir>
	lost+found	<dir>
	dbus-1.0	<dir>
	hdparm		<dir>
	i686		<dir>
	Sin nada	<dir>
	byteswap.h	<file>		 1489
	conio.h		<file>		 1543
	execinfo.h	<file>		 1606
	alloca.h	<file>		 1289
	.Trash-1000	<dir>
	syslog.h	<file>		 24
	termio.h	<file>		 214
	ulimit.h	<file>		 1657
	utime.h		<file>		 1603
	wait.h		<file>		 22
	xlocale.h	<file>		 1764
	lastlog.h	<file>		 126
	libgen.h	<file>		 1465
	memory.h	<file>		 1029
	poll.h		<file>		 22
	pty.h		<file>		 1610
	re_comp.h	<file>		 1024
	sgtty.h		<file>		 1431
	stab.h		<file>		 264
	syscall.h	<file>		 25
	ustat.h		<file>		 23
	PROVA1.TXT	<file>		 21640
	PROVA2.TXT	<file>		 350710
	PROVA3.TXT	<file>		 826733
