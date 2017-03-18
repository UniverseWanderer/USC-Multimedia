You will find

- A program to display and manipulate video. 

This part of the assignment will help you gain a practical understanding of issues that relate to image sampling, aliasing effects, image aspect ratios and pixel aspect ratios. In this assignment you will be given as input a video in one format (standard definition or high definition) and you will be asked to convert it into the other respective format. These problems have very practical applications today ¨C for example, 
1.	High definition 16:9 signals are now a broadcast standard, but there are devices  that might still have a different viewable format  eg 4:3 standard definition formats. The image size has to be reduced and the image(s) need to be down sampled, which will result in aliasing artifacts that need to be corrected with filters. Also, if the pixel aspect ratio is not maintained, it could result in additional distortion.
2.	Consumers today buy high definition television sets, monitors and projectors that can view high definition signals properly. However, a lot of content, especially older content, which is broadcast by TV/cable networks, is still in standard definition NTSC formats. Here the image size has to be increased (up sampled) again causing artifacts and also pixel distortions due to aspect changes. Good filters and image synthesis techniques are typically required to production quality images here.


Accordingly, this part your assignment should take as input either an image or a video stream in one format and produce an output in the other format. Input to your program will be 4 parameters where:
?	The first parameter is the name of the input media file.
?	The next parameter is the name of the output file
?	The third parameter controls the operation of the program. SD2HD should run the code for task 1 above to convert an input video of size 176 x 144 to 960 x 540, HD2SD should run the code for task 2 to convert an input video of size 960 x 540 to 176 x 144. 
?	The last parameter controls the whether or not filtering for anti aliasing should be turned on. A value of 0 indicates this should be off, and a value of 1 indicates it should be switched on.
Here are example command line invocations of your program. 
For task 1
YourProgram.exe C:/myDir/input_vid.rgb  C:/myDir/output_vid.rgb  SD2HD 0
YourProgram.exe C:/myDir/input_vid.rgb  C:/myDir/output_vid.rgb  SD2HD 1
For task 2
YourProgram.exe C:/myDir/input_vid.rgb  C:/myDir/output_vid.rgb  HD2SD 0
YourProgram.exe C:/myDir/input_vid.rgb  C:/myDir/output_vid.rgb  HD2SD 1
