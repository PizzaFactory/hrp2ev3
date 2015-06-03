KERNEL=HRP2
APPLDIR=apploader
SRCLANG=c

if [ "$KERNEL" = "HRP2" ]; then
    OBJNAME=hrp2
elif [ "$KERNEL" = "ASP" ]; then
    OBJNAME=asp
else
    echo 'Error: KERNEL must be HRP2 or ASP.'
    exit 1
fi

# Set OBJEXT if compiled in Cygwin
if [ "$(uname -o)" == "Cygwin" ]; then
    OBJEXT=.exe
else
    OBJEXT=
fi

# Make kernel
./configure -T ev3_gcc -A $APPLDIR -a $APPLDIR -t $APPLDIR -l $SRCLANG
make clean
make >/dev/null

# Build uImage if kernel is generated successfully
if [ $? -eq 0 ]; then
    arm-none-eabi-objcopy -O binary $OBJNAME$OBJEXT $OBJNAME.bin
    mkimage -A arm -O linux -T kernel -C none -a 0xc0008000 -e 0xc0008000 \
        -n "$KERNEL Kernel w/ App Loader (EV3)" -d $OBJNAME.bin uImage
    make realclean >/dev/null
    chmod +x uImage
fi
