g++ simulator.cpp -o simulator
simulator.exe

javac -cp ";./chartJarFiles/*" chart.java
java -cp ";./chartJarFiles/*" chart

DEL simulator.exe
DEL chart.class
DEL processData.txt