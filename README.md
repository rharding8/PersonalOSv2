# PersonalOS
## Note 
This is a personal side project to write OS from scratch. As an computer architect, understanding how OS interact with hardware is essential. Main purpose of this project is to understand how operating system interact with hardware and work profeciently as intermedium between hardware and software. 

## Project Progress 

- Bootloader to load  kernel
- Interrupt handling protocol
- Driver code to control printing word on screen
- Keyboard driver to take user input
V2:
- Properly display unrecognized keys
TODO By Check-In:
- Implement Function Key Functions
- Implement Memory Management Driver
- Show Clearing and Printing Code Buffer Memory
TODO By End:
- Pass and Execute Valid Load and Store Assembly From Input to CPU
- Pass and Execute Peeks and Pokes
STRETCH GOALS:
- Pass and Execute a Valid Jump Instruction
- Implement Checking Assembly Validity
- Streamline Compilation Process
- Make Portable to macOS and WSL


## How to run

#### Install qemu
`sudo apt install qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils`

#### Install nasm
`sudo apt-get install  nasm`

#### Install cross compiler environment
`export PREFIX="/usr/local/i386elfgcc"`

`export TARGET=i386-elf`

`export PATH="$PREFIX/bin:$PATH"`


`mkdir /tmp/src`

`cd /tmp/src`

`curl -O http://ftp.gnu.org/gnu/binutils/binutils-2.24.tar.gz` # If the link 404's, look for a more recent version

`tar xf binutils-2.24.tar.gz`

`mkdir binutils-build`

`cd binutils-build`

`../binutils-2.24/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log`

`make all install 2>&1 | tee make.log`


`cd /tmp/src`

`curl -O https://ftp.gnu.org/gnu/gcc/gcc-4.9.1/gcc-4.9.1.tar.bz2`

`tar xf gcc-4.9.1.tar.bz2`

`mkdir gcc-build`

`cd gcc-build`

`../gcc-4.9.1/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-languages=c --without-headers`

`make all-gcc `

`make all-target-libgcc `

`make install-gcc `

`make install-target-libgcc`

If everything works all right[^1], executing Makefile with make command should compile the OS and `make run` should run the OS.

[^1]: Cross compilation code sequence worked for my workstation. It may not work for all system.

