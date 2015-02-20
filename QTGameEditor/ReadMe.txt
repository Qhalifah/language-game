How to build and run the Game editor:

1. Build the Release version of the SFML_BGM project which requires SFML and Cereal.
	-> Go to the project Properties 
	-> Build targets 
	-> choose Release in Build targets
	-> choose GUI application for Type
2. Copy SFML_BGM.exe from SFML_BGM/bin/Release/SFML_BGM.exe to the build-GameEditor-Desktop_Qt_5_2_1_MinGW_32bit-Debug folder
3. Open the GameEditor Qt Project file, which is in the GameEditor folder, in Qt.
4. Add all files in the GameEditor folder to the project.
5. Add the BGM.h and Communication.h files, which are in the SFML_BGM folder, to the project.