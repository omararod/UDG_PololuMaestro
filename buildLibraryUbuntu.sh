echo copying h files...
if [ ! -d "include" ]; then
mkdir include
fi
cp -r ServoControl/UDG_PololuMaestro.h include

echo Generating linux x86 binaries...
echo compiling source files for x86...
g++ -c -std=c++11 -m32  ServoControl/UDG_PololuMaestro.cpp  ServoControl/ServoMotor.cpp
echo linking and packaging...   
ar rvs libUDG_PololuMaestro.a UDG_PololuMaestro.o  ServoMotor.o
ar rvs UDG_PololuMaestro.a UDG_PololuMaestro.o  ServoMotor.o
echo cleaning...
rm *.o
if [ ! -d "bin/linuxX86" ]; then
mkdir -p bin/linuxX86
fi
mv libUDG_PololuMaestro.a bin/linuxX86
mv UDG_PololuMaestro.a bin/linuxX86


echo Generating linux x64 binaries...
echo compiling source files for x64...
g++ -c -std=c++11 -m64 ServoControl/UDG_PololuMaestro.cpp  ServoControl/ServoMotor.cpp
echo linking and packaging...   
ar rvs libUDG_PololuMaestro.a UDG_PololuMaestro.o  ServoMotor.o
ar rvs UDG_PololuMaestro.a UDG_PololuMaestro.o  ServoMotor.o
echo cleaning...
rm *.o
if [ ! -d "bin/linuxX64" ]; then
mkdir -p bin/linuxX64
fi

mv libUDG_PololuMaestro.a bin/linuxX64/
mv UDG_PololuMaestro.a bin/linuxX64/







echo done :D 
