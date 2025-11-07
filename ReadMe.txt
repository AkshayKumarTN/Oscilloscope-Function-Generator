To View and Edit the Code:

Get the code Editor (Mac and Windows):

1. Install Visual Studio Code: https://code.visualstudio.com/download.
2. Install the C/C++ extensions for VS Code: View / Extensions and search for C/C++.
3. Click File/Open Folder and open the folder with the .c files.  You should now be able to edit and view the source code.

To compile on Windows:

Get the C/C++ Compiler: 

1. Install the Microsoft Visual C++ (MSVC) compiler:
https://visualstudio.microsoft.com/downloads/#remote-tools-for-visual-studio-2022

a) Go all the way to the bottom of the page and select: Build Tools for Visual Studio 2022
b) Check the desktop development, as shown in the Prerequisites on this page:

https://code.visualstudio.com/docs/cpp/config-msvc#_run-vs-code-outside-the-developer-command-prompt

2. In Windows Search type: Developer PowerShell and open it.
3. At the command prompt type: cl 
4. If installed correctly, you'll get a message about Compiler version and the architecture: for x86 or x64.  Remember this.

(Optional if Visual Studio Community is installed: After installation open a command prompt and run the following command:
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64 -- if you want to compile the x64 architecture)

5. Change directory to your *.c files.
6. For x86 architecture type:

cl *.c /Zi /EHsc /nologo /link i386/ftd2xx.lib /out:controller.exe

7. For x64 architecture type:

cl *.c /Zi /EHsc /nologo /link amd64/ftd2xx.lib /out:controller.exe

8. If everything works, you now have a controller.exe.  Run that from the command prompt.
9. Select Send Morse Code and test the LEDs by connecting the power wire to DB0 on the chip.

To compile on Mac:

1. Install the GCC compiler from the command line: brew install gcc
2. If (1) fails that you can install the XCode tools: xcode-select --install
3. Open the command line and change directory to the directory with the source code.  
4. From the command prompt, type: 

gcc *.c libftd2xx.a -I. -L. -framework CoreFoundation -framework IOKit -o controller

This should compile the code and return and executable called controller.
Start the controller from the command line by typing: ./controller



Run Code command
compile command:
cl *.c /Zi /EHsc /nologo /link i386/ftd2xx.lib /out:controller.exe

excute command:
controller.exe 



Find and Open x64 Native Tools Command Prompt
Method 1: Through Start Menu


----------------------------------------------------------------------------------
Added Read And Write file
----------------------------------------------------------------------------------
Steps to Run
Click Start Menu
Type: "x64 Native"
You should see: "x64 Native Tools Command Prompt for VS 2022" (or 2019/2017)
Right-click → Run as Administrator

C:\Windows\System32>d:

cd "D:\Stevens\Course\SSW 565 Software Architecture\Final Project - New\Oscilloscope-Function-Generator"

cl /EHsc /std:c++14 main.cpp ReqWrite.cpp ReqRead.cpp MorseCode.cpp ftd2xx.lib /Fe:oscilloscope.exe

oscilloscope.exe


----------------------------------------------------------------------------------
Example
----------------------------------------------------------------------------------
=== Control Menu ===
1. Write byte (simple example)
2. Read byte (simple example)
3. Batch write example
4. Batch read example
5. Interactive write
6. Interactive read
7. Send Morse Code (Text to Morse)
8. Exit
Enter your choice: 7

=== Morse Code Sender ===
Enter text to convert to Morse code:
hello

You entered: "hello"
Morse translation: .... . .-.. .-.. ---

Choose an option:
1. Send via FTDI and save to file
2. Save to file only (no transmission)
3. Cancel
Enter choice: 1

Transmitting morse code...

=== Sending Morse Code ===
Original text: hello
Morse code: .... . .-.. .-.. ---

Morse code saved to: morse_20251020_165408.txt
Transmission complete!

