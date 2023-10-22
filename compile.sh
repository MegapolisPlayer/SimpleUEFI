rm -rf output

echo "Compiling"

clang \
        -target x86_64-unknown-windows \
        -ffreestanding \
        -fshort-wchar \
        -mno-red-zone \
		-fno-stack-protector \
		-I uefi/ \
		-I uefi/protocol/ \
        -o "obj.o" \
        -c "main.c"

echo "Linking"

mkdir -p output

lld-link \
        -machine:x64 \
        -entry:EfiMain \
		-nodefaultlib \
        -subsystem:efi_application \
        -out:output/BOOTX64.EFI \
		obj.o


echo "Creating disk"

mkdir -p tempbuild
cd tempbuild

dd if=/dev/zero of=UEFI.img bs=1k count=1440
mformat -i UEFI.img -f 1440 ::
mmd -i UEFI.img ::/EFI
mmd -i UEFI.img ::/EFI/BOOT
mcopy -i UEFI.img ../output/BOOTX64.EFI ::/EFI/BOOT

mkdir iso
cp UEFI.img iso
xorriso -as mkisofs -R -f -e UEFI.img -no-emul-boot -o ../output/UEFI.iso iso

cd ..

echo "Cleaning up"

rm -rf tempbuild
rm -f obj.o

