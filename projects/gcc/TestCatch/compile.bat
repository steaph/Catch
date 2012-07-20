@echo off
::
:: Created by Martin on 20/07/2012.
::
:: Compile Catch selftests
::

g++ -m32 -Wall -I../../../include -o TestCatch  ../../SelfTest/TrickyTests.cpp ../../SelfTest/ApproxTests.cpp ../../SelfTest/catch_self_test.cpp ../../SelfTest/catch_self_test.hpp ../../SelfTest/ClassTests.cpp ../../SelfTest/ConditionTests.cpp ../../SelfTest/ExceptionTests.cpp ../../SelfTest/GeneratorTests.cpp ../../SelfTest/MessageTests.cpp ../../SelfTest/MiscTests.cpp ../../SelfTest/TestMain.cpp  && TestCatch
