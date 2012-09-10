@echo off
::
:: Created by Martin on 10/09/2012.
::
:: Collect all embedded help texts
::

echo. > TestCatch.help.txt

for %%t in (-t -l -r -o -s -b -n -a -nt -w -?) do (
   TestCatch -h %%t >> TestCatch.help.txt
)
