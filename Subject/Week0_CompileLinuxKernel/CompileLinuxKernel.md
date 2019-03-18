# Compile Linux Kernel

## Brief Description

In this practice I'll use Ubuntu 18.04 running on VirtualBox.

```txt
$ lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 18.04.2 LTS
Release:	18.04
Codename:	bionic
```

## Download Essential

### Linux Kernel Source Code

[Linux Kernel](https://www.kernel.org/)

In this practice I'll use 4.19.23 version kernel.

```sh
# Download kernel and extract
$ wget https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.19.23.tar.xz
$ tar xvf linux-4.19.23.tar.xz
```

### Essential Program

```sh
# for make (It's already included in build-essential)
# sudo apt install make-gui

# for gcc
sudo apt-get install build-essential

# for make menuconfig (ncurses packages)
sudo apt-get install libncurses5-dev bison flex

# for building (make -j4)
sudo apt-get install libelf-dev

# for openssl/bil.h (OpenSSL library)
sudo apt-get insatll libssl-dev
```

## Configure the Kernel

### Clean the project

* `make mrproper`: Remove all generated files, config and various backup files
* `make clean`: Remove most generated files but keep the config

### Configure the project

A special technique to copy current OS config

```sh
cp /boot/config-$(uname -r) .config
```

* `make config`: basic CLI configure
* `make menuconfig`: CLI interactive configure <= Better use this
* `make xconfig`: GUI interactive configure

- `make oldconfig`: make change on previous setting

+ `make x86_64_defconfig`: default configure for x86 64-bit machine (*much faster*)
+ `make localmodconfig`: use local setting to compile (*much more stable*)

> I copy current OS config and use `make menuconfig`, but I don't know which should be change so I leave the setting original.

## Build the Kernel

* `make`
  * `make -jn`: n means job number. e.g. `make -j4`
* `make zImage`
* `make bzImage`

I use `make -j4` here.

```txt
...
Setup is 15580 bytes (padded to 15872 bytes).
System is 8145 kB
CRC b44da209
Kernel: arch/x86/boot/bzImge is ready  (#1)
```

## Install the Kernel

```sh
sudo make install
```

After this, you can find new kernel in `/boot`

* `config-4.19.23`
* `initrd.img-4.19.23`
* `System.map-4.19.23`
* `vmlinuz-4.19.23`

## Configure Booting Document

**GRUB** (GRand Unified Bootloader)

We need to update the GRUB after we've install the kernel.

```sh
sudo update-grub
```

We can also update some setting in `/etc/default/grub`.

For example set `GRUB_TIMEOUT` to 5 (default 0).

Don't forget to run `sudo update-grub` afterward to update settings.

## Reboot and Enter New Kernel

When reboot. Press *shift* we can select kernel.

> It's in *Advanced options for Ubuntu* option.

![grub](images/grub.png)

When login the system. Use `uname -a` we can check if our system version is 4.19.23 means success.

> I've faced some problem that when it's starting the OS it will flashing with red screen with some sentence on it. And I can't login since it stuck in the login page.
>
> But it somehow login successfully after few reboot...

![uname](images/uname.png)

## Afterword

### make help

```txt
$ make help
Cleaning targets:
  clean		  - Remove most generated files but keep the config and
                    enough build support to build external modules
  mrproper	  - Remove all generated files + config + various backup files
  distclean	  - mrproper + remove editor backup and patch files

Configuration targets:
  config	  - Update current config utilising a line-oriented program
  nconfig         - Update current config utilising a ncurses menu based program
  menuconfig	  - Update current config utilising a menu based program
  xconfig	  - Update current config utilising a Qt based front-end
  gconfig	  - Update current config utilising a GTK+ based front-end
  oldconfig	  - Update current config utilising a provided .config as base
  localmodconfig  - Update current config disabling modules not loaded
  localyesconfig  - Update current config converting local mods to core
  defconfig	  - New config with default from ARCH supplied defconfig
  savedefconfig   - Save current config as ./defconfig (minimal config)
  allnoconfig	  - New config where all options are answered with no
  allyesconfig	  - New config where all options are accepted with yes
  allmodconfig	  - New config selecting modules when possible
  alldefconfig    - New config with all symbols set to default
  randconfig	  - New config with random answer to all options
  listnewconfig   - List new options
  olddefconfig	  - Same as oldconfig but sets new symbols to their
                    default value without prompting
  kvmconfig	  - Enable additional options for kvm guest kernel support
  xenconfig       - Enable additional options for xen dom0 and guest kernel support
  tinyconfig	  - Configure the tiniest possible kernel
  testconfig	  - Run Kconfig unit tests (requires python3 and pytest)

Other generic targets:
  all		  - Build all targets marked with [*]
* vmlinux	  - Build the bare kernel
* modules	  - Build all modules
  modules_install - Install all modules to INSTALL_MOD_PATH (default: /)
  dir/            - Build all files in dir and below
  dir/file.[ois]  - Build specified target only
  dir/file.ll     - Build the LLVM assembly file
                    (requires compiler support for LLVM assembly generation)
  dir/file.lst    - Build specified mixed source/assembly target only
                    (requires a recent binutils and recent build (System.map))
  dir/file.ko     - Build module including final link
  modules_prepare - Set up for building external modules
  tags/TAGS	  - Generate tags file for editors
  cscope	  - Generate cscope index
  gtags           - Generate GNU GLOBAL index
  kernelrelease	  - Output the release version string (use with make -s)
  kernelversion	  - Output the version stored in Makefile (use with make -s)
  image_name	  - Output the image name (use with make -s)
  headers_install - Install sanitised kernel headers to INSTALL_HDR_PATH
                    (default: ./usr)

Static analysers:
  checkstack      - Generate a list of stack hogs
  namespacecheck  - Name space analysis on compiled kernel
  versioncheck    - Sanity check on version.h usage
  includecheck    - Check for duplicate included header files
  export_report   - List the usages of all exported symbols
  headers_check   - Sanity check on exported headers
  headerdep       - Detect inclusion cycles in headers
  coccicheck      - Check with Coccinelle

Kernel selftest:
  kselftest       - Build and run kernel selftest (run as root)
                    Build, install, and boot kernel before
                    running kselftest on it
  kselftest-clean - Remove all generated kselftest files
  kselftest-merge - Merge all the config dependencies of kselftest to existing
                    .config.

Userspace tools targets:
  use "make tools/help"
  or  "cd tools; make help"

Kernel packaging:
  rpm-pkg             - Build both source and binary RPM kernel packages
  binrpm-pkg          - Build only the binary kernel RPM package
  deb-pkg             - Build both source and binary deb kernel packages
  bindeb-pkg          - Build only the binary kernel deb package
  snap-pkg            - Build only the binary kernel snap package (will connect to external hosts)
  tar-pkg             - Build the kernel as an uncompressed tarball
  targz-pkg           - Build the kernel as a gzip compressed tarball
  tarbz2-pkg          - Build the kernel as a bzip2 compressed tarball
  tarxz-pkg           - Build the kernel as a xz compressed tarball
  perf-tar-src-pkg    - Build perf-4.19.23.tar source tarball
  perf-targz-src-pkg  - Build perf-4.19.23.tar.gz source tarball
  perf-tarbz2-src-pkg - Build perf-4.19.23.tar.bz2 source tarball
  perf-tarxz-src-pkg  - Build perf-4.19.23.tar.xz source tarball

Documentation targets:
 Linux kernel internal documentation in different formats from ReST:
  htmldocs        - HTML
  latexdocs       - LaTeX
  pdfdocs         - PDF
  epubdocs        - EPUB
  xmldocs         - XML
  linkcheckdocs   - check for broken external links (will connect to external hosts)
  refcheckdocs    - check for references to non-existing files under Documentation
  cleandocs       - clean all generated files

  make SPHINXDIRS="s1 s2" [target] Generate only docs of folder s1, s2
  valid values for SPHINXDIRS are: networking input crypto media core-api userspace-api gpu maintainer sound kernel-hacking process sh doc-guide vm driver-api filesystems dev-tools admin-guide

  make SPHINX_CONF={conf-file} [target] use *additional* sphinx-build
  configuration. This is e.g. useful to build with nit-picking config.

  Default location for the generated documents is Documentation/output

Architecture specific targets (x86):
* bzImage      - Compressed kernel image (arch/x86/boot/bzImage)
  install      - Install kernel using
                  (your) ~/bin/installkernel or
                  (distribution) /sbin/installkernel or
                  install to $(INSTALL_PATH) and run lilo
  fdimage      - Create 1.4MB boot floppy image (arch/x86/boot/fdimage)
  fdimage144   - Create 1.4MB boot floppy image (arch/x86/boot/fdimage)
  fdimage288   - Create 2.8MB boot floppy image (arch/x86/boot/fdimage)
  isoimage     - Create a boot CD-ROM image (arch/x86/boot/image.iso)
                  bzdisk/fdimage*/isoimage also accept:
                  FDARGS="..."  arguments for the booted kernel
                  FDINITRD=file initrd for the booted kernel

  i386_defconfig           - Build for i386
  x86_64_defconfig         - Build for x86_64

  make V=0|1 [targets] 0 => quiet build (default), 1 => verbose build
  make V=2   [targets] 2 => give reason for rebuild of target
  make O=dir [targets] Locate all output files in "dir", including .config
  make C=1   [targets] Check re-compiled c source with $CHECK (sparse by default)
  make C=2   [targets] Force check of all c source with $CHECK
  make RECORDMCOUNT_WARN=1 [targets] Warn about ignored mcount sections
  make W=n   [targets] Enable extra gcc checks, n=1,2,3 where
		1: warnings which may be relevant and do not occur too often
		2: warnings which occur quite often but may still be relevant
		3: more obscure warnings, can most likely be ignored
		Multiple levels can be combined with W=12 or W=123

Execute "make" or "make all" to build all targets marked with [*]
For further info see the ./README file
```

### Build kernel with KGDB

[Compile Kernel with KGDB support](../../Lab/Lab1_KernelDebug/KernelDebug.md#Compile-Kernel-with-KGDB-support)

```sh
## My recommendation
# Clean up
$ make mrproper
# Make default .config based on 64bit machine
$ make x86_64_defconfig
# Customize the kernel
$ make menuconfig
# Build it
$ make -j"$(nproc)"
```

* [Stackoverflow - How to debug the Linux kernel with GDB and QEMU?](https://stackoverflow.com/questions/11408041/how-to-debug-the-linux-kernel-with-gdb-and-qemu/33203642#33203642)
* [Stackoverflow - Linux kernel live debugging, how it's done and what tools are used?](https://stackoverflow.com/questions/4943857/linux-kernel-live-debugging-how-its-done-and-what-tools-are-used/44226360#44226360)
* [Using kgdb, kdb and the kernel debugger internals](https://www.kernel.org/doc/html/v4.15/dev-tools/kgdb.html)
* [Debugging kernel and modules via gdb](https://www.kernel.org/doc/html/v4.15/dev-tools/gdb-kernel-debugging.html)

#### Use only GDB to debug (TODO)

Use qemu with flag like `-gdb tcp::4321`

* [Stackoverflow - KGDB remote debugging error](https://stackoverflow.com/questions/13748385/kgdb-remote-debugging-error?rq=1)

### mkinitrd / mkinitramfs

[mkinitrd](https://linux.die.net/man/8/mkinitrd)

[mkinitramfs](https://manpages.ubuntu.com/manpages/bionic/en/man8/mkinitramfs.8.html) (in Ubuntu)

```sh
# In the same directory of the linux kernel
$ mkinitramfs -o initrd.img-4.19.23
```

## Resources

* [Linaro HowTo/KernelDeploy](https://wiki.linaro.org/Resources/HowTo/KernelDeploy) - Similar procedure.
