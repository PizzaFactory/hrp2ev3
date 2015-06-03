# DEF_HRP2_ONLY
KERNEL=HRP2
# END_HRP2_ONLY
# DEF_ASP_ONLY
# KERNEL=ASP
# END_ASP_ONLY

if [ "$KERNEL" = "HRP2" ]; then
    OBJNAME=hrp2
elif [ "$KERNEL" = "ASP" ]; then
    OBJNAME=asp
else
    echo 'Error: KERNEL must be HRP2 or ASP.'
    exit 1
fi

# Make kernel
./configure -T ckb1808_gcc
make clean
make >/dev/null

# Set OBJEXT if compiled in Cygwin
if [ "$(uname -o)" == "Cygwin" ]; then
    OBJEXT=.exe
else
    OBJEXT=
fi

# Build uImage
arm-none-eabi-objcopy -O binary $OBJNAME$OBJEXT $OBJNAME.bin
mkimage -A arm -O linux -T kernel -C none -a 0xc0008000 -e 0xc0008000 \
    -n "TOPPERS/$KERNEL Kernel (EV3)" -d $OBJNAME.bin uImage
make clean >/dev/null
chmod +x uImage
