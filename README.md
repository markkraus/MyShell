# Repository Guide
https://drive.google.com/file/d/1vYkRkA1_LLcgz7zWPXs6Voku1SuvR9k-/view?usp=sharing

# MyShell
- Function pointers<br>
- Threading<br>
- Parent/child fork<br>

# Files in this Repository
*<ins> msh.c </ins>*: Program to create a shell terminal with terminal command functionalities.<br>
*<ins>plugin.c</ins>*: Simple plugin to load into the shell program.<br>

# How to Run
gcc msh.c -o msh<br>
./msh<br>
To load the plugin (must be a shared object file): load plugin<br>
To run the plugin: plugin <_command_>
