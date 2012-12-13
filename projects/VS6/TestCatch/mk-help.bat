@echo off
::
:: Created by Martin on 21/09/2012.
::
:: Collect all embedded help texts.
::
TestCatch -h >TestCatch.help.txt
TestCatch -h -t -g -l -r -o -s -b -n -a -n -w -? >>TestCatch.help.txt
