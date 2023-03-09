GenerateProjects.bat

================= OpenGL ====================
call .\vender\premake\bin\premake5_a15.exe vs2017 --renderAPI=opengl
pause

================= Vulkan ====================
call .\vender\premake\bin\premake5_a15.exe vs2017 --renderAPI=vulkan
pause



TODO
	Bone需要以mesh為基準，不然一堆都骨架比mesh多，造成要讀的Bone太多