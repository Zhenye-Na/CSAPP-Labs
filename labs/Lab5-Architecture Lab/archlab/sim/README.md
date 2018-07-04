# Architecture Lab

Cannot finish this because of no library called `fl`.

```sh
$ make clean; make
rm -f *~ core
(cd misc; make clean)
make[1]: Entering directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/misc'
rm -f *.o *.yo *.exe yis yas hcl2c mux4 *~ core.* 
rm -f hcl.tab.c hcl.tab.h lex.yy.c yas-grammar.c
make[1]: Leaving directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/misc'
(cd pipe; make clean)
make[1]: Entering directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/pipe'
rm -f psim pipe-*.c *.o *.exe *~ 
make[1]: Leaving directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/pipe'
(cd seq; make clean)
make[1]: Entering directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/seq'
rm -f ssim ssim+ seq*-*.c *.o *~ *.exe *.yo *.ys
make[1]: Leaving directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/seq'
(cd y86-code; make clean)
make[1]: Entering directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/y86-code'
rm -f *.o *.yis *~ *.yo *.pipe *.seq *.seq+ core
make[1]: Leaving directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/y86-code'
(cd ptest; make clean)
make[1]: Entering directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/ptest'
rm -f *.o *~ *.yo *.ys
make[1]: Leaving directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/ptest'
(cd misc; make all)
make[1]: Entering directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/misc'
gcc -Wall -O1 -g -c yis.c
gcc -Wall -O1 -g -c isa.c
gcc -Wall -O1 -g yis.o isa.o -o yis
gcc -Wall -O1 -g -c yas.c
flex yas-grammar.lex
mv lex.yy.c yas-grammar.c
gcc -O1 -c yas-grammar.c
gcc -Wall -O1 -g yas-grammar.o yas.o isa.o -lfl -o yas
/usr/bin/ld: cannot find -lfl
collect2: error: ld returned 1 exit status
make[1]: *** [yas] Error 1
make[1]: Leaving directory `/home/zna2/Desktop/computer-systems/labs/Lab5-Architecture Lab/archlab/sim/misc'
make: *** [all] Error 2
```

Make sure you have installed `flex` correctly.

```sh
$ flex --version
flex 2.5.37
```

```sh
$ cd /usr/lib
$ ls
alsa                          libpam_misc.so.0.82.0
audit                         libpam.so.0
binfmt.d                      libpam.so.0.83.1
clang                         libpanel.so.5
cpp                           libpanel.so.5.9
crda                          libpanelw.so.5
crt1.o                        libpanelw.so.5.9
crti.o                        libpcprofile.so
crtn.o                        libpcre16.so.0
cups                          libpcre16.so.0.2.0
debug                         libpcre32.so.0
dracut                        libpcre32.so.0.0.0
dtrace                        libpcrecpp.so.0
elfutils                      libpcrecpp.so.0.0.0
fastx2                        libpcreposix.so.0
firewalld                     libpcreposix.so.0.0.1
firmware                      libpcre.so.1
games                         libpcre.so.1.2.0
gcc                           libpng12.so.0
gconv                         libpng12.so.0.50.0
gcrt1.o                       libpng.so.3
gems                          libpng.so.3.50.0
grub                          libpthread-2.17.so
hsqldb                        libpthread_nonshared.a
i686                          libpthread.so
inkscape                      libpthread.so.0
java                          libresolv-2.17.so
java-1.5.0                    libresolv.so
java-1.6.0                    libresolv.so.2
java-1.7.0                    librpcsvc.a
java-1.8.0                    librt-2.17.so
java-ext                      librt.so
jvm                           librt.so.1
jvm-commmon                   libSegFault.so
jvm-exports                   libselinux.so.1
jvm-private                   libsepol.so.1
kbd                           libSM.so.6
kde3                          libSM.so.6.0.1
kde4                          libstdc++.so.6
kdump                         libstdc++.so.6.0.19
kernel                        libsystemd-daemon.so.0
ld-2.17.so                    libsystemd-daemon.so.0.0.12
ld-linux.so.2                 libsystemd-id128.so.0
libanl-2.17.so                libsystemd-id128.so.0.0.28
libanl.so                     libsystemd-journal.so.0
libanl.so.1                   libsystemd-journal.so.0.11.5
libasm-0.170.so               libsystemd-login.so.0
libasm.so.1                   libsystemd-login.so.0.9.3
libattr.so.1                  libsystemd.so.0
libattr.so.1.1.0              libsystemd.so.0.6.0
libaudit.so.1                 libthread_db-1.0.so
libaudit.so.1.0.0             libthread_db.so
libauparse.so.0               libthread_db.so.1
libauparse.so.0.0.0           libtic.so.5
libBrokenLocale-2.17.so       libtic.so.5.9
libBrokenLocale.so            libtiff.so.3
libBrokenLocale.so.1          libtiff.so.3.9.4
libbsd.a                      libtiffxx.so.3
libbsd-compat.a               libtiffxx.so.3.9.4
libbz2.so.1                   libtinfo.so.5
libbz2.so.1.0.6               libtinfo.so.5.9
libc-2.17.so                  libudev.so.1
libcap-ng.so.0                libudev.so.1.6.2
libcap-ng.so.0.0.0            libutil-2.17.so
libcap.so.2                   libutil.so
libcap.so.2.22                libutil.so.1
libcidn-2.17.so               libuuid.so.1
libcidn.so                    libuuid.so.1.3.0
libcidn.so.1                  libX11.so.6
libc_nonshared.a              libX11.so.6.3.0
libcrack.so.2                 libX11-xcb.so.1
libcrack.so.2.9.0             libX11-xcb.so.1.0.0
libcrypt-2.17.so              libXau.so.6
libcrypt.so                   libXau.so.6.0.0
libcrypt.so.1                 libxcb-composite.so.0
libc.so                       libxcb-composite.so.0.0.0
libc.so.6                     libxcb-damage.so.0
libdb-5.3.so                  libxcb-damage.so.0.0.0
libdb-5.so                    libxcb-dpms.so.0
libdl-2.17.so                 libxcb-dpms.so.0.0.0
libdl.so                      libxcb-dri2.so.0
libdl.so.2                    libxcb-dri2.so.0.0.0
libdw-0.170.so                libxcb-dri3.so.0
libdw.so.1                    libxcb-dri3.so.0.0.0
libelf-0.170.so               libxcb-glx.so.0
libelf.so.1                   libxcb-glx.so.0.0.0
libexpat.so.1                 libxcb-present.so.0
libexpat.so.1.6.0             libxcb-present.so.0.0.0
libfontconfig.so.1            libxcb-randr.so.0
libfontconfig.so.1.7.0        libxcb-randr.so.0.1.0
libform.so.5                  libxcb-record.so.0
libform.so.5.9                libxcb-record.so.0.0.0
libformw.so.5                 libxcb-render.so.0
libformw.so.5.9               libxcb-render.so.0.0.0
libfreebl3.chk                libxcb-res.so.0
libfreebl3.so                 libxcb-res.so.0.0.0
libfreeblpriv3.chk            libxcb-screensaver.so.0
libfreeblpriv3.so             libxcb-screensaver.so.0.0.0
libfreetype.so.6              libxcb-shape.so.0
libfreetype.so.6.10.0         libxcb-shape.so.0.0.0
libg.a                        libxcb-shm.so.0
libgcc_s-4.8.5-20150702.so.1  libxcb-shm.so.0.0.0
libgcc_s.so.1                 libxcb.so.1
libgcrypt.so.11               libxcb.so.1.1.0
libgcrypt.so.11.8.2           libxcb-sync.so.1
libgpg-error.so.0             libxcb-sync.so.1.0.0
libgpg-error.so.0.10.0        libxcb-xevie.so.0
libICE.so.6                   libxcb-xevie.so.0.0.0
libICE.so.6.3.0               libxcb-xf86dri.so.0
libieee.a                     libxcb-xf86dri.so.0.0.0
libjpeg.so.62                 libxcb-xfixes.so.0
libjpeg.so.62.1.0             libxcb-xfixes.so.0.0.0
liblz4.so.1                   libxcb-xinerama.so.0
liblz4.so.1.7.5               libxcb-xinerama.so.0.0.0
liblzma.so.5                  libxcb-xinput.so.0
liblzma.so.5.2.2              libxcb-xinput.so.0.1.0
libm-2.17.so                  libxcb-xkb.so.1
libmcheck.a                   libxcb-xkb.so.1.0.0
libmemusage.so                libxcb-xselinux.so.0
libmenu.so.5                  libxcb-xselinux.so.0.0.0
libmenu.so.5.9                libxcb-xtest.so.0
libmenuw.so.5                 libxcb-xtest.so.0.0.0
libmenuw.so.5.9               libxcb-xvmc.so.0
libm.so                       libxcb-xvmc.so.0.0.0
libm.so.6                     libxcb-xv.so.0
libncurses.so.5               libxcb-xv.so.0.0.0
libncurses++.so.5             libXext.so.6
libncurses.so.5.9             libXext.so.6.4.0
libncurses++.so.5.9           libXft.so.2
libncurses++w.so.5            libXft.so.2.3.2
libncursesw.so.5              libXrender.so.1
libncurses++w.so.5.9          libXrender.so.1.3.0
libncursesw.so.5.9            libXss.so.1
libnsl-2.17.so                libXss.so.1.0.0
libnsl.so                     libz.so.1
libnsl.so.1                   libz.so.1.2.7
libnss_compat-2.17.so         locale
libnss_compat.so              lsb
libnss_compat.so.2            Mcrt1.o
libnss_db-2.17.so             modprobe.d
libnss_db.so                  modules
libnss_db.so.2                modules-load.d
libnss_dns-2.17.so            mozilla
libnss_dns.so                 NetworkManager
libnss_dns.so.2               node_modules
libnss_files-2.17.so          polkit-1
libnss_files.so               python2.7
libnss_files.so.2             python3.4
libnss_hesiod-2.17.so         rpm
libnss_hesiod.so              rstudio
libnss_hesiod.so.2            rtkaio
libnss_myhostname.so.2        Scrt1.o
libnss_mymachines.so.2        security
libnss_nis-2.17.so            sendmail
libnss_nisplus-2.17.so        sendmail.postfix
libnss_nisplus.so             sse2
libnss_nisplus.so.2           sysctl.d
libnss_nis.so                 systemd
libnss_nis.so.2               tmpfiles.d
libpamc.so.0                  tuned
libpamc.so.0.82.1             udev
libpam_misc.so.0              yum-plugins
```
