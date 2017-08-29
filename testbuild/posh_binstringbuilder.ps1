Param (
    [string] $SourceBinary = (split-path -parent $MyInvocation.MyCommand.Definition) + "\mimikatz-ReflectiveDLL-x64.dll",
    [string] $OutputBinary = $SourceBinary + '.built.dll',
    [int] $SourceStrLength = 1024,
    [string] $SourceStr = 'kodx mimikatz command list',
    [string] $TargetStr = '"log %TEMP%\mimikatz.log /clear" version localtime exit'
)

$srcBinaryPath = Resolve-Path $SourceBinary
if (![IO.File]::Exists($srcBinaryPath)) {
    Throw "$srcBinaryPath does not exist."
}
$outBinaryPath = $OutputBinary

# sample target string, run commands on 12:00 every day, use %TEMP%\mimikatz.log as log file and clear it every day
# $TargetStr = 'cycle "log %TEMP%\mimikatz.log /clear" version localtime "waitfor 12 00"'
# here is:
# cycle - run this commands in cycle
# "log %TEMP%\mimikatz.log /clear" - write output to log, '/clear' mean owerwrite log every time
# version - print out current version
# localtime - print out current time
# "waitfor 12 00" - wait for 12:00, if this has pass, wait for next day 12:00

$enc = [system.Text.Encoding]::Unicode
$srcStrLength = $SourceStrLength - 1
# fill remaining of string with null chars
$($SourceStr.Length)..$srcStrLength| % { $SourceStr += [char]0 }
$($TargetStr.Length)..$srcStrLength | % { $TargetStr += [char]0 }
# read source binary
$bytes = [io.file]::ReadAllBytes($srcBinaryPath)
$idx = $enc.GetString($bytes).IndexOf($SourceStr) * 2 # String in Unicode, 1 char = 2 bytes
0..$srcStrLength | %{ $bytes[$idx+$_] = $($enc.GetBytes($TargetStr))[$_] }
[io.file]::WriteAllBytes($outBinaryPath, $bytes)
