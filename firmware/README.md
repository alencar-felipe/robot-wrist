## Debug

```
$ openocd -f stlink_bluepill.cfg &
$ arm-none-eabi-gdb build/robot-wrist.elf
(gdb) target remote :3333
```

Reset: ```monitor reset halt```

## References

- https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf
- https://mensi.ch/blog/articles/bare-metal-c-programming-the-blue-pill-part-1
- https://github.com/fcayci/stm32f1-bare-metal
- https://vivonomicon.com/2020/06/28/bare-metal-stm32-programming-part-10-uart-communication/
- https://stackoverflow.com/questions/22317477/setting-up-heap-in-memory-for-arm-embedded-system
- https://github.com/IDWizard/uln2003