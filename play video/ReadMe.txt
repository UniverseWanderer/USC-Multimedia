You will find

- A program to play consistant images as video.

The parameters to your display program will be the filename, width, height, frame rate to displah. Ideally we would like to use standard video frame sizes (eg HD, CIF) but due to memory restrictions, processing power and display monitor sizes, we will use half the standard sizes in both dimensions for this assignment. For instance, HD video is 1920 x 1080, but we¡¯ll work with 960 x 540. Similarly, CIF video is 352x288, but we will work with 176 x 144. Thereby example invocations to your display program will look like

YourDisplay.exe C:/myDir/input_vid.rgb  176 144 10
// Read the input file havig frame width=176, frame height=144, and play the video at 10
// frames per second. This frame size corresponds to the standard CIF video format
YourDisplay.exe C:/myDir/input_vid.rgb  960 540 20
// Read the input file havig frame width=960, frame height=540, and play the video at 10
// frames per second. This frame size corresponds to the standard CIF video format

For the purpose of this assignment, all videos given to you will have the same two sizes mentioned above but may varying number of frames. Additionally -
?	The videos have been natively created at 10 frames/second, which means when played back at 10 fps they seem to play at the right speed. We may choose to play them at different speeds though to evaluate your synchronization.
?	The videos are 3 channel RGB color with each pixel having 3 bytes and 1 byte per channel. These parameters will not change for your assignment, although we might view your video at different frame rates

Make sure your video plays in a loop when it reaches the end respecting the input frame rate parameter.
.
Data Format:
The format of your native input video will consist of a list of pixels frame-by-frame, channel-by-channel with every pixel represented as three bytes (or chars), one byte (or char) per channel. To elaborate, the data will be stored as frames - frame1, frame2, ¡­.. frame n. Each frame is further stored as rows of bytes for each channel - rrr¡­bbb¡­ggg¡­. And each r, g and b is a byte long. So, if for our standard definition video we will have an invocation of 

YourDisplay.exe C:/myDir/input_vid.rgb  176 144 10

The input file will have 176x144 bytes of red of frame1, then 176x144 bytes of blue of frame1, then 176x144 bytes of green of frame1, followed by 176x144 bytes of red of frame2, then 176x144 bytes of blue of frame1, then 176x144 bytes of green of frame1, followed by frame 3 ¡­. And so on.
