# Group16

## Xv6 Operating System Installation Guide

This guide provides step-by-step instructions for installing the Xv6 operating system on Ubuntu and MacOS (Silicon).

Before you begin, make sure you have the following prerequisites installed on your system:

## Ubuntu prerequisites (todo)

## MacOS (Silicon) prerequisites

1. Open a terminal

2. Install Homebrew:

   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

3. Install XCode developer tools:

   ```bash
   xcode-select --install
   ```

4. Install the RISC-V compiler toolchain with Homebrew (arch -arm64):

   ```bash
   brew tap riscv/riscv;
   brew install riscv-tools
   ```

5. Install qemu:

   ```bash
   brew install qemu
   ```

6. Add the following line in the .zshrc file:

   ```text
   export PATH="$PATH:/usr/local/opt/riscv-gnu-toolchain/bin"
   ```

7. Download the qemu-5.1.0 and unzip the downloaded .tar.xz file:

   ```bash
   wget https://download.qemu.org/qemu-5.1.0.tar.xz
   ```

8. Two patches are needed:

   - [PATCH 1](https://patchwork.kernel.org/project/qemu-devel/patch/20210103145055.11074-1-r.bolshakov@yadro.com/) - Create a folder called 'patch' at the same level of the 'qemu-5.1.0' directory and download the .patch file (by clicking the 'mbox' button in the top-right corner of the page at the given link) inside that folder. Then run this from the qemu directory:

     ```bash
     patch -p1 < ../patch/v2-tcg-Fix-execution-on-Apple-Silicon.patch
     ```

   - [PATCH 2](https://gist.github.com/stefannilsson/8a083e07f4103af2520e87fdb1f50efc/revisions) - Find the file util/osdep.c in the 'qemu-5.1.0' directory and add the two lines as shown in the given link.

9. Then run configure with the following command from the qemu directory (some packages might be missing so just use brew to install them if that is the case):

   ```bash
   ./configure --disable-kvm --disable-werror --prefix=/usr/local --target-list="riscv64-softmmu"
   ```

10. And complete the installation with:

    ```bash
    make;
    make install
    ```

## Clone the Xv6 Repository

1. Open a terminal.

2. Clone the Xv6 repository using Git:

   ```bash
   git clone https://github.com/mit-pdos/xv6-riscv.git
   cd xv6-riscv
   ```

## Build and run Xv6

1. Build Xv6:

   ```bash
   make
   ```

2. Run Xv6 using QEMU:

   ```bash
   make qemu
   ```

This command will launch QEMU, and you should see the Xv6 shell.

To exit QEMU you can enter `ctrl+a` and then `x`.

## Cleanup

To clean up the Xv6 build files, you can use:

```bash
make clean
```
