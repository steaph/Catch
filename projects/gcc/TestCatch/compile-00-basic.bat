@echo off
::
:: Created by Martin on 254/07/2012.
::
:: Compile Catch selftests
::

g++ -Wall -I../../../include -o TestCatch.exe  ../../SelfTest/catch_self_test.cpp ../../SelfTest/BasicTests.cpp  &&  TestCatch
