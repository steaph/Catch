@echo off
::
:: Created by Martin on 20/07/2012.
::
:: Compile Catch selftests
::

g++ -m32 -Wall -I../../../include -o TestCatch  ../../SelfTest/catch_self_test.cpp ../../SelfTest/TestMain.cpp ../../SelfTest/ApproxTests.cpp ../../SelfTest/BasicTests.cpp ../../SelfTest/BDDTests.cpp ../../SelfTest/ClassTests.cpp ../../SelfTest/ConditionTests.cpp ../../SelfTest/ExceptionTests.cpp ../../SelfTest/GeneratorTests.cpp ../../SelfTest/MessageTests.cpp ../../SelfTest/MiscTests.cpp ../../SelfTest/PointerTests.cpp ../../SelfTest/TrickyTests.cpp ../../SelfTest/VariadicMacrosTests.cpp  &&  TestCatch
