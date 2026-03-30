%SystemRoot%\System32\reg.exe query "HKLM\Software\Microsoft\Windows\CurrentVersion\App Paths\chrome.exe" >nul 2>&1
IF  not errorlevel 1 (
start chrome.exe --user-data-dir=%~dp0chrome-data  --proxy-server="socks5://127.0.0.1:10808"  --proxy-bypass-list="localhost;127.*;10.*;172.16.*;172.17.*;172.18.*;172.19.*;172.20.*;172.21.*;172.22.*;172.23.*;172.24.*;172.25.*;172.26.*;172.27.*;172.28.*;172.29.*;172.30.*;172.31.*;192.168.*"  https://www.google.com/ncr
) else (
	echo Chrome浏览器不存在或没有正确安装，请尝试重新安装Chrome浏览器
)