# CSE 687 Object Oriented Design Project: MapReduce
## How to get started

The project can be executed in one of two ways, by running via visual studio, or through the command line. The instructions for using either method can be found below.

### <ins>Via Visual Studio IDE:</ins>
   - Open the project solution file `“CSE-687_OOD-Project.sln”` in Visual Studio 19 or above
   - The solution will contain two projects. `“CSE-687_OOD-Project"` and `“CSE-687_OOD-Project-Unit-Tests"`
   - Right click on the `“CSE-687_OOD-Project"` project and click `**“Properties"**`
   - Once the properties window opens, select the `“Debugging”` menu option under the `“Configuration Properties”` dropdown label. 
   - In the `“Command Arguments”` textbox, enter in the following paths to reference the input, intermediate, and output file directories for running the program. Be sure to separate the paths by a `“space”`. These directories are included as part of the project for running locally and for the unit test project as well. The `“input”` directory will include all the **“Shakespear”** test files for running MapReduce. The `“temp”` directory will be where intermediate files are stored during the process, and the `“output”` directory will be where the final output file is stored.
     - .\inputfiles
     - .\tempfiles
     - .\outputfiles
   - Click the green “play” button in Visual Studio to run the project in debug mode. (Or press Ctrl +F5 to run without debugging).


### <ins> Via Command Line:</ins>
   - Open your command line prompt
   - Change directories (cd – command on windows) until you are within the `“\CSE-687_OOD-Project\x64\Debug”` directory of the project.
   - Once you are in this directory, you can start the program by running the following command: 
     -  `“name-of-executable" “path-to-input-directory" “path-to-temp-directory" “path-to-output-directory"`
          -  Ex. `CSE-687_OOD-Project.exe \directory1 \directory2 \directory3`
             - **Be sure to separate path names by a space**
          - `Note: if the path to your directory contains a space within the name, be sure to wrap the directory name in quotes to run properly.`
          -  Ex. `CSE-687_OOD-Project.exe “\My directory1” “\My directory2” “\My directory3”`
             - **Note: When running the project from the command line, the provided paths must either be an “absolute path” or a “relative path” based on the location of the executable file (.exe). The .exe file is in the “CSE-687_OOD-Project\x64\Debug” directory as stated above.**  


    

