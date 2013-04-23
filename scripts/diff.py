""" Command line interface to difflib.py providing diffs in four formats:

* ndiff:    lists every line and highlights interline changes.
* context:  highlights clusters of changes in a before/after format.
* unified:  highlights clusters of changes in an inline format.
* html:     generates side by side comparison with change highlights.

"""

import sys, os, time, difflib, optparse

def main():

    usage = "usage: %prog [options] fromfile tofile"
    parser = optparse.OptionParser(usage)
    parser.add_option("-c", action="store_true", default=False, help='Produce a context format diff (default)')
    parser.add_option("-u", action="store_true", default=False, help='Produce a unified format diff')
    parser.add_option("-m", action="store_true", default=False, help='Produce HTML side by side diff (can use -c and -l in conjunction)')
    parser.add_option("-n", action="store_true", default=False, help='Produce a ndiff format diff')
    parser.add_option("-l", "--lines", type="int", default=3, help='Set number of context lines (default 3)')
    (options, args) = parser.parse_args()

    if len(args) == 0:
        parser.print_help()
        sys.exit(1)
    if len(args) != 2:
        parser.error("need to specify both a fromfile and tofile")

    n = options.lines
    fromfile, tofile = args

    fromdate = time.ctime(os.stat(fromfile).st_mtime)
    todate = time.ctime(os.stat(tofile).st_mtime)
    fromlines = open(fromfile, 'U').readlines()
    tolines = open(tofile, 'U').readlines()

    if options.u:
        diff = difflib.unified_diff(fromlines, tolines, fromfile, tofile, fromdate, todate, n=n)
    elif options.n:
        diff = difflib.ndiff(fromlines, tolines)
    elif options.m:
        diff = """
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
          "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
    <meta http-equiv="Content-Type"
          content="text/html; charset=ISO-8859-1" />
    <title></title>
    <style type="text/css">
        table.diff {font-family:Consolas; font-size:11px; border:medium;}
        .diff_header {background-color:#e0e0e0}
        td.diff_header {text-align:right}
        .diff_next {background-color:#c0c0c0}
        .diff_add {background-color:#aaffaa}
        .diff_chg {background-color:#ffff77}
        .diff_sub {background-color:#ffaaaa}
    </style>
</head>

<body>
"""
        diff += difflib.HtmlDiff(wrapcolumn=90, tabsize=4).make_table(fromlines,tolines,fromfile,tofile,context=options.c,numlines=n)
        diff += """
</body>
</html>
"""
    else:
        diff = difflib.context_diff(fromlines, tolines, fromfile, tofile, fromdate, todate, n=n)

    sys.stdout.writelines(diff)

if __name__ == '__main__':
    main()
