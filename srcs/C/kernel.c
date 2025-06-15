void kernel_main() {
    volatile unsigned char *video_memory = (unsigned char *)0xB8000; // Video memory address
    video_memory[0] = '4';
    video_memory[1] = 0x07;
    video_memory[2] = '2';
    video_memory[3] = 0x07;
}