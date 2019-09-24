; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！
#define MyAppName "PiServiceTool"#define MyAppVersion "2.0"
;#define MyInputPath  "E:\Intergration\PiInstall\inno_setup_source"
;#define MyAppPath  "E:\Intergration\PiInstall\"
;#define XbmcPath   "E:\Intergration\PiInstall\inno_setup_source\xbmc"
#define MyAppPublisher "小派科技（上海）有限责任公司"
#define MyAppURL "http://www.pimaxVR.com"
#define MyAppExeName "PiServiceTool.exe"



[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{53EC1BB1-2939-4334-B59B-E6C48A5CBC39}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
;DefaultDirName={code:PimaxDir}\Runtime
CreateAppDir=no;OutputDir=.\setup;OutputBaseFilename=PiServiceTool_setup{#MyAppVersion}
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin
ArchitecturesInstallIn64BitMode=x64 ia64
ArchitecturesAllowed=x64 ia64

[Languages]
Name: "cn"; MessagesFile: "compiler:Default.isl";
Name: "en"; MessagesFile: "compiler:Languages\English.isl";

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";

[Icons]
Name: "{commondesktop}\{#MyAppExeName}"; Filename: "{code:PimaxDir}\Runtime\{#MyAppExeName}"; Tasks: desktopicon;

[Files]
Source: ".\ISTask.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: noencryption;
;Source: "{#MyInputPath}\out\tool\VS\2010\vcredist10.x86.exe"; DestDir: "{tmp}\"; Flags: ignoreversion;
;Source: "{#MyInputPath}\out\tool\VS\2010\vcredist10.x64.exe"; DestDir: "{tmp}\"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\VS\2012\vcredist12.x86.exe"; DestDir: "{tmp}\"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\VS\2012\vcredist12.x64.exe"; DestDir: "{tmp}\"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\VS\2013\vcredist13.x86.exe"; DestDir: "{tmp}\"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\VS\2013\vcredist13.x64.exe"; DestDir: "{tmp}\"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\VS\2015\vcredist15.x86.exe"; DestDir: "{tmp}\"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\VS\2015\vcredist15.x64.exe"; DestDir: "{tmp}\"; Flags: ignoreversion; AfterInstall: VcAfterInstall; 
Source: "{#MyInputPath}\out\server\dfudriver\Win7\*"; DestDir: "{tmp}\Win7\"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#MyInputPath}\out\server\dfudriver\Win8\*"; DestDir: "{tmp}\Win8\"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#MyInputPath}\out\server\dfudriver\Win8.1\*"; DestDir: "{tmp}\Win8.1\"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#MyInputPath}\out\server\dfudriver\stm_dfu_3040_64.exe"; DestDir: "{tmp}\"; Flags: ignoreversion; AfterInstall: DfuAfterInstall;
Source: "{#MyInputPath}\out\server\dfudriver\dfu\*"; DestDir: "{commonappdata}\pimax\piUpdater"; Flags: promptifolder;    
Source: "{#MyInputPath}\out\server\hmd_settings.json"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\PiService.exe"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\hid-dll\hidapi.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\oculus-dll\PiHmdlib_0_4.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\oculus-dll\PiHmdlib_0_6.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\oculus-dll\PiHmdlib_0_8.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\oculus-dll\msvcr100.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\oculus-dll\msvcr120.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\oculus-dll\msvcp120.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\oculus-dll\ModeSwitch.exe"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\dfu-dll\STDFU.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\dfu-dll\STDFUFiles.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\dfu-dll\STDFUPRT.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\server\dfu-dll\STTubeDevice30.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\tool\VS\kb.cmd"; DestDir: "{tmp}\"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\VS\Windows6.1-KB2670838-x64.msu"; DestDir: "{tmp}\"; Flags: ignoreversion; AfterInstall: WindowsUpdateAfterInstall;
Source: "{#MyInputPath}\out\server\PiServiceLauncher.exe"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion; AfterInstall: LauncherAfterInstall; 
Source: "{#MyInputPath}\out\server\PvrRuntime\*"; DestDir: "{tmp}\"; Flags: ignoreversion; AfterInstall: PvrRuntimeAfterInstall;

Source: "{#MyInputPath}\out\tool\PiServiceTool.exe"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\tool\qt\Qt5Core.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\tool\qt\Qt5Gui.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\tool\qt\Qt5Widgets.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\tool\qt\Qt5Network.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\tool\PiSvc.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\tool\qt\platforms\*"; DestDir: "{code:PimaxDir}\Runtime\platforms"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\extend-runtime\*"; DestDir: "{code:PimaxDir}\Runtime\extend-runtime"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\entend-win10-runtime\*"; DestDir: "{code:PimaxDir}\Runtime\entend-win10-runtime"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\piDisplay\*"; DestDir: "{code:PimaxDir}\Runtime\piDisplay"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\AutoplayVR\*"; DestDir: "{code:PimaxDir}\Runtime\AutoplayVR"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#MyInputPath}\out\tool\PVRPlayer\*"; DestDir: "{code:PimaxDir}\Runtime\PVRPlayer"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#MyInputPath}\out\tool\SteamVR\*"; DestDir: "{code:PimaxDir}\Runtime\SteamVR"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#MyInputPath}\out\tool\VS\redist\x64\*"; DestDir: "{sys}"; Flags: promptifolder uninsneveruninstall onlyifdoesntexist;
Source: "{#MyInputPath}\out\tool\VS\redist\x86\*"; DestDir: "{syswow64}"; Flags: promptifolder uninsneveruninstall onlyifdoesntexist;
;[Components] 
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”
[Code]
var
    ErrorCode: Integer;
    dllInfo: array of String;
    sysInfo: array of String;
    infoIndex: Integer;

function RunTaskInstall(FileName: string; bFullpath: Boolean): Boolean;
external 'RunTask@files:ISTask.dll stdcall delayload';
function KillTaskInstall(ExeFileName: string): Integer;
external 'KillTask@files:ISTask.dll stdcall delayload';

function PimaxDir(Param:String): String; 
var ResultStr: String;
begin
  Result := ExpandConstant('{pf}\Pimax');
  if RegQueryStringValue(HKLM, 'SOFTWARE\Pimax', 'InstallPath', ResultStr) then
  begin
     Result := ResultStr
  end
end;

function InitializeSetup(): Boolean;var ResultStr: String;  ResultCode: Boolean;
  begin
   Exec(ExpandConstant('{sys}\sc.exe'), 'stop PiServiceLauncher', '', SW_HIDE, ewNoWait, ErrorCode);
   Exec(ExpandConstant('{sys}\sc.exe'), 'stop OVRService', '', SW_HIDE, ewNoWait, ErrorCode);
   Exec(ExpandConstant('{sys}\sc.exe'), 'delete PiServiceLauncher', '', SW_HIDE, ewNoWait, ErrorCode);
   KillTaskInstall('PiServiceLauncher.exe');
   KillTaskInstall('PiService.exe');
   KillTaskInstall('pi_server.exe');
   KillTaskInstall('PiServiceTool.exe');
   KillTaskInstall('piServer_x64.exe');
   KillTaskInstall('piDesktop.exe');
   KillTaskInstall('OVRServer_x64.exe');
  Result := True;
  end;

procedure PimaxInitializeWizard();
var ResultStr: String;
    ResultCode: Integer;
    ResultInStr: String;
    ResultSteamStr: String;
begin
    //Result := True;
end;

procedure LauncherAfterInstall();
var ResultCode: Integer;
begin
  begin
  Exec(ExpandConstant('{code:PimaxDir}\Runtime\PiServiceLauncher.exe'), '-i', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
  end;
end;

procedure WindowsUpdateAfterInstall();
var ResultCode: Integer;
begin
  begin
  Exec(ExpandConstant('{tmp}\kb.cmd'), '', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
  end;
end;

procedure DfuAfterInstall();
var Version: TWindowsVersion;
    ResultCode: Integer;
  
begin
    GetWindowsVersionEx(Version);
    if (Version.Major = 6) and (Version.Minor = 1) then
    begin
        Exec(ExpandConstant('{tmp}\Win7\x64\dpinst_amd64.exe'), '/F /S', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;

    if (Version.Major = 6) and (Version.Minor = 2) then
    begin
        Exec(ExpandConstant('{tmp}\Win8\x64\dpinst_amd64.exe'), '/F /S', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;

    if (Version.Major = 6) and (Version.Minor = 3) then
    begin
        Exec(ExpandConstant('{tmp}\Win8.1\x64\dpinst_amd64'), '/F /S', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
    Exec(ExpandConstant('{tmp}\stm_dfu_3040_64.exe'), '/F /SW', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
end;

procedure PvrRuntimeAfterInstall();
var ResultCode: Integer;
begin
   Exec(ExpandConstant('{tmp}\pvr_runtime_setup.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
   Exec(ExpandConstant('{tmp}\pvr_oc_support_setup.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
   Exec(ExpandConstant('{tmp}\pvr_steamvr_driver_setup.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
   Exec(ExpandConstant('{tmp}\K-Lite_Codec_Pack.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
end;

procedure VcAfterInstall();
var ResultCode: Integer;
begin
    //Exec(ExpandConstant('{tmp}\vcredist10.x86.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    //Exec(ExpandConstant('{tmp}\vcredist10.x64.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    Exec(ExpandConstant('{tmp}\vcredist12.x86.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    Exec(ExpandConstant('{tmp}\vcredist12.x64.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    Exec(ExpandConstant('{tmp}\vcredist13.x86.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    Exec(ExpandConstant('{tmp}\vcredist13.x64.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    Exec(ExpandConstant('{tmp}\vcredist15.x86.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    Exec(ExpandConstant('{tmp}\vcredist15.x64.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);

end;
procedure CurUninstallStepChanged(CurUninstallStep:TUninstallStep);var ResultStr: String;
    ResultCode: Integer;
begin  case CurUninstallStep of      usUninstall:          begin // 开始卸载
      Exec(ExpandConstant('{code:PimaxDir}\Runtime\PiServiceLauncher.exe'), '-stop', '', SW_HIDE, ewNoWait, ErrorCode);
      Exec(ExpandConstant('{sys}\sc.exe'), 'delete PiServiceLauncher', '', SW_HIDE, ewNoWait, ErrorCode);
      Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im ' + 'PiService.exe', '', SW_HIDE, ewNoWait, ErrorCode);
      Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im ' + 'pi_server.exe', '', SW_HIDE, ewNoWait, ErrorCode);
      Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im ' + 'PiServiceTool.exe', '', SW_HIDE, ewNoWait, ErrorCode);
      if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{456EB5B8-1D63-446B-B897-032EBA575D4A}}_is1', 'UninstallString', ResultStr) then
      begin    
        ResultStr := RemoveQuotes(ResultStr);
        Exec(ResultStr, '/verysilent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
      end;

      if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{CB9E1459-F09E-4BB3-9962-D5D34475CB16}}_is1', 'UninstallString', ResultStr) then
      begin    
        ResultStr := RemoveQuotes(ResultStr);
        Exec(ResultStr, '/verysilent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
      end;
      end;      usPostUninstall:        begin      //卸载完成 
        // ...insert code to performpost-uninstall tasks here...          //ShellExec('open','http://www.pimaxvr.com', '', '', SW_SHOWNORMAL, ewNoWait,ErrorCode);        end;  end;  
end;

procedure CurStepChanged(CurStep: TSetupStep);
var ResultCode: Integer;
begin  
case CurStep of 
    ssInstall:
      begin
      PimaxInitializeWizard();
      end;
    ssDone:    
      begin // 安装完成
      Exec(ExpandConstant('{code:PimaxDir}\Runtime\PiServiceLauncher.exe'), '-start', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
      end;   
end;  
end;

[Run]
;Filename: "{code:PimaxDir}\Runtime\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppExeName, '&', '&&')}}";  Flags: postinstall nowait skipifsilent
