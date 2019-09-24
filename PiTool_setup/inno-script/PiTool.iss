; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "PiTool"
;#define MyAppVersion "1.0.0.6"
;#define MyInputPath  "E:\Intergration\PiInstall\inno_setup_source"
;#define MyAppPath  "E:\Intergration\PiInstall\"
;#define XbmcPath   "E:\Intergration\PiInstall\inno_setup_source\xbmc"
#define MyAppPublisher "Pimax Technology (Shanghai) Co., Ltd."
#define MyAppURL "http://www.pimaxVR.com"
#define MyAppExeName "PiTool.exe"
#define MyAppShortCut "PiTool"
#define SignString "digitsign"



[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{0D1DA8F2-89A7-4DAC-A9EF-B55E82CDA462}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={code:PimaxDir}\Runtime
DefaultGroupName={#MyAppName}
CreateAppDir=no
;OutputDir=.\setup
;OutputBaseFilename=PiServiceTool_setup{#MyAppVersion}
SetupIconFile=..\Resouces\icon.ico
Uninstallable=yes
UninstallFilesDir={code:PimaxDir}\Runtime
UninstallDisplayName=PiTool
UninstallDisplayIcon=..\Resouces\uninstall.ico
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin
ArchitecturesInstallIn64BitMode=x64 ia64
ArchitecturesAllowed=x64 ia64


[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl";
Name: "chinesesimp"; MessagesFile: "compiler:Languages\ChineseSimplified.isl";

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";

[Icons]
Name: "{commondesktop}\{#MyAppShortCut}"; Filename: "{code:PimaxDir}\Runtime\{#MyAppExeName}"; Tasks: desktopicon;
Name: "{group}\{#MyAppExeName}"; Filename: "{code:PimaxDir}\Runtime\{#MyAppExeName}"

[Files]
Source: ".\ISTask.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: noencryption; 
Source: "{#MyInputPath}\out\tool\PvrRuntime\*"; DestDir: "{tmp}\"; Flags: ignoreversion;
Source: "{#MyInputPath}\out\tool\Font\SetupFont.exe"; DestDir: "{tmp}\"; Flags: ignoreversion
Source: "{#MyInputPath}\out\tool\dfudriver\Win7\*"; DestDir: "{tmp}\Win7\"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#MyInputPath}\out\tool\dfudriver\Win8\*"; DestDir: "{tmp}\Win8\"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#MyInputPath}\out\tool\dfudriver\Win8.1\*"; DestDir: "{tmp}\Win8.1\"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#MyInputPath}\out\tool\dfudriver\p2.dfu"; DestDir: "{code:PimaxDir}\Runtime\dfudriver";Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "{#MyInputPath}\out\tool\dfudriver\stm_dfu_3040_64.exe"; DestDir: "{code:PimaxDir}\Runtime\dfudriver"; Flags: ignoreversion; AfterInstall: DfuAfterInstall; 
Source: "{#MyInputPath}\out\tool\PiService\PiServiceSetup.exe"; DestDir: "{tmp}\"; Flags: ignoreversion; AfterInstall: PvrRuntimeAfterInstall;
Source: "{#MyInputPath}\out\tool\PiTool.exe"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion
Source: "{#MyInputPath}\out\tool\DLL\PiSvc.dll"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion; AfterInstall: PiServiceAfterInstall;
Source: "{#MyInputPath}\out\tool\language\*"; DestDir: "{code:PimaxDir}\Runtime\language"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#MyInputPath}\out\tool\qt\*"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#MyInputPath}\out\tool\Tools\*"; DestDir: "{code:PimaxDir}\Runtime"; Flags: ignoreversion recursesubdirs createallsubdirs

[Registry]
Root: HKLM; Subkey: SOFTWARE\Pimax; ValueType: string; ValueName: "InstallPath"; ValueData: "{code:PimaxDir}"; Flags: uninsdeletekey;
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\{#MyAppExeName}; ValueType: string; ValueName: ""; ValueData: "{code:PimaxDir}\Runtime\{#MyAppExeName}"; Flags: uninsdeletekey;
Root: HKCU; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\Run; ValueType: string; ValueName: "PiTool"; ValueData: "{code:PimaxDir}\Runtime\{#MyAppExeName} hide"; Flags: uninsdeletekey;
Root: HKCU; Subkey: SOFTWARE\Pimax; ValueType: dword; ValueName: "Language"; ValueData: "{code:PimaxLanguage}"; Flags: uninsdeletekey;

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

function PimaxLanguage(Param:String): String; 
var languageName: String;
begin
  Result := '1';
  languageName := ActiveLanguage();
  if 'chinesesimp'=languageName then
  begin
     Result := '0'
  end
end;


function InitializeSetup(): Boolean;
var ResultStr: String;
  ResultCode: Boolean;
  begin
   //MsgBox(ExpandConstant('{cm:piplay360CloseMsg}'), mbInformation, MB_OK);
   Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im ' + 'PiTool.exe', '', SW_HIDE, ewNoWait, ErrorCode);
   KillTaskInstall('PiPlayDesktop.exe');
   Exec(ExpandConstant('{sys}\sc.exe'), 'stop PiServiceLauncher', '', SW_HIDE, ewNoWait, ErrorCode);
   Exec(ExpandConstant('{sys}\sc.exe'), 'stop OVRService', '', SW_HIDE, ewNoWait, ErrorCode);
   Exec(ExpandConstant('{sys}\sc.exe'), 'delete PiServiceLauncher', '', SW_HIDE, ewNoWait, ErrorCode);
   KillTaskInstall('PiServiceLauncher.exe');
   KillTaskInstall('PiService.exe');
   KillTaskInstall('pi_server.exe');
   KillTaskInstall('PiServiceTool.exe');
   KillTaskInstall('piServer_x64.exe');
   KillTaskInstall('piDesktop.exe');
   //KillTaskInstall('OVRServer_x64.exe');
   KillTaskInstall('vrcompositor.exe');
   KillTaskInstall('vrdashboard.exe');
   KillTaskInstall('vrmonitor.exe');
   KillTaskInstall('vrserver.exe');
  Result := True;
  end;

procedure PimaxInitializeWizard();
var ResultStr: String;
    ResultCode: Integer;
    ResultInStr: String;
    ResultSteamStr: String;begin
    if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{0D1DA8F2-89A7-4DAC-A9EF-B55E82CDA462}}_is1', 'UninstallString', ResultStr) then
      begin    
        ResultStr := RemoveQuotes(ResultStr);
    end;end;

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
    //Exec(ExpandConstant('{tmp}\stm_dfu_3040_64.exe'), '/F /SW', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
end;

procedure PvrRuntimeAfterInstall();
var ResultCode: Integer;
begin
   Exec(ExpandConstant('{tmp}\pvr_runtime_setup.exe'), ExpandConstant('/SILENT /LANG={language}'), '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
   Exec(ExpandConstant('{tmp}\pvr_oc_support_setup.exe'), ExpandConstant('/SILENT /LANG={language}'), '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
   Exec(ExpandConstant('{tmp}\pvr_steamvr_driver_setup.exe'), ExpandConstant('/SILENT /LANG={language}'), '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
   Exec(ExpandConstant('{tmp}\pvr_home_setup.exe'), ExpandConstant('/SILENT /LANG={language}'), '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
   Exec(ExpandConstant('{tmp}\SetupFont.exe'), '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
end;

procedure PiServiceAfterInstall();
var ResultCode: Integer;
begin
   Exec(ExpandConstant('{tmp}\PiServiceSetup.exe'), ExpandConstant('/SILENT /LANG={language}'), '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
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

procedure CurUninstallStepChanged(CurUninstallStep:TUninstallStep);
var ResultStr: String;
    ResultCode: Integer;
begin  
case CurUninstallStep of  
    usUninstall:    
      begin // 开始卸载
	    Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im ' + 'PiPlayDesktop.exe', '', SW_HIDE, ewNoWait, ErrorCode);
      Exec(ExpandConstant('{code:PimaxDir}\Runtime\PiServiceLauncher.exe'), '-stop', '', SW_HIDE, ewNoWait, ErrorCode);
      Exec(ExpandConstant('{sys}\sc.exe'), 'delete PiServiceLauncher', '', SW_HIDE, ewNoWait, ErrorCode);
      Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im ' + 'PiService.exe', '', SW_HIDE, ewNoWait, ErrorCode);
      Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im ' + 'pi_server.exe', '', SW_HIDE, ewNoWait, ErrorCode);
      Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im ' + 'PiServiceTool.exe', '', SW_HIDE, ewNoWait, ErrorCode);
	    Exec(ExpandConstant('{sys}\taskkill.exe'), '/f /im ' + 'PiTool.exe', '', SW_HIDE, ewNoWait, ErrorCode);

      if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{ABC17DF7-089A-45AC-AC12-79D48D781155}}_is1', 'UninstallString', ResultStr) then
      begin    
        ResultStr := RemoveQuotes(ResultStr);
        Exec(ResultStr, '/verysilent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
      end;

      if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{456EB5B8-1D63-446B-B897-032EBA575D4A}}_is1', 'UninstallString', ResultStr) then
      begin    
        ResultStr := RemoveQuotes(ResultStr);
        Exec(ResultStr, '/verysilent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
      end;

      if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{1C4FE298-6FF2-489F-9038-B81EB65D59E0}}_is1', 'UninstallString', ResultStr) then
      begin    
        ResultStr := RemoveQuotes(ResultStr);
        Exec(ResultStr, '/verysilent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
      end;

      if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{CB9E1459-F09E-4BB3-9962-D5D34475CB16}}_is1', 'UninstallString', ResultStr) then
      begin    
        ResultStr := RemoveQuotes(ResultStr);
        Exec(ResultStr, '/verysilent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
      end;

      if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{34B78A78-04CA-4D08-83A7-D1AAC7750D32}}_is1', 'UninstallString', ResultStr) then
      begin    
        ResultStr := RemoveQuotes(ResultStr);
        Exec(ResultStr, '/verysilent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
      end;

      //if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{0D1DA8F2-89A7-4DAC-A9EF-B55E82CDA462}}_is1', 'UninstallString', ResultStr) then
      //begin    
      //  ResultStr := RemoveQuotes(ResultStr);
      //  Exec(ResultStr, '/verysilent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
      //end;
      end;  
    usPostUninstall:  
      begin      //卸载完成 
        // ...insert code to performpost-uninstall tasks here...  
        //ShellExec('open','http://www.pimaxvr.com', '', '', SW_SHOWNORMAL, ewNoWait,ErrorCode);  
      end;  
end;  
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
Filename: "{code:PimaxDir}\Runtime\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppExeName, '&', '&&')}}";  Flags: postinstall nowait skipifsilent
