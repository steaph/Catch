@echo off
::
:: Created by Martin on 20/07/2012.
::
:: Compile Catch selftests
::

cl -nologo -W3 -EHsc -GR -I../../../include -FeTestCatch.exe  ../../SelfTest/catch_self_test.cpp ../../SelfTest/TestMain.cpp ../../SelfTest/ApproxTests.cpp ../../SelfTest/BasicTests.cpp ../../SelfTest/ClassTests.cpp ../../SelfTest/ConditionTests.cpp ../../SelfTest/ExceptionTests.cpp ../../SelfTest/GeneratorTests.cpp ../../SelfTest/MessageTests.cpp ../../SelfTest/MiscTests.cpp ../../SelfTest/PointerTests.cpp ../../SelfTest/TrickyTests.cpp  &&  TestCatch
