@echo off

cd ..

rmdir /s /q .vs
rmdir /s /q .vscode
rmdir /s /q bin
rmdir /s /q bin-int
rmdir /s /q _Chorium

del /s /q *.sln
del /s /q *.vcxproj
del /s /q *.filters
del /s /q *.user