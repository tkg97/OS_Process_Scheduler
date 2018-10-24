#!/bin/bash
g++ -o simulator simulator.cpp
./simulator
rm simulator
javac -classpath ";./chartJarFiles/*" chart.java
java -classpath ";./chartJarFiles/*" chart
rm chart.class processData.txt