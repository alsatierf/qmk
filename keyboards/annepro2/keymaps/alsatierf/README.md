### Steps to compile and flash the firmware

```bash
TARGET_DIR=#mount point in Windows
```

> Download https://drive.google.com/file/d/1fmxChuVPSQhRoki-jdNWKVDhH75NVn1A/view to $TARGET_DIR

```bash
make annepro2/c18:alsatierf && cp annepro2_c18_alsatierf.bin $TARGET_DIR
(cd $TARGET_DIR && ./annepro2_tools_x64.exe annepro2_c18_alsatierf.bin)
```